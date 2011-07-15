Charge by Christian Käser
=========================

Charge is a small physics based game for two (later more) players in
which every object may have an electric charge. Positive charge is
symbolized by a red light, negative charge by a blue light. Your goal is
to push the other player off the platform by inverting your token's
charge at the right moment.

There are three kinds of objects:
* Player's token: large black object with claws and player color markers
* Movable obstacle: flat dish
* Fixed obstacle: flat disk with claws

It is highly recommended to play this at native resolution. Otherwise
it might be hard to see details such as the player colors. This will
change as soon as I have time to create better textures.

Controls
--------
* A - Invert red player's charge
* L - Invert blue player's charge

Features
--------
* Lots of lights via deferred shading
* Physics engine to calculate interaction between objects
* Simple to learn, hard to master
* Multiple players on one keyboard

Todo
----
* Map selector
* More lights
* More players
* More different objects
* Puzzle mode
* Nicer models and textures
* Music (Christian Masser/brainstar already promised me some)
* Lots of other stuff

Dependencies
------------
* OpenGL
* GLEW (included)
* Box2D
* Qt

Building
--------
Please use qmake for now. If the data folder doesn't get copied to your
output folder, please copy it manually or Charge might crash.
