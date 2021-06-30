
import maya.cmds as cmds

_MATRIX_LIST_NODE 	= "vufMatrixList"
_MATRIX_NULL_NODE 	= "vufMatrixListNull"

_BSPLINE_NODE 		= "vufCurveBSpline"
_CURVE_NULL_NODE 	= "vufCurveNull"
_CURVE_LOCATOR_NODE = "vufCurveLocator"
#--------------------------------------------
# import vufRM
# reload(vufRM)
# a = vufRM.selection_to_matrix_list()
# b = vufRM.add_null_matrix_to_list(a)
# c = vufRM.add_bspline(b)
# d = vufRM.add_curve_null(c)
# f = vufRM.add_curve_locator(d)
#--------------------------------------------
#	Matrix Node methods
#--------------------------------------------
def dags_to_matrix_list_node( dag_names = [], new_node_name = "matrixCollector", world_space = True):
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

def selection_to_matrix_list(new_node_name = "matrixCollector", world_space = True):
	dags = cmds.ls(sl = True, ap = True, tr = True )
	return dags_to_matrix_list_node(dag_names = dags, new_node_name = new_node_name, world_space = world_space)


def add_null_matrix_to_list( matrix_list_node_name, new_node_name = "matrixNull"):
	if (cmds.objExists(matrix_list_node_name) == True and  cmds.nodeType(matrix_list_node_name) == _MATRIX_LIST_NODE):
		node_name 		= cmds.createNode(_MATRIX_NULL_NODE )
		cmds.connectAttr("{0}.outMatrixList".format(matrix_list_node_name),"{0}.inMatrixList".format(node_name), f = True)
		node_name = cmds.rename(node_name,new_node_name)
		return node_name

#--------------------------------------------
#	BSpline Node methods
#--------------------------------------------
def add_bspline(matrix_null_node_name, new_node_name = "bspline"):
	if (cmds.objExists(matrix_null_node_name) == True and  cmds.attributeQuery("outMatrixList",node = matrix_null_node_name, exists = True) == True):
		node_name 		= cmds.createNode(_BSPLINE_NODE )
		cmds.connectAttr("{0}.outMatrixList".format(matrix_null_node_name),"{0}.xformList".format(node_name), f = True)
		
		node_name = cmds.rename(node_name,new_node_name)
		return node_name

def add_curve_null(curve_node, new_node_name = "curveNull"):
	if (cmds.objExists(curve_node) == True and cmds.attributeQuery("outCurve",node = curve_node, exists = True) == True):
		node_name 		= cmds.createNode(_CURVE_NULL_NODE )
		cmds.connectAttr("{0}.outCurve".format(curve_node),"{0}.inCurve".format(node_name), f = True)
		node_name = cmds.rename(node_name,new_node_name)
		return node_name
	return ""

def add_curve_locator(curve_node, new_node_name = "curveLocator"):
	if (cmds.objExists(curve_node) == True and cmds.attributeQuery("outCurve",node = curve_node, exists = True) == True):
		node_name 		= cmds.createNode(_CURVE_LOCATOR_NODE )
		cmds.connectAttr("{0}.outCurve".format(curve_node),"{0}.inCurve".format(node_name), f = True)
		transform = cmds.pickWalk( node_name, direction='up' )[0]
		node_name = cmds.rename(transform, new_node_name)
		return node_name
	return ""

def add_bspline_fcurve(new_node_name = "fcurve", degree = 3, controls_count = 4):
	index = 0
	items = []
	for i in range(controls_count):
		item = cmds.spaceLocator(n = "loc_{0}_{1}".format(new_node_name,i))
		cmds.xform(item,t=(i/3.0, i/3.0, 0) )
		items.append(item[0])
	matrix_list = dags_to_matrix_list_node(dag_names = items, new_node_name = "matrix_{0}".format(new_node_name), world_space = False)
	spline 		= add_bspline(matrix_null_node_name = matrix_list,new_node_name = "spline_{0}".format(new_node_name))
	curve_draw  = add_curve_locator( curve_node = spline, new_node_name = "locator_{0}".format(new_node_name))
	parent = cmds.spaceLocator(n = new_node_name)
	cmds.parent(items, parent)
	cmds.parent(curve_draw, parent)
	return parent
#--------------------------------------------
#	Bezier Node methods
#--------------------------------------------
#--------------------------------------------
#	quaternion Node methods
#--------------------------------------------

