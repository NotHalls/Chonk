# How To Build

## Dependencies
1. Git
2. CMake 3.27+
3. Clang (Recomended) or MSVC
4. Ninja (Not Required For MSVC)
5. OpenGL 4.6
6. C++ 23+

## Cloning The Repository:
1. Open Your Terminal
2. Go To The Folder Where You Want To Clone The Repo
3. Run This Command
```sh
git clone https://github.com/NotHalls/Chonk.git --recursive
```
> **NOTE:** You **must** Include The `--recursive` as it will clone all the submodules with the main repository.
<br>

> **NOTE:** If you did't clone the repo with `--recursive`, you can just run this command. inside the cloned folder.<br>
the `cd` command takes care of that.
```sh
cd Chonk
git submodule update --init --recursive
```

## Building:
- Make sure you are in the root folder of the project
- Depending On Your Choice Change The Presets. <br>
#### Available Presets Are:
1. Clang-Debug
2. Clang-Release
3. Clang-Export
1. MSVC-Debug
2. MSVC-Release
3. MSVC-Export

The Command To Build The Project:
```sh
cmake --preset <THE-PRESET>
```

## Compiling:
- Make sure you are in the root folder of the project
1. Run this command to compile the project from the cmake build files
> **NOTE:** The process is the same for both MSVC and Clang
```sh
cmake --build build
```
- Now you should get a `Chonk.exe` or just `Chonk` if you are on Linux

<br>

---
<br>

## Building From VSCode
- When you cloned the repository you should probably get a .vscode with it.
- In the folder you will have `tasks.json`.
> **NOTE:** I use the `CMake Tools` extensions so I don't have a `launch.json`. I recomend you get that extension aswell, it's useful. Well if you don't wanna, making a simple `launch.json` shouldn't be that hard

1. Inside VSCode, you need to press `ctrl + shift + p` or what every your keybind is for vscode's command pallet.
2. Now search for `Run Tasks`. (The option you are looking for is `Tasks: <TOOLCHAIN> CMake Build <BUILD_TYPE>`). Select it.

> **NOTE:** In The `CMake Tools` Extension, You need to search for `@ext:ms-vscode.cmake-tools Use C Make Presets` and set the values to `Always` (This Is Just A Recomendation).