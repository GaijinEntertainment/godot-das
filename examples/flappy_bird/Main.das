require boost/godot

// TODO read these
let window_width = 1152.
let window_height = 648.
let pipe_width = 50.
let pipe_height = 200.

let Bird`collided = "bird_collided"

[godot_class(Node2D)]
class Main
	spawn_interval = 1.1
	timer : float = 0.
	ground_level : float
	bird_level : float

	game_over : bool = false
	game_reset : bool = false
	points : int = 0


	pipes : Node2D?
	points_label : Label?
	bird : Node2D?
	can_restart : Timer?

	reset : Signal

	def _ready()
		pipes = native |> find_child("Pipes") as Node2D
		// TODO use get_node("HUD/Points")
		points_label = native |> find_child("Points") as Label

		ground_level = (native |> find_child("Ground") as Node2D |> get_position()).y
		bird = native |> find_child("Bird") as Node2D
		bird_level = (bird |> get_position()).x
		bird |> get_signal(Bird`collided) |> connect(native) <| "bird_collided"

		can_restart = native |> find_child("CanRestart") as Timer
		can_restart |> get_timeout() |> connect(native) <| "show_game_over"

	def _process(dt : float)
		if game_over
			if can_restart |> is_stopped() && Input`is_action_just_pressed("jump")
				reset_game()
			return
		if timer > spawn_interval
			timer = 0.
			spawn_pipes()
		timer += dt

	def spawn_pipes()
		var new_pipes : Node2D?
		load("res://pipes.tscn") <| $ (var pipe_scene : Resource?#)
			new_pipes = pipe_scene as PackedScene |> instantiate() as Node2D
		let initial_pos = Vector2(window_width + pipe_width * .5, randf_range(-pipe_height, pipe_height))
		new_pipes |> set_position(initial_pos)
		bird |> get_signal(Bird`collided) |> connect(new_pipes) <| "stop"
		pipes |> add_child(new_pipes)

	def show_game_over()
		native |> find_child("GameOver") as Label |> set_visible(true)

	def reset_game()
		game_over = false
		timer = 0.
		for pipe in pipes |> iterate_children()
			pipe |> queue_free()
		points = 0
		points_label |> set_text("0")
		native |> find_child("GameOver") as Label |> set_visible(false)
		reset |> emit()

	def get_ground_level()
		return ground_level

	def get_bird_level()
		return bird_level

	def bird_collided()
		game_over = true
		can_restart |> start()

	def add_point()
		points += 1
		points_label |> set_text("{points}")
