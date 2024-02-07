// This file is complitely generated
// TODO `operator . signal` as soon as operator . on pointers is supported

require godot_native
require godot_signals

// Object

let Object`script_changed = "script_changed"

def get_script_changed(obj : Object?)
    return [[Signal name = Object`script_changed, owner = obj]]

let Object`property_list_changed = "property_list_changed"

def get_property_list_changed(obj : Object?)
    return [[Signal name = Object`property_list_changed, owner = obj]]

// Node

let Node`ready = "ready"

def get_ready(obj : Node?)
    return [[Signal name = Node`ready, owner = obj]]

let Node`renamed = "renamed"

def get_renamed(obj : Node?)
    return [[Signal name = Node`renamed, owner = obj]]

let Node`tree_entered = "tree_entered"

def get_tree_entered(obj : Node?)
    return [[Signal name = Node`tree_entered, owner = obj]]

let Node`tree_exiting = "tree_exiting"

def get_tree_exiting(obj : Node?)
    return [[Signal name = Node`tree_exiting, owner = obj]]

let Node`tree_exited = "tree_exited"

def get_tree_exited(obj : Node?)
    return [[Signal name = Node`tree_exited, owner = obj]]

/* TODO support signals with arguments
let Node`child_entered_tree = "child_entered_tree"

def get_child_entered_tree(obj : Node?)
    return [[Signal name = Node`child_entered_tree, owner = obj]]
*/

/* TODO support signals with arguments
let Node`child_exiting_tree = "child_exiting_tree"

def get_child_exiting_tree(obj : Node?)
    return [[Signal name = Node`child_exiting_tree, owner = obj]]
*/

let Node`child_order_changed = "child_order_changed"

def get_child_order_changed(obj : Node?)
    return [[Signal name = Node`child_order_changed, owner = obj]]

/* TODO support signals with arguments
let Node`replacing_by = "replacing_by"

def get_replacing_by(obj : Node?)
    return [[Signal name = Node`replacing_by, owner = obj]]
*/

// Timer

let Timer`timeout = "timeout"

def get_timeout(obj : Timer?)
    return [[Signal name = Timer`timeout, owner = obj]]

// CanvasItem

let CanvasItem`draw = "draw"

def get_draw(obj : CanvasItem?)
    return [[Signal name = CanvasItem`draw, owner = obj]]

let CanvasItem`visibility_changed = "visibility_changed"

def get_visibility_changed(obj : CanvasItem?)
    return [[Signal name = CanvasItem`visibility_changed, owner = obj]]

let CanvasItem`hidden = "hidden"

def get_hidden(obj : CanvasItem?)
    return [[Signal name = CanvasItem`hidden, owner = obj]]

let CanvasItem`item_rect_changed = "item_rect_changed"

def get_item_rect_changed(obj : CanvasItem?)
    return [[Signal name = CanvasItem`item_rect_changed, owner = obj]]

// Sprite2D

let Sprite2D`frame_changed = "frame_changed"

def get_frame_changed(obj : Sprite2D?)
    return [[Signal name = Sprite2D`frame_changed, owner = obj]]

let Sprite2D`texture_changed = "texture_changed"

def get_texture_changed(obj : Sprite2D?)
    return [[Signal name = Sprite2D`texture_changed, owner = obj]]

// Area2D

/* TODO support signals with arguments
let Area2D`body_shape_entered = "body_shape_entered"

def get_body_shape_entered(obj : Area2D?)
    return [[Signal name = Area2D`body_shape_entered, owner = obj]]
*/

/* TODO support signals with arguments
let Area2D`body_shape_exited = "body_shape_exited"

def get_body_shape_exited(obj : Area2D?)
    return [[Signal name = Area2D`body_shape_exited, owner = obj]]
*/

/* TODO support signals with arguments
let Area2D`body_entered = "body_entered"

def get_body_entered(obj : Area2D?)
    return [[Signal name = Area2D`body_entered, owner = obj]]
*/

/* TODO support signals with arguments
let Area2D`body_exited = "body_exited"

def get_body_exited(obj : Area2D?)
    return [[Signal name = Area2D`body_exited, owner = obj]]
*/

/* TODO support signals with arguments
let Area2D`area_shape_entered = "area_shape_entered"

def get_area_shape_entered(obj : Area2D?)
    return [[Signal name = Area2D`area_shape_entered, owner = obj]]
*/

/* TODO support signals with arguments
let Area2D`area_shape_exited = "area_shape_exited"

def get_area_shape_exited(obj : Area2D?)
    return [[Signal name = Area2D`area_shape_exited, owner = obj]]
*/

/* TODO support signals with arguments
let Area2D`area_entered = "area_entered"

def get_area_entered(obj : Area2D?)
    return [[Signal name = Area2D`area_entered, owner = obj]]
*/

/* TODO support signals with arguments
let Area2D`area_exited = "area_exited"

def get_area_exited(obj : Area2D?)
    return [[Signal name = Area2D`area_exited, owner = obj]]
*/

// Window

/* TODO support signals with arguments
let Window`window_input = "window_input"

def get_window_input(obj : Window?)
    return [[Signal name = Window`window_input, owner = obj]]
*/

/* TODO support signals with arguments
let Window`files_dropped = "files_dropped"

def get_files_dropped(obj : Window?)
    return [[Signal name = Window`files_dropped, owner = obj]]
*/

let Window`mouse_entered = "mouse_entered"

def get_mouse_entered(obj : Window?)
    return [[Signal name = Window`mouse_entered, owner = obj]]

let Window`mouse_exited = "mouse_exited"

def get_mouse_exited(obj : Window?)
    return [[Signal name = Window`mouse_exited, owner = obj]]

let Window`focus_entered = "focus_entered"

def get_focus_entered(obj : Window?)
    return [[Signal name = Window`focus_entered, owner = obj]]

let Window`focus_exited = "focus_exited"

def get_focus_exited(obj : Window?)
    return [[Signal name = Window`focus_exited, owner = obj]]

let Window`close_requested = "close_requested"

def get_close_requested(obj : Window?)
    return [[Signal name = Window`close_requested, owner = obj]]

let Window`go_back_requested = "go_back_requested"

def get_go_back_requested(obj : Window?)
    return [[Signal name = Window`go_back_requested, owner = obj]]

let Window`visibility_changed = "visibility_changed"

def get_visibility_changed(obj : Window?)
    return [[Signal name = Window`visibility_changed, owner = obj]]

let Window`about_to_popup = "about_to_popup"

def get_about_to_popup(obj : Window?)
    return [[Signal name = Window`about_to_popup, owner = obj]]

let Window`theme_changed = "theme_changed"

def get_theme_changed(obj : Window?)
    return [[Signal name = Window`theme_changed, owner = obj]]

let Window`dpi_changed = "dpi_changed"

def get_dpi_changed(obj : Window?)
    return [[Signal name = Window`dpi_changed, owner = obj]]

let Window`titlebar_changed = "titlebar_changed"

def get_titlebar_changed(obj : Window?)
    return [[Signal name = Window`titlebar_changed, owner = obj]]

// Resource

let Resource`changed = "changed"

def get_changed(obj : Resource?)
    return [[Signal name = Resource`changed, owner = obj]]

let Resource`setup_local_to_scene_requested = "setup_local_to_scene_requested"

def get_setup_local_to_scene_requested(obj : Resource?)
    return [[Signal name = Resource`setup_local_to_scene_requested, owner = obj]]

