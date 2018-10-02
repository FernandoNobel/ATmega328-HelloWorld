#include "TaskManager.h"
#include <Arduino.h> // Include only for Serial

/**
	@brief  Default constructor for TaskManager.

	@param  nothing
	@return nothing 
*/  
TaskManager::TaskManager() {
	this->time = new TimeStamp();

	for (int i = 0; i < MAXNUMTASK; i++) {
		this->tasks[i] = NULL;
	}
}

/**
	@brief  Default deconstructor for TaskManager.

	@param  nothing
	@return nothing 
*/  
TaskManager::~TaskManager() {

}

/**
	@brief  Initialization function.

	@param  nothing
	@return nothing 
*/  
void TaskManager::init() {
	// Start the timer
	this->timerInit();
}

/**
	@brief  Function called every system tick.

	@param  nothing
	@return nothing 
*/  
void TaskManager::sysTick() {
	// Keep track of the system ticks
	this->time->tick();

	if(time->getTicks() == 0) {
		Serial.print("T : ");
		Serial.print( time->getTicks() );
		Serial.print(" ");
		Serial.print( time->getSeconds() );
		Serial.print(" ");
		Serial.println( time->getMinutes() );
	}

	// Check if there are any tasks that need to be executed
	this->checkTasks();
}

/**
	@brief  Add a task to the task manager

	@param  t Task to add
	@return nothing 
*/  
uint8_t TaskManager::addTask(Task *t) {
	// Check if there is space for a new task
	if(this->numTask >= MAXNUMTASK) return 1;

	// Add the task
	this->tasks[numTask] = t;
	this->numTask++;

	return 0;
}

/**
	@brief  This function init the timer that will generate the system ticks.

	@param  nothing
	@return nothing 
*/  
void TaskManager::timerInit() {
	// Clock 8Hz
	TCNT2  = 0;
	TCCR2A = 0b00000010;		// Configure timer 2 for CTC mode.
	TCCR2B = 0b00000111;		// Start timer at Fcpu / 1024.
	TIMSK2 |= (1 << OCIE2A); 	// Enable CTC interrupt.
	OCR2A   = 251;          	// Set CTC compare value with a prescaler of 1024. 3.2256 ms time to interrupt.
								// If your overload 31 times you get 0.999936 seconds (1 second aprox.).
}

/**
	@brief  This function checks the timestamp of every task and execute them if needed

	@param  nothing
	@return nothing 
*/  
void TaskManager::checkTasks() {
	TimeStamp::Comp comp;

	for (int i = 0; i < this->numTask; i++) {
		
		if( this->tasks[i]->getState() == Task::State::RUN) {
			comp = this->tasks[i]->checkTime(time);

			if ( comp == TimeStamp::Comp::BIGGER ) {
				// Task is out-timed, actualize the time to execute and it will try to execute next time.
				this->tasks[i]->actualizeTime();
			} else if ( comp == TimeStamp::Comp::EQUAL) {
				// Task is on time, actualize the time to execute for the next period and execute it.
				this->tasks[i]->actualizeTime();
				this->tasks[i]->execute();
			}
		}

	}
}

// Associate the taskManager with the timer interrupts.
TaskManager taskManager;

ISR(TIMER2_COMPA_vect) {
	taskManager.sysTick();
}