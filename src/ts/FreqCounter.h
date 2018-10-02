// This library is made fot the ATmega328p
// Uses Timer1 to mesaure frecuency

#ifndef FREQCOUNTER_H_
#define FREQCOUNTER_H_

#include <inttypes.h>

namespace TS {

class FreqCounter {
private:
	uint32_t MAX_COUNT = 65535;
	volatile int8_t count;

public:
	FreqCounter();
	~FreqCounter();
	void 		init();
	uint32_t	read_Hz();
	void 		startReadHz();
	uint32_t 	stopReadHz();
	void 		isrHandler();
private:
	void 		timerInit();
	void 		resetTimer();
	void 		startTimer();
	void 		stopTimer();
};

}	/* namespace TS */

extern TS::FreqCounter freqCounter;

#endif /* FREQCOUNTER_H_ */