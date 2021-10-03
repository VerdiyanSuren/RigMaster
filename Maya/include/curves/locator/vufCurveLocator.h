#ifndef VF_TP_QUAT_LCTR_H
#define VF_TP_QUAT_LCTR_H

#include <locator/vufLocator.h>
#include <curves/vufCurveContatiner.h>
#include <memory>


namespace vufRM
{
	class vufCurveLocator : public MPxLocatorNode
	{
	public:
		vufCurveLocator();
		virtual ~vufCurveLocator();
		virtual void postConstructor() override ;
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
		static MObject g_color_start_attr;
		static MObject g_color_end_attr;
		static MObject g_color_tngnt_attr;
		static MObject g_color_nrml_attr;
		static MObject g_draw_width_arrt;
		static MObject g_draw_start_attr;
		static MObject g_draw_end_attr;
		static MObject g_draw_tangents_attr;
		static MObject g_draw_normals_attr;
		static MObject g_division_attr;
		static MObject g_in_data_attr;
	};
	
	class vufCurveLocatorDrawOverride : public MHWRender::MPxDrawOverride
	{
	private:
		vufCurveLocatorDrawOverride(const MObject& p_obj);
	public:
		virtual ~vufCurveLocatorDrawOverride() {}
		static MHWRender::MPxDrawOverride* creator(const MObject& obj)
		{
			return new vufCurveLocatorDrawOverride(obj);
		}


		virtual MHWRender::DrawAPI	supportedDrawAPIs() const override;
		virtual bool				isBounded(const MDagPath& objPath, const MDagPath& cameraPath) const override;
		virtual MBoundingBox		boundingBox(const MDagPath& objPath, const MDagPath& cameraPath) const override;
		virtual bool				disableInternalBoundingBoxDraw() const override;
		virtual MUserData*			prepareForDraw(const MDagPath& objPath, const MDagPath& cameraPath, const MHWRender::MFrameContext& frameContext, MUserData* oldData) override;
		virtual bool				hasUIDrawables() const override{ return true; }
		virtual void				addUIDrawables(const MDagPath& objPath, MHWRender::MUIDrawManager& drawManager, const MHWRender::MFrameContext& frameContext, const MUserData* data) override;
		
		//static void					draw(const MHWRender::MDrawContext& context, const MUserData* data) {}

	private:
		MBoundingBox m_current_bounding_box;
	};
	
	class vufCurveLocatorData : public MUserData
	{
	public:
		vufCurveLocatorData():MUserData(true) { /*std::cout << "MUserData Construcotr" << std::endl;*/ }
		virtual ~vufCurveLocatorData() { /*std::cout << "MUserData Destructor" << std::endl;*/ }
		MColor		m_color_start;
		MColor		m_color_end;
		MColor		m_color_tngnt;
		MColor		m_color_normal;
		double		m_width;
		double		m_start_param;
		double		m_end_param;
		bool		m_tangent;
		bool		m_normal;
		int			m_division;
		std::shared_ptr<vufMath::vufCurveContainer_4d> m_curve_container_ptr = nullptr;
	};	
}

#endif	//!VF_TP_QUAT_LCTR_H