// This file is complitely generated

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

def operator is Node(native: Node? implicit)
    return true

def operator is Node(native: Object? implicit)
    return _check_native_type_Node(native)

// Timer

def operator as Timer(native: Timer?)
    return native

def operator as Timer(native: Object?)
    if _check_native_type_Timer(native)
        return unsafe(reinterpret<Timer?>(native))
    return null

def operator is Timer(native: Timer? implicit)
    return true

def operator is Timer(native: Object? implicit)
    return _check_native_type_Timer(native)

// CanvasItem

def operator as CanvasItem(native: CanvasItem?)
    return native

def operator as CanvasItem(native: Object?)
    if _check_native_type_CanvasItem(native)
        return unsafe(reinterpret<CanvasItem?>(native))
    return null

def operator is CanvasItem(native: CanvasItem? implicit)
    return true

def operator is CanvasItem(native: Object? implicit)
    return _check_native_type_CanvasItem(native)

// Node2D

def operator as Node2D(native: Node2D?)
    return native

def operator as Node2D(native: Object?)
    if _check_native_type_Node2D(native)
        return unsafe(reinterpret<Node2D?>(native))
    return null

def operator is Node2D(native: Node2D? implicit)
    return true

def operator is Node2D(native: Object? implicit)
    return _check_native_type_Node2D(native)

// Sprite2D

def operator as Sprite2D(native: Sprite2D?)
    return native

def operator as Sprite2D(native: Object?)
    if _check_native_type_Sprite2D(native)
        return unsafe(reinterpret<Sprite2D?>(native))
    return null

def operator is Sprite2D(native: Sprite2D? implicit)
    return true

def operator is Sprite2D(native: Object? implicit)
    return _check_native_type_Sprite2D(native)

// Area2D

def operator as Area2D(native: Area2D?)
    return native

def operator as Area2D(native: Object?)
    if _check_native_type_Area2D(native)
        return unsafe(reinterpret<Area2D?>(native))
    return null

def operator is Area2D(native: Area2D? implicit)
    return true

def operator is Area2D(native: Object? implicit)
    return _check_native_type_Area2D(native)

// Label

def operator as Label(native: Label?)
    return native

def operator as Label(native: Object?)
    if _check_native_type_Label(native)
        return unsafe(reinterpret<Label?>(native))
    return null

def operator is Label(native: Label? implicit)
    return true

def operator is Label(native: Object? implicit)
    return _check_native_type_Label(native)

// Window

def operator as Window(native: Window?)
    return native

def operator as Window(native: Object?)
    if _check_native_type_Window(native)
        return unsafe(reinterpret<Window?>(native))
    return null

def operator is Window(native: Window? implicit)
    return true

def operator is Window(native: Object? implicit)
    return _check_native_type_Window(native)

// RefCounted

def operator as RefCounted(native: RefCounted?)
    return native

def operator as RefCounted(native: Object?)
    if _check_native_type_RefCounted(native)
        return unsafe(reinterpret<RefCounted?>(native))
    return null

def operator is RefCounted(native: RefCounted? implicit)
    return true

def operator is RefCounted(native: Object? implicit)
    return _check_native_type_RefCounted(native)

// Resource

def operator as Resource(native: Resource?)
    return native

def operator as Resource(native: Object?)
    if _check_native_type_Resource(native)
        return unsafe(reinterpret<Resource?>(native))
    return null

def operator is Resource(native: Resource? implicit)
    return true

def operator is Resource(native: Object? implicit)
    return _check_native_type_Resource(native)

def operator as Resource(native: Resource?#)
    unsafe
        return native

def operator as Resource(native: Object?#)
    unsafe
        if _check_native_type_Resource(native)
            return unsafe(reinterpret<Resource?#>(native))
        return null

// PackedScene

def operator as PackedScene(native: PackedScene?)
    return native

def operator as PackedScene(native: Object?)
    if _check_native_type_PackedScene(native)
        return unsafe(reinterpret<PackedScene?>(native))
    return null

def operator is PackedScene(native: PackedScene? implicit)
    return true

def operator is PackedScene(native: Object? implicit)
    return _check_native_type_PackedScene(native)

def operator as PackedScene(native: PackedScene?#)
    unsafe
        return native

def operator as PackedScene(native: Object?#)
    unsafe
        if _check_native_type_PackedScene(native)
            return unsafe(reinterpret<PackedScene?#>(native))
        return null

// InputEvent

def operator as InputEvent(native: InputEvent?)
    return native

def operator as InputEvent(native: Object?)
    if _check_native_type_InputEvent(native)
        return unsafe(reinterpret<InputEvent?>(native))
    return null

def operator is InputEvent(native: InputEvent? implicit)
    return true

def operator is InputEvent(native: Object? implicit)
    return _check_native_type_InputEvent(native)

def operator as InputEvent(native: InputEvent?#)
    unsafe
        return native

def operator as InputEvent(native: Object?#)
    unsafe
        if _check_native_type_InputEvent(native)
            return unsafe(reinterpret<InputEvent?#>(native))
        return null

// InputEventMouseButton

def operator as InputEventMouseButton(native: InputEventMouseButton?)
    return native

def operator as InputEventMouseButton(native: Object?)
    if _check_native_type_InputEventMouseButton(native)
        return unsafe(reinterpret<InputEventMouseButton?>(native))
    return null

def operator is InputEventMouseButton(native: InputEventMouseButton? implicit)
    return true

def operator is InputEventMouseButton(native: Object? implicit)
    return _check_native_type_InputEventMouseButton(native)

def operator as InputEventMouseButton(native: InputEventMouseButton?#)
    unsafe
        return native

def operator as InputEventMouseButton(native: Object?#)
    unsafe
        if _check_native_type_InputEventMouseButton(native)
            return unsafe(reinterpret<InputEventMouseButton?#>(native))
        return null

// Texture2D

def operator as Texture2D(native: Texture2D?)
    return native

def operator as Texture2D(native: Object?)
    if _check_native_type_Texture2D(native)
        return unsafe(reinterpret<Texture2D?>(native))
    return null

def operator is Texture2D(native: Texture2D? implicit)
    return true

def operator is Texture2D(native: Object? implicit)
    return _check_native_type_Texture2D(native)

def operator as Texture2D(native: Texture2D?#)
    unsafe
        return native

def operator as Texture2D(native: Object?#)
    unsafe
        if _check_native_type_Texture2D(native)
            return unsafe(reinterpret<Texture2D?#>(native))
        return null

