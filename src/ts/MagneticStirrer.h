#ifndef MAGNETICSTIRRER_H_
#define MAGNETICSTIRRER_H_

#include "../taskManager/Task.h"
#include <inttypes.h>

namespace TS {

class MagneticStirrer: public Task {
private:

public:
	MagneticStirrer();
	~MagneticStirrer();
	void init();

private:
	void run();

};

}	/* namespace TS */


#endif /* MAGNETICSTIRRER_H_ */