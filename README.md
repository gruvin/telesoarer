# TeleSoarer
## A Lost Model Alarm &amp; Variometer for RC Slope Soarer / Sailplanes

This here is a lost model alarm, recently upgraded to include analogue vario output, for use with Fr-Sky telemetry and Taranis radio. I (will) use it for slope soaring. That is, flying without an internal means of propulsion. So much fun!

The vario, in this, case is really just for fun. I imagine it to be far more useful in a thermalling a sailplane. But I'm doing it anyway.

My previous [lostmodel project](https://github.com/gruvin/lostmodel) is also here, on GitHub. That one uses am ATmega88P MCU.

This so-called, "telesoarer" version gets an upgrade a STM32F103CBT MCU, using ST's new `STM32CubeMX` software, to make short work of starting new projects -- without going all Arduino and stuff, at least.

The alarm itself is LOUD -- becasue that was the original purpose behind making my own. The MCU outputs a ~2.8KHz square wave at around 15% duty cycle. This is fed to an autotransformer, before arriving at a mechancial cavity resonated quartz wafer (piezo) thingy. All this produces a headsplittingly LOUD tone — indoors at least. Why? Well in practice, models get lost in long grass, on windy, noisy slopes. The pultry beepers available off the shelf are all but useless in these conditioins.

This new version also includes a 0.91" OLED display, just for kicks really. It gives the whole thing a more friendly user interface for changing settings, if nothing else. 

The complete system has been tested on a breadboard ...

<img/src="img/breadboard.png">

Amazing how big a prototype circuit can be, before getting onto its final PCB.

As of this writing, the PCB has been designed, but not yet ordered. Here's a KiCAD rendering of the PCB ... because they're just so darn cute ...

<img src="img/kicad_render.png">
<img src="img/kicad_size.png">

The pizeo section can be chopped off and remote mounted. An OLED display goes on the back.

The point of this project was to obtain a nice small footprint, while doing away with all the clunky wires needed for off-the-shelf Fr-Sky telemetry modules (for example). This thing is tiny and will be a welcomed space saver.
