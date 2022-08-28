#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/wdt.h>

#include "usbdrv/usbdrv.h"
#define F_CPU 16000000
#include <util/delay.h>

static uchar replyBuffer[1]; 

static uchar keyPressed(void) {
  if (!(PINB & 1<<PB1)) {
    replyBuffer[0] = 1;
    return 1;
  }
  replyBuffer[0] = 0;
  return 0;
}

USB_PUBLIC uchar usbFunctionSetup(uchar data[8]) {
  usbMsgPtr = replyBuffer;
  if (keyPressed()) {
    PORTC |= 1;
    return sizeof(replyBuffer);
  }
  PORTC &= ~1;
  return 0;
}

int main() {
  PORTB = 0xff;  // activate all pull-ups for PIN B
  DDRB = 0;       // setup port B as input

  DDRC = 1;
  PORTC &= ~1;

  usbInit();
  usbDeviceConnect();

  sei();

  while(1) {
    usbPoll();
  }
  return 0;
}
