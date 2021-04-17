#ifndef VF_RM_DATA_TMPLT_H
#define VF_RM_DATA_TMPLT_H

#include <maya/MPxData.h>
#include <maya/MTypeId.h>
#include <maya/MArgList.h>

#include <memory>

namespace vufRM
{	
	/**  Wrapper around data class. keep data as weak_ptr 
	*/
	template<class T >
	class vufDataTemplate : public MPxData
	{
	public:
		vufDataTemplate():MPxData() {}
		virtual ~vufDataTemplate() {}

		virtual MStatus 	readASCII(	const MArgList& p_args, unsigned int& p_last_element)	override
		{
			//VF_LOG_INFO("READ MPXDATA");
			MStatus l_status;
			int l_type = p_args.asInt(p_last_element++, &l_status);
			std::shared_ptr<T> l_data = nullptr;
			if (l_type != vufData::k_absent_data )
			{
				l_data = std::shared_ptr<T>(new T());
			}
			if (l_data != nullptr)
			{
				l_status = l_data->readASCII(p_args, p_last_element);
				m_data = l_data;
				return l_status;
			}
			return MS::kSuccess;
		}
		virtual MStatus 	readBinary(	std::istream&	p_in,	unsigned int p_length)			override
		{	
			if (p_length <= 0)
			{
				std::cout << "Error while reading" << std::endl;
				return MS::kFailure;
			}
			int	 l_type;
			p_in.read((char*)&l_type, sizeof(int));
			std::shared_ptr<T> l_data = nullptr;
			if (l_type != vufData::k_absent_data)
			{
				l_data = std::shared_ptr<T>(new T());
			}
			if ( l_data != nullptr )
			{				
				MStatus l_status;
				l_status = l_data->readBinary(p_in,p_length);
				m_data = l_data;
				return l_status;
			}
			return MS::kSuccess;
		}
		virtual MStatus 	writeASCII(	std::ostream&	p_out)	override
		{
			//VF_LOG_INFO("WRITE MPXDATA");
			if (m_data == nullptr)
			{
				int l_absent_type = vufData::k_absent_data;
				p_out << l_absent_type <<" " << "\n";
				return MS::kSuccess;
			}
			int l_type = m_data->get_type();
			p_out << l_type << " ";
			return m_data->writeASCII(p_out);
			p_out << "\n";
		}
		virtual MStatus 	writeBinary(std::ostream&	p_out)	override
		{
			if (m_data == nullptr)
			{
				int l_absent_type = vufData::k_absent_data;
				p_out.write((char*)&l_absent_type, sizeof(int));
				return MS::kSuccess;
			}
			int l_type = m_data->get_type();
			p_out.write((char*)&l_type, sizeof(int));
			return m_data->writeBinary(p_out);
		}

		virtual void 		copy(const MPxData& p_src) override
		{
			//std::cout << "COPY" << std::endl;

			if (p_src.typeId() == typeId())
			{
				m_data = ((const vufDataTemplate<T>&)p_src).m_data;
			}
		}
		virtual MTypeId 	typeId()	const override { return g_id; }
		virtual MString 	name()		const override { return MString(g_type_name); }

		void				set_data(std::shared_ptr<T> p_data) { m_data = p_data; }
		std::shared_ptr<T>	get_data() const { return m_data; }

		static void* creator() { return new vufDataTemplate<T>(); }

	//private:
		static const MString    g_type_name;
		static const MTypeId    g_id;

		std::shared_ptr<T>		m_data = nullptr;
	};
}

#endif // !VF_RM_DATA_TMPLT_H
