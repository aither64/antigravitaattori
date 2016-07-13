Antigravitaattori
=================

Multiplayer flying saucer racing game.

This is a forked version of the original game, featuring the following changes:

 - ported to libpng 1.6
 - ported to SDL 2.0
 - dropped autotools, simplified build system
 - support for game controllers (tested with wireless XBOX 360 controller)

## Requirements
 - SDL2
 - libpng
 - libalut
 - libglu
 - libopenal

### Ubuntu 16.04

	$ apt-get install make g++ libsdl2-dev libpng16-dev libalut-dev libglu-dev libopenal-dev

## Build and install

Build and install with make:

	$ make && make install

By default, the game is installed to `/usr/local`. This can be changed with
variable `PREFIX`, e.g:

	$ make PREFIX=/usr && make install PREFIX=/usr
