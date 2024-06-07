#include "main.h"

/* COMPILE-TIME PREFERENCES */
const uint64_t timeSliceDuration_us = 1000;

const enum {
	AXIS_X,
	AXIS_Y,
	AXIS_Z,
	AXIS_A,
	NUM_AXES
} AXES;

const stepperPinout_t stepperPins[NUM_AXES] = {
	[AXIS_X] = {
		.stepPin = 1,
		.directionPin = 2,
	},
	[AXIS_Y] = {
		.stepPin = 3,
		.directionPin = 4,
	},
	[AXIS_Z] = {
		.stepPin = 5,
		.directionPin = 6,
	},
};

/* LOCAL VARS */
static repeating_timer_t timeSliceTimer, stepTimer[NUM_AXES];

static stepCount_t *stepsLastSlice, *stepsThisSlice, *stepsNextSlice;
static timerUserData_t userData[NUM_AXES];

volatile bool haltSteppers = true;

/* PRIVATE METHODS */

// main logic occuring each time a new time slice arrives
static bool timeSliceEvent(repeating_timer_t *culprit) {

	// lock
	haltSteppers = true;

	// observe the changing of time
	free(stepsLastSlice);
	stepsLastSlice = stepsThisSlice;
	stepsThisSlice = stepsNextSlice;
	stepsNextSlice = NULL;

	// TODO set up the next time slice values
		// set directions
		// adjust timers

	// TODO print the last step counts to USB
	
	// unlock
	haltSteppers = false;	// TODO should this stay true if there is no movement to make?
	
}

static bool makeStep(repeating_timer_t *culprit) {

	const bool STATUS_CONTINUE_TIMER = true;

	// don't move if we are told not to
	if (haltSteppers) return STATUS_CONTINUE_TIMER;

	// work out the required hardware
	const int axisToStep = ((timerUserData_t *)(culprit->user_data))->axis;
	const gpioNumber_t axisStepPin = stepperPins[axisToStep].stepPin;

	// take the step
	gpio_put(axisStepPin, HIGH);
	sleep_us(1);
	gpio_put(axisStepPin, LOW);

	// record the step
	++stepsThisSlice[axisToStep];

	return STATUS_CONTINUE_TIMER;
	
}

// TODO
static void setNextSlice() {

	if (stepsNextSlice == NULL) {
		// not yet allocated
		// calloc initialises all bits zero for us
		stepsNextSlice = calloc(NUM_AXES, sizeof(stepCount_t));
	}
	
	// TODO fill in stepsNextSlice

}

/* PUBLIC METHODS */

int main(void) {
	stdio_init_all();

	// set up time slice timer
	// NB negative means to time between *starts* of callbacks
	add_repeating_timer_us(-timeSliceDuration_us, timeSliceEvent, NULL, &timeSliceTimer);

	// set up step timers
	for (uint8_t i = 0; i < NUM_AXES; ++i) {
		timerUserData_t * const axisUserData = &userData[i];
		axisUserData->axis = i;
		add_repeating_timer_us(-timeSliceDuration_us*2, makeStep, axisUserData, &stepTimer[i]);
	}

	return 0;
}
