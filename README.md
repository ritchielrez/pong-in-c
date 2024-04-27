# Pong in C

A simple pong game recreated using the **raylib** library and the **C** programming language.

## Installing dependencies
- Create a `vendor` folder inside the project folder
- Download **raylib** from https://github.com/raysan5/raylib/releases/tag/5.0
- Unzip the zip file
- Put all the contents all the headers, libraries inside a folder called `raylib-5.0`
- Copy `raylib-5.0` into the `vendor` folder

## Building from source
```
cmake -Bbuild -S.
cmake --build build
```

Then run the executable in the `build` folder.

Currently only **Windows** is supported as build platform. But this project can easily be ported to **Linux** and **Mac OS**. The project has been only
tested with **MinGW** on **Windows**.

## Controls
**W/A** keys to control the left paddle and **Left/Right** arrow keys to control the right one.

## Things to be completed
Even though the game is at a playable state, there are couple of things yet to be implemented:
- Enemy AI
- Help screen
- Settings screen
