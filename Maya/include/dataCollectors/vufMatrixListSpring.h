#ifndef VF_RM_SPRING_NODE_H
#define VF_RM_SPRING_NODE_H

#include <maya/MPxNode.h>
#include <maya/MFloatArray.h>
#include <math/vufVector.h>
#include <math/vufMatrix.h>
#include <vector>

namespace vufRM
{
	template< typename T>
	class vufFakeSpringSolver
	{
	public:
		enum
		{
			k_pass,
			k_to_mem,
			k_from_mem
		};
		inline void set_lists(	std::vector < vufMath::vufMatrix4<T>>& p_rest_list,
								std::vector < vufMath::vufMatrix4<T>>& p_deform_list,
								std::vector < vufMath::vufMatrix4<T>>& p_out_list)
		{
			l_rest_list_ptr		= &p_rest_list;
			l_deform_list_ptr	= &p_deform_list;
			l_out_list_ptr		= &p_out_list;
		}
		inline bool init()
		{
			if (l_rest_list_ptr		== nullptr ||
				l_deform_list_ptr	== nullptr ||
				l_out_list_ptr		== nullptr)
			{
				return false;
			}
			auto l_size = l_deform_list_ptr->size();
			if (l_size != m_knots_lin_stif_arr.size())
			{
				m_knots_lin_stif_arr.resize(l_size);
				m_knots_lin_mass_arr.resize(l_size);
				m_knots_lin_damp_arr.resize(l_size);
				m_knots_lin_ampl_arr.resize(l_size);

				m_knots_rot_inhr_arr.resize(l_size);
				m_knots_rot_stif_arr.resize(l_size);
				m_knots_rot_mass_arr.resize(l_size);
				m_knots_rot_damp_arr.resize(l_size);
				m_knots_rot_ampl_arr.resize(l_size);
			}
			return true;
		}

		inline void solve()
		{
			T l_deltaTime = m_current_time - m_prev_time;
			if (l_deltaTime > 0.0)
			{
				// time is changed. lets update state variable
				m_dt_lin = l_deltaTime / (T)m_lin_iter;
				m_dt_rot = l_deltaTime / (T)m_rot_iter;
			}

		}
		void solveVerletLinear()
		{
			
		}

		bool	m_enable		= true;
		int		m_init_frame	= 1;

		int		m_cache_mode	= k_pass;
		int		m_lin_iter		= 1;
		int		m_rot_iter		= 1;
		inline void set_time(T p_time) { m_current_time = p_time; }
	private:
		std::vector < vufMath::vufMatrix4<T>>* l_rest_list_ptr	= nullptr;
		std::vector < vufMath::vufMatrix4<T>>* l_deform_list_ptr = nullptr;
		std::vector < vufMath::vufMatrix4<T>>* l_out_list_ptr	= nullptr;
	public:
		std::vector<T> m_knots_lin_stif_arr	= std::vector<T>();
		std::vector<T> m_knots_lin_mass_arr	= std::vector<T>();
		std::vector<T> m_knots_lin_damp_arr	= std::vector<T>();
		std::vector<vufMath::vufVector4<T>> m_knots_lin_ampl_arr = std::vector<vufMath::vufVector4<T>>();

		std::vector<T> m_knots_rot_inhr_arr	= std::vector<T>();
		std::vector<T> m_knots_rot_stif_arr	= std::vector<T>();
		std::vector<T> m_knots_rot_mass_arr	= std::vector<T>();
		std::vector<T> m_knots_rot_damp_arr	= std::vector<T>();
		std::vector< vufMath::vufVector4<T>> m_knots_rot_ampl_arr	= std::vector< vufMath::vufVector4<T>>();
	private:
		T m_dt_lin = 0.0;
		T m_dt_rot = 0.0;
		T m_current_time = 0.0;// in seconds
		T m_prev_time = 0.0;
	};
	class vufMatrixListSpringNode : public MPxNode
	{
	public:
		vufMatrixListSpringNode();
		virtual ~vufMatrixListSpringNode() {}

		static  void* creator();
		static  MStatus	initialize();
		virtual void 	postConstructor() override;
		virtual MStatus	compute(const MPlug& plug, MDataBlock& data) override;
		virtual MStatus	shouldSave(const MPlug&, bool&) override;
		//virtual MStatus	connectionBroken(const MPlug& p_plug_1, const MPlug& p_plug_2, bool p_as_src) override;
		static const MTypeId	g_id;
		static const MString	g_type_name;
		//attributes
		static MObject	g_data_in_deformed_attr;
		static MObject	g_data_in_rest_attr;

		static MObject	g_in_cache_mode_attr;
		static MObject	g_in_time_attr;
		static MObject	g_in_enable_attr;
		static MObject	g_in_init_frame_attr;
		
		//static MObject	g_in_translation_compound_attr;
		static MObject	g_in_translation_iteration_attr;
		static MObject	g_in_translation_stiffness_attr;
		static MObject	g_in_translation_stiffness_ramp_attr;
		static MObject	g_in_translation_mass_attr;
		static MObject	g_in_translation_mass_ramp_attr;
		static MObject	g_in_translation_dump_attr;
		static MObject	g_in_translation_dump_ramp_attr;
		static MObject	g_in_translation_amplitude_attr;
		static MObject	g_in_translation_amplitude_ramp_attr;

		//static MObject	g_in_rotation_compound_attr;
		static MObject	g_in_rotation_inherit_attr;
		static MObject	g_in_rotation_inherit_ramp_attr;
		static MObject	g_in_rotation_iteration_attr;
		static MObject	g_in_rotation_stiffness_attr;
		static MObject	g_in_rotation_stiffness_ramp_attr;
		static MObject	g_in_rotation_mass_attr;
		static MObject	g_in_rotation_mass_ramp_attr;
		static MObject	g_in_rotation_dump_attr;
		static MObject	g_in_rotation_dump_ramp_attr;
		static MObject	g_in_rotation_amplitude_attr;
		static MObject	g_in_rotation_amplitude_ramp_attr;

		static MObject	g_out_data_attr;
		static MObject	g_data_cache_attr;//cache

	private:
		uint64_t		m_gen_id = 0;
		MFloatArray		m_knots_t_array;

		MFloatArray		m_knots_lin_stif_array;
		MFloatArray		m_knots_lin_mass_array;
		MFloatArray		m_knots_lin_dump_array;
		MFloatArray		m_knots_lin_ampl_array;

		MFloatArray		m_knots_rot_inhrt_array;
		MFloatArray		m_knots_rot_stif_array;
		MFloatArray		m_knots_rot_mass_array;
		MFloatArray		m_knots_rot_dump_array;
		MFloatArray		m_knots_rot_ampl_array;

		vufFakeSpringSolver<double> m_engine = vufFakeSpringSolver<double>();
	};
}
#endif // !VF_RM_SPRING_NODE_H
