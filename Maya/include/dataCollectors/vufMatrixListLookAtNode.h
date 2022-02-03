#ifndef VF_RM_MTRX_LIST_LOOK_UP_H
#define VF_RM_MTRX_LIST_LOOK_UP_H

#include <maya/MPxNode.h>
//#include <maya/MMatrixArray.h>

namespace vufRM
{
	class vufMatrixListLookAtNode : public MPxNode
	{
	public:
		vufMatrixListLookAtNode();
		virtual ~vufMatrixListLookAtNode() {}

		static  void*	creator();
		static  MStatus	initialize();
		virtual void 	postConstructor() override;
		virtual MStatus	compute(const MPlug& plug, MDataBlock& data) override;
		virtual MStatus	connectionBroken(const MPlug& p_plug_1, const MPlug& p_plug_2, bool p_as_src) override;
		static const MTypeId	g_id;
		//attributes
		static MObject	g_data_in_attr;
		static MObject	g_data_out_attr;

		static const MString	g_type_name;
	private:
		bool			m_locked = true;
		uint64_t		m_gen_id;
	};
}
#endif // !VF_RM_MTRX_LIST_LOOK_UP_H
