
import maya.cmds as cmds
import math
#---------------------------------------------------
#			NATRIX LIST NODES AND PLUGE
#---------------------------------------------------
#nodes
_MTRX_LIST_NODE 	= "vfMtrxList"
_MTRX_NULL_NODE 	= "vfMtrxListNull"
_MTRX_LOOK_NODE 	= "vfMtrxListLookAt"
_MTRX_DCMPS_NODE 	= "vfMtrxListDcmps"
_MTRX_LCTR_NODE 	= "vfMtrxListLocator"
#plugs
_PLUG_IN_MMTRX_BLDR 	= "inXForms"
_PLUG_OUT_MMTRX_BLDR 	= "outXForms"
_PLUG_IN_MMTRX_LST 		= "inMatrixList"
_PLUG_OUT_MMTRX_LST 	= "outMatrixList"
#---------------------------------------------------
#			DOUBLE LIST NODES AND PLUGE
#---------------------------------------------------
#nodes
_DBL_LST_NODE 	= "vfDblList"
_DBL_LST_NODE 	= "vfDblListNull"
#---------------------------------------------------
#			CURVE NODES AND PLUGE
#---------------------------------------------------
#node
_BZR3_TNGNT_NODE = "vfBezier3Tngnt"
#---------------------------------------------------
#			CURVE NODES AND PLUGE
#---------------------------------------------------
#nodes
_BSPLINE_NODE 		= "vfCrvBSpline"
_BEZIER_NODE		= "vfCrvBezier"
_CURVE_NULL_NODE 	= "vfCrvNull"
_CURVE_GET_SINGLE 	= "vfCrvGetXForm"
_CURVE_LCTR_NODE = "vfCrvLocator"

_PLUG_IN_CRV 		= "inCurve"
_PLUG_OUT_CRV 		= "outCurve"
#--------------------------------------------
# import vufRM
# reload(vufRM)
# a = vufRM.selection_to_matrix_list()
# b = vufRM.add_null_matrix_to_list(a)
# c = vufRM.add_bspline(b)
# d = vufRM.add_curve_null(c)
# f = vufRM.add_curve_locator(d)
class helpers(object):
	def __init__(self):
		pass
	@staticmethod
	def get_distination_node(source_nodes = [], source_plugs = [], dist_type = [], depth = 10):
		res = {}
		if (depth == 0):
			return res
		for source in source_nodes:			
			if (cmds.objExists(source) == True):
				for plug in source_plugs:
					if (cmds.attributeQuery(plug, node = source, exists = True)  == True):
						connected = cmds.listConnections("{0}.{1}".format(source,plug),d = True, s = False, sh = True)						
						if (connected == None or len(connected) == 0):
							continue
						for obj in connected:
							if (cmds.nodeType(obj) in dist_type):
								res[obj] = obj
						res2 = helpers.get_distination_node(source_nodes 	= connected, 
															source_plugs 	= source_plugs,
															dist_type 		= dist_type,
															depth = depth - 1)
						res.update(res2)
		return res
								
	@staticmethod
	def get_source_node(dist_nodes = [], dist_plugs = [], source_type = [], depth = 10):
		res = {}
		if (depth == 0):
			return res
		for dist in dist_nodes:
			if (cmds.objExists(dist) == True):
				for plug in dist_plugs:
					if (cmds.attributeQuery(plug, node = dist, exists = True)  == True):
						connected = cmds.listConnections("{0}.{1}".format(dist,plug),d = False, s = True, sh = True)
						if (connected == None or len(connected) == 0):
							continue
						for obj in connected:
							if (cmds.nodeType(obj) in source_type):
								res[obj] = obj
						res2 = helpers.get_source_node(	dist_nodes 	= connected, 
														dist_plugs 	= dist_plugs,
														source_type = source_type,
														depth = depth - 1)
						res.update(res2)
		return res
						
				
#--------------------------------------------
#	Matrix Collector Class
#--------------------------------------------
class matrCollector(object):
	def __init__(self):
		pass
	def dags_to_matrix_list_node(self, dag_names = [], new_node_name = "matrixCollector", world_space = True):
		if (len(dag_names) == 0):
			return ""
		index 		= 0;
		node_name 		= cmds.createNode(_MATRIX_LIST_NODE )
		attr_name 	= "worldMatrix[0]"
		if (world_space == False):
			attr_name = "xformMatrix"
		if (node_name == None):
			return ""
		for dag in dag_names:
			cmds.connectAttr("{0}.{1}".format(dag, attr_name),"{0}.xForm[{1}]".format(node_name,index), f = True)
			index += 1
		node_name = cmds.rename(node_name,new_node_name)
		return node_name
	def selection_to_matrix_list(self, new_node_name = "matrixCollector", world_space = True):
		dags = cmds.ls(sl = True, ap = True, tr = True )
		return self.dags_to_matrix_list_node(dag_names = dags, new_node_name = new_node_name, world_space = world_space)
	def add_null_matrix_to_list( self, matrix_list_node_name, new_node_name = "matrixNull"):
		if (cmds.objExists(matrix_list_node_name) == True and  cmds.nodeType(matrix_list_node_name) == _MATRIX_LIST_NODE):
			node_name 		= cmds.createNode(_MATRIX_NULL_NODE )
			cmds.connectAttr("{0}.outMatrixList".format(matrix_list_node_name),"{0}.inMatrixList".format(node_name), f = True)
			node_name = cmds.rename(node_name,new_node_name)
			return node_name
		return ""
	def get_inputs(self, node_name = ""):
		list = []
		if (cmds.objExists(node_name) == True and cmds.nodeType(node_name) == _MATRIX_LIST_NODE):
			list =  cmds.listConnections( "{0}.xForm".format(node_name), d = False, s = True)
			if (list == None):
				list = []		
		return list
	@staticmethod
	def add_locator(transform_node, new_node_name = "MtrxLocator"):
		if (cmds.objExists(transform_node) == True and cmds.attributeQuery(_PLUG_OUT_MMTRX_LST,node = transform_node, exists = True) == True):
			loc_name 		= cmds.createNode(_MTRX_LCTR_NODE )
			cmds.connectAttr("{0}.{1}".format(transform_node,_PLUG_OUT_MMTRX_LST),"{0}.{1}".format(loc_name,_PLUG_IN_MMTRX_LST), f = True)
			transform = cmds.pickWalk( loc_name, direction='up' )[0]
			loc_name = cmds.rename(transform, new_node_name)
			return loc_name
		return ""

#--------------------------------------------
#	Double Collector Class
#--------------------------------------------
class dblCollector(object):
	def __init__(self):
		pass
	def addDoubleListNode(self, values = [], new_node_name = "doubleColector"):
		node_name 	= cmds.createNode(_DOUBLE_LIST_NODE )
		index 		= 0
		for val in values:
			cmds.setAttr("{0}.value[{1}]".format(node_name,index),val)
			index += 1
		node_name = cmds.rename(node_name,new_node_name)
		return node_name
	def addDoubleListNode(self, start = 0, end = 1, count = 5, new_node_name = "doubleColector"):
		node_name 	= cmds.createNode(_DOUBLE_LIST_NODE )
		step = (float)(end - start)
		step = step/(count - 1)
		for i in range(count):
			val = start + i*step
			cmds.setAttr("{0}.value[{1}]".format(node_name,i),val)
		node_name = cmds.rename(node_name,new_node_name)
		return node_name
class Bezier3Tangeents(object):
	def __init__(self):
		pass
	@staticmethod
	def dags_to_bezier_tangents(dag_names = [], new_node_name = "tangentGen", world_space = True):
		if (len(dag_names) < 2):
			return ''
		# choose attribute
		attr_name 	= 'worldMatrix[0]'
		if (world_space == False):
			attr_name = 'xformMatrix'
		# add enum attributes
		for dag in dag_names:			
			if (cmds.attributeQuery("mode", node = dag, exists = True) == True):
				cmds.deleteAttr( dag,at= "mode" )
			cmds.addAttr( dag,longName = "mode", attributeType =  "enum", enumName = "auto:simple:arc:pass", keyable = True  )
			if (cmds.attributeQuery("chord", node = dag, exists = True) == True):
				cmds.deleteAttr( dag,at= "chord" )
			cmds.addAttr( dag,longName = "chord", attributeType =  "float",  keyable = True  )
			cmds.setAttr( dag + ".chord",0.3)
		# create tangent node
		tngn_node 	= cmds.createNode(_BZR3_TNGNT_NODE )
		tr_null 	= cmds.createNode(_MTRX_LIST_NODE)
		index_tangent		= 0
		index_transform 	= 0
		first_tangent 	= ''
		for dag in dag_names:
			# connect 
			cmds.connectAttr('{0}.{1}'.format(	dag, attr_name),'{0}.inputGroup[{1}].xForm'.format(tngn_node,index_tangent), f = True)
			cmds.connectAttr('{0}.mode'.format(	dag),'{0}.inputGroup[{1}].mode'.format(tngn_node,index_tangent), f = True)
			cmds.connectAttr('{0}.chord'.format(dag),'{0}.inputGroup[{1}].chord'.format(tngn_node,index_tangent), f = True)
			# create tangents and nulls
			dec_a = cmds.createNode("decomposeMatrix")
			dec_b = cmds.createNode("decomposeMatrix")
			l_a = cmds.spaceLocator(name = "tngA#")[0]
			l_b = cmds.spaceLocator(name = "tngB#")[0]
			g_a = cmds.group (l_a,name = "nullA#")
			g_b = cmds.group (l_b,name = "nullB#")
			cmds.parent(g_a,dag)
			cmds.parent(g_b,dag)
			# connect out tangents to decomposes
			cmds.connectAttr('{0}.outputGroup[{1}].{2}'.format(tngn_node,index_tangent,'tangA'),'{0}.inputMatrix'.format(dec_a), f = True )
			cmds.connectAttr('{0}.outputGroup[{1}].{2}'.format(tngn_node,index_tangent,'tangB'),'{0}.inputMatrix'.format(dec_b), f = True )
			cmds.connectAttr('{0}.outputRotate'.format(		dec_a),'{0}.rotate'.format(		g_a) ,f = True )
			cmds.connectAttr('{0}.outputTranslate'.format(	dec_a),'{0}.translate'.format(	g_a),f = True )
			cmds.connectAttr('{0}.outputRotate'.format(		dec_b),'{0}.rotate'.format(		g_b),f = True )
			cmds.connectAttr('{0}.outputTranslate'.format(	dec_b),'{0}.translate'.format(	g_b),f = True )	
			#connect to transform list node 
			if (index_tangent == 0):
				first_tangent = l_b
			else:
				cmds.connectAttr('{0}.{1}'.format(l_b, attr_name),'{0}.{1}[{2}]'.format(tr_null,_PLUG_IN_MMTRX_BLDR,index_transform), f = True)				
				index_transform += 1
			cmds.connectAttr('{0}.outputGroup[{1}].{2}'.format(tngn_node, index_tangent,'knot'),'{0}.{1}[{2}]'.format(tr_null,_PLUG_IN_MMTRX_BLDR,index_transform), f = True)
			index_transform += 1
			cmds.connectAttr('{0}.{1}'.format(l_a, attr_name),'{0}.{1}[{2}]'.format(tr_null,_PLUG_IN_MMTRX_BLDR,index_transform), f = True)
			index_transform += 1
			index_tangent += 1
		cmds.connectAttr('{0}.{1}'.format(first_tangent, attr_name),'{0}.{1}[{2}]'.format(tr_null,_PLUG_IN_MMTRX_BLDR,index_transform), f = True)
		matrCollector.add_locator(transform_node = tr_null)
		#bezier = spline.add_bezier(matrix_null_node_name = tr_null)
		#spline.add_curve_locator(curve_node = bezier);
		return (tngn_node, tr_null)
#--------------------------------------------
#	Spline Node methods
#--------------------------------------------
class spline(object):
	@staticmethod
	def add_bspline( matrix_null_node_name, new_node_name = "bspline"):
		if (cmds.objExists(matrix_null_node_name) == True and  cmds.attributeQuery("outMatrixList",node = matrix_null_node_name, exists = True) == True):
			node_name 		= cmds.createNode(_BSPLINE_NODE )
			cmds.connectAttr("{0}.outMatrixList".format(matrix_null_node_name),"{0}.xformList".format(node_name), f = True)
			node_name = cmds.rename(node_name,new_node_name)
			return node_name
	@staticmethod
	def add_bezier(matrix_null_node_name, new_node_name = "bezier"):
		if (cmds.objExists(matrix_null_node_name) == True and  cmds.attributeQuery(_PLUG_OUT_MMTRX_LST,node = matrix_null_node_name, exists = True) == True):
				node_name 		= cmds.createNode(_BEZIER_NODE )
				cmds.connectAttr("{0}.{1}".format(matrix_null_node_name,_PLUG_OUT_MMTRX_LST),"{0}.{1}".format(node_name,_PLUG_IN_MMTRX_LST), f = True)
				node_name = cmds.rename(node_name,new_node_name)
				return node_name
	@staticmethod
	def add_null( curve_node, new_node_name = "curveNull"):
		if (cmds.objExists(curve_node) == True and cmds.attributeQuery("outCurve",node = curve_node, exists = True) == True):
			node_name 		= cmds.createNode(_CURVE_NULL_NODE )
			cmds.connectAttr("{0}.outCurve".format(curve_node),"{0}.inCurve".format(node_name), f = True)
			node_name = cmds.rename(node_name,new_node_name)
			return node_name
		return ""
	@staticmethod
	def add_locator( curve_node, new_node_name = "crvLocator"):
		if (cmds.objExists(curve_node) == True and cmds.attributeQuery(_PLUG_OUT_CRV,node = curve_node, exists = True) == True):
			node_name 		= cmds.createNode(_CURVE_LOCATOR_NODE )
			cmds.connectAttr("{0}.{1}".format(curve_node,_PLUG_OUT_CRV),"{0}.{1}".format(node_name,_PLUG_IN_CRV), f = True)
			transform = cmds.pickWalk( node_name, direction='up' )[0]
			node_name = cmds.rename(transform, new_node_name)
			return node_name
		return ""
	@staticmethod
	def add_bspline_fcurve( new_node_name = "fcurve", degree = 3, controls_count = 4):
		index = 0
		items = []
		for i in range(controls_count):
			item = cmds.spaceLocator(n = "loc_{0}_{1}".format(new_node_name,i))
			cmds.xform(item,t=(i/3.0, i/3.0, 0) )
			items.append(item[0])
		matrix_list = matrCollector().dags_to_matrix_list_node(dag_names = items, new_node_name = "matrix_{0}".format(new_node_name), world_space = False)
		spline 		= self.add_bspline(matrix_null_node_name = matrix_list,new_node_name = "spline_{0}".format(new_node_name))
		curve_draw  = self.add_curve_locator( curve_node = spline, new_node_name = "locator_{0}".format(new_node_name))
		parent = cmds.spaceLocator(n = new_node_name)
		cmds.parent(items, parent)
		cmds.parent(curve_draw, parent)
		return parent
	@staticmethod
	def get_spline( dag_names = [], curve_node_name = "", new_node_name = "getTransform"):
		if (cmds.objExists(curve_node_name) == True and cmds.attributeQuery("outCurve",node = curve_node_name, exists = True) == True):
			param = 0.0
			delta = 0
			if len(dag_names) > 1:
				delta = 1.0 / (len(dag_names) - 1)
			index = 1
			for item in dag_names:
				getNode = cmds.createNode(_CURVE_GET_SINGLE)
				cmds.connectAttr("{0}.outCurve".format(curve_node_name),"{0}.inCurve".format(getNode), f = True)
				cmds.connectAttr("{0}.parentInverseMatrix[0]".format(item),"{0}.parentInverse".format(getNode), f = True)
				cmds.connectAttr("{0}.translate".format(getNode),"{0}.translate".format(item), f = True)
				cmds.connectAttr("{0}.rotation".format(getNode),"{0}.rotate".format(item), f = True)
				cmds.connectAttr("{0}.scale".format(getNode),"{0}.scale".format(item), f = True)
				cmds.setAttr("{0}.offset".format(getNode), param)
				param = param + delta
				node_name = cmds.rename(getNode, "{0}_{1}".format(new_node_name,index))
				index += 1
	@staticmethod
	def test_bspline( controls_count = 10, driven_count = 1000):
		controls = []
		driven = []
		for i in range(controls_count):
			cube = cmds.polyCube( w = 1,h = 1,d = 1,sx = 1,sy = 1,sz = 1., ch = False)[0]
			cmds.setAttr("{0}.tx".format(cube),i * 2)
			controls.append(cube)
		for i in range(driven_count):
			cube = cmds.polyCube( w = 0.5,h = 0.5,d = 0.5,sx = 1,sy = 1,sz = 1., ch = False)[0]
			driven.append(cube)
		mc = matrCollector()
		a = mc.dags_to_matrix_list_node(dag_names = controls)
		b = mc.add_null_matrix_to_list(a)
		c = self.add_bspline(b)
		d = self.add_curve_null(c)
		f = self.add_curve_locator(d)
		self.get_spline(dag_names = driven,curve_node_name = d)
	@staticmethod
	def get_inputs_xfrom_spline(spline_name):
		if (cmds.objExists(spline_name) == True and cmds.nodeType(spline_name) in [_BSPLINE_NODE,_BEZIER_NODE]):
			lst = cmds.listConnections("{0}.xformList".format(spline_name), d = False, s = True)
			if (lst == None or len(lst) == 0):
				return ""
			return lst[0]
		return ""
#--------------------------------------------
#	Quaternion Node methods
#--------------------------------------------
#def add_rotate_transport_to_curve(curve_node,new_node_name = "curveTrasport"):
#	pass
	
