#include <Arduino.h>

// Declared weak in Arduino.h to allow user redefinitions.
int atexit(void (* /*func*/ )()) { return 0; }

// Weak empty variant initialization function.
// May be redefined by variant files.
void initVariant() __attribute__((weak));
void initVariant() { }

void setupUSB() __attribute__((weak));
void setupUSB() { }

void setup() {
  Serial.begin(9600); 
}

void loop() { // run over and over
	Serial.println("Hello World!");
	delay(2000);
}

int main(void)
{
	init();

	initVariant();

#if defined(USBCON)
	USBDevice.attach();
#endif
	
	setup();
	for (;;) {
		loop();
		if (serialEventRun) serialEventRun();
	}
        
	return 0;
}