require boost/godot
require Foo

[godot_class(Node2D)]
class SignalReciever

	def _ready()
		var foo = native |> get_parent() as Foo
		foo.test |> connect(native) <| "signal_from_foo"

	def signal_from_foo()
		print("recieved signal from Foo!")

