import maya.cmds as cmds
from vfRigMaster import vfUI as ui
from vfRigMaster import vfUtils as utils

_NODE_TYPE 			= "vfCrvBSpline"
_COLLECTOR_TYPE 	= "vfMtrxList"
_TITLE 				= "BSpline"
_COLOR 				= (0.5,0.3,0.3)
_WIDTH 				= 650
_HEIGHT 			= 120
_EMPTY_ITEM 			= '----------NONE----------'
_FROM_SELECTIOM_ITEM 	= '-----From Selection-----'



class BSplineGui(ui.Wnd):
	def __init__(self):		
		super(BSplineGui, self).__init__(title = _TITLE, nodeType = _NODE_TYPE, color = _COLOR, width = _WIDTH, height = _HEIGHT)
		self.create_window()
		
	def create_window(self,*args):
		if ( cmds.window( self.wnd, ex = True ) == True):
			cmds.window( self.wnd, e = True, visible = True, iconify = False )
			return
		super(BSplineGui, self).create_window()
		cmds.frameLayout (label= "Create", collapsable =True, collapse = False )
		cmds.rowLayout(	numberOfColumns 	= 3,						
						adjustableColumn3 	= 2, 
						columnAlign			=(1, 'right'),
						columnAttach=[	(1, 'both', 2),
										(2, 'both', 2),
										(3, 'both', 2)])
		cmds.symbolButton(image = "vufIcons/CREATE.png",ann="Create BSpline",c = self.create_bs )		
		self.listFromUI 	= cmds.optionMenu(label = "Create From: ")
		cmds.button(label 	= "Select Candidate", c = self.select_inputs)
		
		
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
		
		
		cmds.setParent(self.cLayout)
		cmds.frameLayout (label= "Locator", collapsable =True, collapse = True )	
		cmds.rowLayout(	numberOfColumns 	= 3,
						columnAlign			=(1, 'right'),
						columnAttach=[	(1, 'both', 2),
										(2, 'both', 2),
										(3, 'both', 2)])		
		cmds.symbolButton(image = "vufIcons/VESELECT.png", ann="Select Locator",bgc=(0.1,0.1,0.1),c = self.select_node)
		cmds.symbolButton(image = "vufIcons/ADD.png",ann="Create Locator for Bspline",c = self.select_node )
		cmds.symbolButton(image = "vufIcons/DELETE.png",ann="Delete Locator forBspline",c = self.select_node )
		
		
		cmds.setParent(self.cLayout)
		cmds.frameLayout (label= "GetQuats", collapsable =True, collapse = True )
		cmds.rowLayout()
		cmds.button(label 	= "Input Collector", c = self.select_inputs)
		
		
		self.refresh_ui()
	
	def refresh_ui(self,*args):
		super(BSplineGui, self).refresh_ui()
		allNodes =  cmds.ls(type = _COLLECTOR_TYPE,l = True)
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
		inputs = cmds.optionMenu(self.listFromUI,query=True,v=True)	
		
		if (inputs == _FROM_SELECTIOM_ITEM):
			mc 			= utils.matrCollector()
			collector 	= mc.selection_to_matrix_list();			
			crv 		= utils.spline()
			spline 		= crv.add_bspline(matrix_null_node_name = collector)
			if (spline == ""):
				cmds.error("Failed to collect matrices.")
			self.refresh_ui()
			
		
	def select_inputs(self,*args):
		pass
		
	def select_inputs(self,*args):
		pass

	def item_selected(self, *args):
		crv_node = cmds.optionMenu(self.listUI,query=True,v=True)	
		print "selected", crv_node