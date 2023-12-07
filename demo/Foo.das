options always_export_initializer = true

require godot

class Foo : Node2D
	def _ready()
		print("It's ready! {x}")
		hello()

	def _enter_tree()
		print("It's enter tree!")
