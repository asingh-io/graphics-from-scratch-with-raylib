#!/bin/sh 

# For fedora install
sudo dnf install alsa-lib-devel mesa-libGL-devel \
    libX11-devel libXrandr-devel libXi-devel \
    libXcursor-devel libXinerama-devel libatomic

git clone https://github.com/raysan5/raylib.git raylib
cd raylib/src

make PLATFORM=PLATFORM_DESKTOP # To make the static version.
make PLATFORM=PLATFORM_DESKTOP RAYLIB_LIBTYPE=SHARED # To make the dynamic shared version.

cd ../..
~                                                                                               │❯ git clone https://github.com/raysan5/raylib.git raylib
