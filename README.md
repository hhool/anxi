# anxi

## Description

This is solution for 2000C machine remote control.

## requirements

- CMake 3.22.0
- C++14
- python3 python 3.8.10
- python2 python 2.7.18
- vs2015 or later
- git version 2.35.1.windows.2 or later
- nsis 3.09.1
  
## get source code

```bash
$ git clone git@github.com:hhool/anxi.git --recursive
$ cd anxi
$ git submodule update --init --recursive
```

## build

```bash
$ cd anxi
$ cmake . -B out -G "Visual Studio 14 2015" -A Win32 -DCMAKE_BUILD_TYPE="Release"
$ cmake --build out --config Release
```

register dmgraph activex control on windows,

only need to run once,

start cmd as administrator

```bash
$ regsvr32 out/bin/Release/dmgraph.dll
```

## run

```bash
$ ./out/bin/Release/anxi
```

## package

```bash
$ cd anxi
$ cmake . -B out\package -G "Visual Studio 14 2015" -A Win32 -DCMAKE_BUILD_TYPE="Release"
$ cmake --build out\package --config Release --target package
```

open explorer and go to `anxi\out\package` folder, you will see `anxi-x.x.x-win32.exe` file.

## install

 [how to install](package/README.install.md)

 [如何安装](package/README.install.zh.md)