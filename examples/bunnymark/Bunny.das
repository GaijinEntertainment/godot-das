require godot
// TODO generate this section
// TODO think of a nicer way to do it

def new_Bunny(): Bunny?
	// creation of user type is a two step process:
	// 1. create native type using memnew - we need to get type of 'native' field in class declaration
	// 2. set this script as script - we need to get a reference to the object representing this script
	var native = Sprite2D_new()
	var promoted: void? = _promote_to_das_type(native, "Bunny")
	return unsafe(reinterpret<Bunny?>(promoted))
// end of section

class Bunny
	native : Sprite2D?
	velocity: Vector2
