
#include <curves/locator/vufCurveLocator.h>
#include <data/vufMayaDataList.h>
#include <curves/vufCurve.h>
#include <curves/vufCurveContatiner.h>
#include <curves/vufCurvesInclude.h>
#include <vufMayaGlobalIncludes.h>

#include <maya/MFnDependencyNode.h>
#include <maya/MFnTypedAttribute.h>
#include <maya/MFnNumericAttribute.h>
#include <maya/MHWGeometryUtilities.h>
#include <maya/MPointArray.h>
#include <maya/MGlobal.h>

using namespace vufRM;
using namespace vufMath;

MObject vufCurveLocator::g_color_start_attr;
MObject vufCurveLocator::g_color_end_attr;
MObject vufCurveLocator::g_draw_start_attr;
MObject vufCurveLocator::g_draw_end_attr;
MObject vufCurveLocator::g_color_tngnt_attr;
MObject vufCurveLocator::g_color_nrml_attr;
MObject vufCurveLocator::g_draw_width_arrt;
MObject vufCurveLocator::g_draw_tangents_attr;
MObject vufCurveLocator::g_draw_normals_attr;
MObject vufCurveLocator::g_division_attr;
MObject	vufCurveLocator::g_in_data_attr;

#pragma region QUAT_CURVE_LOCATOR

vufCurveLocator::vufCurveLocator() {}
vufCurveLocator::~vufCurveLocator() {}
void vufCurveLocator::postConstructor()
{
	MObject l_this_obj = thisMObject();
	MFnDependencyNode fnNode(l_this_obj);
	fnNode.setName(g_type_name + "Shape#");
	MPxLocatorNode::postConstructor();
}
/*
MStatus vufCurveLocator::compute(const MPlug& p_plug, MDataBlock& p_data)
{
	return MPxLocatorNode::compute(p_plug, p_data);
}
*/
bool			vufCurveLocator::isBounded() const
{
	return false;
}
MBoundingBox	vufCurveLocator::boundingBox() const
{
	MBoundingBox l_bbox;
	return l_bbox;
}
MStatus			vufCurveLocator::connectionBroken(const MPlug& p_plug_1, const MPlug& p_plug_2, bool p_as_src)
{
	if (p_plug_1 == g_in_data_attr)
	{	
		MStatus l_status;
		MObject				l_data_obj;
		MFnDependencyNode	l_node(p_plug_1.node());
		MPlug				l_plug = l_node.findPlug(g_in_data_attr, true, &l_status);
		// When we call plug::getValue compute method will be implicity called
		// Is garantee that data hs been created by compute method
		// We can skip all checking but keep them just in case (who knows may be maya api will be changed)
		l_status = l_plug.getValue(l_data_obj);
		MFnPluginData l_pd_fn(l_data_obj);
		mpxCurveWrapper* l_mpx_data = (mpxCurveWrapper*)l_pd_fn.constData(&l_status);
		if (l_mpx_data == nullptr)
		{
			VF_LOG_ERR("vufCurveLocator Connection Broken unexeptable fail");
			return MPxLocatorNode::connectionBroken(p_plug_1, p_plug_2, p_as_src);
		}
		std::shared_ptr<vufCurveData> l_in_data = l_mpx_data->get_data();
		if (l_in_data == nullptr || l_in_data->m_internal_data == nullptr)
		{
			VF_LOG_ERR("vufCurveLocator Connection Broken unexeptable fail");
			return MPxLocatorNode::connectionBroken(p_plug_1, p_plug_2, p_as_src);
		}
		std::shared_ptr<vufCurveData> l_in_data_new = std::shared_ptr<vufCurveData>(new vufCurveData());
		vufCurveContainer_4d& l_container = *(l_in_data->m_internal_data.get());
		l_in_data_new->m_internal_data = l_container.get_copy();
		l_mpx_data->set_data(l_in_data_new);
		l_plug.setMObject(l_pd_fn.object());
	}
	return MPxLocatorNode::connectionBroken( p_plug_1, p_plug_2, p_as_src );
}
void*			vufCurveLocator::creator()
{
	return new vufCurveLocator();
}
MStatus			vufCurveLocator::initialize()
{
	MStatus l_status;
	MFnTypedAttribute		l_typed_attr_fn;
	MFnNumericAttribute		l_numeric_attr_fn;
	// start draw
	VF_RM_CREATE_STORABLE_NUMERIC_ATTR(g_draw_start_attr, "startDrawAt", "sda", kDouble, 0.0);
	l_numeric_attr_fn.setDefault(0.0);
	// end draw
	VF_RM_CREATE_STORABLE_NUMERIC_ATTR(g_draw_end_attr, "endDrawAt", "eda", kDouble, 0.0);
	l_numeric_attr_fn.setDefault(1.0);
	// start color
	g_color_start_attr = l_numeric_attr_fn.createColor("startColor", "sc", &l_status);
	CHECK_MSTATUS_AND_RETURN_IT(l_status);	
	CHECK_MSTATUS(l_numeric_attr_fn.setDefault(1.f,0.f,0.f));
	CHECK_MSTATUS(l_numeric_attr_fn.setStorable(true));
	// end color
	g_color_end_attr = l_numeric_attr_fn.createColor("endColor", "ec", &l_status);
	CHECK_MSTATUS_AND_RETURN_IT(l_status);
	CHECK_MSTATUS(l_numeric_attr_fn.setDefault(0.f, 1.f, 0.f));
	CHECK_MSTATUS(l_numeric_attr_fn.setStorable(true));
	// tangent color
	g_color_tngnt_attr = l_numeric_attr_fn.createColor("tangentColor", "tc", &l_status);
	CHECK_MSTATUS_AND_RETURN_IT(l_status);
	CHECK_MSTATUS(l_numeric_attr_fn.setDefault(1.f, 0.f, 0.f));
	CHECK_MSTATUS(l_numeric_attr_fn.setStorable(true));
	// normal color
	g_color_nrml_attr = l_numeric_attr_fn.createColor("noramlColor", "nc", &l_status);
	CHECK_MSTATUS_AND_RETURN_IT(l_status);
	CHECK_MSTATUS(l_numeric_attr_fn.setDefault(0.f, 1.f, 0.f));
	CHECK_MSTATUS(l_numeric_attr_fn.setStorable(true));
	//width
	VF_RM_CREATE_STORABLE_NUMERIC_ATTR(g_draw_width_arrt, "width", "width", kDouble, 1.0);
	l_numeric_attr_fn.setMin(0.1);
	l_numeric_attr_fn.setDefault(1.);
	// draw tangents
	VF_RM_CREATE_STORABLE_NUMERIC_ATTR(g_draw_tangents_attr, "tangents", "t", kBoolean, false);
	l_numeric_attr_fn.setDefault(1.0);
	// draw normals
	VF_RM_CREATE_STORABLE_NUMERIC_ATTR(g_draw_normals_attr, "normals", "n", kBoolean, false);
	l_numeric_attr_fn.setDefault(false);
	//division
	VF_RM_CREATE_STORABLE_NUMERIC_ATTR(g_division_attr, "division", "dvs", kInt, 50);
	l_numeric_attr_fn.setMin(1);
	l_numeric_attr_fn.setDefault(50);

	g_in_data_attr = l_typed_attr_fn.create("inCurve", "ic", mpxCurveWrapper::g_id, MObject::kNullObj, &l_status);
	CHECK_MSTATUS_AND_RETURN_IT(l_status);
	l_typed_attr_fn.setWritable(true);
	l_typed_attr_fn.setStorable(true);
	l_typed_attr_fn.setHidden(false);
	l_typed_attr_fn.setKeyable(true);
	//l_typed_attr_fn.setDisconnectBehavior(MFnAttribute::kDelete);
	
	// add atributes
	l_status = addAttribute(g_color_start_attr);	CHECK_MSTATUS_AND_RETURN_IT(l_status);
	l_status = addAttribute(g_color_end_attr);		CHECK_MSTATUS_AND_RETURN_IT(l_status);
	l_status = addAttribute(g_color_tngnt_attr);	CHECK_MSTATUS_AND_RETURN_IT(l_status);
	l_status = addAttribute(g_color_nrml_attr);		CHECK_MSTATUS_AND_RETURN_IT(l_status);
	l_status = addAttribute(g_draw_width_arrt);		CHECK_MSTATUS_AND_RETURN_IT(l_status);
	l_status = addAttribute(g_draw_start_attr);		CHECK_MSTATUS_AND_RETURN_IT(l_status);
	l_status = addAttribute(g_draw_end_attr);		CHECK_MSTATUS_AND_RETURN_IT(l_status);
	l_status = addAttribute(g_draw_tangents_attr);	CHECK_MSTATUS_AND_RETURN_IT(l_status);
	l_status = addAttribute(g_draw_normals_attr);	CHECK_MSTATUS_AND_RETURN_IT(l_status);
	l_status = addAttribute(g_division_attr);		CHECK_MSTATUS_AND_RETURN_IT(l_status);
	l_status = addAttribute(g_in_data_attr);		CHECK_MSTATUS_AND_RETURN_IT(l_status);

	return MS::kSuccess;
}
void			vufCurveLocator::draw(	M3dView& p_view, const MDagPath& p_path,
							M3dView::DisplayStyle p_style,
							M3dView::DisplayStatus p_status)
{
	return MPxLocatorNode::draw(p_view, p_path, p_style, p_status);
}
#pragma endregion QUAT_CURVE_LOCATOR
#pragma region QUAT_CURVE_LOCATOR_OVERRIDE
//vufQuatLocatorDrawOverride::vufQuatLocatorDrawOverride(const MObject& obj): MHWRender::MPxDrawOverride(obj, vufQuatLocatorDrawOverride::draw)
vufCurveLocatorDrawOverride::vufCurveLocatorDrawOverride(const MObject& obj) : MHWRender::MPxDrawOverride(obj, nullptr, true) {}
MHWRender::DrawAPI	vufCurveLocatorDrawOverride::supportedDrawAPIs() const
{
	return (MHWRender::kOpenGL | MHWRender::kDirectX11 | MHWRender::kOpenGLCoreProfile);
	// Try this later
	return MHWRender::kAllDevices;
}
bool				vufCurveLocatorDrawOverride::isBounded(const MDagPath& /*objPath*/, const MDagPath& /*cameraPath*/) const
{
	return false;
}
MBoundingBox		vufCurveLocatorDrawOverride::boundingBox(const MDagPath& p_obj_path, const MDagPath& p_camera_path) const
{
	return m_current_bounding_box;
}
bool				vufCurveLocatorDrawOverride::disableInternalBoundingBoxDraw() const
{
	return true;
}
MUserData*			vufCurveLocatorDrawOverride::prepareForDraw(	const MDagPath& p_obj_path,
																	const MDagPath& p_camera_path,
																	const MHWRender::MFrameContext& p_frame_context,
																	MUserData* p_old_data)
{
	if (p_old_data == nullptr)
	{
		p_old_data = new vufCurveLocatorData();
	}
	vufCurveLocatorData* l_locator_data_ptr = (vufCurveLocatorData*)p_old_data;
	
	// Read Attributes in data attribute from node
	MStatus l_status;
	MObject l_this_node = p_obj_path.node();
	// read data
	MObject l_input_data_obj;
	MPlug	l_data_plug(		l_this_node, vufCurveLocator::g_in_data_attr);

	l_status = l_data_plug.getValue(l_input_data_obj);
	if (l_status != MS::kSuccess)
	{
		return nullptr;
	}
	MFnPluginData	l_plugin_data_fn(l_input_data_obj);
	mpxCurveWrapper* l_in_data_wrap_ptr = (mpxCurveWrapper*)l_plugin_data_fn.constData(&l_status);
	if (l_in_data_wrap_ptr == nullptr)
	{
		l_locator_data_ptr->m_curve_container_ptr = nullptr;
		return nullptr;
	}
	auto l_data_ptr = l_in_data_wrap_ptr->get_data();
	l_locator_data_ptr->m_curve_container_ptr = l_data_ptr == nullptr ? nullptr: l_data_ptr->m_internal_data;

	MHWRender::DisplayStatus ds = MHWRender::MGeometryUtilities::displayStatus(p_obj_path);
	// read other attibutes
	MPlug	l_color_start_plug(	l_this_node, vufCurveLocator::g_color_start_attr);
	MPlug	l_color_end_plug(	l_this_node, vufCurveLocator::g_color_end_attr);
	MPlug	l_color_tngnt_plug( l_this_node, vufCurveLocator::g_color_tngnt_attr);
	MPlug	l_color_nrml_plug(	l_this_node, vufCurveLocator::g_color_nrml_attr);
	MPlug	l_width_plug(		l_this_node, vufCurveLocator::g_draw_width_arrt);
	MPlug	l_param_start_plug(	l_this_node, vufCurveLocator::g_draw_start_attr);
	MPlug	l_param_end_plug(	l_this_node, vufCurveLocator::g_draw_end_attr);
	MPlug	l_tangent_plug(		l_this_node, vufCurveLocator::g_draw_tangents_attr);
	MPlug	l_normal_plug(		l_this_node, vufCurveLocator::g_draw_normals_attr);
	MPlug	l_division_plug(	l_this_node, vufCurveLocator::g_division_attr);
	if (ds == MHWRender::DisplayStatus::kActive || ds == MHWRender::DisplayStatus::kLead)
	{
		l_locator_data_ptr->m_color_start.r = 1.0f;
		l_locator_data_ptr->m_color_start.g = 1.0f;
		l_locator_data_ptr->m_color_start.b = 1.0f;

		l_locator_data_ptr->m_color_end.r = 1.0f;
		l_locator_data_ptr->m_color_end.g = 1.0f;
		l_locator_data_ptr->m_color_end.b = 1.0f;

		l_locator_data_ptr->m_color_tngnt.r = 1.0f;
		l_locator_data_ptr->m_color_tngnt.g = 1.0f;
		l_locator_data_ptr->m_color_tngnt.b = 1.0f;

		l_locator_data_ptr->m_color_normal.r = 1.0f;
		l_locator_data_ptr->m_color_normal.g = 1.0f;
		l_locator_data_ptr->m_color_normal.b = 1.0f;
	}
	else
	{
		l_color_start_plug.child(0).getValue(l_locator_data_ptr->m_color_start.r);
		l_color_start_plug.child(1).getValue(l_locator_data_ptr->m_color_start.g);
		l_color_start_plug.child(2).getValue(l_locator_data_ptr->m_color_start.b);

		l_color_end_plug.child(0).getValue(l_locator_data_ptr->m_color_end.r);
		l_color_end_plug.child(1).getValue(l_locator_data_ptr->m_color_end.g);
		l_color_end_plug.child(2).getValue(l_locator_data_ptr->m_color_end.b);

		l_color_tngnt_plug.child(0).getValue(l_locator_data_ptr->m_color_tngnt.r);
		l_color_tngnt_plug.child(1).getValue(l_locator_data_ptr->m_color_tngnt.g);
		l_color_tngnt_plug.child(2).getValue(l_locator_data_ptr->m_color_tngnt.b);

		l_color_nrml_plug.child(0).getValue(l_locator_data_ptr->m_color_normal.r);
		l_color_nrml_plug.child(1).getValue(l_locator_data_ptr->m_color_normal.g);
		l_color_nrml_plug.child(2).getValue(l_locator_data_ptr->m_color_normal.b);
	}
	l_width_plug.getValue(		l_locator_data_ptr->m_width);
	l_param_start_plug.getValue(l_locator_data_ptr->m_start_param);
	l_param_end_plug.getValue(	l_locator_data_ptr->m_end_param);
	l_tangent_plug.getValue(	l_locator_data_ptr->m_tangent);
	l_normal_plug.getValue(		l_locator_data_ptr->m_normal);
	l_division_plug.getValue(	l_locator_data_ptr->m_division);

	return p_old_data;
}
// All draw routinies are here
void				vufCurveLocatorDrawOverride::addUIDrawables(	const MDagPath&					p_obj_path,
																	MHWRender::MUIDrawManager&		p_draw_manager,
																	const MHWRender::MFrameContext& p_frame_context,
																	const MUserData*				p_data)
{
	if (p_data == nullptr)
	{
		return;
	}
	vufCurveLocatorData* l_data = (vufCurveLocatorData*)p_data;
	if (l_data->m_curve_container_ptr == nullptr)
	{
		return;
	}
	p_draw_manager.beginDrawable();
	p_draw_manager.beginDrawInXray();
	p_draw_manager.setDepthPriority(5);

	p_draw_manager.setLineWidth((float)(l_data->m_width));
	double	l_step = (l_data->m_end_param - l_data->m_start_param) / (double)l_data->m_division;
	vufVector_4d	l_start = l_data->m_curve_container_ptr->get_pos_at(0.0 );
	MPoint l_0(l_start.x, l_start.y, l_start.z);
	MPoint l_1;
	for (int i = 0; i <= l_data->m_division; ++i)
	{
		auto l_pos		= l_data->m_curve_container_ptr->get_pos_at(		l_data->m_start_param + l_step * ((double)i));
		//auto l_tangent	= l_data->m_curve_container_ptr->get_tangent_at(	l_data->m_start_param + l_step * ((double)i));
		//auto l_normal = l_data->m_curve_container_ptr->get_normal_at(l_data->m_start_param + l_step * ((double)i));
		//auto l_quat		= l_data->m_curve_container_ptr->get_quaternion_at(	l_data->m_start_param + l_step * ((double)i));
		
		l_1 = l_0;
		l_0.x = l_pos.x;
		l_0.y = l_pos.y;
		l_0.z = l_pos.z;
		float l_w_1 = (float)i / (float)l_data->m_division;
		float l_w_2 = 1.0f - (float)i / (float)l_data->m_division;
		p_draw_manager.setColor(l_data->m_color_start * l_w_1 + l_data->m_color_end * l_w_2);
		p_draw_manager.sphere(MPoint(l_0.x, l_0.y, l_0.z), l_data->m_width * 0.05, true);
		p_draw_manager.line( l_0,l_1);
		//p_draw_manager.sphere(MPoint(l_0.x, l_0.y, l_0.z), 0.05, true);
	}
	if (l_data->m_tangent == true)
	{
		p_draw_manager.setColor(l_data->m_color_tngnt);
		for (int i = 0; i <= l_data->m_division; ++i)
		{
			auto l_pos = l_data->m_curve_container_ptr->get_pos_at(			l_data->m_start_param + l_step * ((double)i));
			auto l_tangent = l_data->m_curve_container_ptr->get_tangent_at(	l_data->m_start_param + l_step * ((double)i));
			l_tangent.normalize_in_place();			
			p_draw_manager.line(*((MPoint*)(&l_pos)), *((MPoint*)(&(l_pos + l_tangent))));
		}
	}
	if (l_data->m_normal == true)
	{
		p_draw_manager.setColor(l_data->m_color_normal);
		for (int i = 0; i <= l_data->m_division; ++i)
		{
			auto l_pos	= l_data->m_curve_container_ptr->get_pos_at(		l_data->m_start_param + l_step * ((double)i));
			auto l_quat = l_data->m_curve_container_ptr->get_quaternion_at(	l_data->m_start_param + l_step * ((double)i));
			vufMatrix_4d l_matr = vufMatrix_4d();
			l_matr.set_quaternion(l_quat);
			auto l_n = l_matr.get_axis_y_4();			
			p_draw_manager.line(*((MPoint*)(&l_pos)), *((MPoint*)(&(l_pos + l_n))));
		}
	}
	
	p_draw_manager.endDrawInXray();
	p_draw_manager.endDrawable();
}
#pragma endregion QUAT_CURVE_LOCATOR_OVERRIDE