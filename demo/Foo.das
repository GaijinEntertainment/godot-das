options always_export_initializer = true
options tool = true // set true to make breakpoints work

require godot
require math

class Foo : Node2D
	angular_speed: float = 6.28

	center: Node2D?
	speed: float = 500.
	velocity: Vector2
	radius: float


	def _ready()
		center = get_parent()->find_child("Center") as Node2D

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
