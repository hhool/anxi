# anxi

## Description

This is solution for 2000C machine remote control.

## requirements

- CMake 3.22.0
- C++14
  
## get source code

```bash
$ git clone git@github.com:hhool/anxi.git --recursive
$ cd anxi
$ git submodule update --init --recursive
```

## build

```bash
$ cd anxi
$ cmake . -B out -G "Visual Studio 14 2015" -A Win32
$ cmake --build out --config Release
```

## run

```bash
$ ./out/bin/Release/anxi
```

## Screenshot
