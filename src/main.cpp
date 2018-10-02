#include <Arduino.h>

#include "extLbr/Wire.h"

#include "myLbr/Shell.h"

#include "taskManager/TaskManager.h"
#include "taskManager/Task.h"

#include "ts/FreqCounter.h"
#include "ts/Turbidostat.h"

// Declared weak in Arduino.h to allow user redefinitions.
int atexit(void (* /*func*/ )()) { return 0; }

// Weak empty variant initialization function.
// May be redefined by variant files.
void initVariant() __attribute__((weak));
void initVariant() { }

void setupUSB() __attribute__((weak));
void setupUSB() { }

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

//------------------------MY CODE-------------------------

Shell *mySh;
TS::Turbidostat *myTS;

// function that executes whenever data is received from master
// this function is registered as an event, see setup()
void receiveEvent(int howMany) {
  while (Wire.available()) {
    // get the new byte:
    char inChar = (char)Wire.read();
    if (mySh->read(inChar)) Serial.println("Read error!");
    mySh->check_msg();
  }
}

void requestEvent() {
  char *buffer;

  if (mySh->getIsProcessing()) {
    Wire.write("1 ERROR: Command is processing.");
    Wire.write('\0');
    return;
  }

  if (mySh->request(&buffer)) {
    Wire.write("2 ERROR: Empty buffer.");
    Wire.write('\0');
  } else {
    Wire.write(buffer);
    Wire.write('\0');
    mySh->reset_out_msg();
  }
}

void setup() {
  // initialize timer
  freqCounter.init();
  taskManager.init();

  Serial.begin(9600);

  cli();
  TIMSK0 &= ~(1 << TOIE0);
  sei();


  // Open serial communications and wait for port to open:
  pinMode(13, OUTPUT);
  Wire.begin(0x42);             // join i2c bus with address #8
  Wire.onReceive(receiveEvent); // register event
  Wire.onRequest(requestEvent);
  digitalWrite(13, HIGH);

  mySh = new Shell();
  mySh->init();

  myTS = new TS::Turbidostat();
  myTS->init();
  myTS->start();

}

void loop() { // run over and over

  mySh->interpret_msg();

}