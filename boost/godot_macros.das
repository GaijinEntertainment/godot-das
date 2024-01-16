module godot_macros shared

require godot_native
require daslib/ast_boost
require daslib/templates_boost

[structure_macro(name="godot_class")]
class GodotClassAnnotation : AstStructureAnnotation

    def override apply(var st : StructurePtr; var group : ModuleGroup; args : AnnotationArgumentList; var errors : das_string) : bool
        let typeName = st.name |> string()
        // operator as Bunny(native : Object?)
        var inscope asFn <- qmacro_function("`as`{typeName}") <| $(native : Object?)
            return unsafe(reinterpret<$t(st)?>(_get_das_type(native, $v(typeName))))
        asFn.flags |= FunctionFlags generated
        compiling_module() |> add_function(asFn)

        // operator is Bunny(native : Object?)
        var inscope isFn <- qmacro_function("`is`{typeName}") <| $(native : Object?)
            return _check_das_type(native, $v(typeName))
        isFn.flags |= FunctionFlags generated
        compiling_module() |> add_function(isFn)

        // are useless?
        // operator as Bunny(native : Object?)
        var inscope asSelfFn <- qmacro_function("`as`{typeName}") <| $(self : $t(st)?)
            return self
        asSelfFn.flags |= FunctionFlags generated
        compiling_module() |> add_function(asSelfFn)

        // operator is Bunny(native : Object?)
        var inscope isSelfFn <- qmacro_function("`is`{typeName}") <| $(self : $t(st)?)
            return true
        isSelfFn.flags |= FunctionFlags generated
        compiling_module() |> add_function(isSelfFn)

        if args |> length() != 1
            errors := "godot_class: base type not passed. Use [godot_class(<native class>)]"
            return false

        for fi in st.fields
            if fi.name == "native"
                errors := "godot_class: field 'native' already exists, remove it"
                return false
        let nativeTypeName = "H<godot_native::{args[0].name}>"
        // TODo trow exception if type is not found
        var inscope nativeType <- parse_mangled_name(nativeTypeName, group, null)
        // you can technically skip this step by parsing "H<godot_native::{args[0].name}>?" right away
        // but I feel like using proper tools for every step is better
        var inscope nativeTypePtr := qmacro_type(type<$t(nativeType)?>)
        st |> add_structure_field("native", nativeTypePtr, [[ExpressionPtr]])

        let nativeCtor = "{nativeType.annotation.name}`new"
        let scriptPath = st.at.fileInfo.name |> string()

        var inscope newFn <- qmacro_function("{st.name}`new") <| $() : $t(st)?
            var native = $c(nativeCtor)()
            var promoted = _promote_to_das_type(native, $v(scriptPath))
            return unsafe(reinterpret<$t(st)?>(promoted))

        newFn.flags |= FunctionFlags generated
        compiling_module() |> add_function(newFn)

        return true