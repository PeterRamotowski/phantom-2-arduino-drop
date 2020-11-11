# DJI Phantom 2 / Naza-M V2 Arduino program for load drop mechanism

Based on Louis Roux's work published on [instructables](https://www.instructables.com/Phantom-2-Vision-Plus-Bait-Drop-Mechanism/)

Prerequisites:
 - Naza-M V2 flight controller
 - Arduino Nano 3.0 ATmega328 module
 - Tower Pro MG995 servo

## Set up

First, try with defaults.
If something is not quite right, adjust constants:

``` 
#define POS_SIGNAL 1300
#define NEG_SIGNAL 1700
```
