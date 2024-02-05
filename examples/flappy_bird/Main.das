require boost/godot

// TODO read these
let window_width = 1152.
let window_height = 648.
let pipe_width = 100.
let pipe_height = 150.

[godot_class(Node2D)]
class Main
	spawn_interval = 1.
	timer : float = 0.

	game_over : bool = false
	points : int = 0
	
	pipe_scene : PackedScene?
	pipes : Node2D?
	
	def _ready()
		pipe_scene = native |> load("res://pipes.tscn") as PackedScene
		pipes = native |> find_child("Pipes") as Node2D
	
	def _process(dt : float)
		if game_over
			return
		if timer > spawn_interval
			timer = 0.
			spawn_pipes()
		timer += dt
		
	def spawn_pipes()
		var new_pipes = pipe_scene |> instantiate() as Node2D
		let initial_pos = Vector2(window_width + pipe_width * .5, randf_range(-pipe_height, pipe_height))
		new_pipes |> set_position(initial_pos)
		pipes |> add_child(new_pipes)
		print("added pipe")

	def is_game_over()
		return game_over

	def bird_collided()
		game_over = true
		
	def add_point()
		points += 1
