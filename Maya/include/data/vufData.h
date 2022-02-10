#ifndef VF_RM_DATA_H
#define VF_RM_DATA_H

#include <maya/MPxData.h>
#include <maya/MFnPluginData.h>
#include <vufLog.h>
#include <data/vufDataTemplate.h>
#include <vufLogMaya.h>
#include <utils/vufMayaUtils.h>
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
*	...
* }
*/

// m_internal could ve null
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
		data_var = std::shared_ptr<DATA_CLASS>(new DATA_CLASS());						\
		l_wrapp_ptr->set_data(data_var);												\
	}																					\
}

// m_internal could ve null
// after this internal data could be nullptr becource some data types is abstract interfaces
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
	/*if (data_var == nullptr)																					\
	{																											\
		data_var = std::shared_ptr<DATA_CLASS>(new DATA_CLASS());												\
		data_var->set_owner_id(ID);																				\
		l_wrapp_ptr->set_data(data_var);																		\
	}																											\*/\
	if (data_var->get_owner_id() != ID)																			\
	{																											\
		auto l_internal_data = data_var->m_internal_data;														\
		if (data_var->m_internal_data != nullptr)																\
		{																										\
			std::cout << "data has been copied" << std::endl;													\
			l_internal_data = data_var->m_internal_data->get_copy();											\
		}																										\
		data_var = std::shared_ptr<DATA_CLASS>(new DATA_CLASS());												\
		data_var->set_owner_id(ID);																				\
		l_wrapp_ptr->set_data(data_var);																		\
		data_var->m_internal_data = l_internal_data;															\
	}																											\
}
#define VF_CHECK_AND_CREATE_INTERNAL_DATA(DATA,TYPE)\
if (DATA->m_internal_data == nullptr)\
{\
	DATA->m_internal_data = std::shared_ptr<TYPE>(new TYPE());\
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
}

/**
* Try to get data from plugin in node
*  This method call node->name() which is not defined outside of node
* internal could be nullptr 
* data_var still reference if you need use VF_RM_NODE_SET_DATA_TO_PLUG
*/

#define VF_RM_NODE_GET_DATA_FROM_PLUG(WRAPPER_CLASS,DATA_CLASS, ATTR, data_var )		\
{																						\
	/* Find Plug by attribute object*/													\
	MPlug	PLUG = l_node.findPlug(ATTR, true, &l_status);								\
	if (l_status != MS::kSuccess)														\
	{																					\
		VF_MAYA_NODE_LOG_ERR(" Failed to find plug");									\
	}																					\
	/* Get data object from plug*/														\
	MObject	l_data_obj;																	\
	l_status = PLUG.getValue(l_data_obj);												\
	if (l_status != MS::kSuccess)														\
	{																					\
		VF_MAYA_NODE_LOG_ERR(" Failed to get value from plug");							\
	}																					\
	else																				\
	{																					\
		/* Cast obj to mpx data*/														\
		MFnPluginData	l_pd_fn(l_data_obj);											\
		WRAPPER_CLASS*	l_mpx_data = (WRAPPER_CLASS*)l_pd_fn.constData(&l_status);		\
		data_var = l_mpx_data == nullptr? nullptr : l_mpx_data->get_data();				\
	}																					\
	if (data_var == nullptr)															\
	{																					\
		data_var = std::shared_ptr<DATA_CLASS>(new DATA_CLASS);							\
	}																					\
}
/**
* Try to set data to plugin in node
*  This method call node->name() which is not defined outside of node
* data_var still reference if you need use VF_RM_NODE_SET_DATA_TO_PLUG
*/
#define VF_RM_NODE_SET_DATA_TO_PLUG(WRAPPER_CLASS,DATA_CLASS, ATTR, data_var )			\
{																						\
	MPlug	PLUG = l_node.findPlug(ATTR, true, &l_status);								\
	if (l_status != MS::kSuccess)														\
	{																					\
		VF_MAYA_NODE_LOG_ERR(" Failed to find plug");									\
	}																					\
	MFnPluginData l_data_creator;														\
	MObject l_obj = l_data_creator.create(WRAPPER_CLASS::g_id, &l_status);				\
	WRAPPER_CLASS* l_wrapp_ptr = (WRAPPER_CLASS*)l_data_creator.constData();			\
	l_wrapp_ptr->set_data(data_var);													\
	PLUG.setMObject(l_obj);																\
}

#define VF_RM_NODE_CONNECT_BROKEN_SIMPLE(WRAPPER_CLASS, DATA_CLASS, ATTR  )					\
{																							\
if (p_plug_1 == ATTR)																		\
{																							\
	MStatus l_status;																		\
	MObject						l_data_obj;													\
	MFnDependencyNode			l_node(p_plug_1.node());									\
	std::shared_ptr<DATA_CLASS>	l_in_data;													\
	VF_RM_NODE_GET_DATA_FROM_PLUG(WRAPPER_CLASS, DATA_CLASS, ATTR, l_in_data);				\
	std::shared_ptr<DATA_CLASS>	l_copy_data = std::shared_ptr<DATA_CLASS>(new DATA_CLASS());\
	if (l_in_data == nullptr || l_in_data->m_internal_data == nullptr)						\
	{																						\
		l_copy_data->m_internal_data = nullptr;												\
		VF_RM_NODE_SET_DATA_TO_PLUG(WRAPPER_CLASS, DATA_CLASS, ATTR, l_copy_data);			\
		return MPxNode::connectionBroken(p_plug_1, p_plug_2, p_as_src);						\
	}																						\
	l_copy_data->m_internal_data = l_in_data->m_internal_data->get_copy();					\
	VF_RM_NODE_SET_DATA_TO_PLUG(WRAPPER_CLASS, DATA_CLASS, ATTR, l_copy_data)				\
}																							\
return MPxNode::connectionBroken(p_plug_1, p_plug_2, p_as_src);								\
}

/**
* Try to get data from plug
*/
// internal could be nullptr
#define VF_RM_GET_DATA_FROM_PLUG(WRAPPER_CLASS,DATA_CLASS, ATTR, data_var )				\
{																						\
	MPlug	PLUG = l_node.findPlug(ATTR, true, &l_status);								\
	if (l_status != MS::kSuccess)														\
	{																					\
		VF_MAYA_NODE_LOG_ERR("Failed to find plug");									\
	}																					\
	MObject	l_data_obj;																	\
	l_status = PLUG.getValue(l_data_obj);												\
	if (l_status != MS::kSuccess)														\
	{																					\
		VF_MAYA_NODE_LOG_ERR("Failed to get value from plug");							\
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
	WRAPPER_CLASS* l_wrapp_ptr = (WRAPPER_CLASS*)l_data_creator.constData();			\
	l_wrapp_ptr->set_data(data_var);													\
	PLUG.setMObject(l_obj);																\
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

#define VF_RM_DECLARE_STANDART_DATA_CLASS(WRAPPER_CALSS_NAME,CLASS_NAME,INNER_CLASS_NAME)				\
class CLASS_NAME :public vufData																		\
	{																									\
	public:																								\
		CLASS_NAME();																					\
		virtual ~CLASS_NAME##();																		\
		virtual MStatus 	readASCII(const MArgList& p_args, unsigned int& p_last_element) override;	\
		virtual MStatus 	readBinary(std::istream& p_in,	  unsigned int	length)			override;	\
		virtual MStatus 	writeASCII(std::ostream& p_out)  override;									\
		virtual MStatus 	writeBinary(std::ostream& p_out) override;									\
		std::shared_ptr<INNER_CLASS_NAME> m_internal_data = nullptr;									\
	};																									\
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

	protected:
		uint64_t	m_owner_node_id = 0;
		void*		m_owner_ptr		= nullptr;
		uint32_t	m_owner_type	= 0;

	};


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
