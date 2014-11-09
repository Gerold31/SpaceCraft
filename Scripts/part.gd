
var mountpoints = {}

export var name = ""

const THRESHOLD = 0.01

func _ready():
	for type in get_node("MountPoints").get_children():
		for y in type.get_children():
			for xz in y.get_children():
				mountpoints[get_node("MountPoints").get_path_to(xz)] = null;
	pass

func add_part(ship_parts):
	var connections = []
	
	for part in ship_parts.get_children():
		for type in get_node("MountPoints").get_children():
			if(type.get_name() == part.name):
				for y in type.get_children():
					for xz in y.get_children():
						if((xz.get_global_transform().origin - part.get_global_transform().origin).length() < THRESHOLD):
							if(test_mountpoint(xz)):
								for part_type in part.get_node("MountPoints").get_children():
									if(part_type.get_name() == name):
										for part_y in part_type.get_children():
											for part_xz in part_y.get_children():
												if((part_xz.get_global_transform().origin - get_global_transform().origin).length() < THRESHOLD):
													if(part.test_mountpoint(part_xz)):
														var connection1 = {}
														connection1["p"] = self
														connection1["m"] = xz
														connection1["o"] = part
														var connection2 = {}
														connection2["p"] = part
														connection2["m"] = part_xz
														connection2["o"] = self
														connections.append(connection1)
														connections.append(connection2)
													else:
														return false
							else:
								return false
	
	for i in connections:
		print("connect " + i["p"].get_path() + "." + i["m"].get_path() + " to " + i["o"].get_path()) 
		i["p"].set_mountpoint(i["m"], i["o"])
	
	return true
	
func test_mountpoint(mountpoint):
	if(mountpoints[get_node("MountPoints").get_path_to(mountpoint)] == null):
		return true
	return false
	
func set_mountpoint(mountpoint, part):
	mountpoints[get_node("MountPoints").get_path_to(mountpoint)] = part