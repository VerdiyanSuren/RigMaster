import maya.cmds as cmds
from vfRigMaster import vfUI as ui
from vfRigMaster import vfUtils as utils

_NODE_TYPE 	= "vfMtrxList"
_NODE_NULL	= "vfMtrxListNull"
_TITLE 		= "Transform_Collector"
_COLOR 		= (0.3,0.5,0.3)
_WIDTH 		= 650
_HEIGHT 	= 120
_WORLD 		= True
_NULL  		= False

		
class TCGui(ui.Wnd):
	def __init__(self):		
		super(TCGui, self).__init__(title = _TITLE, nodeType = _NODE_TYPE, color = _COLOR, width = _WIDTH, height = _HEIGHT)
		self.create_window()
		
	def create_tc(self,*argc):
		_WORLD 	= cmds.checkBox(self.checkWorld, q = True, v = True)
		_NULL 	= cmds.checkBox(self.checkNull, q = True, v = True)
		mc = utils.matrCollector()
		defaultName = ui.EnterNameEx().get_name( text = "mClctr")
		#prefix = ui.EnterNameEx.get_prefix()
		#sufix  = ui.EnterNameEx.get_sufix()
		if (defaultName == "dismiss"):
			return
		node = mc.selection_to_matrix_list(world_space = _WORLD, new_node_name = "{0}{1}{2}".format(ui._PREFIX,defaultName,ui._SUFIX));
		if (node == ""):
			cmds.optionMenu(self.listUI,edit=True,sl=1)
			cmds.error("Failed to collect matrices.")		
		print (node)
		if (_NULL == True):
			print (mc.add_null_matrix_to_list(matrix_list_node_name = node, new_node_name = "{0}{1}Null{2}".format(ui._PREFIX,defaultName,ui._SUFIX )))
		self.refresh_ui()
		self.select_option_item( optionUI = self.listUI, itemName = node)		
	
	def create_window(self,*args):
		if ( cmds.window( self.wnd, ex = True ) == True):
			cmds.window( self.wnd, e = True, visible = True, iconify = False )
			return
		# Creation
		print "HERE"
		super(TCGui, self).create_window()
		cmds.frameLayout (label= "Create", collapsable = True, collapse = False )
		cmds.rowLayout(	numberOfColumns 	= 5,						
						adjustableColumn5 	= 4, 
						columnAlign			=(1, 'right'),
						columnAttach=[	(1, 'both', 2),
										(2, 'both', 2),
										(3, 'both', 2),
										(4, 'both', 2),
										(5, 'both', 2)])
		cmds.symbolButton(image = "vufIcons/CREATE.png",ann="Create Transform Collector from selection",c = self.create_tc, bgc=(0.4,0.2,0.2))
		self.checkWorld 	= cmds.checkBox( label	='World', 		v = _WORLD)
		self.checkNull 		= cmds.checkBox( label 	= "Add Null", 	v = _NULL)
		cmds.button(label 	= "Select inputs", c 	= self.select_inputs)
		cmds.symbolButton(image = "vufIcons/DELETE.png",ann="Delete Transform Collector",c = self.delete_tc, bgc=(0.4,0.2,0.2))
		# Selection
		cmds.setParent(self.cLayout)
		cmds.frameLayout (label= "Select", collapsable = True, collapse = True )
		cmds.rowLayout(	numberOfColumns 	= 3,						
						adjustableColumn3 	= 3, 
						columnAlign			=(1, 'right'),
						columnAttach=[	(1, 'both', 2),
										(2, 'both', 2),
										(3, 'both', 2)])
		cmds.button(label 	= "Drivers", c = self.select_inputs)
		cmds.button(label 	= "Drivens", c = self.select_inputs)
		cmds.button(label 	= "GetDecompose", c = self.select_inputs)
		# Locator
		cmds.setParent(self.cLayout)
		cmds.frameLayout (label= "Locator", collapsable =True, collapse = True )	
		cmds.rowLayout(	numberOfColumns 	= 3,
						columnAlign			=(1, 'right'),
						columnAttach=[	(1, 'both', 2),
										(2, 'both', 2),
										(3, 'both', 2)])		
		cmds.symbolButton(image = "vufIcons/SELECT.png", 	ann="Select Locator",			bgc=(0.2,0.4,0.2), c = self.select_locator)
		cmds.symbolButton(image = "vufIcons/ADD.png",		ann="Add Locator for Bspline",	bgc=(0.2,0.4,0.2), c = self.add_locator )
		cmds.symbolButton(image = "vufIcons/MINUS.png",		ann="Remoce Locator forBspline",bgc=(0.2,0.4,0.2), c = self.remove_locator )
		
		# GetDecompose
		cmds.setParent(self.cLayout)
		cmds.frameLayout (label= "GetDecompose", collapsable =True, collapse = True )
		cmds.rowLayout()
		cmds.button(label 	= "Decompose", c = self.select_inputs)
		
		
		self.refresh_ui()
	
	def delete_tc(self,*args):		
		item 		= cmds.optionMenu(self.listUI,query=True,v=True)
		connected 	= cmds.listConnections("{0}.outMatrixList".format(item,),d = True, s = False, type = _NODE_NULL )
		if (connected != None and len(connected) > 0):
			res = cmds.confirmDialog( title='Confirm', message='Delete Connected Null?', button=['Yes','No'], defaultButton='Yes', cancelButton='No', dismissString='No' )
			if (res == "Yes"):
				cmds.delete(connected)
		if (cmds.objExists(item) == True):
			cmds.delete(item)
		self.refresh_ui()
			
	def select_inputs(self,*args):
		item 	= cmds.optionMenu(self.listUI,query=True,v=True)
		mc 		= utils.matrCollector()
		nodes 	= mc.get_inputs(node_name = item)
		cmds.select(nodes, r = True)
		print (nodes)
		
	def get_node_from_drivers(self,*args):
		nodes = cmds.ls(sl = True)
		list = []
		list = [i for i in nodes if cmds.nodeType(i) == _NODE_TYPE]
		lst = utils.helpers.get_distination_node(	source_nodes = nodes,
													source_plugs = ['worldMatrix','xformMatrix'],
													dist_type = [_NODE_TYPE])
		list = list + lst.keys()
		if (len(list) == 1):
			self.select_option_item(list[0], self.listUI)
			return
			
		if (len(list) > 1):
			res = ui.ChooseListDialog().choose(list = list)
			if (res != "dismiss"):
				self.select_option_item(res, self.listUI)
				return
				
		cmds.optionMenu(self.listUI,edit=True,sl = 1)
	
	def get_node_from_drivens(self,*args):
		nodes = cmds.ls(sl = True)
		list = [i for i in nodes if cmds.nodeType(i) == _NODE_TYPE]
		lst = utils.helpers.get_source_node(	dist_nodes = nodes, 
										dist_plugs = ['outMAtrixList'], 
										source_type = [_NODE_TYPE])
		list = list + lst.keys()								
		if (len(list) == 1):
			self.select_option_item(list[0], self.listUI)
			return
		if (len(list) > 1):
			res = ui.ChooseListDialog().choose(list = list)
			if (res != "dismiss"):
				self.select_option_item(res, self.listUI)
				return
		cmds.optionMenu(self.listUI,edit=True,sl = 1)
		
	def select_locator(self,*args):
		pass
	
	def add_locator(self,*args):
		pass
	
	def remove_locator(self,*args):
		pass


