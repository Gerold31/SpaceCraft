
extends Node

# member variables here, example:
# var a=2
# var b="textvar"

var current_scene = null

func _ready():
	var root = get_tree().get_root()
	current_scene = root.get_child(root.get_child_count()-1)
	pass
	
func change_scene(scn):
	# remove current scene from root and enqueue it for deletion
	#get_scene().get_root().remove_child(current_scene)
	current_scene.queue_free()

	# load and add new scene to root
	var s = ResourceLoader.load(scn)
	current_scene = s.instance()
	get_tree().get_root().add_child(current_scene)
	get_tree().get_root().update_worlds()
