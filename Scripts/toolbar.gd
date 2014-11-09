
extends GridContainer

var selected_item = -1

func _ready():
	set_process_input(true)
	select_item(1)
	pass
	
func _input(event):
	#fix not being able to map numberkeys
	if(event.type == InputEvent.KEY && event.scancode >= 49 && event.scancode <= 49+9 && event.is_pressed() && !event.is_echo()):
		select_item(event.scancode - 49 + 1)
	
	if(event.is_action("select_item_1") && event.is_pressed() && !event.is_echo()):
		select_item(1)
	if(event.is_action("select_item_2") && event.is_pressed() && !event.is_echo()):
		select_item(2)
	if(event.is_action("select_item_3") && event.is_pressed() && !event.is_echo()):
		select_item(3)
	if(event.is_action("select_item_4") && event.is_pressed() && !event.is_echo()):
		select_item(4)
	if(event.is_action("select_item_5") && event.is_pressed() && !event.is_echo()):
		select_item(5)
	if(event.is_action("select_item_6") && event.is_pressed() && !event.is_echo()):
		select_item(6)
	if(event.is_action("select_item_7") && event.is_pressed() && !event.is_echo()):
		select_item(7)
	if(event.is_action("select_item_8") && event.is_pressed() && !event.is_echo()):
		select_item(8)
	if(event.is_action("select_item_9") && event.is_pressed() && !event.is_echo()):
		select_item(9)

func select_item(i):
	if(i != selected_item):
		if(selected_item >= 0):
			get_node("ItemSlot " + str(selected_item)).set_texture(preload("res://GUI/Items/item_border_default.png"))
		selected_item = i
		get_node("ItemSlot " + str(selected_item)).set_texture(preload("res://GUI/Items/item_border_selected.png"))
		get_tree().call_group(0, "toolbar_listener", "_selected_item_changed", get_selected_item())
		

func get_selected_item():
	return get_node("ItemSlot " + str(selected_item) + "/ItemImage").get_texture().get_name()
