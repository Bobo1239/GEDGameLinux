# GEDGameLinux

> Your scientists were so preoccupied with whether or not they could, they
> didn’t stop to think if they should.

This repository contains a bunch of (very hacky) patches and fixes to make
GEDGame and its dependencies compilable and runnable under Linux. (GEDGame
is a template for the "Realtime Computer Graphics" course at the Technical
University of Munich.) Also works under Windows 10 (without the patches
applied) for your tutors convenience (use Visual Studio's native CMake
integration).

Don't judge me for the atrocities you find here...

Tested on Arch Linux and Windows 10. Contributions and suggestions welcome!

## Requirements
- Wine(-staging) (also install `d3dcompiler_43` and `d3dcompiler_47` with winetricks)
- [DXVK](https://github.com/doitsujin/dxvk)

## Instructions
```
git clone https://github.com/Bobo1239/GEDGameLinux.git
cd GEDGameLinux
git submodule init
git submodule update
make apply-linux-patches
mkdir build
cd build
cmake ..
make
./GEDGame.exe
```

## Known Problems
- DXUT GUI text is broken because `sizeof(wchar_t)` is _implementation-defined_
  and of course MSVC choose 16 bits while gcc choose 32 bits (we're linking
  against the native libc while internally using 16 bits so there's a mismatch;
  we've tried both `-fno-short-wchar` (32 bit `wchar_t`) and  `-mno-cygwin` (use
  MSVCRT instead of native libc) but they result in (more) compilation problems)
- Texture-related stuff is still untested atm
- DirectXTex is not ported; Just compile `texconv.exe` under Windows and run it
  with `wine`
- ...

## TODO
- ...
