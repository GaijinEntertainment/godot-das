# DasScript notes

Here you can find notes on implementation of DasScript

## Notes on virtual functions that were not overriden

* `has_static_method` - something to do with static methods
* `update_exports` - actually important!

* Next three will probably be implemented in the near future:
    * `get_member_line`
    * `get_constants`
    * `get_members`

    as they correspond to getting members and constants of the script

* `is_abstract` - does daScript support abstract classes/scripts? if yes, implement this function properly
* `is_placeholder_fallback_enabled` - is overriden in `GDScript`, but I don't yet understand what it does
* `get_class_category` - is not overloaded in GDScript and probably should not be overloaded at all

## Notes on empty functions

Script functions are mostly about reflection, so they should be filled as more and more features are supported

* `get_base_script` - implement when inheritance is supported
* `get_global_name` - class_name-like functionality
* `inherits_script` - same as the first one
* `get_instance_base_type` - same
* `get_documentation` - implement when documentation is supported
* `get_class_icon_path` - for custom class icons, maybe will be implemented in the far future
* Next four are obvious, but I wonder how important they are:
    * `has_script_signal`
    * `get_script_signal_list`
    * `get_script_method_list`
    * `get_script_property_list`
* `get_rpc_config` - don't know what an rpc config is