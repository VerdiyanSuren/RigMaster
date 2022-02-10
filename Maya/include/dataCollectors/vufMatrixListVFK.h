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
			enum
			{
				k_local,
				k_world
			};
			int m_t_space = k_local;
			int m_r_space = k_local;
			double m_t;
			double m_amount;
			double m_sigma_a;
			double m_sigma_b;
			double m_tx, m_ty, m_tz;
			double m_rx, m_ry, m_rz;
			double m_sx, m_sy, m_sz;

			vufMath::vufQuaternion_d	m_quat			= vufMath::vufQuaternion_d();
			vufMath::vufVector_4d		m_scale			= vufMath::vufVector_4d();
			vufMath::vufVector_4d		m_translate_l	= vufMath::vufVector_4d();
			vufMath::vufVector_4d		m_translate_w	= vufMath::vufVector_4d();
			
			vufMath::vufVector_4d		m_scale_dir_x = vufMath::vufVector_4d();
			vufMath::vufVector_4d		m_scale_dir_y = vufMath::vufVector_4d();
			vufMath::vufVector_4d		m_scale_dir_z = vufMath::vufVector_4d();
			
		};
		struct vufInputList
		{
			double m_l;
			double m_t;
			vufMath::vufVector_4d m_dir_x;
			vufMath::vufVector_4d m_dir_y;
			vufMath::vufVector_4d m_dir_z;

			//vufMath::vufVector_4d		m_dir_to_next;
			vufMath::vufVector_4d  m_coord_l;
			//vufMath::vufVector_4d		m_scale;
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

		static MObject	g_in_effectors_attr;
		static MObject	g_t_attr;
		static MObject	g_amount_attr;
		static MObject	g_sigma_a_attr;
		static MObject	g_sigma_b_attr;
		static MObject	g_translate_attr;
		static MObject	g_rot_compound_attr, g_rx_attr, g_ry_attr, g_rz_attr;
		static MObject	g_scale_attr;
		static MObject	g_space_t_attr;
		static MObject	g_space_r_attr;
		
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

		inline double compute_input_matrix_param( const std::vector<vufMath::vufMatrix_4d>& p_in_array);
		inline void compute_srt_for_effectors(const std::vector<vufMath::vufMatrix_4d>& p_in_array);
		inline void compute_out_array(	unsigned int p_xfrms_count, unsigned int p_eff_count,
										const std::vector<vufMath::vufMatrix_4d>& p_in_array, 
										std::vector<vufMath::vufMatrix_4d>& p_out_array);
		inline vufIndex find_list_index(double p_t, unsigned int p_total_count) const;
	};
}
#endif // !VF_RM_MTRX_LST_VFK_H
