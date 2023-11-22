# About GDExtension

**TODO:** document my thoughts and findings about why GDExtension is not suitable for language bindings

Reference: https://sampruden.github.io/posts/godot-is-not-the-new-unity

In short: GDExtension is a pretty thick glue between the engine and an extension, in our case we call C++ function from another C++ function, which makes no sence to do via this layer. It's way more efficient to call engine code directly, it worth sacrificing some of the convenience of GDExtension for that