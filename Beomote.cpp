#include "Beomote.h"

Beomote Beo;

void Beomote::initialize(int pin) {
	irPin = pin;
	// Setting the pinmode of the IR pin and resetting the listener
	pinMode(irPin, INPUT);
	reset();

	// Clearing Control Register A
	TCCR1A = 0;
	// Setting the phase and frequency correct pwm, and stopping the timer
	TCCR1B = _BV(WGM13);

	long cycles = (F_CPU / 2000000) * TICK;
	ICR1 = cycles;

	TCCR1B &= ~(_BV(CS10) | _BV(CS11) | _BV(CS12));

	// Setting the timer overflow interrupt enable bit
	TIMSK1 = _BV(TOIE1);

	// Resetting clock select register, and starts the clock with no prescale
	TCCR1B |= _BV(CS10);
}

void Beomote::reset() {
	index = -1;
	irReceived = 0;

	link = 0x00;
	address = 0x00;
	command = 0x00;
}

int Beomote::receive(BeoCommand &cmd) {

	if (irReceived) {
		cmd.link = link;
		cmd.address = (beo_address)address;
		cmd.command = (beo_command)command;

		reset();
		return 1;
	}

	return 0;
}

void Beomote::timerISR() {
	if (irReceived)
		return;

	timer++;

	int irData = digitalRead(irPin);

	if (irData == LOW && lastState == HIGH) {
		int beoCode = 0;
		int beoBit;

		if (abs(timer - BEO_ZERO) < 2) {
			beoCode = BEO_ZERO;
			beoBit = lastBeoBit = 0;
		}
		else if (abs(timer - BEO_SAME) < 2) {
			beoCode = BEO_SAME;
			beoBit = lastBeoBit;
		}
		else if (abs(timer - BEO_ONE) < 2) {
			beoCode = BEO_ONE;
			beoBit = lastBeoBit = 1;
		}
		else if (abs(timer - BEO_STOP) < 2) {
			beoCode = BEO_STOP;
		}
		else if (abs(timer - BEO_START) < 2) {
			beoCode = BEO_START;
		}
		else
			index = -1;

		if (index == -1) {
			if (beoCode == BEO_START)
				index = 0;
		}
		else if (index == 0) {
			link = beoBit;
			index++;
		}
		else if (index < 9) {
			address = address << 1;
			address |= beoBit;

			index++;
		}
		else if (index < 17) {
			command = command << 1;
			command |= beoBit;

			index++;
		}
		else if (index == 17) {
			if (beoCode == BEO_STOP)
				irReceived = 1;
			else
				index = -1;
		}

		timer = 0;
		lastState = LOW;
	}
	else if (irData == HIGH && lastState == LOW) {
		lastState = HIGH;
	}
}

ISR(TIMER1_OVF_vect) {
	Beo.timerISR();
}
