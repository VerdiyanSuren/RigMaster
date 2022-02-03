#ifndef VF_RM_CRV_BEZIER_TNGNTS_H
#define VF_RM_CRV_BEZIER_TNGNTS_H

#include <maya/MPxNode.h>
#include <maya/MMatrix.h>
#include <vector>

namespace vufRM
{	
	class vufCurveBezier3Tangents :public MPxNode
	{
	private:
		struct vufKnotInfo
		{
			short	m_mode;
			double	m_chord_length;
			MMatrix m_tangent_a;
			MMatrix m_tangent_b;
			MMatrix m_knot;
		};
		std::vector<vufKnotInfo> m_knots_array;
		inline void auto_tangent(const vufKnotInfo& p_prev, const vufKnotInfo& p_next, vufKnotInfo& p_current);
	public:
		enum
		{
			k_pass,
			k_auto,
			k_simple_chord,
			k_arc_arc
		};
		vufCurveBezier3Tangents():MPxNode(){}
		virtual ~vufCurveBezier3Tangents() {}
		static  void* creator();
		static  MStatus	initialize();
		virtual MStatus	compute(const MPlug& plug, MDataBlock& data) override;


		static const MTypeId	g_id;
		static const MString	g_type_name;

		static MObject g_closed_attr;
		static MObject g_inputs_group_attr;
		static MObject g_cntrl_xform_attr;

		static MObject g_mode_attr;
		static MObject g_chord_part_attr;

		static MObject g_out_group_attr;
		static MObject g_out_knot_attr;
		static MObject g_out_tang_a_attr;
		static MObject g_out_tang_b_attr;

		//static MObject ParentXFormAttr;
	};
}

#endif // !VF_RM_CRV_BEZIER_TNGNTS_H
