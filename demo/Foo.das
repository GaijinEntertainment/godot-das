options always_export_initializer = true
options tool = true // so breakpoints work

require godot

class Foo : Node2D
	def _ready()
		print("It's ready!")

	def _enter_tree()
		print("It's enter tree!")

	def _process()
		rotate(0.01)
