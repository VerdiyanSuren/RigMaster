#ifndef VF_MATH_OBJECT_H
#define VF_MATH_OBJECT_H

#include <string>
#include <vector>
#include <memory>

namespace vufMath
{
	template<typename>	class vufVector2;
	template<typename>	class vufVector3;
	template<typename>	class vufVector4;

	template<typename>	class vufMatrix2;
	template<typename>	class vufMatrix3;
	template<typename>	class vufMatrix4;

	template<typename>	class vufQuaternion;
	template<typename>	class vufQuaternionObject;
	template<typename>	class vufQuaternionArrayObject;

	template<typename, template <typename> class VECTOR >	class vufVectorObject;
	template<typename, template <typename> class VECTOR >	class vufVectorArrayObject;
	template<typename, template <typename> class MATRIX >	class vufMatrixObject;
	template<typename, template <typename> class MATRIX >	class vufMatrixArrayObject;
	//class vufMatrixObject3;
	//class vufMatrixObject4;

	//template<typename T>	class vufMatrixObject2;
	//template<typename T>	class vufMatrixObject3;
	//template<typename T>	class vufMatrixObject4;

	template<typename> class vufNumericArrayObject;
	
	class vufCustomObject;
	class vufCustomArrayObject;
	
	template<typename T>	class vufPointObject;

	enum class vufObjectType :unsigned int
	{
		// undefined 
		k_null_object,
		// standart vectors
		k_vector_2f_object,
		k_vector_3f_object,
		k_vector_4f_object,
		k_vector_2d_object,
		k_vector_3d_object,
		k_vector_4d_object,
		// standart vectors array
		k_vector_2f_array_object,
		k_vector_3f_array_object,
		k_vector_4f_array_object,
		k_vector_2d_array_object,
		k_vector_3d_array_object,
		k_vector_4d_array_object,


		// quaternions
		k_quaternion_f_object,
		k_quaternion_d_object,
		//quaternuiins array
		k_quaternion_f_array_object,
		k_quaternion_d_array_object,


		//standart matricies
		k_matrix_2f_object,
		k_matrix_3f_object,
		k_matrix_4f_object,
		k_matrix_2d_object,
		k_matrix_3d_object,
		k_matrix_4d_object,
		//standart matricies array
		k_matrix_2f_array_object,
		k_matrix_3f_array_object,
		k_matrix_4f_array_object,
		k_matrix_2d_array_object,
		k_matrix_3d_array_object,
		k_matrix_4d_array_object,


		// free dimensions vectors
		k_vector_Nf_object,
		k_vector_Nd_object,
		// free dimensions vectors array
		k_vector_Nf_array_object,
		k_vector_Nd_array_object,


		// free  dimensions square matricies
		k_matrix_Nf_object,
		k_matrix_Nd_object,
		// free dimensions square matricies array
		k_matrix_Nf_array_object,
		k_matrix_Nd_array_object,


		// free dimensions  matricies
		k_matrix_NMf_object,
		k_matrix_NMd_object,
		// free dimensions  matricies array
		k_matrix_NN_array_object,
		k_matrix_NM_array_object,


		// numeric objects
		k_int_object,
		k_float_object,
		k_double_object,
		// numeric objects array
		k_int_array_object,
		k_float_array_object,
		k_double_array_object,
		k_bool_array_object,
		//custom object inherited from vufObjec
		k_custom_object,
		// custom object array
		k_custom_array_object
	};

	class vufObject
	{
	public:
		vufObject() {}
		virtual ~vufObject() {}

		virtual bool		is_array()					const = 0;
		virtual bool		is_numeric()				const = 0;

		virtual std::shared_ptr<vufObject > make_copy()							const = 0;
		virtual uint64_t					get_detailed_count()				const = 0
		{
			return 0;
		}
		virtual std::shared_ptr<vufObject>	get_detailed_at(uint64_t p_index)	const = 0
		{
			return nullptr;
		}
		/** Serializations*/
		virtual std::string			to_string(int p_precision = -1, uint32_t p_tab_count = 0)	const = 0;
		virtual uint64_t			from_string(const std::string& p_str, uint64_t p_offset = 0)= 0;
		virtual uint64_t			to_binary(std::vector<unsigned char>& p_buff) const			= 0;
		virtual uint64_t			from_binary(const std::vector<unsigned char>& p_buff, uint64_t p_offset = 0) = 0;

		virtual vufObjectType		get_type() const = 0;
		// standart vectors
		virtual std::shared_ptr< vufVectorObject<float,  vufVector2> >		as_vector_object_2f()	const { return nullptr; }
		virtual std::shared_ptr< vufVectorObject<float,  vufVector3> >		as_vector_object_3f()	const { return nullptr; }
		virtual std::shared_ptr< vufVectorObject<float,  vufVector4> >		as_vector_object_4f()	const { return nullptr; }
		virtual std::shared_ptr< vufVectorObject<double, vufVector2> >		as_vector_object_2d()	const { return nullptr; }
		virtual std::shared_ptr< vufVectorObject<double, vufVector3> >		as_vector_object_3d()	const { return nullptr; }
		virtual std::shared_ptr< vufVectorObject<double, vufVector4> >		as_vector_object_4d()	const { return nullptr; }
		// standart vectors array
		virtual std::shared_ptr< vufVectorArrayObject<float,vufVector2>>	as_vector_array_object_2f()	const { return nullptr; }
		virtual std::shared_ptr< vufVectorArrayObject<float,vufVector3>>	as_vector_array_object_3f()	const { return nullptr; }
		virtual std::shared_ptr< vufVectorArrayObject<float,vufVector4>>	as_vector_array_object_4f()	const { return nullptr; }
		virtual std::shared_ptr< vufVectorArrayObject<double, vufVector2>>	as_vector_array_object_2d()	const { return nullptr; }
		virtual std::shared_ptr< vufVectorArrayObject<double, vufVector3>>	as_vector_array_object_3d()	const { return nullptr; }
		virtual std::shared_ptr< vufVectorArrayObject<double, vufVector4>>	as_vector_array_object_4d()	const { return nullptr; }
		//quaternions
		virtual std::shared_ptr< vufQuaternionObject<float > >				as_quaternion_object_f() const { return nullptr; }
		virtual std::shared_ptr< vufQuaternionObject<double> >				as_quaternion_object_d() const { return nullptr; }
		// quaternion array
		virtual std::shared_ptr< vufQuaternionArrayObject<float> >			as_quaternion_array_object_f()	const { return nullptr; }
		virtual std::shared_ptr< vufQuaternionArrayObject<double> >			as_quaternion_array_object_d()	const { return nullptr; }
		//standart matricies
		virtual std::shared_ptr < vufMatrixObject<double,vufMatrix2 >>		as_matrix_obj_2d()		const { return nullptr; }
		virtual std::shared_ptr < vufMatrixObject<double,vufMatrix3 >>		as_matrix_obj_3d()		const { return nullptr; }
		virtual std::shared_ptr < vufMatrixObject<double,vufMatrix4>>		as_matrix_obj_4d()		const { return nullptr; }
		virtual std::shared_ptr < vufMatrixObject<float, vufMatrix2 >>		as_matrix_obj_2f()		const { return nullptr; }
		virtual std::shared_ptr < vufMatrixObject<float, vufMatrix3 >>		as_matrix_obj_3f()		const { return nullptr; }
		virtual std::shared_ptr < vufMatrixObject<float, vufMatrix4>>		as_matrix_obj_4f()		const { return nullptr; }
		// standart matricies array
		virtual std::shared_ptr < vufMatrixArrayObject<double, vufMatrix2> >	as_matrix_array_obj_2d()	const { return nullptr; }
		virtual std::shared_ptr < vufMatrixArrayObject<double, vufMatrix3> >	as_matrix_array_obj_3d()	const { return nullptr; }
		virtual std::shared_ptr < vufMatrixArrayObject<double, vufMatrix4> >	as_matrix_array_obj_4d()	const { return nullptr; }
		virtual std::shared_ptr < vufMatrixArrayObject<float,  vufMatrix2> >	as_matrix_array_obj_2f()	const { return nullptr; }
		virtual std::shared_ptr < vufMatrixArrayObject<float,  vufMatrix3> >	as_matrix_array_obj_3f()	const { return nullptr; }
		virtual std::shared_ptr < vufMatrixArrayObject<float,  vufMatrix4> >	as_matrix_array_obj_4f()	const { return nullptr; }
		// numeric array
		virtual std::shared_ptr < vufNumericArrayObject<float> >	as_float_array_object()		const { return nullptr; }
		virtual std::shared_ptr < vufNumericArrayObject<double> >	as_double_array_object()	const { return nullptr; }
		virtual std::shared_ptr < vufNumericArrayObject<int> >		as_int_array_object()		const { return nullptr; }
		virtual std::shared_ptr < vufNumericArrayObject<bool> >		as_bool_array_object()		const { return nullptr; }

		/* TO DO
		// add
		// free dimensions vectors
		// free dimensional vector arrays
		// free dimensions square matricies
		// free dimensions square matricies array
		// free dimensions matricies 
		// free dimensions matricies array
		// numeric objects
		// custom objects
		// custom object array
		*/
		
	protected:
		std::weak_ptr<vufObject>	m_this;
	};
}
#endif // !VF_MATH_OBJECT_H
