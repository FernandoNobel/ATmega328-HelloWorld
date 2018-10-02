#include "FreqCounter.h"

#include <util/delay.h>
#include <avr/interrupt.h>

namespace TS {

FreqCounter::FreqCounter() {

}

FreqCounter::~FreqCounter() {

}

void FreqCounter::init() {
	timerInit();
}

uint32_t FreqCounter::read_Hz() {
	resetTimer();

	startTimer();
	_delay_us(1000000); // wait time and start counting ticks
	stopTimer();

	uint32_t freq = (TCNT1 + count * MAX_COUNT) * 1;

	return freq;
}

void FreqCounter::startReadHz() {
	resetTimer();

	startTimer();
}

uint32_t FreqCounter::stopReadHz() {
	stopTimer();

	uint32_t freq = (TCNT1 + count * MAX_COUNT) / 0.99898;

	return freq;
	
}


void FreqCounter::isrHandler() {
	count++;
}

void FreqCounter::timerInit() {
	TCCR1A = 0b00000000;

	stopTimer();

	// initialize counter
	resetTimer();
}

void FreqCounter::resetTimer() {
	// Reset the count in timer 1
	TCNT1 = 0;
	count = 0;
}

void FreqCounter::startTimer() {
	TIMSK1 = 0b00000001;	// Enable interrupt from overload of timer 1

  //TCCR1B = 0b00000110;	// External clock source on T1 pin. Clock on fallin edge
	TCCR1B = 0b00000101;	// Internal clock with 1024 prescaler.
	//TCCR1B = 0b00000100;	// Internal clock with 256 prescaler.
	//TCCR1B = 0b00000011;	// Internal clock with 64 prescaler.
	//TCCR1B = 0b00000010;	// Internal clock with 8 prescaler.
	//TCCR1B = 0b00000001;	// Internal clock with 1 prescaler.
}

void FreqCounter::stopTimer() {
	TCCR1B = 0b00000000;	// No clock source.
	TIMSK1 = 0b00000000;	// Disable interrupt from overload of timer 1
}

}   /* namespace TS */

TS::FreqCounter freqCounter;

ISR(TIMER1_OVF_vect) {
	freqCounter.isrHandler();
}
