
extends RigidBody

var contacts = 0;

func _ready():
	self.connect("body_enter", self, "on_body_enter")
	self.connect("body_exit", self, "on_body_exit")
	pass
	
func on_body_enter(body):
	contacts+=1;
	get_node("MeshInstance").set_material_override(preload("res://Materials/place_fail.mtl"))
	#print(body.get_name() + " enters " + get_name() + "; contacts " + str(contacts))
	
func on_body_exit(body):
	contacts-=1;
	if(contacts == 0):
		get_node("MeshInstance").set_material_override(preload("res://Materials/place.mtl"))
	#print(body.get_name() + " exits " + get_name() + "; contacts " + str(contacts))

func get_contact_count():
	return contacts;

