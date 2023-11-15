# Godot-Das: daScript bindings for Godot

In the future!


## Download

To build the project, you will need daScript repo, so download it as a submodule:

```
git clone git@github.com:ilyabelow/godot-das.git
cd godot-das
git submodule update --init
```

Don't use clone with `--recurse-submodules` as it will download daScript submodules which are not needed

## Bulding

Don't forget to clone with `--recurse-submodules` do download daScript repo. To build, use CMake:

```
cmake CMakeFiles.txt
make
```

This builds daScript and its static library. Then, a dynamic library from code in *src/* is build. For now, it just runs *tutorial00.cpp* from *daScript/examples/tutorial/* on extention initialization

If you wish to use a newer version of Godot, you need to update *gdextension_interface.h*. For this, run

```
cd gdextension
godot --dump-gdextension-interface
```

## Testing

Run

```
godot demo/project.godot
```

It should print `this is nano tutorial`, which is the daScript script that is being run on initialization! After this a bunch of error will follow because I don't yet initialize gdextension properly
