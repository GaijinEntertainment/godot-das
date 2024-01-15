require boost/godot
require math
require Bar


[godot_class(Node2D)]
class Foo
	angular_speed: float = 6.28

	center: Node2D?
	speed: float = 500.
	velocity: Vector2
	radius: float

	def _input(event: InputEvent?)
		if event is InputEventMouseButton && event |> is_pressed()
			let mouse_event = event as InputEventMouseButton
			if mouse_event |> get_button_index() == MouseButton LEFT
				print("left")
			if mouse_event |> get_button_index() == MouseButton RIGHT
				print("right")


	def _ready()
		print("from Bar.das: {ABRACADABRA}")
		var parent = native |> get_parent() as Node

		for child in parent |> iterate_children()
			let translation = Vector2(300, 0)
			child as Node2D |> translate(translation)

		center = parent |> find_child("Center", true, true) as Node2D
		print("parent is Node2D={parent is Node2D}")
		print("{parent |> get_name()}, {native |> get_name()}, {center |> get_name()}")

		let to_center = center |> get_position() - native |> get_position()
		radius = length(to_center)
		let to_center_dir = to_center / radius
		velocity = speed * Vector2(to_center_dir.y, -to_center_dir.x)


	def get_acc(): Vector2
		let center_pos = center |> get_position()
		let this_pose = native |> get_position()
		let to_center = center_pos - this_pose
		return to_center * (speed * speed) /  (radius * radius)

	def _process(dt: float)
		native |> rotate(dt * angular_speed)
		velocity += get_acc() * dt
		let translation = dt * velocity
		native |> translate(translation)

	// call to see error hadling
	def i_am_error()
		var empty_ptr: int?
		*empty_ptr = 5
