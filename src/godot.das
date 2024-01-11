require godot

class Engine
    def static get_frames_per_second(): float
        return _Engine_get_frames_per_second()

// Wrapper creation from native pointers
//
// "Compile time" casts that already take a corresponding type and don't perform any type change,
// e.g. operator as Node(native: Node?), are required in cases when the function returns
// correct type and we just need to wrap it into a wrapper class - e.g. get_parent() as Node
//
// `?as` operators are the same as `as`, but in case of null they quetly return null instead of printing an error

// as

def operator as Object(native: Object?)
    if native != null
        return native
    error("cannot cast null")
    return null

def operator as Node2D(native: Node2D?)
    if native != null
        return native
    error("cannot cast null to Node2D")
    return null

def operator as Node2D(native: Object?)
    if _check_native_type_Node2D(native)
        return unsafe(reinterpret<Node2D?>(native))
    if native == null
        error("cannot cast null")
    else
        error("type mismatch: cannot cast to Node2D")
    return null

def operator as Node(native: Node?)
    if native != null
        return native
    error("cannot cast null")
    return null

def operator as Node(native: Object?)
    if _check_native_type_Node(native)
        return unsafe(reinterpret<Node?>(native))
    if native == null
        error("cannot cast null")
    else
        error("type mismatch: cannot cast to Node")
    return null

// ?as

def operator ?as Object(native: Object?)
    return native

def operator ?as Node2D(native: Node2D?)
    return native

def operator ?as Node2D(native: Object?)
    if native != null
        return native as Node2D
    return null

def operator ?as Node(native: Node?)
    return native

def operator ?as Node(native: Object?)
    if native != null
        return native as Node
    return null


// is

def operator is Object(native: Object?)
    return true

def operator is Node(native: Node?)
    return true

def operator is Node2D(native: Node2D?)
    return true

def operator is Node(native: Object?)
    return _check_native_type_Node(native)

def operator is Node2D(native: Object?)
    return _check_native_type_Node2D(native)

// okay, all operators above are for demonstration of what should be generated for every type
// below are operators that are actually used in the code. I will not write every cast for every type
// because it will be too much and quickly become very messy

def operator is InputEventMouseButton(native: Object?)
    return _check_native_type_InputEventMouseButton(native)

def operator as InputEventMouseButton(native: Object?)
    if _check_native_type_InputEventMouseButton(native)
        return unsafe(reinterpret<InputEventMouseButton?>(native))
    if native == null
        error("cannot cast null")
    else
        error("type mismatch: cannot cast to InputEventMouseButton")
    return null

def operator as Label(native: Object?)
    if _check_native_type_Label(native)
        return unsafe(reinterpret<Label?>(native))
    if native == null
        error("cannot cast null")
    else
        error("type mismatch: cannot cast to Label")
    return null

def operator as Texture2D(native: Object?)
    if _check_native_type_Texture2D(native)
        return unsafe(reinterpret<Texture2D?>(native))
    if native == null
        error("cannot cast null")
    else
        error("type mismatch: cannot cast to Label")
    return null

// iteration functions
// note that iterate_children will break if you add/remove children during iteration

def get_children(node: Node?)
    let child_count = node |> get_child_count()
    var children: array<Node?>
    children |> reserve(child_count)
    for i in range(0, child_count)
        children |> push(node |> get_child(i))
    return <- children

def iterate_children(node: Node?)
    return <- generator<Node?> () <|
        let child_count = node |> get_child_count()
        for i in range(0, child_count)
            yield node |> get_child(i)
        return false
