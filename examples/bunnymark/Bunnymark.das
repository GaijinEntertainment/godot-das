options tool = true

require godot

require math
require Bunny

class Bunnymark
	native : Node2D?
	bunnies : Node2D?

	generation : int = 0

	top_left: Vector2
	bottom_right: Vector2

	fps_counter : Label?

	def _ready()
		bunnies = native |> find_child("Bunnies") as Node2D
		fps_counter = native |> find_child("FPS") as Label
		// top_left = bunny_texture.get_size() * 0.5
		// bottom_right = Vector2(get_window().size) - bunny_texture.get_size() * 0.5

	def _input(event: InputEvent?)
		if event is InputEventMouseButton && event |> is_pressed()
			let mouse_event = event as InputEventMouseButton
			if mouse_event |> get_button_index() == MouseButton LEFT
				add_bunnies(5, Vector2(100, 100))

	def add_bunnies(count : int; position : Vector2)
		for i in range(count)
			add_bunny(position, 2. * PI * float(i) / float(count))
		generation += 1

	def add_bunny(position : Vector2; rotation : float)
		var bunny = Bunny`create()
		bunny.native |> set_position(position)
		// bunny.texture = bunny_texture
		// bunny.modulate = colors[generation % colors.size()]
		// bunny.velocity = Vector2.ONE.rotated(rotation) * BUNNY_VELOCITY * randf_range(0.5, 1.5)
		bunnies |> add_child(bunny.native)

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

		fps_counter |> set_text("FPS = {int(Engine`get_frames_per_second())}\nBunnies = {bunnies |> get_child_count()}")
