#ifndef VF_RM_MTRX_LST_VFK_H
#define VF_RM_MTRX_LST_VFK_H

#include <maya/MPxNode.h>
#include <math/vufQuaternion.h>
#include <math/vufMatrix.h>
#include <vector>
#include <functional>

namespace vufRM
{
	class vufMatrixListVFK : public MPxNode
	{
	private:
		enum
		{
			k_xyz,
			k_yzx,
			k_zxy,
			k_xzy,
			k_yxz,
			k_zyx
		};
		enum
		{
			k_gauss,
			k_linear,
			k_smooth,
			k_psmooth
		};
		enum
		{
			k_local,
			k_picked,
			k_world
		};
		enum
		{
			k_hierarchy,
			k_bounded
		};
		struct vufEffectors
		{
			double	m_t				= 0;
			int		m_mode			= k_gauss;
			double	m_amount		= 1.0;
			double	m_sigma_a		= 0.5;
			double	m_sigma_b		= 0.5;
			double	m_const_a		= 0.0;
			double	m_const_b		= 0.0;
			double	m_rx = 0.0, m_ry = 0.0, m_rz = 0.0;
			double	m_rg = 0; // gimbal lock 4rth axis
			double	m_twist = 0.0;
			int		m_twist_mode	 = k_hierarchy;
			int		m_rotation_order = k_xyz;
			vufMath::vufVector_4d m_scale			= vufMath::vufVector_4d();
			vufMath::vufVector_4d m_translate		= vufMath::vufVector_4d();

			unsigned int m_next = 0;
			unsigned int m_prev = 0;
			double m_w_1 = 0;
			double m_w_2 = 0;
		};
		struct vufInputList
		{
			double m_l; // length to this knot
			double m_t; // param of this knot
			double m_rx, m_ry, m_rz;
			vufMath::vufVector_4d m_dir_x = vufMath::vufVector_4d();	//
			vufMath::vufVector_4d m_dir_y = vufMath::vufVector_4d();	//
			vufMath::vufVector_4d m_dir_z = vufMath::vufVector_4d();	//

			vufMath::vufVector_4d m_dir_x_n = vufMath::vufVector_4d();
			vufMath::vufVector_4d m_dir_y_n = vufMath::vufVector_4d();
			vufMath::vufVector_4d m_dir_z_n = vufMath::vufVector_4d();
			//vufMath::vufVector_4d		m_dir_to_next;
			vufMath::vufVector_4d		m_coord_p	= vufMath::vufVector_4d();
			vufMath::vufQuaternion_d	m_quat_w	= vufMath::vufQuaternion_d();
			vufMath::vufQuaternion_d	m_quat_r	= vufMath::vufQuaternion_d();
			vufMath::vufVector_4d	m_scale_origin	= vufMath::vufVector_4d();
			vufMath::vufVector_4d	m_scale_apply	= vufMath::vufVector_4d();
			//vufMath::vufVector_4d	m_frame_pos		= vufMath::vufVector_4d();
			std::vector<double>		m_effector_weights;
			std::vector<double>		m_effector_weights_r;
			//vufMath::vufQuaternion_d	m_quat;
			//vufMath::vufVector_4d		m_translate;
		};
		struct vufOuts
		{
			vufMath::vufMatrix_4d m_parent;
			vufMath::vufMatrix_4d m_inverse;
		};
		struct vufIndex
		{
			unsigned int m_prev;
			unsigned int m_next;
		};
	public:
		vufMatrixListVFK();
		virtual ~vufMatrixListVFK() {}

		static  void*	creator();
		static  MStatus	initialize();
		virtual void 	postConstructor() override;
		virtual MStatus	compute(const MPlug& plug, MDataBlock& data) override;
		virtual MStatus	connectionBroken(const MPlug& p_plug_1, const MPlug& p_plug_2, bool p_as_src) override;
		static const MTypeId	g_id;
		//attributes
		static MObject	g_in_data_attr;
		static MObject	g_out_data_attr;

		static MObject	g_scale_t_attr;

		static MObject	g_in_effectors_attr;
		static MObject	g_mix_mode_attr;
		static MObject	g_t_attr;
		static MObject	g_amount_attr;
		static MObject	g_sigma_a_attr;
		static MObject	g_cnst_a_attr;
		static MObject	g_sigma_b_attr;
		static MObject	g_cnst_b_attr;
		static MObject	g_rot_compound_attr, g_rx_attr, g_ry_attr, g_rz_attr;
		static MObject	g_scale_attr;
		static MObject	g_translate_attr;
		static MObject	g_twist_attr;
		static MObject	g_twist_mode_attr;
		static MObject	g_rotation_order;


		static MObject  g_out_parent_attr;
		static MObject	g_out_inverse_attr;
		static MObject	g_outs_attr;

		static const MString	g_type_name;
	private:
	private:
		uint64_t					m_gen_id		= 0;
		std::vector<vufEffectors>	m_eff_arr		= std::vector<vufEffectors>();
		std::vector<vufInputList>	m_in_t_arr		= std::vector<vufInputList>();
		std::vector<vufOuts>		m_outs_arr		= std::vector<vufOuts>();
		double						m_scale_t		= 1.0;
		double						m_length		= 0.0;

		inline double	compute_input_matrix_param( const std::vector<vufMath::vufMatrix_4d>& p_in_array);
		inline void		compute_out_array(	const std::vector<vufMath::vufMatrix_4d>& p_in_array, 
											std::vector<vufMath::vufMatrix_4d>& p_out_array);
		inline void compute_out_helpers(std::vector<vufMath::vufMatrix_4d>& p_out_array);
		inline vufIndex find_list_index(double p_t, unsigned int p_total_count) const;
		
		static vufMath::vufQuaternion_d xyz_order(double x, double y, double z, const vufMath::vufVector_4d& axis); 
		static vufMath::vufQuaternion_d xyz_inverse_order(double x, double y, double z, const vufMath::vufVector_4d& axis);

		static vufMath::vufQuaternion_d yzx_order(double x, double y, double z, const vufMath::vufVector_4d& axis);
		static vufMath::vufQuaternion_d yzx_inverse_order(double x, double y, double z, const vufMath::vufVector_4d& axis);

		static vufMath::vufQuaternion_d zxy_order(double x, double y, double z, const vufMath::vufVector_4d& axis);
		static vufMath::vufQuaternion_d zxy_inverse_order(double x, double y, double z);

		static vufMath::vufQuaternion_d xzy_order(double x, double y, double z);
		static vufMath::vufQuaternion_d xzy_inverse_order(double x, double y, double z);

		static vufMath::vufQuaternion_d yxz_order(double x, double y, double z);
		static vufMath::vufQuaternion_d yxz_inverse_order(double x, double y, double z);

		static vufMath::vufQuaternion_d zyx_order(double x, double y, double z);
		static vufMath::vufQuaternion_d zyx_inverse_order(double x, double y, double z);

		static std::function<vufMath::vufQuaternion_d(double, double, double)> m_order_array[];
		static std::function<vufMath::vufQuaternion_d(double, double, double)> m_order_inverse_array[];
	};
}
#endif // !VF_RM_MTRX_LST_VFK_H
