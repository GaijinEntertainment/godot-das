require boost/godot
require Main


[godot_class(Node2D)]
class Pipes

	velocity = Vector2(-200, 0)
	game : Main?
	
	def _ready()
		game = native |> get_parent() |> get_parent() as Main

	def _process(dt : float)
		if game->is_game_over()
			return
		let translation = velocity * dt
		native |> translate(translation)
		
		let pos = native |> get_position()
		if pos.x < - pipe_width * 0.5
			native |> queue_free()
