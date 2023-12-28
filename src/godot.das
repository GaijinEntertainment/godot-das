require godot

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
