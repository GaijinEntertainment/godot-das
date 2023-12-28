options always_export_initializer = true
options tool = true // set true to make breakpoints work

require godot
require math
require Bar
require daslib/constant_expression

// TODO generate this section with macros
def operator as Foo(native : ObjectNative?)
    return unsafe(reinterpret<Foo>(_check_dascript_type(native, "Foo")))

def operator ?as Foo(native : ObjectNative?)
    if native != null
        return native as Foo(native)
    return null

def operator as Foo(obj : Object?)
	if obj != null
    	return obj.native as Foo
	error("cannot cast null")
	return null

def operator ?as Foo(native : ObjectNative?)
    if native != null
        return obj.native as Foo
    return null
// end of section

class Foo : Node2D
	angular_speed: float = 6.28

	center: Node2D?
	speed: float = 500.
	velocity: Vector2
	radius: float


	def _ready()
		print("from Bar.das: {ABRACADABRA}")
		center = (get_parent() as Node)->find_child("Center") as Node2D

		let to_center = center->get_position() - get_position()
		radius = length(to_center)
		let to_center_dir = to_center / radius
		velocity = speed * Vector2(to_center_dir.y, -to_center_dir.x)


	def get_acc(): Vector2
		let to_center = center->get_position() - get_position()

		return to_center * (speed * speed) /  (radius * radius)

	def _process(dt: float)
		rotate(dt * angular_speed)
		velocity += get_acc() * dt
		translate(dt * velocity)

	// call to see error hadling
	def i_am_error()
		var empty_ptr: int?
		*empty_ptr = 5
