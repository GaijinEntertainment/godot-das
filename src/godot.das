require godot

class Object
    native: ObjectNative?

class Node : Object
    def Node(_native: ObjectNative?)
        native = _native

    def get_parent(): Node?
        return new Node(*unsafe(reinterpret<NodeNative?>(native)) |> _Node_get_parent())

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

def operator as Node2D(obj: Object?)
    return new Node2D(obj.native)

def operator as Node(obj: Object?)
    return new Node(obj.native)