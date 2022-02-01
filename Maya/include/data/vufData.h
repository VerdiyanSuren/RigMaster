#ifndef VF_RM_DATA_H
#define VF_RM_DATA_H

#include <maya/MPxData.h>
#include <maya/MFnPluginData.h>
#include <vufLog.h>
#include "data/vufDataTemplate.h"
#include <memory>

/**
* Try to read custom data from output handle
* If data doesnt exist then create new data 
* 
* Example
* MStatus vufTestNode::compute(const MPlug& p_plug, MDataBlock& p_data)
* { ...
*	std::shared_ptr<vufDataTest> l_src_data;
*	VF_TP_GET_DATA_FROM_OUT_AND_CREATE(mpxTestWrapper, vufDataTest, p_data, g_data_src_attr, l_src_data);
*	l_src_data->set_owner_node(m_node_unique_id);
*	...
* }
*/
#define VF_RM_GET_DATA_FROM_OUT_AND_CREATE(WRAPPER_CLASS, DATA_CLASS, data_block, attr, data_var)	\
{																						\
	MDataHandle	 l_out_handle	= data_block.outputValue(attr);							\
	WRAPPER_CLASS *l_wrapp_ptr	= (WRAPPER_CLASS*)l_out_handle.asPluginData();			\
	if (l_wrapp_ptr == nullptr)															\
	{																					\
		/*std::cout << "Data is Empty" <<std::endl;*/									\
		MFnPluginData l_data_creator;													\
		l_out_handle.set(l_data_creator.create(WRAPPER_CLASS::g_id, &l_status));		\
		CHECK_MSTATUS_AND_RETURN_IT(l_status);											\
		l_wrapp_ptr = (WRAPPER_CLASS*)l_out_handle.asPluginData();						\
	}																					\
	data_var = l_wrapp_ptr->get_data();													\
	if (data_var == nullptr)															\
	{																					\
		/*std::cout << "Wanna constructor" <<std::endl;*/								\
		data_var = std::shared_ptr<DATA_CLASS>(new DATA_CLASS);							\
		l_wrapp_ptr->set_data(data_var);												\
	}																					\
}

#define VF_RM_GET_DATA_FROM_OUT_AND_MAKE_REF_UNIQUE(WRAPPER_CLASS, DATA_CLASS, data_block, attr, data_var,ID)	\
{																												\
	MDataHandle	 l_out_handle	= data_block.outputValue(attr);													\
	WRAPPER_CLASS *l_wrapp_ptr	= (WRAPPER_CLASS*)l_out_handle.asPluginData();									\
	if (l_wrapp_ptr == nullptr)																					\
	{																											\
		/*std::cout << "Data is Empty" <<std::endl;*/															\
		MFnPluginData l_data_creator;																			\
		l_out_handle.set(l_data_creator.create(WRAPPER_CLASS::g_id, &l_status));								\
		CHECK_MSTATUS_AND_RETURN_IT(l_status);																	\
		l_wrapp_ptr = (WRAPPER_CLASS*)l_out_handle.asPluginData();												\
	}																											\
	data_var = l_wrapp_ptr->get_data();																			\
	if (data_var == nullptr)																					\
	{																											\
		/*std::cout << "Wanna constructor" <<std::endl;*/														\
		data_var = std::shared_ptr<DATA_CLASS>(new DATA_CLASS);													\
		data_var->set_owner_id(ID);																				\
		l_wrapp_ptr->set_data(data_var);																		\
	}																											\
	if (data_var->get_owner_id() != ID)																			\
	{																											\
		auto l_internal_data = data_var->m_internal_data;														\
		if (data_var->m_internal_data != nullptr)																\
		{																										\
			l_internal_data = data_var->m_internal_data->get_copy();											\
		}																										\
		data_var = std::shared_ptr<DATA_CLASS>(new DATA_CLASS);													\
		data_var->set_owner_id(ID);																				\
		l_wrapp_ptr->set_data(data_var);																		\
		data_var->m_internal_data = l_internal_data;															\
	}																											\
}

#define VF_RM_INIT_AND_ADD_HIDDEN_ATTR(ATTR,LONG_NAME,SHORT_NAME,WRAPPER_CLASS)									\
{																												\
	ATTR = l_typed_attr_fn.create(LONG_NAME, SHORT_NAME, WRAPPER_CLASS::g_id, MObject::kNullObj, &l_status);	\
	CHECK_MSTATUS_AND_RETURN_IT(l_status);																		\
	l_typed_attr_fn.setStorable(true);																			\
	l_typed_attr_fn.setConnectable(false);																		\
	l_typed_attr_fn.setHidden(true);																			\
	l_status = addAttribute(ATTR);	CHECK_MSTATUS_AND_RETURN_IT(l_status);										\
}

/**
* Try to read data from input
*/
#define VF_RM_GET_DATA_FROM_IN(WRAPPER_CLASS, DATA_CLASS, data_block, attr, data_var)	\
{																						\
	MDataHandle		l_in_handle = data_block.inputValue(attr);							\
	WRAPPER_CLASS*	l_wrapp_ptr = (WRAPPER_CLASS*)l_in_handle.asPluginData();			\
	data_var = (l_wrapp_ptr == nullptr ? nullptr : l_wrapp_ptr->get_data());			\
}																						\

#define VF_RM_GET_INTERNAL_FROM_IN(WRAPPER_CLASS,INTERNAL_CLASS,data_block, attr, data_var)	\
{																							\
	MDataHandle		l_in_handle = data_block.inputValue(attr);								\
	WRAPPER_CLASS*	l_wrapp_ptr = (WRAPPER_CLASS*)l_in_handle.asPluginData();				\
	auto l_data_var = (l_wrapp_ptr == nullptr ? nullptr : l_wrapp_ptr->get_data());			\
	data_var		= (l_data_var == nullptr) ? nullptr : l_data_var->m_internal_data;		\
}																							\

/**
*  Try to set out data
*/
#define VF_RM_SET_DATA_TO_OUT(WRAPPER_CLASS, DATA_CLASS, data_block, attr, data_var)	\
{																						\
	MDataHandle	 l_out_handle	= data_block.outputValue(attr);							\
	MFnPluginData l_data_creator;														\
	MObject l_obj = l_data_creator.create(WRAPPER_CLASS::g_id, &l_status);				\
	l_out_handle.set(l_obj);															\
	WRAPPER_CLASS* l_wrapp_ptr = (WRAPPER_CLASS*)l_data_creator.constData();			\
	l_wrapp_ptr->set_data(data_var);													\
}																						\

/**
* Try to get data from plug
*/
#define VF_RM_GET_DATA_FROM_PLUG(WRAPPER_CLASS,DATA_CLASS, ATTR, data_var )				\
{																						\
	MPlug	PLUG = l_node.findPlug(ATTR, true, &l_status);								\
	if (l_status != MS::kSuccess)														\
	{																					\
		VF_LOG_ERR("Failed to find plug");												\
	}																					\
	MObject	l_data_obj;																	\
	l_status = PLUG.getValue(l_data_obj);												\
	if (l_status != MS::kSuccess)														\
	{																					\
		VF_LOG_ERR("Failed to get value from plug");									\
	}																					\
	else																				\
	{																					\
		MFnPluginData	l_pd_fn(l_data_obj);											\
		WRAPPER_CLASS*	l_mpx_data = (WRAPPER_CLASS*)l_pd_fn.constData(&l_status);		\
		data_var = l_mpx_data == nullptr? nullptr : l_mpx_data->get_data();				\
	}																					\
	if (data_var == nullptr)															\
	{																					\
		/*std::cout << "Wanna constructor" <<std::endl;*/								\
		data_var = std::shared_ptr<DATA_CLASS>(new DATA_CLASS);							\
	}																					\
}
#define VF_RM_GET_INTERNAL_DATA_FROM_PLUG(WRAPPER_CLASS, PLUG, data_var )				\
{																						\
	MObject	l_data_obj;																	\
	l_status = PLUG.getValue(l_data_obj);												\
	if (l_status != MS::kSuccess)														\
	{																					\
		VF_LOG_ERR("Failed to get value from plug");									\
	}																					\
	else																				\
	{																					\
		MFnPluginData	l_pd_fn(l_data_obj);											\
		WRAPPER_CLASS*	l_wrapp_ptr = (WRAPPER_CLASS*)l_pd_fn.constData(&l_status);		\
		auto l_data_var = (l_wrapp_ptr == nullptr ? nullptr : l_wrapp_ptr->get_data());		\
		data_var		= (l_data_var == nullptr) ? nullptr : l_data_var->m_internal_data;	\
	}																					\
}
/**
* Try to set data to plug
*/
#define VF_RM_SET_DATA_TO_PLUG(WRAPPER_CLASS,DATA_CLASS, PLUG, data_var )				\
{																						\
	MFnPluginData l_data_creator;														\
	MObject l_obj = l_data_creator.create(WRAPPER_CLASS::g_id, &l_status);				\
	PLUG.setMObject(l_obj);																\
	WRAPPER_CLASS* l_wrapp_ptr = (WRAPPER_CLASS*)l_data_creator.constData();			\
	l_wrapp_ptr->set_data(data_var);													\
}

#define VF_RM_SET_INTERNAL_DATA_TO_PLUG(WRAPPER_CLASS,DATA_CLASS, PLUG, data_var )		\
{																						\
	MFnPluginData l_data_creator;														\
	MObject l_obj = l_data_creator.create(WRAPPER_CLASS::g_id, &l_status);				\
	WRAPPER_CLASS* l_wrapp_ptr = (WRAPPER_CLASS*)l_data_creator.constData();			\
	auto l_new_data = std::shared_ptr<DATA_CLASS>(new DATA_CLASS());					\
	l_new_data->m_internal_data = data_var;												\
	l_wrapp_ptr->set_data(l_new_data);													\
	PLUG.setMObject(l_obj);																\
}



#define VF_RM_DECLARE_DATA_BODY(CLASS_NAME)														\
public:																							\
CLASS_NAME();																					\
virtual ~CLASS_NAME##();																		\
virtual MStatus 	readASCII(const MArgList& p_args, unsigned int& p_last_element) override;	\
virtual MStatus 	readBinary(std::istream& p_in,	  unsigned int	length)			override;	\
virtual MStatus 	writeASCII(std::ostream& p_out)  override;									\
virtual MStatus 	writeBinary(std::ostream& p_out) override;									\
virtual int			get_type()	const override;													\
virtual void		log_me()	const override;													\

#define VF_RM_DECLARE_STANDART_DATA_CLASS(WRAPPER_CALSS_NAME,CLASS_NAME,INNER_CLASS_NAME)		\
class CLASS_NAME :public vufData																\
	{																							\
	public:																						\
		VF_RM_DECLARE_DATA_BODY(CLASS_NAME);													\
		std::shared_ptr<INNER_CLASS_NAME> m_internal_data = nullptr;							\
	};																							\
	using WRAPPER_CALSS_NAME = vufDataTemplate<CLASS_NAME>;

namespace vufRM
{
	/**
	* Interface off custom data.
	*/
	class vufData
	{
	public:
		vufData() :m_owner_node_id(0) {}
		virtual ~vufData() {};
		enum
		{
			k_absent_data = 0,
			k_lua_port_data,
			k_lua_script_data,
			k_transform_list_data,
			k_curve_data,
			k_curve_quat_data,
			k_curve_scale_data,
			k_curve_rebuild_data,
			k_test_data
		};
		void		set_owner_id(uint64_t p_new_owner_id)	{ m_owner_node_id = p_new_owner_id; }
		uint64_t	get_owner_id() const					{ return m_owner_node_id; }

		void		set_owner_ptr(void* p_ptr)				{ m_owner_ptr = p_ptr; }
		void*		get_owner_ptr() const					{ return m_owner_ptr; }

		void		set_owner_type(uint32_t p_owner_type)	{ m_owner_type = p_owner_type; }
		uint32_t	get_owner_type() const					{ return m_owner_type; }
		// serialize methods for mpx data overrides
		virtual MStatus 	readASCII(	const MArgList& p_args, unsigned int&	p_last_element) = 0;
		virtual MStatus 	readBinary(	std::istream&	p_in,	unsigned int	length)			= 0;
		virtual MStatus 	writeASCII(	std::ostream&	p_out) = 0;
		virtual MStatus 	writeBinary(std::ostream&	p_out) = 0;

		virtual int						get_type()	const = 0;
		virtual void					log_me()	const = 0;
		//static methods
		static std::shared_ptr<vufData> create_data(int p_type);
		static int						get_data_type(std::shared_ptr<vufData> p_data)
		{
			return p_data == nullptr ? k_absent_data : p_data->get_type();
		}
	protected:
		uint64_t	m_owner_node_id = 0;
		void*		m_owner_ptr		= nullptr;
		uint32_t	m_owner_type	= 0;

	};

	class vufDataTest :public vufData
	{
	public:
		VF_RM_DECLARE_DATA_BODY(vufDataTest)
		
		double		m_value;
		int			m_random = 0;
		int			m_type	 = 0; // 1 - child  2- dist
	};
	using mpxTestWrapper = vufDataTemplate<vufDataTest>;

	// methods to manipulate with custom data from command
	template<typename CLASS_WRAPPER, typename CLASS_NAME, typename INNER_CLASS_NAME>
	std::shared_ptr<INNER_CLASS_NAME> get_inner_data(MFnDependencyNode& p_node,MObject& p_attr)
	{
		MStatus l_status;
		MPlug	l_txt_plug = p_node.findPlug(p_attr,true, &l_status);
		if (l_status != MS::kSuccess)
		{
			VF_LOG_INFO("Failed to find plug");
			return nullptr;
		}

		MObject l_data;
		
		l_status = l_txt_plug.getValue(l_data);
		if (l_status != MS::kSuccess)
		{	
			VF_LOG_INFO("Failed to get value");
			return nullptr;
		}
		
		MFnPluginData l_data_fn;
		l_status = l_data_fn.setObject(l_data);
		if (l_status != MS::kSuccess)
		{
			VF_LOG_INFO("Failed to set Object");
			return nullptr;
		}
		
		auto l_data_ptr = (CLASS_WRAPPER*)l_data_fn.constData(&l_status);
		if (l_status != MS::kSuccess)
		{
			VF_LOG_INFO("Failed to get MPxData");
			return nullptr;
		}
		
		auto l_class_data = l_data_ptr->get_data();
		if (l_class_data == nullptr)
		{
			VF_LOG_INFO("Failed to get data");
			return nullptr;
		}

		return l_class_data->m_internal_data;
	}



}
#endif // !VF_RM_DATA_H
