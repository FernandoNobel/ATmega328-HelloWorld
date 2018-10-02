#include "Turbidostat.h"

namespace TS {

Turbidostat::Turbidostat() {
	myMS = new MagneticStirrer();
}

Turbidostat::~Turbidostat() {

}

void Turbidostat::init() {
	myMS->init();
}

void Turbidostat::start() {
	myMS->start();
}

void Turbidostat::stop() {
	myMS->stop();
}


}   /* namespace TS */