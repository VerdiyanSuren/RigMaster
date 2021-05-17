#ifndef VF_LUA_MAYA_STATIC_H
#define VF_LUA_MAYA_STATIC_H


namespace vufRM
{
	class vufLuaMayaStatic
	{
	public:
		static constexpr char* g_mvec_tbl_name		= "MVector";
		static constexpr char* g_mvec_meta_name		= "MVectorM";
		static constexpr char* g_mvec_arr_tbl_name	= "MVectorArray";
		static constexpr char* g_mvec_arr_meta_name = "MVectorArrayM";

		static constexpr char* g_mpoint_tbl_name		= "MPoint";
		static constexpr char* g_mpoint_meta_name		= "MPointM";
		static constexpr char* g_mpoint_arr_tbl_name	= "MPointArray";
		static constexpr char* g_mpoint_arr_meta_name	= "MPointArrayM";

		static constexpr char* g_mquat_tbl_name		= "MQuaternion";
		static constexpr char* g_mquat_meta_name	= "MQuaternionM";

		static constexpr char* g_mmatrix_tbl_name	= "MMatrix";
		static constexpr char* g_mmatrix_meta_name	= "MMatrixM";

		static constexpr char* g_meuler_tbl_name	= "MEulerRotation";
		static constexpr char* g_meuler_meta_name	= "MEulerRotationM";

		static constexpr char* g_mint_arr_tbl_name		= "MIntArray";
		static constexpr char* g_mint_arr_meta_name		= "MIntArrayM";
		static constexpr char* g_mdouble_arr_tbl_name	= "MDoubleArray";
		static constexpr char* g_mdouble_arr_meta_name	= "MDoubleArrayM";
	};
}
#endif // !VF_LUA_MAYA_STATIC_H
