import maya.cmds as cmds
from vfRigMaster import vfUI as ui
from vfRigMaster import vfUtils as utils

_NODE_TYPE 	= "vfMtrxList"
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
		_NULL 	= cmds.checkBox(self.checkWorld, q = True, v = True)
		mc = utils.matrCollector()
		node = mc.selection_to_matrix_list(world_space = _WORLD);
		if (node == ""):
			cmds.optionMenu(self.listUI,edit=True,sl=1)
			cmds.error("Failed to collect matrices.")		
		print (node)
		print mc.add_null_matrix_to_list(matrix_list_node_name = node) 
		self.refresh_ui()
		N = cmds.optionMenu(self.listUI, query=True, itemListLong = True)	
		ndx = 1
		for i in N:
			s = cmds.menuItem(i, query=True, label = True)
			if s == node:
				cmds.optionMenu(self.listUI,edit=True,sl=ndx)	
				return
			ndx = ndx + 1
	
	def create_window(self,*args):
		if ( cmds.window( self.wnd, ex = True ) == True):
			cmds.window( self.wnd, e = True, visible = True, iconify = False )
			return
		super(TCGui, self).create_window()
		cmds.frameLayout (label= "Create", collapsable =True, collapse = False )
		cmds.rowLayout(	numberOfColumns 	= 4,						
						adjustableColumn4 	= 4, 
						columnAlign			=(1, 'right'),
						columnAttach=[	(1, 'both', 2),
										(2, 'both', 2),
										(3, 'both', 2),
										(4, 'both', 2)])
		cmds.symbolButton(image = "vufIcons/CREATE.png",ann="Create Transform Collector from selection",c = self.create_tc, bgc=(0.8,0.1,0.1))
		self.checkWorld 	= cmds.checkBox( label	='World', 		v = _WORLD)
		self.addNull 		= cmds.checkBox( label 	= "Add Null", 	v = _NULL)
		cmds.button(label 	= "Select inputs", c 	= self.select_inputs)
		
		self.refresh_ui()
	
	def select_inputs(self,*args):
		item 	= cmds.optionMenu(self.listUI,query=True,v=True)
		mc 		= utils.matrCollector()
		nodes 	= mc.get_inputs(node_name = item)
		cmds.select(nodes, r = True)
		print (nodes)
		
	def get_node_from_scene(self,*args):
		nodes = cmds.ls(sl = True)
		for i in nodes:
			candidates = cmds.listConnections(i,d = True, s = False)
			if candidates == None or len(candidates) == 0:
				continue
			for j in candidates:
				if (cmds.nodeType(j) == _NODE_TYPE):
					self.refresh_ui()
					N = cmds.optionMenu(self.listUI, query=True, itemListLong = True)	
					ndx = 1
					for item in N:
						s = cmds.menuItem(item, query=True, label = True)
						if s == j:
							cmds.optionMenu(self.listUI,edit=True,sl=ndx)	
							return
						ndx = ndx + 1
		cmds.optionMenu(self.listUI,edit=True,sl=1)			
					
			
					
					
