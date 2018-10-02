#include "MagneticStirrer.h"
#include <Arduino.h>

namespace TS {

MagneticStirrer::MagneticStirrer() {

}

MagneticStirrer::~MagneticStirrer() {

}

void MagneticStirrer::init() {
	setPeriod(0,2,0);
}

void MagneticStirrer::run() {
	Serial.println("Run: MagneticStirrer!");	
}

}   /* namespace TS */
