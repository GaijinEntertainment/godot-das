module godot_macros shared

require godot_native
require daslib/ast_boost
require daslib/templates_boost

[structure_macro(name="godot_class")]
class GodotClassAnnotation : AstStructureAnnotation

    def override apply(var st : StructurePtr; var group : ModuleGroup; args : AnnotationArgumentList; var errors : das_string) : bool
        let typeName = st.name |> string()
        var inscope asFn <- qmacro_function("`as`{typeName}") <| $(native : Object?)
            return unsafe(reinterpret<$t(st)?>(_get_das_type(native, $v(typeName))))

        asFn.flags |= FunctionFlags generated
        compiling_module() |> add_function(asFn)

        var inscope isFn <- qmacro_function("`is`{typeName}") <| $(native : Object?)
            return _check_das_type(native, $v(typeName))

        isFn.flags |= FunctionFlags generated
        compiling_module() |> add_function(isFn)

        // TODO
        // def operator as Foo(obj : Foo?)
        // 	return obj

        // TODO
        // def operator is Foo(obj : Foo?)
        // 	return true

        var inscope nativeType : smart_ptr<ast::TypeDecl>
        for f in st.fields
            if f.name == "native"
                nativeType := f._type
                break

        // to eliminate all these checks it's better to hide annotation in a base class with predefined native field
        if nativeType == null || nativeType.baseType != Type tPointer || nativeType.firstType.baseType != Type tHandle
            errors := "godot_class: native field not found. Please add a field named 'native' of type 'Object?'"
            return false

        let nativeCtor = "{nativeType.firstType.annotation.name}`new"

        var inscope newFn <- qmacro_method("{st.name}`new", st) <| $() : $t(st)?
            var native = $c(nativeCtor)()
            var promoted = _promote_to_das_type(native, $v(typeName))
            return unsafe(reinterpret<$t(st)?>(promoted))

        newFn.flags |= FunctionFlags generated
        compiling_module() |> add_function(newFn)

        return true