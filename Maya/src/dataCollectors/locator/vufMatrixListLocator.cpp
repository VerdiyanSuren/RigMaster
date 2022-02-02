#include <data/vufMayaDataList.h>
#include <dataCollectors/locator/vufMatrixListLocator.h>

#include <dataCollectors/vufMatrixListNullNode.h>
#include <vufMayaGlobalIncludes.h>
#include <math/vufMatrix.h>

#include <maya/MFnDependencyNode.h>
#include <maya/MFnTypedAttribute.h>
#include <maya/MFnNumericAttribute.h>
#include <maya/MHWGeometryUtilities.h>
#include <maya/MPointArray.h>
#include <maya/MGlobal.h>

using namespace vufRM;
using namespace vufMath;


MObject vufMatrixListLocator::g_draw_attr;
MObject vufMatrixListLocator::g_dot_attr;
MObject vufMatrixListLocator::g_tripod_attr;
MObject vufMatrixListLocator::g_size_attr;
MObject vufMatrixListLocator::g_connect_attr;
MObject vufMatrixListLocator::g_draw_width_attr;
MObject vufMatrixListLocator::g_data_in_attr;

#pragma region MATR_LIST_LOCATOR
vufMatrixListLocator::vufMatrixListLocator():MPxLocatorNode(){}
vufMatrixListLocator::~vufMatrixListLocator(){}
void			vufMatrixListLocator::postConstructor()
{
	MObject l_this_obj = thisMObject();
	MFnDependencyNode fnNode(l_this_obj);
	fnNode.setName(g_type_name + "Shape#");
	MPxLocatorNode::postConstructor();
}
bool			vufMatrixListLocator::isBounded() const
{
	return false;
}
MBoundingBox	vufMatrixListLocator::boundingBox() const
{
	MBoundingBox l_bbox;
	return l_bbox;
}
MStatus			vufMatrixListLocator::connectionBroken(const MPlug& p_plug_1, const MPlug& p_plug_2, bool p_as_src)
{
	if (p_plug_1 == g_data_in_attr)
	{
		MStatus l_status;
		MObject				l_data_obj;
		MFnDependencyNode	l_node(p_plug_1.node());
		std::shared_ptr<vufMatrixListData>	l_in_data;
		VF_RM_GET_DATA_FROM_PLUG(mpxMatrixListWrapper, vufMatrixListData, g_data_in_attr, l_in_data);
		l_in_data->m_internal_data = l_in_data->m_internal_data->get_copy();
		// When we call plug::getValue compute method will be implicity called
		// Is garantee that data hs been created by compute method
		// We can skip all checking but keep them just in case (who knows may be maya api will be changed)
	}
	return MPxLocatorNode::connectionBroken(p_plug_1, p_plug_2, p_as_src);
}
void*			vufMatrixListLocator::creator()
{
	return new vufMatrixListLocator();
}
MStatus			vufMatrixListLocator::initialize()
{
	MStatus l_status;
	MFnTypedAttribute		l_typed_attr_fn;
	MFnNumericAttribute		l_numeric_attr_fn;
	VF_RM_CREATE_STORABLE_NUMERIC_ATTR(g_draw_attr,		"draw",		"dr",	kBoolean,	true);	
	VF_RM_CREATE_STORABLE_NUMERIC_ATTR(g_dot_attr,		"dot",		"dot",	kBoolean,	false);
	VF_RM_CREATE_STORABLE_NUMERIC_ATTR(g_tripod_attr,	"tripod",	"trp",	kBoolean,	true);
	VF_RM_CREATE_STORABLE_NUMERIC_ATTR(g_connect_attr,	"line",		"ln",	kBoolean, true);
	VF_RM_CREATE_STORABLE_NUMERIC_ATTR(g_size_attr,		"dotSize",	"dsz",	kDouble, 1.0);
	l_numeric_attr_fn.setDefault(1.0);
	l_numeric_attr_fn.setMin(0.000001);
	VF_RM_CREATE_STORABLE_NUMERIC_ATTR(g_draw_width_attr,	"lineWidth", "lw", kDouble, 1.0);
	l_numeric_attr_fn.setDefault(1.0);
	l_numeric_attr_fn.setMin(0.000001);
	g_data_in_attr = l_typed_attr_fn.create("inMatrixList", "iml", mpxMatrixListWrapper::g_id, MObject::kNullObj, &l_status);
	CHECK_MSTATUS_AND_RETURN_IT(l_status);
	l_typed_attr_fn.setWritable(true);
	l_typed_attr_fn.setStorable(true);
	l_typed_attr_fn.setHidden(false);
	l_typed_attr_fn.setKeyable(true);

	// Add Attributes
	l_status = addAttribute(g_draw_attr);		CHECK_MSTATUS_AND_RETURN_IT(l_status);
	l_status = addAttribute(g_dot_attr);		CHECK_MSTATUS_AND_RETURN_IT(l_status);
	l_status = addAttribute(g_tripod_attr);		CHECK_MSTATUS_AND_RETURN_IT(l_status);
	l_status = addAttribute(g_connect_attr);	CHECK_MSTATUS_AND_RETURN_IT(l_status);
	l_status = addAttribute(g_draw_width_attr);	CHECK_MSTATUS_AND_RETURN_IT(l_status);
	l_status = addAttribute(g_data_in_attr);	CHECK_MSTATUS_AND_RETURN_IT(l_status);

	return MS::kSuccess;
}
void			vufMatrixListLocator::draw(	M3dView& p_view, const MDagPath& p_path,
											M3dView::DisplayStyle p_style,
											M3dView::DisplayStatus p_status)
{
	return MPxLocatorNode::draw(p_view, p_path, p_style, p_status);
}

#pragma endregion MATR_LIST_LOCATOR
#pragma region MATR_LIST_LOCATOR_OVERRIDE
vufMatrixListLocatorDrawOverride::vufMatrixListLocatorDrawOverride(const MObject& obj) : MHWRender::MPxDrawOverride(obj, nullptr, true) {}
MHWRender::DrawAPI	vufMatrixListLocatorDrawOverride::supportedDrawAPIs() const
{
	return (MHWRender::kOpenGL | MHWRender::kDirectX11 | MHWRender::kOpenGLCoreProfile);
	// Try this later
	return MHWRender::kAllDevices;
}
bool				vufMatrixListLocatorDrawOverride::isBounded(const MDagPath& /*objPath*/, const MDagPath& /*cameraPath*/) const
{
	return false;
}
MBoundingBox		vufMatrixListLocatorDrawOverride::boundingBox(const MDagPath& p_obj_path, const MDagPath& p_camera_path) const
{
	return MBoundingBox();
}
bool				vufMatrixListLocatorDrawOverride::disableInternalBoundingBoxDraw() const
{
	return true;
}
MUserData*			vufMatrixListLocatorDrawOverride::prepareForDraw(	const MDagPath& p_obj_path,
																		const MDagPath& p_camera_path,
																		const MHWRender::MFrameContext& p_frame_context,
																		MUserData* p_old_data)
{
	if (p_old_data == nullptr)
	{
		p_old_data = new vufMatrixListLocatorData();
	}
	vufMatrixListLocatorData* l_locator_data_ptr = (vufMatrixListLocatorData*)p_old_data;

	// Read Attributes in data attribute from node
	MStatus l_status;
	MObject l_this_node = p_obj_path.node();
	// read data
	MObject l_input_data_obj;
	MPlug	l_data_plug(l_this_node, vufMatrixListLocator::g_data_in_attr);

	l_status = l_data_plug.getValue(l_input_data_obj);
	if (l_status != MS::kSuccess)
	{
		return nullptr;
	}
	MFnPluginData	l_plugin_data_fn(l_input_data_obj);
	mpxMatrixListWrapper* l_in_data_wrap_ptr = (mpxMatrixListWrapper*)l_plugin_data_fn.constData(&l_status);
	if (l_in_data_wrap_ptr == nullptr)
	{
		l_locator_data_ptr->m_data_ptr = nullptr;
		return nullptr;
	}
	auto l_data_ptr = l_in_data_wrap_ptr->get_data();
	l_locator_data_ptr->m_data_ptr = l_data_ptr == nullptr ? nullptr : l_data_ptr->m_internal_data;

	MHWRender::DisplayStatus ds = MHWRender::MGeometryUtilities::displayStatus(p_obj_path);
	// read other attibutes
	MPlug	l_draw_plug(l_this_node,		vufMatrixListLocator::g_draw_attr);
	MPlug	l_draw_dot_plug(l_this_node,	vufMatrixListLocator::g_dot_attr);
	MPlug	l_draw_tripod_plug(l_this_node, vufMatrixListLocator::g_tripod_attr);
	MPlug	l_dot_size_plug(l_this_node,	vufMatrixListLocator::g_size_attr);
MPlug	l_draw_lines_plug(l_this_node, vufMatrixListLocator::g_connect_attr);
MPlug	l_line_width_plug(l_this_node, vufMatrixListLocator::g_draw_width_attr);

if (ds == MHWRender::DisplayStatus::kActive || ds == MHWRender::DisplayStatus::kLead)
{
	// selected all white
	l_locator_data_ptr->m_color_dot.r = 1.0f;
	l_locator_data_ptr->m_color_dot.g = 1.0f;
	l_locator_data_ptr->m_color_dot.b = 1.0f;

	l_locator_data_ptr->m_color_tripod_x.r = 1.0f;
	l_locator_data_ptr->m_color_tripod_x.g = 1.0f;
	l_locator_data_ptr->m_color_tripod_x.b = 1.0f;

	l_locator_data_ptr->m_color_tripod_y.r = 1.0f;
	l_locator_data_ptr->m_color_tripod_y.g = 1.0f;
	l_locator_data_ptr->m_color_tripod_y.b = 1.0f;

	l_locator_data_ptr->m_color_tripod_z.r = 1.0f;
	l_locator_data_ptr->m_color_tripod_z.g = 1.0f;
	l_locator_data_ptr->m_color_tripod_z.b = 1.0f;

	l_locator_data_ptr->m_color_line.r = 1.0f;
	l_locator_data_ptr->m_color_line.g = 1.0f;
	l_locator_data_ptr->m_color_line.b = 1.0f;
}
else
{
	l_locator_data_ptr->m_color_dot.r = 1.0f;
	l_locator_data_ptr->m_color_dot.g = 1.0f;
	l_locator_data_ptr->m_color_dot.b = 0.0f;

	l_locator_data_ptr->m_color_tripod_x.r = 1.0f;
	l_locator_data_ptr->m_color_tripod_x.g = 0.0f;
	l_locator_data_ptr->m_color_tripod_x.b = 0.0f;

	l_locator_data_ptr->m_color_tripod_y.r = 0.0f;
	l_locator_data_ptr->m_color_tripod_y.g = 1.0f;
	l_locator_data_ptr->m_color_tripod_y.b = 0.0f;

	l_locator_data_ptr->m_color_tripod_z.r = 0.0f;
	l_locator_data_ptr->m_color_tripod_z.g = 0.0f;
	l_locator_data_ptr->m_color_tripod_z.b = 1.0f;

	l_locator_data_ptr->m_color_line.r = 0.0f;
	l_locator_data_ptr->m_color_line.g = 0.6f;
	l_locator_data_ptr->m_color_line.b = 0.0f;
}

l_draw_plug.getValue(l_locator_data_ptr->m_draw);
l_draw_dot_plug.getValue(l_locator_data_ptr->m_draw_dot);
l_draw_tripod_plug.getValue(l_locator_data_ptr->m_draw_tripods);
l_dot_size_plug.getValue(l_locator_data_ptr->m_dot_size);
l_draw_lines_plug.getValue(l_locator_data_ptr->m_draw_connections);
l_line_width_plug.getValue(l_locator_data_ptr->m_line_width);

return p_old_data;
}
void				vufMatrixListLocatorDrawOverride::addUIDrawables(const MDagPath& p_obj_path,
	MHWRender::MUIDrawManager& p_draw_manager,
	const MHWRender::MFrameContext& p_frame_context,
	const MUserData* p_data)
{
	if (p_data == nullptr)
	{
		return;
	}
	vufMatrixListLocatorData* l_data = (vufMatrixListLocatorData*)p_data;
	if (l_data->m_data_ptr == nullptr || l_data->m_draw == true)
	{
		return;
	}
	p_draw_manager.beginDrawable();
	p_draw_manager.beginDrawInXray();
	p_draw_manager.setDepthPriority(5);

	auto& l_array = l_data->m_data_ptr->m_array_v;
	int l_sz = (int)l_array.size();
	if (l_sz == 0)
	{
		return;
	}
	// lines
	if (l_data->m_draw_connections == true && l_sz > 1)
	{
		p_draw_manager.setColor(l_data->m_color_line);
		p_draw_manager.setLineWidth((float)(l_data->m_line_width));
		auto l_pos = l_array[0].get_translation_4();
		for (int i = 1; i < l_sz; ++i)
		{
			auto l_pos_2 = l_array[i].get_translation_4();
			p_draw_manager.line(*((MPoint*)(&l_pos)), *((MPoint*)(&(l_pos_2))));
			l_pos = l_pos_2;
		}
	}
	if (l_data->m_draw_tripods == true)
	{
		p_draw_manager.setLineWidth((float)(l_data->m_line_width));
		for (int i = 0; i < l_sz; ++i)
		{
			auto l_pos	= l_array[i].get_translation_4();
			auto l_x	= l_array[i].get_axis_x_4() * l_data->m_dot_size; 
			auto l_y	= l_array[i].get_axis_y_4() * l_data->m_dot_size;
			auto l_z	= l_array[i].get_axis_z_4() * l_data->m_dot_size;
			p_draw_manager.setColor(l_data->m_color_tripod_x);
			p_draw_manager.line(*((MPoint*)(&l_pos)), *((MPoint*)(&(l_x))));
			p_draw_manager.setColor(l_data->m_color_tripod_y);
			p_draw_manager.line(*((MPoint*)(&l_pos)), *((MPoint*)(&(l_y))));
			p_draw_manager.setColor(l_data->m_color_tripod_z);
			p_draw_manager.line(*((MPoint*)(&l_pos)), *((MPoint*)(&(l_z))));
		}
	}
	if (l_data->m_draw_dot == true)
	{
		p_draw_manager.setColor(l_data->m_color_line);
		p_draw_manager.setPointSize((float)(l_data->m_dot_size));
		for (int i = 0; i < l_sz; ++i)
		{
			auto l_pos = l_array[i].get_translation_4();
			p_draw_manager.point(*((MPoint*)(&l_pos)));
		}
	}
	p_draw_manager.endDrawInXray();
	p_draw_manager.endDrawable();
}

#pragma endregion