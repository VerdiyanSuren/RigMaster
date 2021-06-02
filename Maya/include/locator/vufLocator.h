#ifndef VF_RM_LCTR_H
#define	VF_RM_LCTR_H

#include <maya/MPxLocatorNode.h>
#include <maya/MDrawRegistry.h>
#include <maya/MPxDrawOverride.h>
#include <maya/MUserData.h>
/**
* Register routine
* Your locator class has to have  
*   static const MString g_type_name;
*   static const MString g_drawDb_classification;// "drawdb/geometry/cvColorDrawOverride"
*   static const MString g_draw_registrant_id("cvColorPlugin");
*/
#define VF_RM_REGISTER_LOCATOR(LOCATOR_CLASS, PLUGIN_VAR, MSTATUS_VAR)	\
MSTATUS_VAR = PLUGIN_VAR.registerNode( LOCATOR_CLASS::g_type_name, LOCATOR_CLASS::g_id, &LOCATOR_CLASS::creator, &LOCATOR_CLASS::initialize,MPxNode::kLocatorNode, &LOCATOR_CLASS::g_drawDb_classification); \
CHECK_MSTATUS_AND_RETURN_IT(MSTATUS_VAR);	\
MSTATUS_VAR = MHWRender::MDrawRegistry::registerDrawOverrideCreator(LOCATOR_CLASS::g_drawDb_classification, g_draw_registrant_id, LOCATOR_CLASS##DrawOverride::creator); \
CHECK_MSTATUS_AND_RETURN_IT(MSTATUS_VAR);	\

#define VF_RM_DEREGISTER_LOCATOR(LOCATOR_CLASS, PLUGIN_VAR, MSTATUS_VAR)	\
MSTATUS_VAR = PLUGIN_VAR.deregisterNode( LOCATOR_CLASS::g_id );				\
CHECK_MSTATUS_AND_RETURN_IT(MSTATUS_VAR);									\
MSTATUS_VAR = MHWRender::MDrawRegistry::deregisterDrawOverrideCreator(LOCATOR_CLASS::g_drawDb_classification, g_draw_registrant_id); \
CHECK_MSTATUS_AND_RETURN_IT(MSTATUS_VAR);									\

#define VF_RM_GENERATE_LOCATOR_BODY(LOCATOR_CLASS)									\
	LOCATOR_CLASS();																\
	virtual ~##LOCATOR_CLASS();														\
	virtual void postConstructor() override;										\
	virtual MStatus   		compute(const MPlug& plug, MDataBlock& data) override;	\
	virtual void            draw(	M3dView& view, const MDagPath& path,			\
									M3dView::DisplayStyle style,					\
									M3dView::DisplayStatus status) override;		\
	virtual bool            isBounded()		const override;							\
	virtual MBoundingBox    boundingBox()	const override;							\
	static  void* creator();														\
	static  MStatus         initialize();											\
																					\
	static	MTypeId			g_id;													\
	static const MString    g_type_name;											\
	static  MString			g_drawDb_classification;								\

#endif // ! VF_RM_LCTR_H
