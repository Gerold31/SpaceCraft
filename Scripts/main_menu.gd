
extends Node


func _ready():
	get_node("ButtonQuit").connect("pressed", self, "_on_buttonquit_pressed")
	get_node("ButtonPlay").connect("pressed", self, "_on_buttonplay_pressed")
	get_node("QuitDialog").connect("confirmed", self, "_quit")

func _on_buttonquit_pressed():  
	get_node("QuitDialog").show()

func _on_buttonplay_pressed():
	get_node("/root/root").change_scene("res://Scenes/main_scene.scn")

func _quit():
	OS.get_main_loop().quit()