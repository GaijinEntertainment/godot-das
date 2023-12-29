# Godot-Das: daScript bindings for Godot


<img src="logo.png" width="200" height="200">


This project is a Godot module that ~~integrates~~ will integrate [daScript](https://dascript.org/) scripting language into Godot game engine

## Download

This project is build as an engine submodule (see why not as a GDExtension in docs), so you will need to download Godot repo:

```
git clone -b 4.2 git@github.com:godotengine/godot.git
```

The develompent is done on Godot's `4.2` branch (just for safety)

Next, clone this repo as a submodule in *godot/modules/*. Rename the directory to *dascript*:

```
cd godot/modules
git clone git@github.com:ilyabelow/godot-das.git dascript
```

You will also need daScript, so download its repo as a submodule:

```
cd dascript
git submodule update --init
```

Don't use clone with `--recurse-submodules` as it will download daScript submodules which are not needed

## Bulding

First, build daScript static library:

```
cmake -B __cmake_temp
cmake --build __cmake_temp -j<cores>
```

This will put *liblibDaScript.a* in *lib* directory. The project will only need headers from *daScript/include/*, so you can delete the rest in *daScript* directory

Next, build Godot:

```
cd <godot root directory>
scons (or pyston-scons if configured)
```

daScript module will be enabled by default, you can disable it with `module_dascript_enabled=no` (or change `is_enabled` function in `config.py`)

Use `linker=lld` or `linker=mold` because default `ld` is super slow

> Important: daScript is built with `g++`, so make sure **not** to use `clang` for building Godot!

*Temporary solution!* if you want to move godot folder, make sure to recompile it or set `COMPILE_FOR_IN_PLACE_TEST = False` in *SCSub*. Otherwise I recommend you leaving it because it removed the nessesety to recompile Godot every time you change `godot.das`

## Testing

Run

```
bin/<your godot binary> modules/dascript/demo/project.godot
```

It should print `It's enter tree!` and `It's ready!` and rotate! You can remove `options tool = true` to disable tool mode (so the object does nothing in editor)

Add `--windowed` if using a big monitor

Don't forget to add `dev_build=yes` to scons config for debugging. If you want a release build, remove `set(CMAKE_BUILD_TYPE Debug)` from *CMakeLists.txt*

Build and debug jobs, as well as intellisense configuration for VSCode can be found in *vscode_files*, to use them

```
cd <godot root directory>
ln -s modules/dascript/_vscode_files .vscode
```

