# RTCwakeUpComputer
> A low-cost, hardware-based solution for turning a computer on and off at designated times, centered around the DS3231 RTC.

I repurposed an old computer as a Linux FTP server to automatically back up all sorts of online projects. Because backups happen once a week during the night, it makes no sense to let the computer run 24/7. 
How lovely would it be if the computer automatically turned on, made the backup, and then turned off again? That's the reason why I invented RTCwakeUpComputer: a fully hardware-based solution to turn your computer on and off at specific times. It can be built within ~2 hours and should cost no more than 15 euros in materials.

> **Note**
> Modern computer systems can turn themselves on and off with all sorts of fancy tricks, such as [Wake-On-Lan (WOL)](https://en.wikipedia.org/wiki/Wake-on-LAN). Those tricks are almost always easier to implement, since they are based on software. Older computers, such as mine, do not have these luxuries.

![The final result, built into a computer.](/Pictures/finalproduct.jpg)
*Figure 1: The final result, built into a computer.*

## Table of contents

  * [Features](#features)
  * [Working principle](#working-principle)
  * [Hardware design](#hardware-design)
  * [Software design](#software-design)
  * [Improvements and limitations](#improvements-and-limitations)
  * [License](#license)
  * [Acknowledgments](#acknowledgments)
  * [Footnotes](#footnotes)


## Features

RTCwakeUpComputer is no ordinary clock. It features:

* A compact design that fits in box of ~10x5x3cm and can be built into a computer case;
* Fully customizable timeslots to turn your computer on and off;
* The ability to run besides existing methods of turning your system on/off, such as the power button;
* The ability to detect if a scheduled turn on/off was executed correctly, and to retry if not;
* The ability to keep accurate track of time, even if USB power is lost.

## Working principle

This project is centred around the DS3231. The DS3231 is a Real-Time Clock (RTC) module, meaning that it can keep time accurately. But how do we actually turn on a computer using such a thing? 

If you have ever built (or taken apart) a desktop PC, you might be familiar with the `POWER SWITCH` jumper on the motherboard (also known as one of the 'front panel connectors'). If the two `POWER SWITCH` pins are shorted, the computer turns on (or off). 
So, I use an Arduino Nano to control a relay. The relay briefly shorts the `POWER SWITCH` pins to turn the system on or off. And that is functionally everything you need to make a hardware-based computer switch.

But wait, there's more. How do we validate that the computer is indeed turned on? We are running this thing in the middle of the night, so 'just' checking in on it is not an option. 
For that, we abuse another pair of jumpers on the motherboard: the `POWER LED` pins. The `POWER LED` pins are used to control the status LED (if present in your computer case). If the computer is turned on, there will be a voltage on the `POWER LED` pins. If the computer is turned off, there won't be. I use the Arduino to sense if a voltage is present, and hence I'm able to validate if a power on/off request was successful.

Lastly, a question that one might have is: "Why bother with all of this? Why not slam one of those time-controllable outlets between your computer and the power socket? That is cheaper and less of a hassle."
True, but that also means a 'hard' shutdown. Hard shutdowns can cause all sorts of problems, such as hard-drive write failures. Which is exactly what I do not want happening with a server that is used for backups. Hence, I prefer a 'soft' shutdown. **RTCwakeUpComputer results in the same kind of shutdown as a human operator would cause.**[^HARDSHUTDOWN]

[^HARDSHUTDOWN]: Another disadvantage of just cutting the power is that there is no guarantee that the computer will turn on if the power is restored, unless you specifically tell it to in your BIOS.

![Front panel connectors in a computer.](/Pictures/connectors.jpg)
*Figure 2: Front panel connectors in a computer.*


## Hardware design

These components are needed for the build:

* 1x Arduino Nano
* 1x DS3231 RTC on a development board with a backup battery
* 1x 5V relay
* 1x BS170 MOSFET, but any low-power N-channel MOSFET will do
* 1x diode, I used the 1N4007, but most regular diodes will work fine
* various wires
* various male headers
* soldering gear
* perfboard or any mounting solution of your choice
* USB cable to power and program the Arduino
* (optional) a box to put the circuit in. ~10x5x3cm will be okay.

I devised a schematic that can be seen in fig. 3. It goes beyond the scope of this repository to explain every detail about the circuit. However, I shall elaborate on some design choices below.

Firstly, a relay that is not 'logic level' cannot be driven from an Arduino. I opted for a BS170 MOSFET (as a switch) to drive the relay. Any low-power MOSFET is fine here, I just had a BS170 lying around. The diode is a flyback diode. I use it to protect the MOSFET from the back EMF generated by the coil in the relay.

I have a DS3231 board that can be accessed via I2C, so I used that protocol.

The last important thing to note about the circuit is that I designed it to work in tandem with the existing computer hardware. I included pin headers to connect the power button from the computer case in parallel to the output of the relay. This way, both the relay and the original power button can be used to turn the computer on and off.

![Circuit Schematic.](/Pictures/circuit.png)
*Figure 3: Circuit Schematic.*

I soldered the circuit on a piece of perfboard.

![Soldered realisation on perfboard.](/Pictures/frontback.jpg)
*Figure 4: Soldered realisation on perfboard.*

## Software design

The needed code can be found in `RTCwakeUpComputer.ino`. It uses one third-party library (by Adafruit), `RTClib.h`, that I have included as a ZIP file. I'm not a software engineer, so I keep my code simple and functional. 

Once every minute, the Arduino fetches the current time from the RTC:

```cpp
DateTime now = rtc.now();
```

I want the backup to run on Tuesdays at 03:00h. However, I'm now living in UTC+2, but because of daylight saving time, 03:00h might become 02:00h at some point. I have no clue if the library is able to deal with DST, nor do I know if the backup program is. Hence, it is best to build in some safety margin. Let's say that the computer should turn on between 01:00h and 05:00h.

```cpp
  if (now.dayOfTheWeek() == TUESDAY) {
    if (now.hour() > 0 && now.hour() < 5) { // computer should be turned on between 01:00h and 05:00h
      if (validateStatus(false)) { // if the computer is not turned on
        switchComputer(); // turn on
        delay(60000); // wait for one minute before trying again
      }
    }
      else if (now.hour() == 5) { // this is the dead zone: the computer can never be turned on between the 05:00h and 06:00h. I can accept this for my application.
        if (validateStatus(true)) { // if the computer is on
          switchComputer(); // turn off
          delay(180000); // wait for three minutes before trying again
        }
      }
    }
```

Wait. What are `switchComputer()` and `validateStatus()`? Those are custom-made functions. `switchComputer()` mimics human behaviour to switch the computer on, i.e. hold the power button and release after one second. `validateStatus()` uses the status LED of the computer to validate if the computer indeed turned on or off.

So, the only thing left is to deploy the system, and wait patiently.

![The final circuit deployed in a computer case.](/Pictures/hookedup.jpg)
*Figure 5: The final circuit deployed in a computer case.*

And... it works!

![It works.](/Pictures/off.jpg)
*Figure 6: Woohoo!*


## Improvements and limitations

There are two main improvements that can be made:

- [ ] Find out if the system is able to deal with DST, and/or implement a way to cope with it.[^DST]
- [ ] Shorten the death zone time. That is the time the computer cannot be turned on. See the part in the code that states `else if (now.hour() == 5)`. It exists because it is a lazy way to make sure that the computer turns off after 05:00h.

[^DST]: I cannot think of reasons why this would be of utmost importance. The system will run perfectly fine if you build in a (small) safety margin, of which an example can be seen in code in the software design section.

## License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.

## Acknowledgments

This project uses the [`RTClib`](https://github.com/adafruit/RTClib) library by Adafruit to use the DS3231 with an Arduino. [Adafruit's tutorial](https://learn.adafruit.com/ds1307-real-time-clock-breakout-board-kit/overview) proved useful in learning more about RTCs.

## Footnotes
<!---
gets rendered automatically
-->

Some footnotes (mostly nerdy additions):
