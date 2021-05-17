#include <maya/MGlobal.h>
#include <maya/MFnTypedAttribute.h>
#include <maya/MFnNumericAttribute.h>
#include <maya/MFnCompoundAttribute.h>
#include <maya/MFnUnitAttribute.h>
#include <maya/MFnMatrixAttribute.h>
#include <maya/MArrayDataHandle.h>
#include <maya/MArrayDataBuilder.h>
#include <maya/MTime.h>
#include <maya/MAngle.h>
#include <maya/MVector.h>

#include <expressions/nodes/vufMayaLuaExpressionNode.h>
#include <data/vufMayaDataList.h>
#include <vufLuaWrapper.h>
#include <utils/vufMayaUtils.h>
#include <vufLuaVector4.h>
#include <vufLuaQuaternion.h>
#include <vufLuaMatrix4.h>
#include <expressions/luaWrappers/vufLuaMPoint.h>
#include <expressions/luaWrappers/vufLuaMVector.h>
#include <expressions/luaWrappers/vufLuaMMatrix.h>
#include <expressions/luaWrappers/vufLuaMQuaternion.h>
#include <expressions/luaWrappers/vufLuaMVectorArray.h>
#include <expressions/luaWrappers/vufLuaMPointArray.h>

using namespace vuf;
using namespace vufRM;


MObject	vufMayaLuaExpressionNode::g_script_attr;
MObject	vufMayaLuaExpressionNode::g_ports_script_attr;

MObject	vufMayaLuaExpressionNode::g_input_ports_attr;
MObject	vufMayaLuaExpressionNode::g_input_time_attr;
MObject	vufMayaLuaExpressionNode::g_input_number_attr;
MObject	vufMayaLuaExpressionNode::g_input_angle_attr;
MObject	vufMayaLuaExpressionNode::g_input_vector_attr;

MObject	vufMayaLuaExpressionNode::g_input_matrix_attr;
MObject	vufMayaLuaExpressionNode::g_input_mesh_attr;
MObject	vufMayaLuaExpressionNode::g_input_curve_attr;
MObject	vufMayaLuaExpressionNode::g_input_surface_attr;

MObject	vufMayaLuaExpressionNode::g_output_ports_attr;
MObject	vufMayaLuaExpressionNode::g_output_number_attr;
MObject	vufMayaLuaExpressionNode::g_output_angle_attr;
MObject	vufMayaLuaExpressionNode::g_output_matrix_attr;

vufMayaLuaExpressionNode::vufMayaLuaExpressionNode()
{
	//m_lua_machine.open_machine();
}
vufMayaLuaExpressionNode::~vufMayaLuaExpressionNode()
{
	//m_lua_machine.close_machine();
}
void* vufMayaLuaExpressionNode::creator()
{
	return new vufMayaLuaExpressionNode();
}
MStatus	vufMayaLuaExpressionNode::initialize()
{
	MStatus l_status;
	MFnTypedAttribute		l_typed_attr_fn;
	MFnNumericAttribute		l_numeric_attr_fn;
	MFnMatrixAttribute		l_matrix_attr_fn;
	MFnCompoundAttribute	l_compound_attr_fn;
	MFnUnitAttribute		l_unit_attr_fn;

	// Ports definition expression
	g_ports_script_attr = l_typed_attr_fn.create("inPortScript", "ips", mpxMayaLuaPortWrapper::g_id, MObject::kNullObj, &l_status);
	CHECK_MSTATUS_AND_RETURN_IT(l_status);
	CHECK_MSTATUS(l_typed_attr_fn.setStorable(true));
	CHECK_MSTATUS(l_typed_attr_fn.setWritable(true));
	CHECK_MSTATUS(l_typed_attr_fn.setReadable(false));
	CHECK_MSTATUS(l_typed_attr_fn.setKeyable(true));

	//Script 
	g_script_attr = l_typed_attr_fn.create("script","scr", mpxMayaLuaTxtWrapper::g_id, MObject::kNullObj, &l_status);
	CHECK_MSTATUS_AND_RETURN_IT(l_status);
	CHECK_MSTATUS(l_typed_attr_fn.setStorable(true));
	CHECK_MSTATUS(l_typed_attr_fn.setWritable(true));
	CHECK_MSTATUS(l_typed_attr_fn.setReadable(false));
	CHECK_MSTATUS(l_typed_attr_fn.setKeyable(true));
#pragma region INPUT_PORTS
	//Time
	g_input_time_attr = l_unit_attr_fn.create("inTime", "int", MFnUnitAttribute::kTime, 0.0, &l_status);
	CHECK_MSTATUS_AND_RETURN_IT(l_status);
	CHECK_MSTATUS(l_unit_attr_fn.setWritable(true));
	CHECK_MSTATUS(l_unit_attr_fn.setArray(true));
	CHECK_MSTATUS(l_unit_attr_fn.setUsesArrayDataBuilder(true));
	CHECK_MSTATUS(l_unit_attr_fn.setDisconnectBehavior(MFnAttribute::kDelete));
	// Numbers
	g_input_number_attr = l_numeric_attr_fn.create("inNumber", "inn", MFnNumericData::kDouble, 0.0, &l_status);
	CHECK_MSTATUS_AND_RETURN_IT(l_status);
	CHECK_MSTATUS(l_numeric_attr_fn.setWritable(true));
	CHECK_MSTATUS(l_numeric_attr_fn.setArray(true));
	CHECK_MSTATUS(l_numeric_attr_fn.setUsesArrayDataBuilder(true));
	CHECK_MSTATUS(l_numeric_attr_fn.setDisconnectBehavior(MFnAttribute::kDelete));
	// Angles
	g_input_angle_attr = l_unit_attr_fn.create("inAngle", "ina", MFnUnitAttribute::kAngle, 0.0, &l_status);
	CHECK_MSTATUS_AND_RETURN_IT(l_status);
	CHECK_MSTATUS(l_unit_attr_fn.setWritable(true));
	CHECK_MSTATUS(l_unit_attr_fn.setArray(true));
	CHECK_MSTATUS(l_unit_attr_fn.setUsesArrayDataBuilder(true));
	CHECK_MSTATUS(l_unit_attr_fn.setDisconnectBehavior(MFnAttribute::kDelete));
	// Vector
	g_input_vector_attr = l_numeric_attr_fn.create("inVector", "inv", MFnNumericData::k3Double, 0.0, &l_status);
	CHECK_MSTATUS_AND_RETURN_IT(l_status);
	CHECK_MSTATUS(l_numeric_attr_fn.setWritable(true));
	CHECK_MSTATUS(l_numeric_attr_fn.setArray(true));
	CHECK_MSTATUS(l_numeric_attr_fn.setUsesArrayDataBuilder(true));
	CHECK_MSTATUS(l_numeric_attr_fn.setDisconnectBehavior(MFnAttribute::kDelete));
	// Matrix
	g_input_matrix_attr = l_matrix_attr_fn.create("inMatrix", "inm",MFnMatrixAttribute::kDouble,&l_status);
	CHECK_MSTATUS_AND_RETURN_IT(l_status);
	CHECK_MSTATUS(l_matrix_attr_fn.setWritable(true));
	CHECK_MSTATUS(l_matrix_attr_fn.setArray(true));
	CHECK_MSTATUS(l_matrix_attr_fn.setUsesArrayDataBuilder(true));
	CHECK_MSTATUS(l_matrix_attr_fn.setDisconnectBehavior(MFnAttribute::kDelete));
	// Mesh
	g_input_mesh_attr = l_typed_attr_fn.create("inMesh", "msh", MFnData::kMesh,MObject::kNullObj, &l_status);
	CHECK_MSTATUS(l_typed_attr_fn.setWritable(true));
	CHECK_MSTATUS(l_typed_attr_fn.setArray(true));
	CHECK_MSTATUS(l_typed_attr_fn.setUsesArrayDataBuilder(true));
	CHECK_MSTATUS(l_typed_attr_fn.setDisconnectBehavior(MFnAttribute::kDelete));
	// Curve
	g_input_curve_attr = l_typed_attr_fn.create("inCurve", "crv", MFnData::kNurbsCurve, MObject::kNullObj, &l_status);
	CHECK_MSTATUS(l_typed_attr_fn.setWritable(true));
	CHECK_MSTATUS(l_typed_attr_fn.setArray(true));
	CHECK_MSTATUS(l_typed_attr_fn.setUsesArrayDataBuilder(true));
	CHECK_MSTATUS(l_typed_attr_fn.setDisconnectBehavior(MFnAttribute::kDelete));
	// Surface
	g_input_surface_attr = l_typed_attr_fn.create("inSurface", "srv", MFnData::kNurbsSurface, MObject::kNullObj, &l_status);
	CHECK_MSTATUS(l_typed_attr_fn.setWritable(true));
	CHECK_MSTATUS(l_typed_attr_fn.setArray(true));
	CHECK_MSTATUS(l_typed_attr_fn.setUsesArrayDataBuilder(true));
	CHECK_MSTATUS(l_typed_attr_fn.setDisconnectBehavior(MFnAttribute::kDelete));


	// Compound input
	g_input_ports_attr = l_compound_attr_fn.create("inputPorts", "ip", &l_status);
	CHECK_MSTATUS_AND_RETURN_IT(l_status);
	CHECK_MSTATUS(l_compound_attr_fn.setWritable(true));
	CHECK_MSTATUS(l_compound_attr_fn.setReadable(false));
	CHECK_MSTATUS(l_compound_attr_fn.addChild(g_input_time_attr));
	CHECK_MSTATUS(l_compound_attr_fn.addChild(g_input_number_attr));
	CHECK_MSTATUS(l_compound_attr_fn.addChild(g_input_angle_attr));
	CHECK_MSTATUS(l_compound_attr_fn.addChild(g_input_vector_attr)); 
	CHECK_MSTATUS(l_compound_attr_fn.addChild(g_input_matrix_attr));
	CHECK_MSTATUS(l_compound_attr_fn.addChild(g_input_mesh_attr));
	CHECK_MSTATUS(l_compound_attr_fn.addChild(g_input_curve_attr));
	CHECK_MSTATUS(l_compound_attr_fn.addChild(g_input_surface_attr));
	CHECK_MSTATUS(l_compound_attr_fn.setKeyable(true));

#pragma endregion
#pragma region OUTPUT_PORTS
	g_output_number_attr = l_numeric_attr_fn.create("outNumber", "onm", MFnNumericData::kDouble, 0.0, &l_status);
	CHECK_MSTATUS_AND_RETURN_IT(l_status);
	CHECK_MSTATUS(l_numeric_attr_fn.setArray(true));
	CHECK_MSTATUS(l_numeric_attr_fn.setUsesArrayDataBuilder(true));

	g_output_ports_attr = l_compound_attr_fn.create("outputPorts", "op", &l_status);
	CHECK_MSTATUS_AND_RETURN_IT(l_status);
	CHECK_MSTATUS(l_compound_attr_fn.setWritable(false));
	CHECK_MSTATUS(l_compound_attr_fn.setReadable(true));
	CHECK_MSTATUS(l_compound_attr_fn.addChild(g_output_number_attr));


#pragma endregion
	// Add attributes
	l_status = addAttribute(g_ports_script_attr);	CHECK_MSTATUS_AND_RETURN_IT(l_status);
	l_status = addAttribute(g_script_attr);			CHECK_MSTATUS_AND_RETURN_IT(l_status);
	l_status = addAttribute(g_input_ports_attr);	CHECK_MSTATUS_AND_RETURN_IT(l_status);
	l_status = addAttribute(g_output_ports_attr);	CHECK_MSTATUS_AND_RETURN_IT(l_status);

	// Attributes affect
	l_status = attributeAffects(g_ports_script_attr,	g_output_ports_attr);	CHECK_MSTATUS_AND_RETURN_IT(l_status);
	l_status = attributeAffects(g_script_attr,			g_output_ports_attr);	CHECK_MSTATUS_AND_RETURN_IT(l_status);
	l_status = attributeAffects(g_input_ports_attr,		g_output_ports_attr);	CHECK_MSTATUS_AND_RETURN_IT(l_status);


	return MS::kSuccess;
}
MStatus	vufMayaLuaExpressionNode::compute(const MPlug& p_plug, MDataBlock& p_data)
{
	std::cout << "COMPUTE: " << name() << " " << p_plug.name() << std::endl;
	if (p_plug == g_output_ports_attr || p_plug == g_output_number_attr)
	{
		std::cout << "COMPUTE PLUG: " << name() << std::endl;
		MStatus l_status;
		std::shared_ptr<vufMayaLuaPortInternalData>		l_in_port_data;
		std::shared_ptr<vuf::vufTxt>					l_in_script_data;
		//------------------------------------------------------------------------------
		// Handle input port data
		VF_RM_GET_INTERNAL_FROM_IN(mpxMayaLuaPortWrapper, vufMayaLuaPortInternalData, p_data, g_ports_script_attr, l_in_port_data);
		if (l_in_port_data == nullptr)
		{
			VF_LOG_ERR( vufMayaUtils::mstr_2_str( this->name() + " Port data is null."));
			return MS::kSuccess;
		}
		// Handle input script data
		VF_RM_GET_INTERNAL_FROM_IN(mpxMayaLuaTxtWrapper, vuf::vufTxt, p_data, g_script_attr, l_in_script_data);
		if (l_in_script_data == nullptr)
		{
			VF_LOG_ERR(vufMayaUtils::mstr_2_str(this->name() + " script data is null."));
			return MS::kSuccess;
		}
		
		bool l_should_restart = (l_in_port_data->m_need_lua_reset == true) || 
								(l_in_script_data->get_hash() != m_script_hash);
		// Check should we restart lua machune or not
		if (l_should_restart == true)
		{
			VF_LOG_INFO("RESTART LUA_MACHINE");
			m_lua_machine.close_machine();
			m_lua_machine.open_machine();
			vufLuaVector4<double>::		registrator( m_lua_machine.get_lua_state());
			vufLuaMatrix4<double>::		registrator( m_lua_machine.get_lua_state());
			vufLuaQuaternion<double>::	registrator( m_lua_machine.get_lua_state());
			vufLuaMVector::				registrator(m_lua_machine.get_lua_state());
			vufLuaMPoint::				registrator(m_lua_machine.get_lua_state());
			vufLuaMMatrix::				registrator(m_lua_machine.get_lua_state());
			vufLuaMQuaternion::			registrator(m_lua_machine.get_lua_state());
			vufLuaMVectorArray::		registrator(m_lua_machine.get_lua_state());
			vufLuaMPointArray::			registrator(m_lua_machine.get_lua_state());
			m_script_hash = l_in_script_data->get_hash();
			l_in_port_data->m_need_lua_reset = false;
		}		
		if (set_lua_globals(p_data, l_in_port_data) == false)
		{
			VF_LOG_INFO("Failed to set Globals");
			return MS::kSuccess;
		}
		// Check should we redefine function
		if (l_should_restart == true)
		{
			// refefine function
			m_lua_machine.add_script(l_in_script_data->get_script());
			if (m_lua_machine.do_string() == false)
			{
				VF_LOG_ERR(vufMayaUtils::mstr_2_str(this->name() + " failed to run script."));
				return MS::kSuccess;
			}
			// compile compute
			if (m_lua_machine.load_and_compile(m_evaluation_script) == false)
			{
				VF_LOG_ERR(vufMayaUtils::mstr_2_str(this->name() + " failed to compile."));
				return MS::kSuccess;
			}
		}		
		// call compiled function call 
		if (m_lua_machine.do_compiled() == false)
		{
			VF_LOG_ERR(vufMayaUtils::mstr_2_str(this->name() + " Failed to evaluate compiled script."));
			return MS::kSuccess;
		}		
		//--------------------------------------------------------------------------------------------------------------
		// Set outputs
		if (set_maya_outputs(p_data, l_in_port_data) == false)
		{
			VF_LOG_ERR(vufMayaUtils::mstr_2_str(this->name() + " Failed to get output variables."));
			return MS::kSuccess;
		}	
		//m_lua_machine.dump_stack();
		p_data.setClean(g_output_ports_attr);
		p_data.setClean(g_output_number_attr);
		
		return MS::kSuccess;
	}
	return MS::kUnknownParameter;
}

bool vufMayaLuaExpressionNode::set_lua_globals( MDataBlock& p_data, std::shared_ptr<vufMayaLuaPortInternalData> p_port_data)
{
	MStatus		l_status;
	lua_State* L = m_lua_machine.get_lua_state();
	
	// read ports
	//---------------------------------------------------------------------------------------------------------
	// Input ports

	// read in time
	{
		MArrayDataHandle l_in_time_array_h = p_data.inputValue(g_input_time_attr);
		for (uint64_t i = 0; i < p_port_data->m_in_time_port.size(); ++i)
		{
			l_status = l_in_time_array_h.jumpToElement(p_port_data->m_in_time_port[i].m_index);
			if (l_status != MS::kSuccess)
			{
				VF_LOG_ERR(vufMayaUtils::mstr_2_str(this->name() + " Cant handle in time port "));
				return false;
			}
			double l_double = l_in_time_array_h.inputValue().asTime().value();
			lua_pushnumber(L, l_double);
			lua_setglobal(L, p_port_data->m_in_time_port[i].m_lua_var_name.c_str());		// push Lua global variable
		}
	}
	//read in double
	{
		MArrayDataHandle l_in_double_array_h = p_data.inputValue(g_input_number_attr);
		for (uint64_t i = 0; i < p_port_data->m_in_double_port.size(); ++i)
		{
			l_status = l_in_double_array_h.jumpToElement(p_port_data->m_in_double_port[i].m_index);
			if (l_status != MS::kSuccess)
			{
				VF_LOG_ERR(vufMayaUtils::mstr_2_str(this->name() + " Cant handle in double port "));
				return false;
			}
			double l_double = l_in_double_array_h.inputValue().asDouble();
			lua_pushnumber(L, l_double);
			lua_setglobal(L, p_port_data->m_in_double_port[i].m_lua_var_name.c_str());		// push Lua global variable
		}
	}
	//read in angle
	{
		MArrayDataHandle l_in_angle_array_h = p_data.inputValue(g_input_angle_attr);
		for (uint64_t i = 0; i < p_port_data->m_in_angle_port.size(); ++i)
		{
			l_status = l_in_angle_array_h.jumpToElement(p_port_data->m_in_angle_port[i].m_index);
			if (l_status != MS::kSuccess)
			{
				VF_LOG_ERR(vufMayaUtils::mstr_2_str(this->name() + " Cant handle in angle port "));
				return false;
			}
			double l_double = l_in_angle_array_h.inputValue().asAngle().asDegrees();
			lua_pushnumber(L, l_double);
			lua_setglobal(L, p_port_data->m_in_angle_port[i].m_lua_var_name.c_str());		// push Lua global variable
		}
	}
	//read in vector
	{
		MArrayDataHandle l_in_vector_array_h = p_data.inputValue(g_input_vector_attr);
		for (uint64_t i = 0; i < p_port_data->m_in_vector_port.size(); ++i)
		{
			l_status = l_in_vector_array_h.jumpToElement(p_port_data->m_in_vector_port[i].m_index);
			if (l_status != MS::kSuccess)
			{
				VF_LOG_ERR(vufMayaUtils::mstr_2_str(this->name() + " Cant handle in vector port "));
				return false;
			}
			MVector l_mvec = l_in_vector_array_h.inputValue().asVector();
			vufVector4<double>* l_vec = (vufVector4<double>*) & l_mvec;
			vufLuaVector4<double>::set_global(L, p_port_data->m_in_vector_port[i].m_lua_var_name, *l_vec);
		}
	}
	// read in matrix
	{
		MArrayDataHandle l_in_matrix_array_h = p_data.inputValue(g_input_matrix_attr);
		for (uint64_t i = 0; i < p_port_data->m_in_matrix_port.size(); ++i)
		{
			l_status = l_in_matrix_array_h.jumpToElement(p_port_data->m_in_matrix_port[i].m_index);
			if (l_status != MS::kSuccess)
			{
				VF_LOG_ERR(vufMayaUtils::mstr_2_str(this->name() + " Cant handle in vector port "));
				return false;
			}
			MMatrix l_mmatr = l_in_matrix_array_h.inputValue().asMatrix();
			vufMatrix4<double>* l_mat = (vufMatrix4<double>*) & l_mmatr;
			vufLuaMatrix4<double>::set_global(L, p_port_data->m_in_matrix_port[i].m_lua_var_name, *l_mat);
		}
	}
	// read in mesh
	// read in curve
	// read in surface
	//---------------------------------------------------------------------------------------------------------
	// Output ports

	//read output double
	for (uint64_t i = 0; i < p_port_data->m_out_double_port.size(); ++i)
	{
		double l_double = 0.0;
		lua_pushnumber(L, l_double);
		lua_setglobal(L, p_port_data->m_out_double_port[i].m_lua_var_name.c_str());		// push Lua global variable
	}

	return true;
}
bool vufMayaLuaExpressionNode::set_maya_outputs(MDataBlock& p_data, std::shared_ptr<vufMayaLuaPortInternalData> p_port_data)
{
	MStatus l_status;
	lua_State* L = m_lua_machine.get_lua_state();
	
	// output doubles
	MArrayDataHandle	l_out_double_array_h = p_data.outputValue(g_output_number_attr);
	MArrayDataBuilder	l_out_double_array_builder = l_out_double_array_h.builder(&l_status);
	for (uint64_t i = 0; i < p_port_data->m_out_double_port.size(); ++i)
	{
		MDataHandle l_out_handle = l_out_double_array_builder.addElement(p_port_data->m_in_double_port[i].m_index, &l_status);
		if (l_status != MS::kSuccess)
		{
			MGlobal::displayError(this->name() + " Cant buld handle in out double port ");
			return MS::kSuccess;
		}
		double l_double = 0.0;
		lua_getglobal(L, p_port_data->m_out_double_port[i].m_lua_var_name.c_str());		// push Lua global variable
		if (lua_isnumber(L, -1))	// is last elemnt in stack function a number
		{
			l_double = (double)lua_tonumber(L, -1);
			lua_pop(L,1);
		}
		l_out_handle.set(l_double);
	}
	return true;
}

