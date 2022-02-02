#ifndef VF_RM_MATRIX_LST_LCTR_NODE_H
#define VF_RM_MATRIX_LST_LCTR_NODE_H

#include <locator/vufLocator.h>
#include <vufObjectArrayFn.h>
#include <math/vufMatrix.h>
#include <memory>

namespace vufRM
{
	class vufMatrixListLocator : public MPxLocatorNode
	{
	public:
		vufMatrixListLocator();
		virtual ~vufMatrixListLocator();
		virtual void postConstructor() override;
		//virtual MStatus   		compute(const MPlug& plug, MDataBlock& data) override;

		virtual void            draw(	M3dView& view, const MDagPath& path,
										M3dView::DisplayStyle style,
										M3dView::DisplayStatus status) override;
		virtual bool            isBounded()		const override;
		virtual MBoundingBox    boundingBox()	const override;
		virtual MStatus			connectionBroken(const MPlug& p_plug_1, const MPlug& p_plug_2, bool p_as_src) override;
		static  void*			creator();
		static  MStatus         initialize();

		static	const MTypeId	g_id;
		static	const MString   g_type_name;
		static	const MString	g_drawDb_classification;
		// attributes
		static MObject g_draw_attr;
		static MObject g_dot_attr;
		static MObject g_tripod_attr;
		static MObject g_size_attr;
		static MObject g_connect_attr;
		static MObject g_draw_width_attr;
		static MObject g_data_in_attr;
	};
	class vufMatrixListLocatorDrawOverride : public MHWRender::MPxDrawOverride
	{
	private:
		vufMatrixListLocatorDrawOverride(const MObject& p_obj);
	public:
		virtual ~vufMatrixListLocatorDrawOverride() {}
		static MHWRender::MPxDrawOverride* creator(const MObject& obj)
		{
			return new vufMatrixListLocatorDrawOverride(obj);
		}


		virtual MHWRender::DrawAPI	supportedDrawAPIs()	const override;
		virtual bool				isBounded(const MDagPath& objPath, const MDagPath& cameraPath) const override;
		virtual MBoundingBox		boundingBox(const MDagPath& objPath, const MDagPath& cameraPath) const override;
		virtual bool				disableInternalBoundingBoxDraw() const override;
		virtual MUserData*			prepareForDraw(const MDagPath& objPath, const MDagPath& cameraPath, const MHWRender::MFrameContext& frameContext, MUserData* oldData) override;
		virtual bool				hasUIDrawables() const override { return true; }
		virtual void				addUIDrawables(const MDagPath& objPath, MHWRender::MUIDrawManager& drawManager, const MHWRender::MFrameContext& frameContext, const MUserData* data) override;

		//static void					draw(const MHWRender::MDrawContext& context, const MUserData* data) {}

	private:
		MBoundingBox m_current_bounding_box;
	};
	class vufMatrixListLocatorData : public MUserData
	{
	public:
		vufMatrixListLocatorData() :MUserData(true) { /*std::cout << "MUserData Construcotr" << std::endl;*/ }
		virtual ~vufMatrixListLocatorData() { /*std::cout << "MUserData Destructor" << std::endl;*/ }
		MColor		m_color_dot;
		MColor		m_color_tripod_x;
		MColor		m_color_tripod_y;
		MColor		m_color_tripod_z;
		MColor		m_color_line;
		double		m_dot_size;
		double		m_line_width;
		bool		m_draw;
		bool		m_draw_dot;
		bool		m_draw_tripods;
		bool		m_draw_connections;
		std::shared_ptr<vufMath::vufObjectArray<vufMath::vufMatrix_4d>> m_data_ptr = nullptr;
	};
}

#endif //!VF_RM_MATRIX_LST_LCTR_NODE_H