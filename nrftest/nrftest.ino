#include <RF24.h>
#include <SPI.h>
#include <nRF24L01.h>
#include <printf.h>

// Set up nRF24L01 radio on SPI bus plus pins 7 & 8

RF24 radio(9, 10);

//
// Channel info
//

const uint8_t num_channels = 126;
uint8_t values[num_channels];

//
// Setup
//

void setup(void) {
  //
  // Print preamble
  //

  Serial.begin(115200);
  while (!Serial) {
    ;  // wait for serial port to connect. Needed for native USB port only
  }

  printf_begin();
  Serial.println(F("\n\rRF24/examples/scanner/"));

  //
  // Setup and configure rf radio
  //

  radio.begin();
  radio.setAutoAck(false);

  // Get into standby mode
  radio.startListening();
  radio.stopListening();
  radio.printDetails();

  //delay(1000);
  // Print out header, high then low digit
  int i = 0;
  while (i < num_channels) {
    Serial.print(i >> 4, HEX);
    ++i;
  }
  Serial.println();
  i = 0;
  while (i < num_channels) {
    Serial.print(i & 0xf, HEX);
    ++i;
  }
  Serial.println();
  //delay(1000);
}

//
// Loop
//

const int num_reps = 100;
bool constCarrierMode = 0;

void loop(void) {
  // Send g over Serial to begin CCW output
  // Configure the channel and power level below

  if (Serial.available()) {
    char c = Serial.read();
    if (c == 'g') {
      constCarrierMode = 1;
      radio.stopListening();
      delay(2);
      Serial.println("Starting Carrier Out");
      radio.startConstCarrier(RF24_PA_LOW, 40);
    } else if (c == 'e') {
      constCarrierMode = 0;
      radio.stopConstCarrier();
      Serial.println("Stopping Carrier Out");
    }
  }

  if (constCarrierMode == 0) {
    // Clear measurement values
    memset(values, 0, sizeof(values));

    // Scan all channels num_reps times
    int rep_counter = num_reps;
    while (rep_counter--) {
      int i = num_channels;
      while (i--) {
        // Select this channel
        radio.setChannel(i);

        // Listen for a little
        radio.startListening();
        delayMicroseconds(128);
        radio.stopListening();

        // Did we get a carrier?
        if (radio.testCarrier()) {
          ++values[i];
        }
      }
    }


    // Print out channel measurements, clamped to a single hex digit
    int i = 0;
    while (i < num_channels) {
      if (values[i])
        Serial.print(min(0xf, values[i]), HEX);
      else
        Serial.print(F("-"));

      ++i;
    }
    Serial.println();

  }  //If constCarrierMode == 0
}
