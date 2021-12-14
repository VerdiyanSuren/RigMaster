import maya.cmds as cmds
from vfRigMaster import vfUI as ui
from vfRigMaster import vfUtils as utils

_NODE_TYPE 			= "vfCrvBSpline"
_COLLECTOR_TYPE 	= "vfMtrxList"
_COLLECTOR_NULL		= "vfMtrxListNull"
_LOCATOR_TYPE		= "vfCrvLocator"
_CURVE_NULL			= "vfCrvNull"
_TITLE 				= "BSpline"
_COLOR 				= (0.5,0.3,0.3)
_WIDTH 				= 650
_HEIGHT 			= 120
_EMPTY_ITEM 			= '----------NONE----------'
_FROM_SELECTIOM_ITEM 	= '-----From Selection-----'
_ADDNULL  				= False
_ADDLOCATOR				= True



class BSplineGui(ui.Wnd):
	def __init__(self):		
		super(BSplineGui, self).__init__(title = _TITLE, nodeType = _NODE_TYPE, color = _COLOR, width = _WIDTH, height = _HEIGHT)
		self.create_window()
		
	def create_window(self,*args):
		if ( cmds.window( self.wnd, ex = True ) == True):
			cmds.window( self.wnd, e = True, visible = True, iconify = False )
			return
		# Creation
		super(BSplineGui, self).create_window()
		cmds.frameLayout (label= "Create", collapsable =True, collapse = False )
		cmds.rowLayout(	numberOfColumns 	= 6,						
						adjustableColumn6 	= 4, 
						columnAlign			=(1, 'right'),
						columnAttach=[	(1, 'both', 2),
										(2, 'both', 2),
										(3, 'both', 2),
										(4, 'both', 2),
										(5, 'both', 2)])
		cmds.symbolButton(image = "vufIcons/CREATE.png",ann="Create BSpline",c = self.create_bs, bgc = (0.4,0.2,0.2) )
		self.checkNull 		= cmds.checkBox( label 	= "Add Null", 		v = _ADDNULL)
		self.checkLctr 		= cmds.checkBox( label 	= "Add Locator", 	v = _ADDLOCATOR)
		self.listFromUI 	= cmds.optionMenu(label = "Create From: ")
		cmds.button(label 	= "Select Candidate", c = self.select_inputs)
		cmds.symbolButton(image = "vufIcons/DELETE.png",ann="Delete Transform Collector",c = self.delete_bs, bgc=(0.4,0.2,0.2))
		
		# Selection
		cmds.setParent(self.cLayout)
		cmds.frameLayout (label= "Select", collapsable =True, collapse = True )
		cmds.rowLayout(	numberOfColumns 	= 4,						
						adjustableColumn4 	= 4, 
						columnAlign			=(1, 'right'),
						columnAttach=[	(1, 'both', 2),
										(2, 'both', 2),
										(3, 'both', 2),
										(4, 'both', 2)])
		cmds.button(label 	= "Input Collector", c = self.select_inputs)
		cmds.button(label 	= "Drivers", c = self.select_inputs)
		cmds.button(label 	= "Drivens", c = self.select_inputs)
		cmds.button(label 	= "GetQuats", c = self.select_inputs)
		
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
		
		# Get Quats
		cmds.setParent(self.cLayout)
		cmds.frameLayout (label= "GetQuats", collapsable =True, collapse = True )
		cmds.rowLayout()
		cmds.button(label 	= "Input Collector", c = self.select_inputs)
		
		
		self.refresh_ui()
	
	def refresh_ui(self,*args):
		super(BSplineGui, self).refresh_ui()
		allNodes =  cmds.ls(type = [_COLLECTOR_TYPE,_COLLECTOR_NULL],l = True)
		entries = cmds.optionMenu( self.listFromUI , query=True, itemListLong=True )
		if entries != None:
			for i in entries:	
				cmds.deleteUI( i, mi = True  )
		cmds.menuItem( label = _FROM_SELECTIOM_ITEM, parent = self.listFromUI)
		if len(allNodes) == 0:
			return
		for i in allNodes:
			cmds.menuItem(label=i,parent = self.listFromUI)	
			
	def create_bs(self,*args):
		inputs 		= cmds.optionMenu(self.listFromUI,query=True,v=True)	
		_ADDNULL 	= cmds.checkBox(self.checkNull, q = True, v = True)
		_ADDLOCATOR	= cmds.checkBox(self.checkLctr, q = True, v = True)
		collector = ""
		defaultName = ui.EnterNameEx().get_name( text = "BSpline" )
		if (defaultName == "dismiss"):
			return
		if (inputs == _FROM_SELECTIOM_ITEM):
			mc 			= utils.matrCollector()
			collector 	= mc.selection_to_matrix_list( new_node_name = "{0}mClctr{1}{2}".format(ui._PREFIX,defaultName,ui._SUFIX) )
		else:
			collector 	= inputs
			
		crv 		= utils.spline()
		spline 		= crv.add_bspline( matrix_null_node_name = collector, new_node_name = "{0}{1}{2}".format(ui._PREFIX,defaultName,ui._SUFIX))
		
		lastNode 	= spline
		if (spline == ""):
			cmds.error("Failed to create bspline.")
		print (spline)
		if (_ADDNULL == True):
			lastNode = crv.add_curve_null( curve_node = spline, new_node_name = "{0}{1}CrvNull{2}".format(ui._PREFIX,defaultName, ui._SUFIX) )
			print (lastNode)
		if (_ADDLOCATOR == True):
			 print (crv.add_curve_locator(curve_node = lastNode, new_node_name = "{0}{1}Lctr{1}".format(ui._PREFIX,defaultName,ui._SUFIX) ) )
		self.refresh_ui()
		self.select_option_item( itemName = spline, optionUI = self.listUI)
		self.select_option_item( itemName = collector, optionUI = self.listFromUI)
	
	def delete_bs(self,*args):
		item 			= cmds.optionMenu(self.listUI,query=True,v=True)
		connectedNull 	= cmds.listConnections("{0}.outCurve".format(item),d = True, s = False, type = _CURVE_NULL )
		connectedLoc 	= cmds.listConnections("{0}.outCurve".format(item),d = True, s = False, type = _LOCATOR_TYPE )
		if (connectedLoc == None):
			connectedLoc = []
			
		if (connectedNull != None and len(connectedNull) > 0):		
			for i in connectedNull:
				list = cmds.listConnections("{0}.outCurve".format(i),d = True, s = False, type = _LOCATOR_TYPE )
				if (list != None):
					connectedLoc = connectedLoc + list
					
			res = cmds.confirmDialog( title='Confirm', message='Delete Connected Null {0}?', button=['Yes','No'], defaultButton='Yes', cancelButton='No', dismissString='dismiss' )
			if (res == "Yes"):
				cmds.delete(connectedNull)
			if (res == "dismiss"):
				return
		
		if ( len(connectedLoc) > 0):
			res = cmds.confirmDialog( title='Confirm', message='Delete Connected Locator {0}?', button=['Yes','No'], defaultButton='Yes', cancelButton='No', dismissString='dismiss' )
			if (res == "Yes"):
				cmds.delete(connectedLoc)
			if (res == "dismiss"):
				return
				
		if (cmds.objExists(item) == True):
			cmds.delete(item)	
		
		self.refresh_ui()
		
	def select_inputs(self,*args):
		inputs = cmds.optionMenu(self.listFromUI,query=True,v=True)
		if (inputs == _FROM_SELECTIOM_ITEM):
			selNodes = cmds.ls(sl = True)
			cmds.select(selNodes,r = True)
			return
		cmds.select(inputs, r = True)

	def item_selected(self, *args):
		crv_node = cmds.optionMenu(self.listUI,query=True,v=True)
		crv 		= utils.spline()
		res 		= crv.get_inputs_xfrom_spline(spline_name = crv_node)
		if (res != ""):
			self.select_option_item( itemName = res, optionUI = self.listFromUI)
			return
		self.select_option_item( itemName = _FROM_SELECTIOM_ITEM, optionUI = self.listFromUI)
		
	def get_node_from_scene(self,*args):
		nodes = cmds.ls(sl = True)
		# if selected is bsline
		list = []
		for i in nodes:
			if ( cmds.nodeType(i) == _NODE_TYPE ):
				list.append(i)
		if (len(list) == 1):
			self.refresh_ui()
			self.select_option_item(list[0], self.listUI)
			self.item_selected()
			return
		 
		if (len(list) > 1):
			res = ui.ChooseListDialog().choose(list = list)
			if (res != "dismiss"):
				self.refresh_ui()
				self.select_option_item(res, self.listUI)
				self.item_selected()
				return
			else:
				cmds.optionMenu(self.listUI,edit=True,sl = 1)
				self.item_selected()
				return
		# if selected is locator
		shape = cmds.listRelatives(nodes[0])
		if (shape != None and len(shape) != 0 and cmds.nodeType(shape[0]) == _LOCATOR_TYPE ):
			list = utils.helpers.get_source_node(	dist_nodes = [shape[0]],
													dist_plugs = ['inCurve'],
													source_type = [_NODE_TYPE]).keys()
			if (len(list) == 1):
				self.refresh_ui()
				self.select_option_item(list[0], self.listUI)
				self.item_selected()
				return
				
			if (len(list) > 1):
				res = ui.ChooseListDialog().choose(list = list)
				if (res != "dismiss"):
					self.refresh_ui()
					self.select_option_item(res, self.listUI)
				self.item_selected()
				return
			else:
				cmds.optionMenu(self.listUI,edit=True,sl = 1)
				self.item_selected()
				return
		# if selected is controls of matrix collector
		list = []
		for i in nodes:
			lst = utils.helpers.get_distination_node(	source_nodes = nodes,
														source_plugs = ['worldMatrix','xformMatrix','outMatrixList'],
														dist_type = [_NODE_TYPE])
			list = list + lst.keys()
		if (len(list) == 1):
			self.refresh_ui()
			self.select_option_item(list[0], self.listUI)
			self.item_selected()
			return
			
		if (len(list) > 1):
			res = ui.ChooseListDialog().choose(list = list)
			if (res != "dismiss"):
				self.refresh_ui()
				self.select_option_item(res, self.listUI)
				self.item_selected()
				return
						
		cmds.optionMenu(self.listUI,edit=True,sl = 1)
		self.item_selected()
		
	def select_locator(self,*args):
		crv_node = cmds.optionMenu(self.listUI,query=True,v=True)
		locators = utils.helpers.get_distination_node(	source_nodes = [crv_node],
														source_plugs = ['outCurve'],
														dist_type = [_LOCATOR_TYPE]).keys()
		cmds.select(locators,r = True)
		
	def add_locator(self,*args):
		crv_node = cmds.optionMenu(self.listUI,query=True,v=True)
		connectedNull 	= cmds.listConnections("{0}.outCurve".format(crv_node),d = True, s = False, type = _CURVE_NULL )
		connectedLoc 	= cmds.listConnections("{0}.outCurve".format(crv_node),d = True, s = False, type = _LOCATOR_TYPE )
		if (connectedLoc != None and len(connectedLoc) != 0):
			cmds.select(connectedLoc, r = True)
			return
		if (connectedNull != None and len(connectedNull) != 0):
			lctr = utils.spline().add_curve_locator( curve_node = connectedNull[0])
			cmds.select(lctr, r = True)
			return
		lctr = utils.spline().add_curve_locator( curve_node = crv_node)
		cmds.select(lctr, r = True)
		
	def remove_locator(self,*args):
		crv_node = cmds.optionMenu(self.listUI,query=True,v=True)
		locators = utils.helpers.get_distination_node(	source_nodes = [crv_node],
														source_plugs = ['outCurve'],
														dist_type = [_LOCATOR_TYPE]).keys()
														
		transformList = cmds.listRelatives(locators, parent=True, fullPath=True)
		cmds.delete(transformList, s = True)
		
