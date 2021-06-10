
#include <curves/locator/vufCurveLocator.h>
#include <data/vufMayaDataList.h>
#include <curves/vufCurve.h>
#include <curves/vufCurveContatiner.h>

#include <maya/MFnDependencyNode.h>
#include <maya/MFnTypedAttribute.h>
#include <maya/MHWGeometryUtilities.h>
#include <maya/MPointArray.h>
#include <maya/MGlobal.h>

using namespace vufRM;
using namespace vufMath;

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
MStatus vufCurveLocator::compute(const MPlug& p_plug, MDataBlock& p_data)
{
	return MPxLocatorNode::compute(p_plug, p_data);
}
bool	vufCurveLocator::isBounded() const
{
	return false;
}
MBoundingBox vufCurveLocator::boundingBox() const
{
	MBoundingBox l_bbox;
	return l_bbox;
}
void* vufCurveLocator::creator()
{
	return new vufCurveLocator();
}
MStatus vufCurveLocator::initialize()
{
	MStatus l_status;
	MFnTypedAttribute		l_typed_attr_fn;

	g_in_data_attr = l_typed_attr_fn.create("inputCurve", "ic", mpxCurveWrapper::g_id, MObject::kNullObj, &l_status);
	CHECK_MSTATUS_AND_RETURN_IT(l_status);
	l_typed_attr_fn.setWritable(true);
	l_typed_attr_fn.setStorable(true);
	l_typed_attr_fn.setHidden(false);
	l_typed_attr_fn.setKeyable(true);
	//l_typed_attr_fn.setDisconnectBehavior(MFnAttribute::kDelete);
	
	l_status = addAttribute(g_in_data_attr);	CHECK_MSTATUS_AND_RETURN_IT(l_status);
	return MS::kSuccess;
}
void vufCurveLocator::draw(	M3dView& p_view, const MDagPath& p_path,
							M3dView::DisplayStyle p_style,
							M3dView::DisplayStatus p_status)
{
	return MPxLocatorNode::draw(p_view, p_path, p_style, p_status);
}
#pragma endregion QUAT_CURVE_LOCATOR
#pragma region QUAT_CURVE_LOCATOR_OVERRIDE
//vufQuatLocatorDrawOverride::vufQuatLocatorDrawOverride(const MObject& obj): MHWRender::MPxDrawOverride(obj, vufQuatLocatorDrawOverride::draw)
vufCurveLocatorDrawOverride::vufCurveLocatorDrawOverride(const MObject& obj) : MHWRender::MPxDrawOverride(obj, nullptr, true)
{}
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
	return false;
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
	MObject l_input_data_obj;
	MPlug	p_data_plug( l_this_node, vufCurveLocator::g_in_data_attr);
	
	l_status = p_data_plug.getValue(l_input_data_obj);
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
	vufCurveLocatorData* l_locator_data_ptr = (vufCurveLocatorData*)p_data;
	if (l_locator_data_ptr->m_curve_container_ptr == nullptr)
	{
		return;
	}
	std::shared_ptr<vufMath::vufCurve_4d> l_curve_ptr = l_locator_data_ptr->m_curve_container_ptr->get_curve_ptr();
	if (l_curve_ptr == nullptr || l_curve_ptr->is_valid() == false)
	{
		return;
	}	
	//p_draw_manager.beginDrawable();
	p_draw_manager.beginDrawInXray();

	p_draw_manager.setColor(MColor(1.0, .0, .0));
	double	l_step = 1.0 / 100.;
	vufVector_4d	l_start = l_locator_data_ptr->m_curve_container_ptr->get_pos_at(0.0 );
	MPoint l_0(l_start.x, l_start.y, l_start.z);
	MPoint l_1;
	for (int i = 0; i < 100; ++i)
	{
		p_draw_manager.setColor(MColor( (float)i/100.f, 1.0f - (float)i / 100.0f, .0f));
		auto l_pos		= l_locator_data_ptr->m_curve_container_ptr->get_pos_at(l_step * ((double)i));
		auto l_tangent	= l_locator_data_ptr->m_curve_container_ptr->get_tangent_at(l_step * ((double)i));
		auto l_normal	= l_locator_data_ptr->m_curve_container_ptr->get_normal_at(l_step * ((double)i));
		auto l_quat = l_locator_data_ptr->m_curve_container_ptr->get_quaternion_at(l_step * ((double)i));
		l_tangent.normalize_in_place();
		l_normal.normalize_in_place();
		auto l_binormal = l_tangent.get_cross(l_normal);
		l_binormal.normalize_in_place();

		vufMatrix_4d l_matr = vufMatrix_4d();
		vufMatrix_4d l_matr_frame = vufMatrix_4d();
		l_matr_frame.set_axis_x(l_tangent);
		l_matr_frame.set_axis_y(l_normal);
		l_matr_frame.set_axis_z(l_binormal);
		//auto l_quat = l_matr.get_quaternion();
		auto l_quat_frame = l_matr_frame.get_quaternion();
		l_matr_frame.set_quaternion(l_quat_frame);
		l_tangent	= l_matr_frame.get_axis_x_4();
		l_normal	= l_matr_frame.get_axis_y_4();

		l_matr.set_quaternion(l_quat);
		auto l_t	= l_matr.get_axis_x_4();
		auto l_n	= l_matr.get_axis_y_4();
		l_1 = l_0;
		l_0.x = l_pos.x;
		l_0.y = l_pos.y;
		l_0.z = l_pos.z;

		p_draw_manager.sphere(MPoint(l_0.x, l_0.y, l_0.z), 0.05, true);
		p_draw_manager.line( l_0,l_1);
		p_draw_manager.sphere(MPoint(l_0.x, l_0.y, l_0.z), 0.05, true);

		p_draw_manager.setColor(MColor(1.0f, .0f, .0f));
		p_draw_manager.line(*((MPoint*)(&l_pos)), *((MPoint*)(&(l_pos + l_tangent))));
		p_draw_manager.setColor(MColor(0.0f, .0f, 1.0f));
		p_draw_manager.line(*((MPoint*)(&l_pos)), *((MPoint*)(&(l_pos + l_normal* 2))));
		p_draw_manager.setColor(MColor(0.0f, 1.0f, .0f));
		p_draw_manager.line(*((MPoint*)(&l_pos)), *((MPoint*)(&(l_pos + l_n))));
	}

	p_draw_manager.endDrawInXray();
	//p_draw_manager.endDrawable();
}
#pragma endregion QUAT_CURVE_LOCATOR_OVERRIDE