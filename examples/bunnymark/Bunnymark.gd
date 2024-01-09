extends Node2D

var bunny_texture: CompressedTexture2D = preload("res://bunny.png")

var fps_counter: Label
var bunnies: Node2D

const BUNNY_VELOCITY = 200.
const BUNNIES_PER_CLICK = 100

var generation := 0
var colors: Array[Color] = [Color.WHITE,
							Color.AQUA,
							Color.VIOLET,
							Color.DEEP_PINK,
							Color.SEA_GREEN,
							Color.NAVY_BLUE,
							Color.GOLD,
							Color.SALMON]

var top_left: Vector2
var bottom_right: Vector2


func _ready() -> void:
	fps_counter = find_child("FPS") as Label
	bunnies = find_child("Bunnies") as Node2D
	
	top_left = bunny_texture.get_size() * 0.5
	bottom_right = Vector2(get_window().size) - bunny_texture.get_size() * 0.5


func _input(event: InputEvent) -> void:
	if event is InputEventMouseButton and event.is_pressed():
		var event_mouse := event as InputEventMouseButton
		if event_mouse.button_index == MOUSE_BUTTON_LEFT:
			add_bunnies(BUNNIES_PER_CLICK, get_global_mouse_position())

func add_bunnies(count: int, position: Vector2) -> void:
	for i in range(count):
		add_bunny(position, 2. * PI * i / count)
	generation += 1

func add_bunny(position: Vector2, rotation: float) -> void:
	var bunny := Bunny.new()
	bunny.texture = bunny_texture
	bunny.modulate = colors[generation % colors.size()]
	bunny.velocity = Vector2.ONE.rotated(rotation) * BUNNY_VELOCITY * randf_range(0.5, 1.5)
	bunny.position = position
	bunnies.add_child(bunny)

func _process(delta: float) -> void:
	for node in bunnies.get_children():
		var bunny := node as Bunny
		bunny.position += bunny.velocity * delta

		if bunny.position.x < top_left.x:
			bunny.velocity.x *= -1
			bunny.position.x = top_left.x
		if bunny.position.y < top_left.y:
			bunny.velocity.y *= -1
			bunny.position.y = top_left.y
		if bunny.position.x > bottom_right.x:
			bunny.velocity.x *= -1
			bunny.position.x = bottom_right.x
		if bunny.position.y > bottom_right.y:
			bunny.velocity.y *= -1
			bunny.position.y = bottom_right.y

	fps_counter.text = "FPS = %d\nBunnies = %d" % [Engine.get_frames_per_second(), bunnies.get_child_count()]
