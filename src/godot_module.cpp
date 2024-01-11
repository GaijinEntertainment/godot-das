#include "das_script_instance.h"
#include "das_script_language.h"

#include <daScript/daScript.h>
#include <daScript/misc/platform.h>

#include <scene/2d/node_2d.h>
#include <scene/2d/sprite_2d.h>
#include <core/math/vector2.h>
#include <scene/gui/label.h>
#include <core/io/resource_loader.h>
#include <scene/main/window.h>
#include <scene/resources/texture.h>

#include <vector>
#include <fstream>

// taken from dagor/prog/gameLibs/publicInclude/dasModules/dasModulesCommon.h
// typeName and das_alias do not seem to be vital but may be required in the future
#define MAKE_TYPE_FACTORY_ALIAS(TYPE, DAS_DECL_TYPE)           \
namespace das {                                                \
  template <>                                                  \
  struct typeFactory<TYPE>                                     \
  {                                                            \
    static TypeDeclPtr make(const ModuleLibrary &lib)          \
    {                                                          \
      auto t = make_smart<TypeDecl>(Type::DAS_DECL_TYPE);      \
      t->alias = #TYPE;                                        \
      t->aotAlias = true;                                      \
      return t;                                                \
    }                                                          \
  };                                                           \
}

#define MAKE_NATIVE_TYPE_FACTORY(TYPE) MAKE_TYPE_FACTORY(TYPE, TYPE)


#define BIND_NATIVE_BASE(TYPE)\
struct TYPE##Annotation : das::ManagedStructureAnnotation<TYPE> {\
    TYPE##Annotation(das::ModuleLibrary & ml) : ManagedStructureAnnotation(#TYPE, ml) { }\
};\
das::smart_ptr<TYPE##Annotation> TYPE##ManagedStructureAnnotation = das::make_smart<TYPE##Annotation>(lib);\
addAnnotation(TYPE##ManagedStructureAnnotation);


#define BIND_NATIVE_TYPE(TYPE, PARENT)\
struct TYPE##Annotation : das::ManagedStructureAnnotation<TYPE> {\
    das::TypeDeclPtr parentType;\
    das::smart_ptr<das::ManagedStructureAnnotation<PARENT>> parent_annotation;\
    \
    TYPE##Annotation(das::ModuleLibrary & ml,\
                     das::smart_ptr<das::ManagedStructureAnnotation<PARENT>> parent_annotation) :\
                     ManagedStructureAnnotation(#TYPE, ml),\
                     parent_annotation(parent_annotation),\
                     parentType(das::makeType<PARENT>(ml)) {}\
    \
    bool canBeSubstituted(TypeAnnotation *pass_type) const override {\
        return parentType->annotation == pass_type || parent_annotation->canBeSubstituted(pass_type);\
    }\
};\
das::smart_ptr<TYPE##Annotation> TYPE##ManagedStructureAnnotation = das::make_smart<TYPE##Annotation>(lib, PARENT##ManagedStructureAnnotation);\
addAnnotation(TYPE##ManagedStructureAnnotation);

template <typename T>
bool _check_native_type(const Object* obj) {
    return dynamic_cast<const T*>(obj) != nullptr;
}

#define BIND_TYPE_CHECKER(TYPE)  das::addExtern<DAS_BIND_FUN(_check_native_type<TYPE>)>(*this, lib, "_check_native_type_"#TYPE, das::SideEffects::none, "_check_native_type<"#TYPE">");

#define CTX_AT das::Context *ctx, das::LineInfoArg *at
#define CHECK_IF_NULL_MSG(PTR, MSG) if (PTR == nullptr) { ctx->throw_error_at(at, MSG); return {}; }
#define CHECK_IF_NULL(PTR) CHECK_IF_NULL_MSG(PTR, "dereferencing null pointer")
#define CHECK_IF_NULL_VOID(PTR) if (PTR == nullptr) { ctx->throw_error_at(at, "dereferencing null pointer"); return; }

char* _get_dascript_type(const Object* obj, const char* name, CTX_AT) {
    if (obj == nullptr) {
        ctx->throw_error_at(at, "cannot cast null");
        return nullptr;
    }
    DasScriptInstance* instance = dynamic_cast<DasScriptInstance*>(obj->get_script_instance());
    if (instance == nullptr || strcmp(instance->get_das_script()->get_class_name(), name) != 0) {
        // two cases: either the object does not have a das script instance, or the das script instance is not of the correct type
        ctx->throw_error_at(at, (std::string("type mismatch: cannot cast native type to user type ") + std::string(name)).c_str());
        return nullptr;
    }
    return instance->get_class_ptr();
}

bool _check_dascript_type(const Object* obj, const char* name) {
    if (obj == nullptr) {
        return false;
    }
    DasScriptInstance* instance = dynamic_cast<DasScriptInstance*>(obj->get_script_instance());
    if (instance == nullptr || strcmp(instance->get_das_script()->get_class_name(), name) != 0) {
        return false;
    }
    return true;
}

void* _promote_to_das_type(Object* obj, const char* script_name, CTX_AT) {
    DasScript* das_script = DasScriptLanguage::get_singleton()->get_script(script_name);
    CHECK_IF_NULL_MSG(das_script, (std::string("cannot find script ") + std::string(script_name)).c_str());
    obj->set_script(das_script);
    return reinterpret_cast<DasScriptInstance*>(obj->get_script_instance())->get_class_ptr();
}

MAKE_NATIVE_TYPE_FACTORY(Object)
MAKE_NATIVE_TYPE_FACTORY(Node)
MAKE_NATIVE_TYPE_FACTORY(CanvasItem)
MAKE_NATIVE_TYPE_FACTORY(Node2D)
MAKE_NATIVE_TYPE_FACTORY(Label)
MAKE_NATIVE_TYPE_FACTORY(Sprite2D)
MAKE_NATIVE_TYPE_FACTORY(Window)


MAKE_NATIVE_TYPE_FACTORY(Resource)
MAKE_NATIVE_TYPE_FACTORY(Texture2D)
MAKE_NATIVE_TYPE_FACTORY(InputEvent)
MAKE_NATIVE_TYPE_FACTORY(InputEventMouseButton)

DAS_BIND_ENUM_CAST(MouseButton)
DAS_BASE_BIND_ENUM(MouseButton, MouseButton, NONE, LEFT, RIGHT, MIDDLE)

MAKE_TYPE_FACTORY_ALIAS(Vector2, tFloat2);
template <> struct das::cast<Vector2> : das::cast_fVec_half<Vector2> {};

Resource* _load(Object* obj, const char* p_path, CTX_AT) {
    CHECK_IF_NULL_MSG(obj, "cannot bind resource to null object");
    auto script_instance = dynamic_cast<DasScriptInstance*>(obj->get_script_instance());
    CHECK_IF_NULL_MSG(script_instance, "cannot bind resource to non-das object");
    Ref<Resource> res = ResourceLoader::load(p_path);
    if (res.is_valid()) {
        // tmp solution
        // TODO support das::smart_ptr<Resource> or Ref<Resource> in das
        // so ref counting is handled by das
        script_instance->bind_resource(res);
    }
    return res.ptr();
}

Node* _Node_find_child(const Node* node, const char* p_pattern, CTX_AT) {
    CHECK_IF_NULL(node)
    // TODO add p_recursive and p_owned
    return node->find_child(p_pattern, true, true);
}

Node* _Node_get_parent(const Node* node, CTX_AT) {
    CHECK_IF_NULL(node)
    return node->get_parent();
}

const char* _Node_get_name(const Node* node, CTX_AT) {
    CHECK_IF_NULL(node)
    // TODO bind StringName?
    return ctx->stringHeap->allocateString(String(node->get_name()).utf8().get_data());
}

Node* _Node_get_child(const Node* node, int p_index, CTX_AT) {
    CHECK_IF_NULL(node)
    // TODO add p_include_internal
    return node->get_child(p_index, false);
}

int _Node_get_child_count(const Node* node, CTX_AT) {
    CHECK_IF_NULL(node)
    // TODO add p_include_internal
    return node->get_child_count(false);
}

void _Node_add_child(Node* node, Node* p_child, CTX_AT) {
    CHECK_IF_NULL_VOID(node)
    node->add_child(p_child);
}

Window* _Node_get_window(const Node* node, CTX_AT) {
    CHECK_IF_NULL(node)
    return node->get_window();
}

void _Node2D_rotate(Node2D* node2d, float p_radians, CTX_AT) {
    CHECK_IF_NULL_VOID(node2d)
    node2d->rotate(p_radians);
}

void _Node2D_translate(Node2D* node2d, const Vector2 p_amount, CTX_AT) {
    CHECK_IF_NULL_VOID(node2d)
    node2d->translate(p_amount);
}

void _Node2D_set_position(Node2D* node2d, const Vector2 p_pos, CTX_AT) {
    CHECK_IF_NULL_VOID(node2d)
    node2d->set_position(p_pos);
}

Vector2 _Node2D_get_position(const Node2D* node2d, CTX_AT) {
    CHECK_IF_NULL(node2d)
    return node2d->get_position();
}

MouseButton _InputEventMouseButton_get_button_index(const InputEventMouseButton* event, CTX_AT) {
    CHECK_IF_NULL(event)
    return event->get_button_index();
}

bool _InputEvent_is_pressed(const InputEvent* event, CTX_AT) {
    CHECK_IF_NULL(event)
    return event->is_pressed();
}

Sprite2D* _Sprite2D_new() {
    return memnew(Sprite2D);
}

void _Label_set_text(Label* label, const char* text, CTX_AT) {
    CHECK_IF_NULL_VOID(label)
    label->set_text(text);
}

float _Engine_get_frames_per_second() {
    return Engine::get_singleton()->get_frames_per_second();
}

void _Sprite2D_set_texture(Sprite2D* sprite, Texture2D* texture, CTX_AT) {
    CHECK_IF_NULL_VOID(sprite)
    sprite->set_texture(texture);
}

Vector2 _Texture2D_get_size(Texture2D* texture, CTX_AT) {
    CHECK_IF_NULL(texture)
    return texture->get_size();
}

Vector2 _Window_get_size(Window* window, CTX_AT) {
    CHECK_IF_NULL(window)
    return Vector2{window->get_size()}; // returning Vector2 instead of Vector2i
}

// get_global_mouse_position
Vector2 _CanvasItem_get_global_mouse_position(CanvasItem* canvas_item, CTX_AT) {
    CHECK_IF_NULL(canvas_item)
    return canvas_item->get_global_mouse_position();
}

class Module_Godot : public das::Module {
public:

    Module_Godot() : Module("godot") {
        das::ModuleLibrary lib(this);

        BIND_NATIVE_BASE(Object)
        BIND_NATIVE_TYPE(Node, Object)
        BIND_NATIVE_TYPE(CanvasItem, Node)
        BIND_NATIVE_TYPE(Node2D, CanvasItem)
        BIND_NATIVE_TYPE(Sprite2D, Node2D)
        BIND_NATIVE_TYPE(Label, CanvasItem)
        BIND_NATIVE_TYPE(Window, Node)

        BIND_NATIVE_TYPE(Resource, Object)
        BIND_NATIVE_TYPE(Texture2D, Resource)
        BIND_NATIVE_TYPE(InputEvent, Resource)
        BIND_NATIVE_TYPE(InputEventMouseButton, InputEvent)

		addEnumeration(das::make_smart<EnumerationMouseButton>());

        das::addExtern<DAS_BIND_FUN(_Node2D_rotate)>(*this, lib, "rotate", das::SideEffects::modifyExternal, "_Node2D_rotate");
        das::addExtern<DAS_BIND_FUN(_Node2D_translate)>(*this, lib, "translate", das::SideEffects::modifyExternal, "_Node2D_translate");
        das::addExtern<DAS_BIND_FUN(_Node2D_get_position)>(*this, lib, "get_position", das::SideEffects::modifyExternal, "_Node2D_get_position");
        das::addExtern<DAS_BIND_FUN(_Node2D_set_position)>(*this, lib, "set_position", das::SideEffects::modifyExternal, "_Node2D_set_position");
        das::addExtern<DAS_BIND_FUN(_Node_get_parent)>(*this, lib, "get_parent", das::SideEffects::modifyExternal, "_Node_get_parent");
        das::addExtern<DAS_BIND_FUN(_Node_find_child)>(*this, lib, "find_child", das::SideEffects::modifyExternal, "_Node_find_child");
        das::addExtern<DAS_BIND_FUN(_Node_get_name)>(*this, lib, "get_name", das::SideEffects::modifyExternal, "_Node_get_name");
        das::addExtern<DAS_BIND_FUN(_Node_get_child)>(*this, lib, "get_child", das::SideEffects::modifyExternal, "_Node_get_child");
        das::addExtern<DAS_BIND_FUN(_Node_get_child_count)>(*this, lib, "get_child_count", das::SideEffects::modifyExternal, "_Node_get_child_count");
        das::addExtern<DAS_BIND_FUN(_Node_add_child)>(*this, lib, "add_child", das::SideEffects::modifyExternal, "_Node_add_child");
        das::addExtern<DAS_BIND_FUN(_InputEventMouseButton_get_button_index)>(*this, lib, "get_button_index", das::SideEffects::modifyExternal, "_InputEventMouseButton_get_button_index");
        das::addExtern<DAS_BIND_FUN(_InputEvent_is_pressed)>(*this, lib, "is_pressed", das::SideEffects::modifyExternal, "_InputEvent_is_pressed");
        das::addExtern<DAS_BIND_FUN(_Sprite2D_new)>(*this, lib, "Sprite2D_new", das::SideEffects::modifyExternal, "_Sprite2D_new");
        das::addExtern<DAS_BIND_FUN(_promote_to_das_type)>(*this, lib, "_promote_to_das_type", das::SideEffects::modifyExternal, "_promote_to_das_type");
        das::addExtern<DAS_BIND_FUN(_Label_set_text)>(*this, lib, "set_text", das::SideEffects::modifyExternal, "_Label_set_text");
        das::addExtern<DAS_BIND_FUN(_Engine_get_frames_per_second)>(*this, lib, "_Engine_get_frames_per_second", das::SideEffects::modifyExternal, "_Engine_get_frames_per_second");
        das::addExtern<DAS_BIND_FUN(_load)>(*this, lib, "load", das::SideEffects::modifyExternal, "_load");
        das::addExtern<DAS_BIND_FUN(_Sprite2D_set_texture)>(*this, lib, "set_texture", das::SideEffects::modifyExternal, "_Sprite2D_set_texture");
        das::addExtern<DAS_BIND_FUN(_Texture2D_get_size)>(*this, lib, "get_size", das::SideEffects::modifyExternal, "_Texture2D_get_size");
        das::addExtern<DAS_BIND_FUN(_Node_get_window)>(*this, lib, "get_window", das::SideEffects::modifyExternal, "_Node_get_window");
        das::addExtern<DAS_BIND_FUN(_Window_get_size)>(*this, lib, "get_size", das::SideEffects::modifyExternal, "_Window_get_size");
        das::addExtern<DAS_BIND_FUN(_CanvasItem_get_global_mouse_position)>(*this, lib, "get_global_mouse_position", das::SideEffects::modifyExternal, "_CanvasItem_get_global_mouse_position");

        BIND_TYPE_CHECKER(Node)
        BIND_TYPE_CHECKER(Node2D)
        BIND_TYPE_CHECKER(InputEventMouseButton)
        BIND_TYPE_CHECKER(Label)
        BIND_TYPE_CHECKER(Texture2D)

        das::addExtern<DAS_BIND_FUN(_get_dascript_type)>(*this, lib, "_get_dascript_type", das::SideEffects::modifyExternal, "_get_dascript_type");
        das::addExtern<DAS_BIND_FUN(_check_dascript_type)>(*this, lib, "_check_dascript_type", das::SideEffects::modifyExternal, "_check_dascript_type");

        addAlias(das::typeFactory<Vector2>::make(lib));

        options["tool"] = das::Type::tBool;
#ifdef GODOT_DAS_MODULE_PATH
        const char* godot_module_full_path = GODOT_DAS_MODULE_PATH"/src/godot.das";
        std::ifstream input{godot_module_full_path, std::ios_base::binary};
        std::vector<unsigned char> buffer{std::istreambuf_iterator< char>(input), {} };
        unsigned char* modules_dascript_src_godot_das = buffer.data();
        unsigned int modules_dascript_src_godot_das_len = buffer.size();
#else
        #include "godot.das.inc"
#endif
        compileBuiltinModule("godot.das", modules_dascript_src_godot_das, modules_dascript_src_godot_das_len);
    }
};

REGISTER_MODULE(Module_Godot);
