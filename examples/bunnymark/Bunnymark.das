require boost/godot

require math
require Bunny

let BUNNY_VELOCITY = 200.
let BUNNIES_PER_CLICK = 100

let colors : array<Color> <- [{Color Color`named("WHITE");
							 Color`named("AQUA");
							 Color`named("VIOLET");
							 Color`named("DEEP_PINK");
							 Color`named("SEA_GREEN");
							 Color`named("NAVY_BLUE");
							 Color`named("GOLD");
						 	 Color`named("SALMON")}]

[godot_class(Node2D)]
class Bunnymark
	bunnies : Node2D?

	generation : int = 0

	top_left: Vector2
	bottom_right: Vector2

	fps_counter : Label?

	bunny_texture : Texture2D?

	def _ready()
		bunnies = native |> find_child("Bunnies") as Node2D
		fps_counter = native |> find_child("FPS") as Label

		bunny_texture = native |> load("res://bunny.png") as Texture2D
		top_left = bunny_texture |> get_size() * 0.5
		bottom_right = native |> get_window() |> get_size() - bunny_texture |> get_size() * 0.5

	def _input(event: InputEvent?)
		if event is InputEventMouseButton && event |> is_pressed()
			let mouse_event = event as InputEventMouseButton
			if mouse_event |> get_button_index() == MouseButton LEFT
				add_bunnies(BUNNIES_PER_CLICK, native |> get_global_mouse_position())

	def add_bunnies(count : int; position : Vector2)
		for i in range(count)
			add_bunny(position, 2. * PI * float(i) / float(count))
		generation += 1

	def add_bunny(position : Vector2; rotation : float)
		var bunny = Bunny`new()
		bunny.native |> set_position(position)
		bunny.native |> set_texture(bunny_texture)
		bunny.native |> set_modulate(colors[generation % colors |> length()])
		bunny.velocity = Vector2(cos(rotation), sin(rotation)) * BUNNY_VELOCITY * float(randf_range(0.5, 1.5))
		bunnies |> add_child(bunny.native, false, Node`InternalMode INTERNAL_MODE_FRONT)

	def _process(delta: float)
		for node in bunnies |> get_children()
			var bunny = node as Bunny
			var new_position = bunny.native |> get_position() + bunny.velocity * delta

			if new_position.x < top_left.x
				bunny.velocity.x *= -1.
				new_position.x = top_left.x
			if new_position.y < top_left.y
				bunny.velocity.y *= -1.
				new_position.y = top_left.y
			if new_position.x > bottom_right.x
				bunny.velocity.x *= -1.
				new_position.x = bottom_right.x
			if new_position.y > bottom_right.y
				bunny.velocity.y *= -1.
				new_position.y = bottom_right.y
			bunny.native |> set_position(new_position)

		fps_counter |> set_text("I'm Daslang!\nFPS = {int(Engine`get_frames_per_second())}\nBunnies = {bunnies |> get_child_count(false)}")
