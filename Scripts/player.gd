extends RigidBody

export var view_sensitivity = 0.3
export var yaw = 0
export var pitch = 0
const walk_speed = 5
const jump_speed = 4
const max_accel = 0.02
const air_accel = 0.1

var jetpack = true;

var god_mode = false;
var collision_shapes = {};

func _ready():
	set_process_input(true)
	for i in range(get_shape_count()):
		collision_shapes[get_shape(i)] = get_shape_transform(i)
	
	print("layer_mask: " + str(get_layer_mask()))

func _input(event):
	if(event.type == InputEvent.MOUSE_MOTION):
		yaw = fmod(yaw - event.relative_x * view_sensitivity, 360)
		pitch = max(min(pitch - event.relative_y * view_sensitivity, 90), -90)
		get_node("Yaw").set_rotation(Vector3(0, deg2rad(yaw), 0))
		get_node("Yaw/Camera").set_rotation(Vector3(deg2rad(pitch), 0, 0))
		
	if(event.is_action("god_mode") && event.is_pressed() && !event.is_echo()):
		god_mode = !god_mode
		
		if(god_mode):
			print("entering godmode")
			#clear_shapes()
			set_layer_mask(0)
		else:
			print("leaving godmode")
			set_layer_mask(1);
			#for shape in collision_shapes:
			#	add_shape(shape, collision_shapes[shape])
		
		print("layer_mask: " + str(get_layer_mask()))
		

func _integrate_forces(state):
	var aim
	if(jetpack == true):
		aim = get_node("Yaw/Camera").get_global_transform().basis
	else:
		aim = get_node("Yaw").get_global_transform().basis
	
	var direction = Vector3()
	if Input.is_action_pressed("move_forwards"):
		direction -= aim[2]
	if Input.is_action_pressed("move_backwards"):
		direction += aim[2]
	if Input.is_action_pressed("move_left"):
		direction -= aim[0]
	if Input.is_action_pressed("move_right"):
		direction += aim[0]
	if(jetpack == true):
		if(Input.is_action_pressed("jump")):
			direction += aim[1]
		if(Input.is_action_pressed("crouch")):
			direction -= aim[1]

	direction = direction.normalized()
	
	var ray = get_node("RayCast")
	if ray.is_colliding() && jetpack == false:
		var up = state.get_total_gravity().normalized()
		var normal = ray.get_collision_normal()
		var floor_velocity = Vector3()
		var object = ray.get_collider()
		if object extends RigidBody or object extends StaticBody:
			var point = ray.get_collision_point() - object.get_translation()
			var floor_angular_vel = Vector3()
			if object extends RigidBody:
				floor_velocity = object.get_linear_velocity()
				floor_angular_vel = object.get_angular_velocity()
			elif object extends StaticBody:
				floor_velocity = object.get_constant_linear_velocity()
				floor_angular_vel = object.get_constant_angular_velocity()
			# Surely there should be a function to convert euler angles to a 3x3 matrix
			var transform = Matrix3(Vector3(1, 0, 0), floor_angular_vel.x)
			transform = transform.rotated(Vector3(0, 1, 0), floor_angular_vel.y)
			transform = transform.rotated(Vector3(0, 0, 1), floor_angular_vel.z)
			floor_velocity += transform.xform_inv(point) - point
			yaw = fmod(yaw + rad2deg(floor_angular_vel.y) * state.get_step(), 360)
			get_node("Yaw").set_rotation(Vector3(0, deg2rad(yaw), 0))
		var speed = walk_speed
		var diff = floor_velocity + direction * walk_speed - state.get_linear_velocity()
		var vertdiff = aim[1] * diff.dot(aim[1])
		diff -= vertdiff
		diff = diff.normalized() * clamp(diff.length(), 0, max_accel / state.get_step())
		diff += vertdiff
		apply_impulse(Vector3(), diff * get_mass())
		if Input.is_action_pressed("jump"):
			apply_impulse(Vector3(), normal * jump_speed * get_mass())
	else:
		apply_impulse(Vector3(), direction * air_accel * get_mass())
	
	state.integrate_forces()

func _enter_scene():
	Input.set_mouse_mode(Input.MOUSE_MODE_CAPTURED)

func _exit_scene():
	Input.set_mouse_mode(Input.MOUSE_MODE_VISIBLE)
