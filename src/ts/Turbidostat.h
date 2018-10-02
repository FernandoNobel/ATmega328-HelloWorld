#ifndef TURBIDOSTAT_H_
#define TURBIDOSTAT_H_

#include <inttypes.h>
#include "../taskManager/Task.h"
#include "MagneticStirrer.h"

namespace TS {

class Turbidostat {
private:
	
	Task *myMS;

public:
	Turbidostat();
	~Turbidostat();
	void init();
	void start();
	void stop();

private:

};

}	/* namespace TS */

#endif /* TURBIDOSTAT_H_ */