# MINI BATTLESHIP

So I wanted to make a tiny battleship style game on an STC15 microcontroller with one rotary encoder and a tiny OLED screen. The whole thing is written in C for SDCC and pretty much everything is done from scratch, including the OLED driver and the trig math. No floating point, no libraries, just lookup tables and suffering 💀

The player controls a ship that moves horizontally while an enemy ship moves back and forth across the screen. You choose an angle with the rotary encoder, fire, and try to hit the enemy while both ships are moving. Every level the enemy gets faster so eventually it turns into complete chaos.

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
* Bullet trajectory uses integer trig lookup tables
* Bullet inherits player ship velocity when fired
* Hit detection system with increasing difficulty every level
* Multiple menu screens controlled entirely with one rotary encoder
* OLED driver written from scratch with software I2C bit banging
* No floating point math used anywhere

## Controls

Short press from the main screen opens the aiming menu.

Turn the rotary encoder to select a firing angle from -90 to 90 degrees.

Press again to fire.

Long press opens the ship speed menu where movement speed can be changed from -10 to 10.

Really long press opens the orders menu with pause functionality.

Press on any result screen returns to the game.

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

That means firing while moving actually changes where the shot lands which makes the game feel way more interesting than just static artillery.

## Interrupts

Timer 0 interrupt updates:

* enemy movement
* player movement
* bullet position

External interrupt 4 handles rotary encoder input.

Movement updates happen every 2 timer ticks using a divider counter.

## OLED Driver

The OLED code was written completely from scratch using raw software I2C bit banging. No external display libraries are used. The font table is stored directly in flash memory using `__code`.

Honestly this part took way longer than the actual game because debugging tiny displays at like 1 AM is pain 😭

## Planned Features

Things I still want to add:

* Actual wind mechanics
* Configurable bullet speed
* Better sound effects
* Multiple enemy ships
* Terrain or obstacles
* Score system

## Build Info

Written in C for the SDCC compiler using:

* `stc15.h`
* `__interrupt`
* `__code`

Target MCU:

* STC15W204S

## Why I Made This

I mostly made this because I thought it would be funny to build a full artillery game with one knob and a tiny OLED. Also I wanted to get better at embedded graphics and interrupts without relying on Arduino libraries for everything.

Turns out writing your own display driver and integer trig system on an 8051 style microcontroller is kinda cursed but also really fun.
