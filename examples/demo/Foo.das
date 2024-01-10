options tool = false // set true to make breakpoints work

require godot
require math
require Bar

// TODO generate this section with macros
def operator as Foo(obj : Foo?)
	if obj != null
		return obj
	error("cannot cast null")
	return null

def operator as Foo(native : Object?)
	return unsafe(reinterpret<Foo?>(_get_dascript_type(native, "Foo")))

def operator ?as Foo(obj : Foo?)
	return obj

def operator ?as Foo(native : Object?)
	if native != null
		return native as Foo
	return null

def operator is(obj : Foo?)
	return true

def operator is(native : Object?)
	return _check_dascript_type(native, "Foo")
// end of section

class Foo
	native: Node2D?

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
			child as Node2D |> translate(Vector2(300, 0))

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
		native |> translate(dt * velocity)

	// call to see error hadling
	def i_am_error()
		var empty_ptr: int?
		*empty_ptr = 5
