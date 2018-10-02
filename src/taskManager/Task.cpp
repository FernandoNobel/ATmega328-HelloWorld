#include "Task.h"
#include "TaskManager.h"
#include <avr/interrupt.h>

/**
	@brief  Default constructor for Task.

	@param  nothing
	@return nothing 
*/  
Task::Task() {
	// Initially not running.
	this->state = STOP;

	this->time = new TimeStamp();
	this->period = new TimeStamp();
	this->setTime(0,0,0);
	this->setPeriod(0,5,0);	

	// Add this task to the taskManager to keep track of it.
	taskManager.addTask(this);
}

/**
	@brief  Default deconstructor for Task.

	@param  nothing 
	@return nothing 
*/  
Task::~Task() {

}

/**
	@brief  Intialize the task. This can be used to reset the task.

	@param  nothing 
	@return nothing 
*/  
void Task::init() {
	// Override this method with your code
}

/**
	@brief  This is the function that will be executed.

	@param  nothing 
	@return nothing 
*/  
void Task::run() {
	// Override this method with your code
}

/**
	@brief  Set to execute this task.

	@param  nothing 
	@return nothing 
*/  
void Task::start() {
	this->state = RUN;
}

/**
	@brief  Set to not execute this task.

	@param  nothing 
	@return nothing 
*/  
void Task::stop() {
	this->state = STOP;
}

/**
	@brief  Set the time when the task will be executed.
			This time is automatically updated with the period.

	@param  t systemTicks (31 systemTicks are 1 second) 
	@param	s seconds
	@param	m minutes
	@return nothing 
*/  
void Task::setTime(uint8_t t, uint8_t s, uint16_t m) {
	this->time->set(t,s,m);
}

/**
	@brief  Set the period time to repeat the task.

	@param  t systemTicks (31 systemTicks are 1 second) 
	@param	s seconds
	@param	m minutes
	@return nothing 
*/  
void Task::setPeriod(uint8_t t, uint8_t s, uint16_t m) {
	this->period->set(t,s,m);
}

/**
	@brief  This is the function that will called from the taskManager.

	@param  nothing 
	@return nothing 
*/  
void Task::execute() {
	// Enable interruptions to avoid system freeze when task cosume much time.
	sei();

	run();
}

/**
	@brief  Check if the task has to be executed.

	@param  other TimeStamp that will be compared to this->time. 
	@return Result of the comparation.
*/  
TimeStamp::Comp Task::checkTime(TimeStamp *other) {
	return time->compare(other);
}

/**
	@brief	Set the next time to execute the task using the period.

	@param  nothing
	@return nothing
*/  
void Task::actualizeTime() {
	time->add(period);
}

/**
	@brief	Get the state of the task. If it is running o no.

	@param  nothing 
	@return The state of the task.
*/ 
Task::State Task::getState() {
	return this->state;
}