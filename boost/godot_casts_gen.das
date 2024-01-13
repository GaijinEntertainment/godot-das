// This file should be complitely generated

// "Compile time" casts that already take a corresponding type and don't perform any type change,
// e.g. operator as Node(native: Node?), are required in cases when the function returns
// correct type and we just need to wrap it into a wrapper class - e.g. get_parent() as Node

// For now, as operates as GDScript as - return null if cast fail, and return null if casting null

require godot_native

// Object

def operator as Object(native: Object?)
    return native

def operator is Object(native: Object?)
    return true

// Node

def operator as Node(native: Node?)
    return native

def operator as Node(native: Object?)
    if _check_native_type_Node(native)
        return unsafe(reinterpret<Node?>(native))
    return null

def operator is Node(native: Node?)
    return true

def operator is Node(native: Object?)
    return _check_native_type_Node(native)

// Node2D

def operator as Node2D(native: Node2D?)
    return native

def operator as Node2D(native: Object?)
    if _check_native_type_Node2D(native)
        return unsafe(reinterpret<Node2D?>(native))
    return null

def operator is Node2D(native: Node2D?)
    return true

def operator is Node2D(native: Object?)
    return _check_native_type_Node2D(native)

// InputEventMouseButton

def operator as InputEventMouseButton(native: Object?)
    if _check_native_type_InputEventMouseButton(native)
        return unsafe(reinterpret<InputEventMouseButton?>(native))
    return null

def operator is InputEventMouseButton(native: Object?)
    return _check_native_type_InputEventMouseButton(native)

// extra

def operator as Label(native: Object?)
    if _check_native_type_Label(native)
        return unsafe(reinterpret<Label?>(native))
    return null

def operator as Texture2D(native: Object?)
    if _check_native_type_Texture2D(native)
        return unsafe(reinterpret<Texture2D?>(native))
    return null
