#ifndef VF_MATH_CRV_SLIDE_H
#define VF_MATH_CRV_SLIDE_H

#include <curves/vufCurve.h>
#include <curves/vufCurveContatiner.h>

namespace vufMath
{
	template<class T, template<typename> class V>	class vufCurveContainer;

	template <class T, template<typename> class V >
	class vufCurveSlide : public vufCurve<T, V>
	{
	public:
		vufCurveSlide() :vufCurve<T, V>()()
		{
			vufCurve<T, V>::m_degree = 0;
			vufCurve<T, V>::m_close = false;
		}
		virtual ~vufCurveBlend() {}
		static  std::shared_ptr< vufCurveSlide > create()
		{
			std::shared_ptr< vufCurveSlide >  l_ptr = std::shared_ptr<vufCurveSlide>(new vufCurveSlide());
			VF_MATH_CURVE_CREATOR_BODY(l_ptr);
			return l_ptr;
		}
	private:
		std::shared_ptr< vufCurveContainer<T, V> >	m_container_ptr = nullptr;

	};

	using vufCurveSlide_2f = vufCurveSlide<float,  vufVector2>;
	using vufCurveSlide_2d = vufCurveSlide<double, vufVector2>;
	using vufCurveSlide_3f = vufCurveSlide<float,  vufVector3>;
	using vufCurveSlide_3d = vufCurveSlide<double, vufVector3>;
	using vufCurveSlide_4f = vufCurveSlide<float,  vufVector4>;
	using vufCurveSlide_4d = vufCurveSlide<double, vufVector4>;

}
#endif // !VF_MATH_CRV_SLIDE_H
