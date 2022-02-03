from vfRigMaster import vfUI as ui
from vfRigMaster import vfMatrixCollector as mc
from vfRigMaster import vfBspline as bs
from vfRigMaster import vfUtils as utls

import maya.cmds as cmds
_MENU_NAME 			= 'vfRigMaster'
_DEFAULT_NAME 		= 'mClctr'

def load_ui():
	root = ui.create_window_menu(_MENU_NAME)
	cmds.menuItem(label = 'Rigging', subMenu=True, tearOff = True )	
	# matrix colector
	cmds.menuItem(label = 'MatrixList', 	c = vf_matrix_list, ann = 'Select controls and run')
	cmds.menuItem(optionBox=True, 			c = vf_matrix_list_adv)	
	
	cmds.menuItem(label = 'Curves', subMenu=True, tearOff = True )	
	
	cmds.setParent( '..', menu=True )
	cmds.menuItem(label = 'Utils', subMenu=True, tearOff = True )	
	
def unload_ui():
	ui.delete_window_menu(_MENU_NAME)
	
def vf_matrix_list(*args):
	m_c		= utls.matrCollector()
	node 	= m_c.selection_to_matrix_list(world_space = True, new_node_name = "{0}{1}{2}".format(ui._PREFIX,_DEFAULT_NAME,ui._SUFIX));

def vf_matrix_list_adv(*args):
	mc.TCGui().show()