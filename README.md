# TeleSoarer
## A Lost Model Alarm &amp; Variometer for RC Slope Soarer / Sailplanes

This here is a lost model alarm, recently upgraded to include analogue vario output, for use with Fr-Sky telemetry and Taranis radio. I use it for slope soaring. The vario in this case is really just for fun. I imagine it to be more useful in a thermalling a sailplane ... and just as fun.

I already had the lost model alarm portion from my [lostmodel project](https://github.com/gruvin/lostmodel), here on GitHub. 
So, I took that, upgraded the ATmega88P an ATmega328P to get more Flash and changed the firmware from GCC/Makefile to Arduino IDE, just to make it a little easier for others, really. (An Atmel programmer is still required, since I didn't include the extra few components to allow the Arduino IDE uploader thingy to work. Space savings, you know! ;-)

The alarm itself is more than first meets the eye. The MCU outputs a 2.8KHz square wave at around 15% duty cycle. This is fed to a mechancially (cavity) resonated quartz wafer thingy to produce a LOUD noise. Useful on those windy days, from hiding places in the long grass! (The frequency and duty cycle were found experiementally, for maximum audio output.)

It's all been tested on a breadboard. As of this writing, the PCB has not yet been produced. 

Here's a KiCAD PCB rendering ... because they're just so damned cute ...

<img src="img/kicad_render.png">
<img src="img/kicad_size.png">

The board is about 37x16mm -- though the pizeo section can be chopped off and remote mounted, to give a board just 23x16mm.

In fact, the entire point of this project was to obtain a nice small footprint, while doing away with all the clunky wires needed for off-the-shelf Fr-Sky telemetry modules, for example. This thing is tiny and a welcomed space saver.
