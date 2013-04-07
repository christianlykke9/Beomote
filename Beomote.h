#ifndef _BEOMOTE_H
#define _BEOMOTE_H

#include "Commands.h"
#include <Arduino.h>
#include <avr/io.h>
#include <avr/interrupt.h>

// Common divisors for 3125 = 1, 5, 25, 125, 625, 3125
#define TICK 625

// Defining the Bang & Olufsen commands
#define BEO_ZERO    (3125 / TICK)
#define BEO_SAME    (6250 / TICK)
#define BEO_ONE     (9375 / TICK)
#define BEO_STOP    (12500 / TICK)
#define BEO_START   (15625 / TICK)

typedef struct {
    boolean link;
    beo_address address;
    beo_command command;
} BeoCommand;

class Beomote {
    public:
        void initialize(int pin);
        int receive(BeoCommand &cmd);

        // This method is only to be called by the interrupt service routine
        void timerISR();

    private:
        int irPin;

        int lastState;
        int lastBeoBit;

        boolean irReceived;

        boolean link;
        unsigned char address;
        unsigned char command;

        int timer;
        int index;

        void reset();
};
extern Beomote Beo;


#endif