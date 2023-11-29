# DasScriptLanguage notes

Here you can find notes on implementation of DasScriptLanguage

## Notes on not overriden functions

* `call_const` - ScriptInstance definition says `// implement if language supports const functions`
* `is_placeholder`, `property_set_fallback` and `property_get_fallback` are only overridden by PlaceHolderScriptInstance and should stay default by an actual ScriptInstance
* `refcount_incremented` and `refcount_decremented` - needed for custom reference counting, see ScriptInstance declaration for details
* `to_string` - I need to understand what counts as a strigification of an instance

## Other notes

`GDScriptInstance::get_method_list` and `GDScript::get_script_method_list` are identical - they both return script lists INCLUDING base scripts. But `GDScriptInstance::has_method` searches base scripts, while `GDScript::has_method` does not. Is this a bug or a feature? I should invastigate the semantics of these methods. For now I will just copy semantics from GDScript, but this may be a bug. Or may be not