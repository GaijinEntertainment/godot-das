require boost/godot
require math
require Main

let bird_height = 50.

[godot_class(Node2D)]
class Bird
	suspended : bool = true

	velocity : Vector2
	gravity = Vector2(0., 1000.)
	
	collider : Area2D?
	game : Main?

	def _ready()
		collider = native |> find_child("Collider") as Area2D
		game = native |> get_parent() as Main
	
	def move(dt : float)
		if Input`is_action_just_pressed("jump")
			velocity = Vector2(0.,-500.)
			suspended = false
		if !suspended
			velocity += gravity * dt
		let translation = velocity * dt
		native |> translate(translation)
		var pos = native |> get_position()
		if pos.y < -bird_height*0.5
			pos.y = -bird_height*0.5
			native |> set_position(pos)

	def check_collision()
		var inscope collisions <- collider |> get_overlapping_areas()
		if collisions |> length() > 0
			game->bird_collided()

	def _process(dt : float)
		check_collision()
		
		move(dt)
	

