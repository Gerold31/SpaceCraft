
extends Spatial

# member variables here, example:
# var a=2
# var b="textvar"

func _ready():
	PhysicsServer.area_set_param(get_world().get_space(), PhysicsServer.AREA_PARAM_GRAVITY, 0)
	PhysicsServer.area_set_param(get_world().get_space(), PhysicsServer.AREA_PARAM_DENSITY, 1)
	pass


