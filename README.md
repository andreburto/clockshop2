# FEATHERMAY'S CLOCKSHOP 2.0

### About

The first version of [Feathermay's Clockshop](http://littlebits.cc/projects/feathermay-s-clock-shop) was a 2014 project where I built and programmed a working clock. This second version is an attempt to make a better, internet-connected clock using a [Particle Photon](https://docs.particle.io/guide/getting-started/intro/photon/).

This clock uses two MCU boards: Particle Photon and [Arduino at Heart bit](https://littlebits.cc/bits/arduino). The former acts as an interface to the web to the latter through a simple serial connection. The latter controls the display, tracks changes in time, and turns the MP3 player on and off. [Click here](https://raw.githubusercontent.com/andreburto/clockshop2/master/src/insides.jpg) to see the electronic components.

### Component List

* 1X Particle Photon
* 1x Arduino bit
* 1x [MP3 Player bit](https://littlebits.cc/bits/mp3-player)
* 2x [Proto Module bits](https://littlebits.cc/bits/proto)
* 1x 5v breadboard power supply
* 1x breadboard
* 1x [Mounting Boards](https://littlebits.cc/accessories/mounting-board)
* 8x male breadboad wires
* Bricks. Lots and lots of building bricks.

### Current Features

* Keeps time very well.
* Time can be set from a web interface.
* Time synchronization can be turned off or on.
* Plays music through MP3 Player bit.

### To Do

Things that need to be completed or that I would like to add:

* Allow the setting of time when the music starts playing, ala an alarm.
* Automatically handle DST.
* Put a small web server on the Photon; get away from their cloud.

### License

The MIT License (MIT)

Copyright (c) 2015

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.