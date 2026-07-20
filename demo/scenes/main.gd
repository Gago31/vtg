extends Node2D

@onready var video_sprite_wing_it: VideoSprite = %VideoSprite_WingIt


func _ready() -> void:
	video_sprite_wing_it.play()
