
import maya.cmds as cmds
import math

_MATRIX_LIST_NODE 	= "vfMtrxList"
_MATRIX_NULL_NODE 	= "vfMtrxListNull"
_DOUBLE_LIST_NODE 	= "vfDblList"

_BSPLINE_NODE 		= "vfCrvBSpline"
_BEZIER_NODE		= "vfCrvBezier"
_CURVE_NULL_NODE 	= "vfCrvNull"
_CURVE_GET_SINGLE 	= "vfCrvGetXForm"
_CURVE_LOCATOR_NODE = "vfCrvLocator"
#--------------------------------------------
# import vufRM
# reload(vufRM)
# a = vufRM.selection_to_matrix_list()
# b = vufRM.add_null_matrix_to_list(a)
# c = vufRM.add_bspline(b)
# d = vufRM.add_curve_null(c)
# f = vufRM.add_curve_locator(d)
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
#--------------------------------------------
#	Spline Node methods
#--------------------------------------------
class spline(object):
	def add_bspline(self, matrix_null_node_name, new_node_name = "bspline"):
		if (cmds.objExists(matrix_null_node_name) == True and  cmds.attributeQuery("outMatrixList",node = matrix_null_node_name, exists = True) == True):
			node_name 		= cmds.createNode(_BSPLINE_NODE )
			cmds.connectAttr("{0}.outMatrixList".format(matrix_null_node_name),"{0}.xformList".format(node_name), f = True)
			node_name = cmds.rename(node_name,new_node_name)
			return node_name
	def add_bezier(matrix_null_node_name, new_node_name = "bezier"):
		if (cmds.objExists(matrix_null_node_name) == True and  cmds.attributeQuery("outMatrixList",node = matrix_null_node_name, exists = True) == True):
				node_name 		= cmds.createNode(vufCurveBezier )
				cmds.connectAttr("{0}.outMatrixList".format(matrix_null_node_name),"{0}.xformList".format(node_name), f = True)
				node_name = cmds.rename(node_name,new_node_name)
				return node_name
	def add_curve_null(self, curve_node, new_node_name = "curveNull"):
		if (cmds.objExists(curve_node) == True and cmds.attributeQuery("outCurve",node = curve_node, exists = True) == True):
			node_name 		= cmds.createNode(_CURVE_NULL_NODE )
			cmds.connectAttr("{0}.outCurve".format(curve_node),"{0}.inCurve".format(node_name), f = True)
			node_name = cmds.rename(node_name,new_node_name)
			return node_name
		return ""
	def add_curve_locator(self, curve_node, new_node_name = "curveLocator"):
		if (cmds.objExists(curve_node) == True and cmds.attributeQuery("outCurve",node = curve_node, exists = True) == True):
			node_name 		= cmds.createNode(_CURVE_LOCATOR_NODE )
			cmds.connectAttr("{0}.outCurve".format(curve_node),"{0}.inCurve".format(node_name), f = True)
			transform = cmds.pickWalk( node_name, direction='up' )[0]
			node_name = cmds.rename(transform, new_node_name)
			return node_name
		return ""
	def add_bspline_fcurve(self, new_node_name = "fcurve", degree = 3, controls_count = 4):
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
	def get_spline(self, dag_names = [], curve_node_name = "", new_node_name = "getTransform"):
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
	def test_bspline(self,controls_count = 10, driven_count = 1000):
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
#--------------------------------------------
#	Quaternion Node methods
#--------------------------------------------
#def add_rotate_transport_to_curve(curve_node,new_node_name = "curveTrasport"):
#	pass
	
