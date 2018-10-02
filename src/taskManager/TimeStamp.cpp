#include "TimeStamp.h"

/**
	@brief  Default constructor for TimeStamp.

	@param  nothing
	@return nothing 
*/  
TimeStamp::TimeStamp() {
	// The time stamp is set to zero.
	this->set(0,0,0);
}

/**
	@brief  Custom constructor for TimeStamp.

	@param  t System ticks.
	@param  s Seconds.
	@param  m Minutes.
	@return nothing 
*/  
TimeStamp::TimeStamp(uint8_t t, uint8_t s, uint16_t m) {
	// The time stamp is set to passed values.
	this->set(t,s,m);
}

/**
	@brief  Default deconstructor for TimeStamp.

	@param  nothing 
	@return nothing 
*/  
TimeStamp::~TimeStamp() {

}

/**
	@brief  Set the time stamp to passed values.

	@param  t System ticks.
	@param  s Seconds.
	@param  m Minutes.
	@return nothing  
*/  
void TimeStamp::set(uint8_t t, uint8_t s, uint16_t m){
	this->ticks = t;
	this->sec = s;
	this->min = m;
}

/**
	@brief  Add the time values of other time stamp to this.

	@param  other The other time stamp.
	@return nothing 
*/  
void TimeStamp::add(TimeStamp *other) {
	this->ticks += other->ticks;

	if(this->ticks > 30) {
		this->ticks -= 31;
		this->sec ++;
	}

	this->sec += other->sec;
	if(this->sec > 59) {
		this->sec -= 60;
		this->min ++;
	}

	this->min += other->min;
}

/**
	@brief  Compares this time stamp with other.

	@param  other The other time stamp.
	@return The result of the comparation. 
*/  
TimeStamp::Comp TimeStamp::compare(TimeStamp *other) {
	// Check if they are equal.
	if (this->min == other->min && this->sec == other->sec && this->ticks == other->ticks ) {
		return EQUAL;
	}

	// Check if other is bigger.
	if ((this->min < other->min) ||
	   ((this->min == other->min ) && (this->sec < other->sec)) ||
	   ((this->min == other->min ) && (this->sec < other->sec) && (this->ticks < other->ticks)) ) {
		return BIGGER;
	}

	// Other is smaller than this.
	return SMALLER;
}

/**
	@brief  Increment the system ticks in one.

	@param  nothing
	@return nothing
*/  
void TimeStamp::tick() {
	this->ticks ++;

	if(this->ticks > 30) {
		this->ticks = 0;
		this->sec ++;
	}

	if(this->sec > 59) {
		this->sec = 0;
		this->min ++;
	}
}
