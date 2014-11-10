
extends RayCast

# member variables here, example:
# var a=2
# var b="textvar"

var debug

var part_names = {}
var parts = {}
var parts_place = {}
var part_instances = {}
var part_place_instances = {}
var part_type = null
var mount_y = 1
var lastHit = null


func _ready():
	# Initalization here
	set_process_input(true)
	debug = get_node("../HUD/Debug")
	part_names["item_wall_square_1x1.png"] = "s"
	part_names["item_wall_rect_1x2.png"] = "r"
	part_names["item_wall_triangle_1x1x2.png"] = "t1"
	part_names["item_wall_triangle_2x2x2.png"] = "t2"
	
	parts["s"] = preload("res://Scenes/wall_square_1x1.scn")
	parts["r"] = preload("res://Scenes/wall_rect_1x2.scn")
	parts["t1"] = preload("res://Scenes/wall_triangle_1x1x2.scn")
	parts["t2"] = preload("res://Scenes/wall_triangle_2x2x2.scn")
	parts_place["s"] = preload("res://Scenes/wall_square_1x1_place.scn")
	parts_place["r"] = preload("res://Scenes/wall_rect_1x2_place.scn")
	parts_place["t1"] = preload("res://Scenes/wall_triangle_1x1x2_place.scn")
	parts_place["t2"] = preload("res://Scenes/wall_triangle_2x2x2_place.scn")
	
	for i in parts:
		part_instances[i] = parts[i].instance()
		part_place_instances[i] = parts_place[i].instance()
		part_place_instances[i].set_ray_pickable(false)
	
	add_to_group("toolbar_listener")
	
	_selected_item_changed(get_node("../HUD/Toolbar").get_selected_item())
	
	pass

func _input(event):
	var update = false
	if(event.is_action("mount_y_next") && event.is_pressed() && !event.is_echo()):
		mount_y += 1
		update = true
	if(event.is_action("mount_y_prev") && event.is_pressed() && !event.is_echo()):
		mount_y -= 1
		update = true
	
	if(event.type == InputEvent.MOUSE_MOTION || update):
		update_place_part()
	
	if(event.is_action("place_part") && event.is_pressed() && !event.is_echo() && lastHit != null && part_place_instances[part_type].get_contact_count() == 0):
		var new_part = parts[part_type].instance()
		var part = lastHit.get_node("../../../../")
		var ship_parts = part.get_parent()
		new_part.set_name(str(ship_parts.get_child_count()))
		ship_parts.add_child(new_part)
		
		new_part.set_global_transform(lastHit.get_global_transform())
				
		if(new_part.add_part(ship_parts)):
			hide_part()
		else:
			print("part not placable")
			ship_parts.remove_child(new_part)
			new_part.queue_free()
		

func update_place_part():
	var hit = false
	if(part_type && is_colliding() == true):
		var collider = get_collider()
		debug.set_text(collider.get_name() + " " + str(collider.get_global_transform()))
		if(collider extends preload("res://Scripts/part.gd")):
			var closest = null;
			var dist = -1;
			var mount_points = collider.get_node("MountPoints/" + str(part_type))
			if(mount_points):
				var max_y = mount_points.get_child_count()
				mount_y %= max_y
				if(mount_y < 0):
					mount_y += max_y
				for child in mount_points.get_child(mount_y).get_children():
					var d = (child.get_global_transform().origin - get_collision_point()).length()
					if(d < dist || dist < 0):
						closest = child
						dist = d
				
				if(closest != null):
					if(lastHit != null):
						hide_part()
					if(collider.test_mountpoint(closest)):
						show_part(closest)
						hit = true
	
	if(hit == false && lastHit != null):
		hide_part()
		
func _selected_item_changed(new_item):
	if(part_names.has(new_item)):
		hide_part()
		part_type = part_names[new_item]
		update_place_part()
	else:
		part_type = null
		
func show_part(node):
	var part_place = part_place_instances[part_type]
	lastHit = node
	lastHit.add_child(part_place)
	
	
func hide_part():
	if(lastHit != null):
		lastHit.remove_child(part_place_instances[part_type])
	lastHit = null;
	