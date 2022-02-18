import maya.cmds as cmds
from vfRigMaster import vfUtils as uts

class matrTest(object):
	def __init__(self):
		pass
	'''
	Example 
	
	from vfRigMaster import vfTest as tst
	reload (tst)
	tst.matrTest.vfk_test(count_of_chains = 10, fk_count = 10)
	'''
	
	@staticmethod
	def vfk_test(count_of_chains = 4, fk_count = 1, param_scale = 1):
		drivers = []
		drivens = []
		effects = []
		# 	create drivers
		for i in range(0,count_of_chains):
			a = cmds.polyCube( n = 'driver#',w = 0.5 )[0]
			cmds.setAttr('{0}.translateX'.format(a),i)
			drivers.append(a)
		print ('drivers: ',drivers)
		# 	create drivens
		for i in range(0,count_of_chains):
			a = cmds.polySphere( n = 'driven#',r = 0.4 )[0]
			cmds.setAttr('{0}.translateX'.format(a),i)
			drivens.append(a)
		print ('drivers: ',drivens)
		#	create effectors
		for i in range(0,fk_count):
			a = cmds.circle( n = 'eff#',r = 1, nrx = 1, nry = 0, nrz = 0 )[0]
			cmds.setAttr('{0}.translateX'.format(a),i)
			effects.append(a)
		print ('effectors: ',effects)
		
		res = {}
		res['collector']   = uts.matr.create_from_dags( dags = drivers, world_space = True	) # create collector from selected objects
		res['inputs']      = uts.matr.get_inputs(       node_name = res['collector'])
		res['look_at']     = uts.matr.add_look(			node_name = res['collector'])
		res['vfk']         = uts.matr.add_vfk(			node_name = res['look_at'], effectors = effects, param_scale = param_scale, new_node_name = "vfkTest" )
		res['null']        = uts.matr.add_null(			node_name = res['vfk'])
		res['locator']     = uts.matr.add_locator(		node_name = res['null'])
		res['decompose']   = uts.matr.add_decompose(	node_name = res['vfk'], dags_to = drivens )
		print res
		#counter = 0.0;
		#for eff in effects:
		#	cmds.setAttr('{0}.param'.format(eff), counter/(len(effects)-1))
		#	counter += 1
		cmds.setAttr('{0}.useParent'.format(res['decompose']),0)
		
		
		
		
		
		
