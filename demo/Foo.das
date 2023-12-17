options always_export_initializer = true
options tool = true // so breakpoints work

require godot


class Foo : Node2D
	angular_speed: float = 6.28
	timer: float = 0.0
	seconds: int = 0

	def _ready()
		print("It's ready!")

	def _enter_tree()
		print("It's enter tree!")

	def get_vel(): Vector2
		return 300. * Vector2(0.5 - timer, 0.)

	def _process(dt: float)
		rotate(dt * angular_speed)
		translate(dt * get_vel())
		timer += dt
		if timer > 1.
			timer = 0.
			seconds += 1
			angular_speed *= -1.
			print("{seconds} second passed")

		// uncomment to test error handling
		// var empty_ptr: int?
		// *empty_ptr = 5