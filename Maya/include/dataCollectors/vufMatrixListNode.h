#ifndef VF_RM_MATRIX_LST_NODE_H
#define VF_RM_MATRIX_LST_NODE_H

#include <maya/MPxNode.h>
//#include <maya/MMatrixArray.h>

namespace vufRM
{
	class vufMatrixListNode : public MPxNode
	{
	public:
		vufMatrixListNode();
		virtual ~vufMatrixListNode() {}

		static  void*	creator();
		static  MStatus	initialize();
		virtual void 	postConstructor() override;
		virtual MStatus	compute(const MPlug& plug, MDataBlock& data) override;
		//virtual MStatus	connectionBroken(const MPlug& p_my_plug, const MPlug& p_other_plug, bool asSrc) override;

		static const MTypeId	g_id;
		//attributes
		static MObject	g_transform_in_attr;
		static MObject	g_data_out_attr;

		static const MString	g_type_name;
	private:
		uint64_t		m_gen_id;
	};
}
#endif // !VF_RM_MATRIX_LST_NODE_H
