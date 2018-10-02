/**
	@brief 	This class is used to store and manage time stamps. TimeStamp mesaure system ticks, seconds and minutes.
	@author	Fernando Nóbel Santos Navarro
	@email	fersann1@upv.es
*/
#ifndef TIMESTAMP_H_
#define TIMESTAMP_H_

#include <inttypes.h>

class TimeStamp {
public:
	// Enum used to compare time stamps.
	enum Comp {
		SMALLER,
		EQUAL,
		BIGGER
	};

private:
	volatile uint8_t 	ticks;	// System ticks. (31 systemTicks are 1 second).
	volatile uint8_t 	sec;	// Seconds.
	uint16_t 			min;	// Minutes.

public:
	TimeStamp();
	TimeStamp(uint8_t t, uint8_t s, uint16_t m);
	~TimeStamp();

	void 	 	set(uint8_t t, uint8_t s, uint16_t m);
	void 	 	add(TimeStamp *other);
	Comp 		compare(TimeStamp *other);
	void 	 	tick();
	uint8_t  	getTicks() 		{ return this->ticks; };
	uint8_t	 	getSeconds() 	{ return this->sec;   };
	uint16_t	getMinutes() 	{ return this->min;   };

};

#endif /* TIMESTAMP_H_ */