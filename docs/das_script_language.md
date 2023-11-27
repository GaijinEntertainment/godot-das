# DasScriptLanguage notes

Here you can find notes on implementation of DasScriptLanguage

## Notes on virtual functions that were not overriden

* `is_using_templates` - override when templates for different base nodes are supported
* `validate_path` - override if script file path is forbidden due to conflicts with language itself
* `supports_documentation` - override to return true when script documentation is supported
* `can_inherit_from_file` - override to return true once file inheritance is supported
* `open_in_external_editor` - das doesn't have a dedicated external editor, so  don't ever override
* `overrides_external_editor` - also is needed only for c-sharp
* `complete_code` - code completion goes here
* `lookup_code` - not sure what this does, but it's also a part of completion mechanism
* `add_named_global_constant` - override when support global constants via autoloads
* `remove_named_global_constant` - same
* `thread_enter` - override when supporting thread pools
* `thread_exit` - same

## Notes on empty functions

* `get_doc_comment_delimiters` - daScript doesn't support documentation comments as I understand, so this function will be empty
* `get_built_in_templates` - templates for different base nodes, pairs with `is_using_templates`
* `validate` - validate script text and print errors? pretty important
* `find_function` - used by connecting signals
* `create_function` - same
* `auto_indent_code` - something to do with auto indentation, obviously
* `add_global_constant` - integration with autoloaders
* `debug_\*` - function for built in debugger
* `reload_all_scripts` - used by debugger only
* `reload_tool_script` - ??? but it's important I guess
* `get_public_*` - ??? something to do with testing
* `profiling_*` - profiling functions, to be supported
* `frame` - same
* `get_global_class_name` - for global classes (which can be created or inherited from any script), need support
## Other notes

`Mutex mutex;` -  why do we need a mutex here?