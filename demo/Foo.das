options always_export_initializer = true

require godot

class Foo
	def _ready()
		print("It's ready! {TEST}")

	def _enter_tree()
		print("It's enter tree!")
