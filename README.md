MINI-BATTLESHIP — STC15 Artillery Game










A tiny real-time artillery game written completely from scratch for the STC15W204S microcontroller.
The game runs on a 128×64 I2C OLED display with a rotary encoder for input and a buzzer for sound.

Everything is hand-written:

OLED driver
software I2C implementation
font renderer
physics
menu system
interrupt handlers
integer-only trigonometry

No Arduino libraries, no floating point math, and no external graphics libraries are used.

Gameplay

The player controls a moving ship and must destroy an enemy ship moving across the screen.

To hit the target:

choose a firing angle using the rotary encoder
compensate for your ship’s movement
predict enemy motion
fire before the enemy changes direction

Each successful hit increases enemy speed, making later rounds harder.

The projectile system includes:

horizontal velocity
vertical velocity
gravity
inherited player ship momentum

which creates real artillery-style ballistic gameplay despite running on an 8051-based MCU.

Hardware
Component	Details
Microcontroller	STC15W204S
Display	128×64 OLED
OLED Interface	Software I2C
OLED Address	0x3C
Input	Rotary encoder with push button
Audio	Passive buzzer
Compiler	SDCC
Pinout
Pin	Function
P3.0	Rotary encoder CLK
P3.1	Rotary encoder push button
P3.2	Rotary encoder DT
P3.3	Buzzer
P5.4	OLED SDA
P5.5	OLED SCL
Features
Real-time projectile physics
Integer-only ballistic calculations
Custom sine lookup table
Gravity simulation
Interrupt-driven movement
Rotary encoder UI
Multi-screen menu system
Sound effects through buzzer
Enemy AI with boundary bouncing
Increasing difficulty progression
Fully custom SSD1306 OLED driver
Physics System

The game uses integer-only motion calculations.

Projectile motion is calculated using:

lookup-table sine values
velocity integration
fixed timestep updates

No floating point math is used anywhere in the project.

Projectile motion includes gravity:

yBulletMov = yBulletMov - 4;
xBulletPos = xBulletPos + xBulletMov;
yBulletPos = yBulletPos + yBulletMov;

Shots also inherit player ship velocity:

xBulletMov = shipMovX + projectileVelocity;

which means firing while moving changes the projectile trajectory realistically.

Trigonometry Implementation

To avoid expensive floating point operations on the 8051 architecture, the game uses precomputed lookup tables stored in flash memory:

unsigned char __code sinD[91];
unsigned char __code degrees[91];

The lookup table stores scaled sine values from:

0° to 90°

Projectile vectors are generated entirely through integer math.

Menu System

The game uses a simple state-machine architecture.

Menu ID	Screen
0	Main gameplay
1	Aim selection
2	Ship speed selection
3	Orders menu
4	Pause screen
5	Miss screen
6	Hit screen
7	Out-of-bounds screen
Controls
Action	Result
Rotate encoder	Change values
Short press	Open aiming menu
Press in aim menu	Fire
Long press	Ship speed menu
Very long press	Pause/orders menu
Press on result screen	Return to game
Display Information

The main screen shows:

enemy position
player position
enemy speed
player speed
projectile coordinates
wind value placeholder

Example:

WIND: 0
E: 10000  P: 0
ES: 3     PS: 2
BX: 1200  BY: 8400
Interrupt Architecture
Timer 0 Interrupt

Handles:

enemy movement
player movement
projectile physics
collision detection
External Interrupt 4

Handles:

rotary encoder input
OLED Driver

The SSD1306 OLED driver was written completely from scratch using bit-banged I2C.

Implemented features include:

command mode
page addressing
cursor positioning
font rendering
screen clearing
ASCII text drawing

No external display libraries are used.

Repository Structure
mini-battleship/
├── src/
│   ├── main.c
│   ├── oled.c
│   ├── oled.h
│   └── fonts.h
├── images/
│   ├── game.jpg
│   └── hardware.jpg
├── schematic/
│   └── wiring.png
└── README.md
Building

Compile using SDCC:

sdcc main.c

Flash the generated binary to the STC15W204S using an STC programmer.

Planned Features
Real wind system
Adjustable projectile power
Improved sound effects
Sprite graphics
Better collision system
Smarter enemy movement
Optimized partial OLED redraws
High score tracking
Challenges and Lessons Learned
Integer Physics on 8051 Hardware

The STC15 is fast for an 8051 derivative, but floating point math is still expensive.
The entire physics engine was therefore implemented using:

integer math
lookup tables
fixed timestep updates

This kept the game responsive while still allowing curved ballistic trajectories.

OLED Rendering Performance

Because the OLED uses software I2C, full-screen redraws are relatively expensive.
Large screen updates can cause visible slowdown, especially during gameplay.

Future versions will likely move to:

partial redraws
sprite-based rendering
dirty-region updates
Interrupt Timing

Rotary encoder handling and projectile motion both rely on interrupts.
Care had to be taken to avoid:

missed encoder transitions
unstable timing
ISR slowdowns

especially when using 16-bit arithmetic on an 8051 architecture.

Tools Used
Tool	Purpose
SDCC	C compiler
STC-ISP	Flashing utility
SSD1306 OLED	Display
Logic analyzer	Debugging encoder + I2C
STC15W204S	Main MCU
Future Direction

This project started as an experiment in:

low-level embedded graphics
interrupt-driven game design
integer-only physics

and evolved into a complete miniature artillery game engine running on an 8051-class microcontroller.

Future revisions may expand into:

terrain deformation
multiplayer over UART
AI targeting
larger map systems
more advanced physics
License

MIT. See LICENSE for details.
