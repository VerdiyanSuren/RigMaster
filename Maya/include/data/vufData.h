#ifndef VF_RM_DATA_H
#define VF_RM_DATA_H

#include <maya/MPxData.h>
#include <maya/MFnPluginData.h>

#include "data/vufDataTemplate.h"
#include <memory>

/**
* Try to read custom data from output handle
* If data doesnt exist then create new data 
* 
* Example
* MStatus vufTestNode::compute(const MPlug& p_plug, MDataBlock& p_data)
* { ...
*	std::shared_ptr<vufDataTest> l_src_data;
*	VF_TP_GET_DATA_FROM_OUT_AND_CREATE(mpxTestWrapper, vufDataTest, p_data, g_data_src_attr, l_src_data);
*	l_src_data->set_owner_node(m_node_unique_id);
*	...
* }
*/
#define VF_RM_GET_DATA_FROM_OUT_AND_CREATE(WRAPPER_CLASS, DATA_CLASS, data_block, attr, data_var)	\
{																						\
	MDataHandle	 l_out_handle	= data_block.outputValue(attr);							\
	WRAPPER_CLASS *l_wrapp_ptr	= (WRAPPER_CLASS*)l_out_handle.asPluginData();			\
	if (l_wrapp_ptr == nullptr)															\
	{																					\
		/*std::cout << "Data is Empty" <<std::endl;*/									\
		MFnPluginData l_data_creator;													\
		l_out_handle.set(l_data_creator.create(WRAPPER_CLASS::g_id, &l_status));		\
		CHECK_MSTATUS_AND_RETURN_IT(l_status);											\
		l_wrapp_ptr = (WRAPPER_CLASS*)l_out_handle.asPluginData();						\
	}																					\
	data_var = l_wrapp_ptr->get_data();													\
	if (data_var == nullptr)															\
	{																					\
		/*std::cout << "Wanna constructor" <<std::endl;*/								\
		data_var = std::shared_ptr<DATA_CLASS>(new DATA_CLASS);							\
		l_wrapp_ptr->set_data(data_var);												\
	}																					\
}																						\

/**
* Try to read data from input
*/
#define VF_RM_GET_DATA_FROM_IN(WRAPPER_CLASS, DATA_CLASS, data_block, attr, data_var)	\
{																						\
	MDataHandle		l_in_handle = data_block.inputValue(attr);							\
	WRAPPER_CLASS*	l_wrapp_ptr = (WRAPPER_CLASS*)l_in_handle.asPluginData();			\
	data_var = (l_wrapp_ptr == nullptr ? nullptr : l_wrapp_ptr->get_data());			\
}																						\

/**
*  Try to set out data
*/
#define VF_RM_SET_DATA_TO_OUT(WRAPPER_CLASS, DATA_CLASS, data_block, attr, data_var)	\
{																						\
	MDataHandle	 l_out_handle	= data_block.outputValue(attr);							\
	MFnPluginData l_data_creator;														\
	MObject l_obj = l_data_creator.create(WRAPPER_CLASS::g_id, &l_status);				\
	l_out_handle.set(l_obj);															\
	WRAPPER_CLASS* l_wrapp_ptr = (WRAPPER_CLASS*)l_data_creator.constData();			\
	l_wrapp_ptr->set_data(data_var);													\
}																						\

#define VF_RM_DECLARE_DATA_BODY(CLASS_NAME)														\
public:																							\
CLASS_NAME();																					\
virtual ~CLASS_NAME##();																		\
virtual MStatus 	readASCII(const MArgList& p_args, unsigned int& p_last_element) override;	\
virtual MStatus 	readBinary(std::istream& p_in,	  unsigned int	length)			override;	\
virtual MStatus 	writeASCII(std::ostream& p_out)  override;									\
virtual MStatus 	writeBinary(std::ostream& p_out) override;									\
virtual int			get_type()	const override;													\
virtual void		log_me()	const override;													\

#define VF_RM_DECLARE_STANDART_DATA_CLASS(WRAPPER_CALSS_NAME,CLASS_NAME,INNER_CLASS_NAME)		\
class CLASS_NAME :public vufData																\
	{																							\
	public:																						\
		VF_RM_DECLARE_DATA_BODY(CLASS_NAME);													\
		std::shared_ptr<INNER_CLASS_NAME> m_internal_data = nullptr;									\
	};																							\
	using WRAPPER_CALSS_NAME = vufDataTemplate<CLASS_NAME>;

namespace vufRM
{
	/**
	* Interface off custom data.
	*/
	class vufData
	{
	public:
		vufData() :m_owner_node_id(0) {}
		virtual ~vufData() {};
		enum
		{
			k_absent_data,
			k_txt_data,
			k_test_data,
			k_curve_data
		};
		void		set_owner_id(uint64_t p_new_owner_id) { m_owner_node_id = p_new_owner_id; }
		uint64_t	get_owner_id() const { return m_owner_node_id; }
		
		// serialize methods for mpx data overrides
		virtual MStatus 	readASCII(	const MArgList& p_args, unsigned int&	p_last_element) = 0;
		virtual MStatus 	readBinary(	std::istream&	p_in,	unsigned int	length)			= 0;
		virtual MStatus 	writeASCII(	std::ostream&	p_out) = 0;
		virtual MStatus 	writeBinary(std::ostream&	p_out) = 0;

		virtual int						get_type()	const = 0;
		virtual void					log_me()	const = 0;
		//static methods
		static std::shared_ptr<vufData> create_data(int p_type);
		static int						get_data_type(std::shared_ptr<vufData> p_data)
		{
			return p_data == nullptr ? k_absent_data : p_data->get_type();
		}
	protected:
		uint64_t m_owner_node_id = 0;
	};

	class vufDataTest :public vufData
	{
	public:
		VF_RM_DECLARE_DATA_BODY(vufDataTest)
		
		double		m_value;
		int			m_random = 0;
		int			m_type	 = 0; // 1 - child  2- dist
	};
	using mpxTestWrapper = vufDataTemplate<vufDataTest>;

	// methods to manipulate with custom data
	template<typename CLASS_WRAPPER, typename CLASS_NAME, typename INNER_CLASS_NAME>
	std::shared_ptr<INNER_CLASS_NAME> get_inner_data(MFnDependencyNode& p_node,MObject& p_attr)
	{
		MStatus l_status;
		MPlug	l_txt_plug = p_node.findPlug(p_attr,true, &l_status);
		MObject l_data;
		
		l_status = l_txt_plug.getValue(l_data);
		if (l_status != MS::kSuccess)
		{			
			return nullptr;
		}
		
		MFnPluginData l_data_fn;
		l_status = l_data_fn.setObject(l_data);
		if (l_status != MS::kSuccess)
		{
			return nullptr;
		}
		
		auto l_data_ptr = (CLASS_WRAPPER*)l_data_fn.constData(&l_status);
		if (l_status != MS::kSuccess)
		{
			return nullptr;
		}
		
		auto l_class_data = l_data_ptr->get_data();
		if (l_class_data == nullptr)
		{
			return nullptr;
		}

		return l_class_data->m_internal_data;
	}



}
#endif // !VF_RM_DATA_H
