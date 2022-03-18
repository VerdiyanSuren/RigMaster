#ifndef VF_RM_SPRING_NODE_H
#define VF_RM_SPRING_NODE_H

#include <maya/MPxNode.h>
#include <maya/MFloatArray.h>
#include <math/vufVector.h>
#include <math/vufMatrix.h>
#include <math/vufVector.h>
#include <vector>

namespace vufRM
{
	class vufFakeSpringSolver
	{
	public:
		enum
		{
			k_pass,
			k_to_mem,
			k_from_mem
		};
		inline void set_lists(	std::vector < vufMath::vufMatrix_4d>& p_deform_list,
								std::vector < vufMath::vufMatrix_4d>& p_out_list,
								std::vector < vufMath::vufMatrix_4d>& p_rest_list
							)
		{
			m_in_ptr	= &p_deform_list;
			m_out_ptr	= &p_out_list;
			m_rest_ptr	= &p_rest_list;
		}
		bool init();
		void solve();
		void solveVerletLinear(double p_delta_time);
		void solveVerletRot(double p_delta_time);
	private:
		bool	m_valid			= false;
		bool	m_enable_lin	= true;
		bool	m_enable_rot	= true;
		//bool	m_pin_first		= false;
		//bool	m_pin_last	= false;
		double	m_init_time		= 1.0;
		int		m_cache_mode	= k_pass;
		int		m_lin_iter		= 1;
		int		m_rot_iter		= 1;
	public:
		inline void set_enable_lin(bool val)		{ m_enable_lin = val; }
		inline void set_enable_rot(bool val)		{ m_enable_rot = val; }
		inline void set_init_time(double p_time)	{ m_init_time = p_time; }
		inline void set_cache_mode(int p_mode)		{ m_cache_mode = p_mode; }
		inline void set_time(double p_time)			{ m_current_time = p_time; }
		inline void set_lin_iter(int p_iter)		{ m_lin_iter = p_iter < 1 ? 1 :p_iter; }
		inline void set_rot_iter(int p_iter)		{ m_rot_iter = p_iter < 1 ? 1 : p_iter; }
		inline void compute_radiuses();
	private:
		// pointers to extern arrays of rest> deformed and simulation result
		std::vector < vufMath::vufMatrix_4d>* m_in_ptr		= nullptr;
		std::vector < vufMath::vufMatrix_4d>* m_out_ptr		= nullptr;
		std::vector < vufMath::vufMatrix_4d>* m_rest_ptr	= nullptr;
		// linear spring
		std::vector<vufMath::vufVector_4d> m_lin_mstr_prev	= std::vector<vufMath::vufVector_4d>();

		std::vector<vufMath::vufVector_4d> m_lin_p_curr	= std::vector<vufMath::vufVector_4d>();	//R
		std::vector<vufMath::vufVector_4d> m_lin_p_prev	= std::vector<vufMath::vufVector_4d>();	//R prime
		std::vector<vufMath::vufVector_4d> m_lin_res_offset = std::vector<vufMath::vufVector_4d>();
		// rotation spring
		std::vector<double> m_radius = std::vector<double>();
		std::vector<vufMath::vufVector_4d> m_rot_mstr_prev		= std::vector<vufMath::vufVector_4d>();
		std::vector<vufMath::vufVector_4d> m_rot_mstr_vel		= std::vector<vufMath::vufVector_4d>();
		std::vector<vufMath::vufVector_4d> m_rot_mstr_axis_prev = std::vector<vufMath::vufVector_4d>();
		std::vector<vufMath::vufVector_4d> m_rot_mstr_angle_prev= std::vector<vufMath::vufVector_4d>();
		
		std::vector<vufMath::vufVector_4d> m_x_axis_curr		= std::vector<vufMath::vufVector_4d>();
		std::vector<vufMath::vufVector_4d> m_x_axis_prev		= std::vector<vufMath::vufVector_4d>();
		std::vector<vufMath::vufVector_4d> m_rot_res_axis_offset = std::vector<vufMath::vufVector_4d>();
		std::vector<double>				   m_rot_res_angle_offset= std::vector<double>();
		// gravity
		vufMath::vufVector_4d				 m_gravity			= vufMath::vufVector_4d();
	public:
		std::vector<double> m_lin_stif	= std::vector<double>();
		std::vector<double> m_lin_mass	= std::vector<double>();
		std::vector<double> m_lin_damp	= std::vector<double>();
		std::vector<vufMath::vufVector_4d> m_lin_ampl = std::vector<vufMath::vufVector_4d>();

		std::vector<double> m_rot_inhr	= std::vector<double>();
		std::vector<double> m_rot_stif	= std::vector<double>();
		std::vector<double> m_rot_mass	= std::vector<double>();
		std::vector<double> m_rot_damp	= std::vector<double>();
		std::vector< vufMath::vufVector_4d> m_rot_ampl	= std::vector< vufMath::vufVector_4d>();
	private:
		double	m_current_time = 0.0;// in seconds
		double	m_prev_time = 0.0;

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
		static MObject	g_in_enable_lin_attr;
		static MObject	g_in_enable_rot_attr;
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

		vufFakeSpringSolver m_engine = vufFakeSpringSolver();
	};
}
#endif // !VF_RM_SPRING_NODE_H
