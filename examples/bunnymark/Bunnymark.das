options always_export_initializer = true // do not remove this option


require godot
require Bunny

class Bunnymark
	native : Node2D?
	bunnies : Node2D?

	def _ready()
		bunnies = native |> find_child("Bunnies") as Node2D

	def _input(event: InputEvent?)
		if event is InputEventMouseButton && event |> is_pressed()
			let mouse_event = event as InputEventMouseButton
			if mouse_event |> get_button_index() == MouseButton LEFT
				add_bunny(Vector2(100, 100))


	def add_bunny(position : Vector2)
		var bunny = new_Bunny()
		bunny.native |> set_position(position)
		// bunny.texture = bunny_texture
		// bunny.modulate = colors[generation % colors.size()]
		// bunny.velocity = Vector2.ONE.rotated(rotation) * BUNNY_VELOCITY * randf_range(0.5, 1.5)
		bunnies |> add_child(bunny.native)
