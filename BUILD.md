# How To Build

## Dependencies
1. Git
2. CMake
3. Clang (Recomended) or MSVC
4. Ninja (Not Required For MSVC)
5. OpenGL 4.6
6. C++ 23

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
1. Run this command to build from CMake
### For Clang
```sh
cmake -B build -G Ninja -DCMAKE_BUILD_TYPE=Release -DCMAKE_C_COMPILER=clang -DCMAKE_CXX_COMPILER=clang++
```
> **NOTE:** If you want the **Debug** build, then you need to change the `-DCMAKE_BUILD_TYPE=Release` to `-DCMAKE_BUILD_TYPE=Debug`.
### For MSVC
```sh
cmake -B build -G "Visual Studio 17 2022"
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
> **NOTE:** You will need to change some things around if you are on Linux
- When you cloned the repository you should probably get a .vscode with it.
- In the folder you will have `tasks.json` and `launch.json`.

1. Inside VSCode, you can need to press `ctrl + shift + p` or what every your keybind is for vscode's command pallet.
2. Now search for `Run Tasks`. (The option you are looking for is `Tasks: Run Tasks`). Select it.
3. You should get a hord of options. Search for `Clang Make Build (<BUILD_TYPE>)`.
   - Change the `Clang` to `MSVC` if you want to build with the MSVC compiler.
4. Now either go to the Debug Menu in vscode and click the run button.
   - Or press `F5` or whatever you keybind is for running the application. (look for `Debug: Continue` option in your keybindings if you dont know what key that is).