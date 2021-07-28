#ifndef VF_RM_DATA_CLTR_INCLUDE_H
#define VF_RM_DATA_CLTR_INCLUDE_H

#define VF_RM_GET_MATRIX_ARRAY_FROM_IN(ATTR,ARRAY)					\
{																	\
	MDataHandle l_input_data = p_data.inputValue(g_transfoms_attr);	\
	MObject		l_in_obj = l_input_data.data();						\
	MFnMatrixArrayData l_in_data_fn(l_in_obj, &l_status);			\
	ARRAY = l_in_data_fn.array(&l_status);							\
}

#define VF_RM_GET_DOUBLE_ARRAY_FROM_IN(ATTR,ARRAY)					\
{																	\
	MDataHandle l_input_data = p_data.inputValue(g_transfoms_attr);	\
	MObject		l_in_obj = l_input_data.data();						\
	MFnDoubleArrayData l_in_data_fn(l_in_obj, &l_status);			\
	ARRAY = l_in_data_fn.array(&l_status);							\
}

#endif //!VF_RM_DATA_CLTR_INCLUDE_H