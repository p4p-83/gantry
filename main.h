#pragma once

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

#include <pico/stdlib.h>
#include <pico/time.h>

#include <hardware/gpio.h>

/* TYPES */
typedef uint16_t stepCount_t;
typedef uint8_t stepDirection_t;
typedef uint8_t gpioNumber_t;
typedef int axisNumber_t;

// https://stackoverflow.com/questions/71270954/how-to-properly-use-typedef-for-structs-in-c
typedef struct timerUserData_t {
    axisNumber_t axis;
	uint8_t stepDirection;
} timerUserData_t;

typedef struct stepperPinout_t {
	gpioNumber_t stepPin, directionPin;
} stepperPinout_t;

typedef struct stepperCommand_t {
	stepCount_t numberOfSteps;
	stepDirection_t direction;
} stepperCommand_t;

/* CONVENIENCE */
#define LOW 0
#define HIGH 1