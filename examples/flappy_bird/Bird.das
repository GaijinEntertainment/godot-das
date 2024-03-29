require boost/godot
require math
require Main

let bird_height = 50.
let gravity = Vector2(0., 1000.)
let boost = Vector2(0.,-500.)

[godot_class(Node2D)]
class Bird
	suspended : bool = true
	crashed : bool = false
	collided : bool = false
	reset : bool = false

	velocity : Vector2
	initial_pos : Vector2
	rotation : float = 0.
	ang_speed : float = 0.
	up_timer : float = 0.

	collider : Area2D?
	game : Main?

	bird_collided : Signal

	def _ready()
		collider = native |> find_child("Collider") as Area2D
		game = native |> get_parent() as Main
		game.reset |> connect(native) <| "reset_bird"

		initial_pos = native |> get_position()

		native |> find_child("Collider") as Area2D |> get_area_entered() |> connect(native) <| "overlapped"

	def move(dt : float)
		if !collided && !reset && Input`is_action_just_pressed("jump")
			velocity = boost
			ang_speed = -4. * PI
			suspended = false
			up_timer = 0.
		reset = false

		if !suspended && !crashed
			velocity += gravity * dt
		let translation = velocity * dt
		native |> translate(translation)

		var pos = native |> get_position()
		if pos.y < -bird_height*0.5
			pos.y = -bird_height*0.5
			native |> set_position(pos)
		if pos.y + bird_height * .5 > game->get_ground_level()
			crashed = true
			ang_speed = 0.
			velocity = Vector2(0, 0)

	def rotate(dt : float)
		let up_time = abs(boost.y / gravity.y) * 1.5
		if rotation <= -PI * 0.2 && ang_speed <= 0.
			ang_speed = 0.
			up_timer += dt
			if up_timer >= up_time
				up_timer = 0.
				ang_speed = 2.*PI
		if rotation >= 0.5 * PI && ang_speed > 0.
			ang_speed = 0.
		rotation += ang_speed * dt
		native |> set_rotation(rotation)

	def overlapped(with_who : Area2D?)
		bird_collided |> emit()
		collided = true
		suspended = false

	def reset_bird()
		suspended = true
		crashed = false
		collided = false
		reset = true
		velocity = Vector2(0, 0)
		native |> set_position(initial_pos)
		rotation = 0.
		native |> set_rotation(0.)
		ang_speed = 0.
		up_timer = 0.

	def _process(dt : float)
		move(dt)
		rotate(dt)
