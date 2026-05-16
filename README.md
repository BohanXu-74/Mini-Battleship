# MINI BATTLESHIP

So I wanted to make a tiny battleship style game on an STC15 microcontroller with one rotary encoder and a tiny OLED screen. The whole thing is written in C for SDCC and pretty much everything is done from scratch, including the OLED driver and the trig math. No floating point, no libraries, just lookup tables and integer math.

The player controls a moving ship and has to fire at an enemy ship moving across the screen. Turning the rotary encoder changes the firing angle, pressing changes ship speed, and long pressing opens the menu system. Wind also affects the projectile, so shots do not always land where you expect.

Every time you hit the enemy, it gets faster, so after a few rounds the timing gets kinda difficult.

## Hardware

* STC15W204S microcontroller
* 128x64 OLED display using I2C at address 0x3C
* Rotary encoder with push button
* Passive buzzer
* Breadboard wiring

## Pin Connections

| Pin  | Function              |
| ---- | --------------------- |
| P3.0 | Rotary Encoder CLK    |
| P3.1 | Rotary Encoder Button |
| P3.2 | Rotary Encoder DT     |
| P3.3 | Buzzer                |
| P5.4 | OLED SDA              |
| P5.5 | OLED SCL              |

## Features

* Enemy ship moves automatically and bounces at screen boundaries
* Player ship speed can be changed from negative to positive values
* Wind affects bullet trajectory
* Bullet trajectory uses integer trig lookup tables
* Bullet inherits player ship velocity when fired
* Hit detection system with increasing difficulty every level
* Multiple menu screens controlled with one rotary encoder
* OLED driver written completely from scratch
* No floating point math used anywhere

## Controls

Turning the rotary encoder changes the firing angle from -90 to 90 degrees.

Pressing the encoder changes ship speed.

Long pressing opens the orders menu with pause functionality.

## Menu States

| State | Screen               |
| ----- | -------------------- |
| 0     | Main game display    |
| 1     | Aim selection        |
| 2     | Ship speed selection |
| 3     | Orders menu          |
| 4     | Pause screen         |
| 5     | Miss screen          |
| 6     | Hit screen           |
| 7     | Out of bounds screen |

## How The Physics Works

The projectile system uses a sine lookup table stored in flash memory instead of floating point math. Angles are converted using precomputed values from 0 to 90 degrees.

The bullet's horizontal movement is affected by:

* firing angle
* bullet speed
* player ship velocity at the moment of firing
* wind

That means firing while moving changes where the shot lands instead of always shooting the same distance.

## Interrupts

Timer 0 interrupt updates:

* enemy movement
* player movement
* bullet position

External interrupt 4 handles rotary encoder input.

Movement updates happen every 2 timer ticks using a divider counter.

## OLED Driver

The OLED code was written completely from scratch using raw software I2C bit banging. No external display libraries are used. The font table is stored directly in flash memory.

## Repository Layout

```text
Mini-Battleship/
├── src/
│   ├── main.c
│   └── main.ihx
├── LICENSE
└── README.md
```

## Things I Want To Add

* A 3D printed case
* Better sound effects
* Smarter enemy movement
* Better score system

## Build Info

Written in C for the SDCC compiler.

Target MCU:

* STC15W204S

## Why I Made This

I mostly made this because I thought it would be fun to build a full artillery game with one knob and a tiny OLED. I also wanted to get better at embedded systems, interrupts, and writing drivers without relying on Arduino libraries for everything.

It ended up teaching me a lot about timers, lookup tables, and handling multiple game systems on a tiny microcontroller.
