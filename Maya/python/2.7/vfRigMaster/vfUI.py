import maya.cmds as cmds

_EMPTY_ITEM = '----------NONE----------'
_WIDTH 		= 650
_HEIGHT 	= 120

#	MENU ROUTINES
def does_window_menu_exist( menuName, windowName = 'MayaWindow'):
	# search in main window by label
    arr = cmds.window(windowName,q = True, ma = True)
    l_candidate = None
    for i in arr:
        b = cmds.menu(i,q = True,l = True)
        if (b == menuName):
            l_candidate = i
    return l_candidate

def delete_window_menu( menuName, windowName = 'MayaWindow'):
	# delete menu by label from main window
	flag = True
	while flag:
		b = does_window_menu_exist(menuName = menuName,windowName = windowName )
		print (b)
		if (b != None):
			cmds.deleteUI(b,m = True)
		else:
			flag = False
			
def create_window_menu( menuName,windowName = 'MayaWindow' , tear = True):
	delete_window_menu(menuName, windowName)
	return cmds.menu(label = menuName, p = windowName,tearOff = tear)



def add_submenu( menuName,parentMenu = "", tear = True):
	if ( parentMenu != ""):
		cmds.setParent( parentMenu, menu=True )
	return cmds.menuItem(label = menuName, subMenu=True, tearOff = tear )	

def add_menu( menuName,parentMenu = "", command = "", optionCommand = "", annotation = ""):
	if ( parentMenu != ""):
		cmds.setParent( parentMenu, menu=True )
	res = cmds.menuItem(label = menuName, c = "", ann = annotation )
	if ( optionCommand != ""):
		cmds.menuItem(optionBox=True, 	c = optionCommand)
	return res
	

# WINDOW ROUTINES
	
class Wnd(object):
	def __init__(self, title = "Test", nodeType = "Unknown", color = (0.5,0.3,0.3), width = _WIDTH, height = _HEIGHT):
		self.wnd 		= nodeType + "Wnd"
		self.title 		= title
		self.nodeType 	= nodeType
		self.width 		= width
		self.height 	= height
		self.color 		= color
		#self.create_window()
		
	def create_window(self):
		if ( cmds.window( self.wnd, ex = True ) == True):
			cmds.window( self.wnd, e = True, visible = True, iconify = False )
			return
		self.wnd  		= cmds.window( self.wnd, menuBar=True, title= self.title, widthHeight=(self.width,self.height), resizeToFitChildren = True)
		self.cLayout 	= cmds.columnLayout( adjustableColumn = True)
		cmds.rowLayout(	numberOfColumns 	= 4,						
						adjustableColumn4 	= 3, 
						columnAlign			=(1, 'right'),
						columnAttach=[	(1, 'both', 2),
										(2, 'both', 2),
										(3, 'both', 20),
										(4, 'both', 2)],	
						bgc = self.color)
		cmds.symbolButton(image = "vufIcons/REFRESH.png",ann="Refresh list of nodes",c = self.refresh_ui )
		cmds.symbolButton(image = "vufIcons/SELECT.png", ann="Select node",c = self.select_node)
		self.listUI 			= cmds.optionMenu(label = self.nodeType  + ": ", cc = self.item_selected)
		cmds.symbolButton(image = "vufIcons/Back.png", ann="Get From Scene",c = self.get_node_from_scene)
		cmds.setParent(self.cLayout)
		
	def show(self):
		cmds.showWindow( self.wnd )
		
	def refresh_ui(self,*args):
		allNodes =  cmds.ls(type = self.nodeType,l = True)
		entries = cmds.optionMenu( self.listUI , query=True, itemListLong=True )
		if entries != None:
			for i in entries:	
				cmds.deleteUI( i, mi = True  )
		cmds.menuItem( label = _EMPTY_ITEM, parent = self.listUI)
		if len(allNodes) == 0:
			return
		for i in allNodes:
			cmds.menuItem(label=i,parent = self.listUI)
			
	def select_node(self,*args):
		node = cmds.optionMenu(self.listUI,query=True,v=True)
		cmds.select(node,r = True)
		
	def item_selected(self, *args):
		pass
		
	def get_node_from_scene(self,*args):
		pass
		