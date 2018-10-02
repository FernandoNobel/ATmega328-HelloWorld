/**
	@brief 	This class is used to manage the Task objects and excute them in time.
			It uses the Timer2 of the ATmega328p to generate the system ticks.
	@author	Fernando Nóbel Santos Navarro
	@email	fersann1@upv.es
*/
#ifndef TASKMANAGER_H_
#define TASKMANAGER_H_

#include <inttypes.h>
#include "TimeStamp.h"
#include "Task.h"

#define MAXNUMTASK 10 // Maximun number of task that can be handled.

class TaskManager {
private:
	// System time. It will be used as a reference for excuting the tasks.
	TimeStamp *time;

	Task 	*tasks[MAXNUMTASK]; // Array that contains the tasks
	uint8_t numTask;			// Number of task in tasks

public:
	TaskManager();
	~TaskManager();
	
	void 	init();
	void 	sysTick();
	uint8_t addTask(Task *t);

private:
	void 	timerInit();
	void 	checkTasks();
	
};

// Defined taskManager to be used globally. Only one taskManager obj must exist in the code!
extern TaskManager taskManager;

#endif /* TASKMANAGER_H_ */