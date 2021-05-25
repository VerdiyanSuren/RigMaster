#ifndef VF_TP_TRNSFRM_LST_DATA_H
#define VF_TP_TRNSFRM_LST_DATA_H
#include <data/vufData.h>
#include <vufMatrix.h>

namespace vufRM
{
	class vufTransformListData : public vufData
	{
	public:
		VF_RM_DECLARE_DATA_BODY(vufTransformListData);

		std::shared_ptr<vuf::vufMatrixArrayObject_4d> m_transform_ptr = nullptr;
	};
	using mpxTransformListWrapper = vufDataTemplate<vufTransformListData>;
}
#endif // !VF_TP_TRNSFRM_LST_DATA_H
