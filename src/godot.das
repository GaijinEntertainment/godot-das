require godot
require daslib/constant_expression


class Object
    native: ObjectNative?
    def Object(_native: ObjectNative?)
        native = _native


class Node : Object
    def Node(_native: NodeNative?)
        native = _native

    def get_parent(): NodeNative?
        return *unsafe(reinterpret<NodeNative?>(native)) |> _Node_get_parent()

    def find_child(pattern : string; recursive : bool = true; owned : bool = true): NodeNative?
        return *unsafe(reinterpret<NodeNative?>(native)) |> _Node_find_child(pattern, recursive, owned)


class Node2D : Node
    def Node2D(_native: Node2DNative?)
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
//
// "Compile time" casts that already take a corresponding type and don't perform any type change,
// e.g. operator as Node(native: NodeNative?), are required in cases when the function returns
// correct type and we just need to wrap it into a wrapper class - e.g. get_parent() as Node
//
// `?as` operators are the same as `as`, but in case of null they quetly return null instead of printing an error

// as

def operator as Object(native: ObjectNative?)
    if native != null
        return new Object(native)
    error("cannot cast null")
    return null

def operator as Node2D(native: Node2DNative?)
    if native != null
        return new Node2D(native)
    error("cannot cast null to Node2D")
    return null

def operator as Node2D(native: ObjectNative?)
    if _check_native_type_Node2D(native)
        return new Node2D(unsafe(reinterpret<Node2DNative?>(native)))
    if native == null
        error("cannot cast null")
    else
        error("type mismatch: cannot cast to Node2D")
    return null

def operator as Node(native: NodeNative?)
    if native != null
        return new Node(native)
    error("cannot cast null")
    return null

def operator as Node(native: ObjectNative?)
    if _check_native_type_Node(native)
        return new Node(unsafe(reinterpret<NodeNative?>(native)))
    if native == null
        error("cannot cast null")
    else
        error("type mismatch: cannot cast to Node")
    return null

// ?as

def operator ?as Object(native: ObjectNative?)
    if native != null
        return new Object(native)
    return null

def operator ?as Node2D(native: Node2DNative?)
    if native != null
        return new Node2D(native)
    return null

def operator ?as Node2D(native: ObjectNative?)
    if native != null
        return native as Node2D
    return null

def operator ?as Node(native: NodeNative?)
    if native != null
        return new Node(native)
    return null

def operator ?as Node(native: ObjectNative?)
    if native != null
        return native as Node
    return null

// Wrapper recreation
//
// all the work is done in `as` for native pointers, this extra inderection might be slow
// and maybe this call should be unravelled in the future
//
// casts as X(obj: X?) are unnecessary, but I include them for completeness,
// they might be useful in some cases for an upcast for example

// as

def operator as Object(obj: Object?)
    if obj != null
        return obj
    error("cannot cast null")
    return null

def operator as Node2D(obj: Node?)
    if obj != null
        return obj
    error("cannot cast null")
    return null

def operator as Node2D(obj: Object?)
    if obj != null
        return obj.native as Node2D
    error("cannot cast null")
    return null

def operator as Node(obj: Node?)
    if obj != null
        return obj
    error("cannot cast null")
    return null

def operator as Node(obj: Object?)
    if obj != null
        return obj.native as Node
    error("cannot cast null")
    return null

// ?as

def operator ?as Object(obj: Object?)
    if obj != null
        return obj
    return null

def operator ?as Node2D(obj: Node?)
    if obj != null
        return obj
    return null

def operator ?as Node2D(obj: Object?)
    if obj != null
        return obj.native as Node2D
    return null

def operator ?as Node(obj: Node?)
    if obj != null
        return obj
    return null

def operator ?as Node(obj: Object?)
    if obj != null
        return obj.native as Node
    return null
