#ifndef VF_TP_CURVE_RBLD_DATA_H
#define VF_TP_CURVE_RBLD_DATA_H

#include "data/vufData.h"
#include "math/curves/vufCurve.h"
#include "math/curves/vufCurveRebuildFn.h"

namespace vufRM
{
	class vufCurveRebuildData_4d :public vufData
	{
	public:
		VF_TP_DECLARE_DATA_BODY(vufCurveRebuildData_4d);

		std::shared_ptr<vufMath::vufCurveRebuildFn_4d> m_curve_rebuild_ptr = nullptr;
	};
	using mpxCurveRebuildWrapper = vufDataTemplate<vufCurveRebuildData_4d>;
}

#endif // !VF_TP_CURVE_RBLD_DATA_H
