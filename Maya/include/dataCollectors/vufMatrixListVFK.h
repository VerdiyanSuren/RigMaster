#ifndef VF_RM_MTRX_LST_VFK_H
#define VF_RM_MTRX_LST_VFK_H

#include <maya/MPxNode.h>
#include <math/vufQuaternion.h>
#include <math/vufMatrix.h>
#include <vector>

namespace vufRM
{
	class vufMatrixListVFK : public MPxNode
	{
	private:
		struct vufEffectors
		{
			double m_t;
			double m_amount;
			double m_sigma_a;
			double m_sigma_b;
			double m_const_a;
			double m_const_b;
			double m_rx, m_ry, m_rz;
			vufMath::vufVector_4d m_scale;
			unsigned int m_next;
			unsigned int m_prev;
			double m_w_1 = 0;
			double m_w_2 = 0;
		};
		struct vufInputList
		{
			double m_l;
			double m_t;
			vufMath::vufVector_4d m_dir_x;
			vufMath::vufVector_4d m_dir_y;
			vufMath::vufVector_4d m_dir_z;

			vufMath::vufVector_4d m_dir_x_n;
			vufMath::vufVector_4d m_dir_y_n;
			vufMath::vufVector_4d m_dir_z_n;
			//vufMath::vufVector_4d		m_dir_to_next;
			vufMath::vufVector_4d		m_coord_p	= vufMath::vufVector_4d();
			vufMath::vufQuaternion_d	m_quat_w	= vufMath::vufQuaternion_d();
			vufMath::vufQuaternion_d	m_quat_r	= vufMath::vufQuaternion_d();
			vufMath::vufVector_4d m_scale_origin;
			vufMath::vufVector_4d m_scale_apply;
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
		static MObject	g_max_t_attr;
		static MObject	g_in_effectors_attr;
		static MObject	g_t_attr;
		static MObject	g_amount_attr;
		static MObject	g_sigma_a_attr;
		static MObject	g_cnst_a_attr;
		static MObject	g_sigma_b_attr;
		static MObject	g_cnst_b_attr;
		static MObject	g_rot_compound_attr, g_rx_attr, g_ry_attr, g_rz_attr;
		static MObject	g_scale_attr;
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
		double						m_scale_t			= 1.0;

		inline double	compute_input_matrix_param( const std::vector<vufMath::vufMatrix_4d>& p_in_array);
		inline void		compute_out_array(	unsigned int p_xfrms_count, unsigned int p_eff_count,
											const std::vector<vufMath::vufMatrix_4d>& p_in_array, 
											std::vector<vufMath::vufMatrix_4d>& p_out_array);
		//inline void compute_out_effectors()
		inline vufIndex find_list_index(double p_t, unsigned int p_total_count) const;
	};
}
#endif // !VF_RM_MTRX_LST_VFK_H
