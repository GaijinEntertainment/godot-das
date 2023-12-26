require godot
require daslib/constant_expression


class Object
    native: ObjectNative?
    def Object(_native: ObjectNative?)
        native = _native


class Node : Object
    def Node(_native: ObjectNative?)
        native = _native

    def get_parent(): NodeNative?
        return *unsafe(reinterpret<NodeNative?>(native)) |> _Node_get_parent()

    def find_child(pattern : string; recursive : bool = true; owned : bool = true): NodeNative?
        return *unsafe(reinterpret<NodeNative?>(native)) |> _Node_find_child(pattern, recursive, owned)


class Node2D : Node
    def Node2D(_native: ObjectNative?)
        native = _native

    def rotate(radians: float): void
        *unsafe(reinterpret<Node2DNative?>(native)) |> _Node2D_rotate(radians)

    def translate(offset: Vector2): void
        *unsafe(reinterpret<Node2DNative?>(native)) |> _Node2D_translate(offset)

    def set_position(position: Vector2): void
        *unsafe(reinterpret<Node2DNative?>(native)) |> _Node2D_set_position(position)

    def get_position(): Vector2
        return *unsafe(reinterpret<Node2DNative?>(native)) |> _Node2D_get_position()


// Wrapper creation from native pointers

def operator as Object(native: ObjectNative?)
    if native != null
        return new Object(native)
    return null

def operator as Node2D(native: ObjectNative?)
    if native != null && check_godot_type_Node2D(native)
        return new Node2D(native)
    return null

def operator as Node(native: ObjectNative?)
    if native != null && check_godot_type_Node(native)
        return new Node(native)
    return null


// Wrapper recreation

def operator as Object(obj: Object?)
    return obj

def operator as Node2D(obj: Object?)
    if check_godot_type_Node2D(obj.native)
        return new Node2D(obj.native)
    return null

def operator as Node(obj: Object?)
    if check_godot_type_Node(obj.native)
        return new Node(obj.native)
    return null
