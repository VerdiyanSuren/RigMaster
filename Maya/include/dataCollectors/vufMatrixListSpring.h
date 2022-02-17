#ifndef VF_RM_SPRING_NODE_H
#define VF_RM_SPRING_NODE_H

#include <maya/MPxNode.h>

namespace vufRM
{
	class vufMatrixListSpringNode : public MPxNode
	{
	public:
		vufMatrixListSpringNode();
		virtual ~vufMatrixListSpringNode() {}

		static  void* creator();
		static  MStatus	initialize();
		virtual void 	postConstructor() override;
		virtual MStatus	compute(const MPlug& plug, MDataBlock& data) override;
		virtual MStatus	connectionBroken(const MPlug& p_plug_1, const MPlug& p_plug_2, bool p_as_src) override;
		static const MTypeId	g_id;
		static const MString	g_type_name;
		//attributes
		static MObject	g_data_in_deformed_attr;
		static MObject	g_data_in_rest_attr;

		static MObject	g_in_cache_mode_attr;
		static MObject	g_in_time_attr;
		static MObject	g_in_enable_attr;
		static MObject	g_in_init_frame_attr;
		
		static MObject	g_in_translation_compound_attr;
		static MObject	g_in_translation_iteration_attr;
		static MObject	g_in_translation_stiffness_attr;
		static MObject	g_in_translation_stiffness_ramp_attr;
		static MObject	g_in_translation_mass_attr;
		static MObject	g_in_translation_mass_ramp_attr;
		static MObject	g_in_translation_damp_attr;
		static MObject	g_in_translation_damp_ramp_attr;
		static MObject	g_in_translation_amplitude_attr;
		static MObject	g_in_translation_amplitude_ramp_attr;

		static MObject	g_in_rotation_compound_attr;
		static MObject	g_in_rotation_inherit_attr;
		static MObject	g_in_rotation_inherit_ramp_attr;
		static MObject	g_in_rotation_iteration_attr;
		static MObject	g_in_rotation_stiffness_attr;
		static MObject	g_in_rotation_stiffness_ramp_attr;
		static MObject	g_in_rotation_mass_attr;
		static MObject	g_in_rotation_mass_ramp_attr;
		static MObject	g_in_rotation_damp_attr;
		static MObject	g_in_rotation_damp_ramp_attr;
		static MObject	g_in_rotation_amplitude_attr;
		static MObject	g_in_rotation_amplitude_ramp_attr;

		static MObject	g_out_data_attr;
		static MObject	g_data_cache_attr;//cache

	private:
		bool			m_locked = true;
		uint64_t		m_gen_id;
	};
}
#endif // !VF_RM_SPRING_NODE_H
