#include <utils/vufMayaUtils.h>
#include <maya/MPlugArray.h>
#include <maya/MPlug.h>
#include <maya/MSelectionList.h>
#include <coreUtils/vufStringUtils.h>
#include <maya/MFnNumericAttribute.h>
#include <maya/MFnTypedAttribute.h>
#include <maya/MFnUnitAttribute.h>
#include <maya/MGlobal.h>

using namespace vufRM;

//input plaugs rerieve
MString vufMayaUtils::get_input_connected_node_name(const MFnDependencyNode& p_node, const MObject& p_attr)
{
	MStatus l_status;
	MPlug l_port_plug = p_node.findPlug(p_attr, true, &l_status);
	if (l_status != MS::kSuccess)
	{
		return MString();
	}
	MPlugArray l_port_input_arr;
	l_port_plug.connectedTo(l_port_input_arr, true, false, &l_status);
	if (l_status != MS::kSuccess || l_port_input_arr.length() == 0)
	{
		return MString();
	}
	if (l_port_input_arr[0].node().isNull() == true)
	{
		return MString();
	}
	return MFnDependencyNode(l_port_input_arr[0].node()).name();
}
MObject vufMayaUtils::get_object_by_name(const MString& p_str)
{
	MStatus l_status;
	MSelectionList l_list;
	l_list.add(p_str);
	MObject l_obj;
	l_status = l_list.getDependNode(0, l_obj);
	
	return l_status == MS::kSuccess ? l_obj : MObject();
}
std::string vufMayaUtils::get_plug_type(const MPlug& p_plug)
{
	MObject l_attribute = p_plug.attribute();
	if (l_attribute.hasFn(MFn::kNumericAttribute))
	{
		MFnNumericAttribute l_fn_attrib(l_attribute);
		switch (l_fn_attrib.unitType())
		{
			case MFnNumericData::kBoolean:
			{
				return "boolean";
			}
			case MFnNumericData::kShort:
			{
				return "short";
			}
			case MFnNumericData::kLong:
			{
				return "long";
			}
			case MFnNumericData::kFloat:
			{
				return "float";
			}
			case MFnNumericData::kDouble:
			{
				return "double";
			}
			case MFnNumericData::k3Float:
			{
				return "float3";
			}
			case MFnNumericData::k3Double:
			{
				return "double3";
			}
			default:
				return "unknown numeric";
		}
	}
	if (l_attribute.hasFn(MFn::kTypedAttribute))
	{
		MFnTypedAttribute l_fn_attrib(l_attribute);
		switch (l_fn_attrib.attrType())
		{
		case MFnData::kMatrix:
		{
			return "matrix";
		}
		case MFnData::kMatrixArray:
		{
			return "matrix array";
		}
		case MFnData::kDoubleArray:
		{
			return "double array";
		}
		case MFnData::kMesh:
		{
			return "float";
		}
		case MFnData::kNurbsCurve:
		{
			return "curve";
		}
		case MFnData::kNurbsSurface:
		{
			return "surface";
		}
		case MFnData::kIntArray:
		{
			return "int array";
		}
		default:
			return "unknown typed";
		}
	}
	if (l_attribute.hasFn(MFn::kUnitAttribute))
	{
		MFnUnitAttribute l_fn_attrib(l_attribute);
		switch (l_fn_attrib.unitType())
		{
		case MFnUnitAttribute::kAngle:
		{
			return "angle";
		}
		case MFnUnitAttribute::kTime:
		{
			return "time";
		}
		case MFnUnitAttribute::kDistance:
		{
			return "distance";
		}
		default:
			return "unknown unit";
		}
	}
	return "unknown";
}


//converssion
std::wstring		vufMayaUtils::mstr_2_wstr(const MString& p_mstr)
{
	return p_mstr.asWChar();
}
std::string			vufMayaUtils::mstr_2_str(const MString& p_mstr)
{
	return p_mstr.asChar();
}
QString				vufMayaUtils::mstr_2_qstr(const MString& p_mstr)
{
	return QString::fromStdWString( p_mstr.asWChar());
}

std::string			vufMayaUtils::wstr_2_str(const std::wstring& p_wstr)
{
	return vuf::vufStringUtils::wstring_to_string(p_wstr);
}
MString				vufMayaUtils::wstr_2_mstr(const std::wstring& p_wstr)
{
	return MString(p_wstr.c_str());
}
QString				vufMayaUtils::wstr_2_qstr(const std::wstring& p_wstr)
{
	return QString::fromStdWString(p_wstr);
}

std::wstring		vufMayaUtils::str_2_wstr(const std::string& p_str)
{
	return vuf::vufStringUtils::string_to_wstring(p_str);
}
MString			vufMayaUtils::str_2_mstr(const std::string& p_str)
{
	return MString(p_str.c_str());
}
QString			vufMayaUtils::str_2_qstr(const std::string& p_str)
{
	return QString::fromStdString(p_str);
}

MString			vufMayaUtils::qstr_2_mstr(const QString& p_qstr)
{
	return MString(wstr_2_mstr( p_qstr.toStdWString() ));
}
std::wstring	vufMayaUtils::qstr_2_wstr(const QString& p_qstr)
{
	return p_qstr.toStdWString();
}
std::string		vufMayaUtils::qstr_2_str(const QString& p_qstr)
{
	return p_qstr.toStdString();
}

