import maya.cmds as cmds

def get_tag():
	sel_obj = cmds.ls(sl = True, tr = True, ap = True)
	
	if len( sel_obj ) == 0:
		return ""
	obj = sel_obj[-1]
	
	if cmds.attributeQuery( "vufTag", node = obj, exists = True) == False:
		return ""
	
	if cmds.getAttr( obj + ".vufTag", typ = True) == "string":
		return cmds.getAttr( obj + ".vufTag")
	
	return ""	
	
		
def add_tag( p_name = ""):
	sel_obj = cmds.ls(sl = True, tr = True, ap = True)
	
	print sel_obj
	for obj in sel_obj:
		if cmds.attributeQuery( "vufTag", node = obj, exists = True) == False:
			cmds.addAttr( obj, longName = "vufTag", dataType = "string")
			
		if cmds.getAttr( obj + ".vufTag", typ = True) == "string":
			cmds.setAttr( obj + ".vufTag", p_name, typ = "string")
			


