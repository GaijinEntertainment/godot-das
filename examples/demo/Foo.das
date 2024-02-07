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

	test : Signal

	def check_entered_child(node : Node2D?)
		// Okay, I found a problem here
		// callp only sends Object*, not SpecificNodeType*
		// This means that you can unsafely recieve any derived type of Object
		// For example, this function takes Node2D* (should take Node*)
		// and `node is Node2D` always return true even though it's a Timer
		// I could remove `is Type(obj : Type?)` so is always do a proper check
		// but this is a nifty shortcut
		// Probably we should somehow check function signature on connect
		print("check_entered_child! got {node} {node is Timer} {node is Node2D}")

	def _enter_tree()
		native |> get_child_entered_tree() |> connect(native) <| "check_entered_child"

	def _input(event: InputEvent?)
		if event is InputEventMouseButton && event |> is_pressed()
			let mouse_event = event as InputEventMouseButton
			test |> emit()
			if mouse_event |> get_button_index() == MouseButton LEFT
				print("left")
			if mouse_event |> get_button_index() == MouseButton RIGHT
				print("right")
			if mouse_event |> get_button_index() == MouseButton MIDDLE
				print("parent is ready? {native |> get_parent() as Node |> is_node_ready()}")

	def _ready()
		print("Internal name of `test` signal is {test.name}")

		var timer = native |> find_child("Timer") as Timer
		timer |> get_timeout() |> connect(native) <| "on_timeout"

		print("from Bar.das: {ABRACADABRA}")
		var parent = native |> get_parent() as Node
		print("parent is ready? {parent |> is_node_ready()}")

		var inscope children <- parent |> get_children()
		for child in children
			let translation = Vector2(300, 0)
			child as Node2D |> translate(translation)

		center = parent |> find_child("Center") as Node2D
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

	def on_timeout()
		print("recieved signal from timer!")

	// call to see error hadling
	def i_am_error()
		var empty_ptr: int?
		*empty_ptr = 5
