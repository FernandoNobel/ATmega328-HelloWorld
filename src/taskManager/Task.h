/**
	@brief 	This class is used to implement tasks that have to be executed periodicaly overtime.
	@author	Fernando Nóbel Santos Navarro
	@email	fersann1@upv.es
*/
#ifndef TASK_H_
#define TASK_H_

#include "TimeStamp.h"
#include <inttypes.h>

class Task {
public:
	// Possible task state
	enum State {
		STOP,	// Task is stopped
		RUN		// Rask is running
	};

private:
	friend class TaskManager;

	State	state;			// State of the task
	TimeStamp 	*time;		// TimeStamp when to execute this task.
	TimeStamp	*period;	// Period of repeting this task.

public:
	Task();
	~Task();

	virtual void init();
	virtual void run() = 0;
	
	void	start();
	void 	stop();	
	void 	setTime(uint8_t t, uint8_t s, uint16_t m);
	void 	setPeriod(uint8_t t, uint8_t s, uint16_t m);

private:
	void 			execute();
	TimeStamp::Comp	checkTime(TimeStamp *other);
	void 			actualizeTime(); 
	State			getState();
	
};

#endif /* TASK_H_ */

// Example of creating a task

/*

class HelloTask: public Task {
  int x ;

  void init() {
  	setTime(0,5,0); // Set period to 5 seconds
    x = 20;
  }

  void run() {
    Serial.print(x);
    Serial.println("Hello task!");
  }
  
};

HelloTask *ht = new HelloTask();
ht->init();
ht->start();

*/