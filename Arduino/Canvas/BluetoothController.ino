#include <Arduino.h>
#include <SPI.h>
#include "Adafruit_BLE.h"
#include "Adafruit_BluefruitLE_UART.h"

//#include "BluetoothController.h"

#if SOFTWARE_SERIAL_AVAILABLE
#include <SoftwareSerial.h>
#endif

/*=========================================================================
    APPLICATION SETTINGS

      FACTORYRESET_ENABLE       Perform a factory reset when running this sketch
     
                                Enabling this will put your Bluefruit LE module
                              in a 'known good' state and clear any config
                              data set in previous sketches or projects, so
                                running this at least once is a good idea.
     
                                When deploying your project, however, you will
                              want to disable factory reset by setting this
                              value to 0.  If you are making changes to your
                                Bluefruit LE device via AT commands, and those
                              changes aren't persisting across resets, this
                              is the reason why.  Factory reset will erase
                              the non-volatile memory where config data is
                              stored, setting it back to factory default
                              values.
         
                                Some sketches that require you to bond to a
                              central device (HID mouse, keyboard, etc.)
                              won't work at all with this feature enabled
                              since the factory reset will clear all of the
                              bonding data stored on the chip, meaning the
                              central device won't be able to reconnect.
    MINIMUM_FIRMWARE_VERSION  Minimum firmware version to have some new features
    MODE_LED_BEHAVIOUR        LED activity, valid options are
                              "DISABLE" or "MODE" or "BLEUART" or
                              "HWUART"  or "SPI"  or "MANUAL"
    -----------------------------------------------------------------------*/
#define FACTORYRESET_ENABLE         1
#define MINIMUM_FIRMWARE_VERSION    "0.8.0"
#define MODE_LED_BEHAVIOUR          "MODE"
/*=========================================================================*/

// Create the bluefruit object, either software serial...uncomment these lines
SoftwareSerial bluefruitSS = SoftwareSerial(BLUEFRUIT_SWUART_TXD_PIN, BLUEFRUIT_SWUART_RXD_PIN);

Adafruit_BluefruitLE_UART ble(bluefruitSS, BLUEFRUIT_UART_MODE_PIN,
                              BLUEFRUIT_UART_CTS_PIN, BLUEFRUIT_UART_RTS_PIN);

// A small helper
void error(const __FlashStringHelper*err) {
  Serial.println(err);
  while (1);
}

/**************************************************************************/
/*!
    @brief  Sets up the HW an the BLE module (this function is called
            automatically on startup)
*/
/**************************************************************************/
void BluetoothController::setup(void)
{
  Serial.println(F("Adafruit Bluefruit Callbacks Example"));
  Serial.println(F("-------------------------------------"));

  /* Initialise the module */
  Serial.print(F("Initialising the Bluefruit LE module: "));

  if ( !ble.begin(VERBOSE_MODE) )
  {
    error(F("Couldn't find Bluefruit, make sure it's in CoMmanD mode & check wiring?"));
  }
  Serial.println( F("OK!") );

  ble.println("AT+GAPDEVNAME=Magic Drawing Canvas");
  ble.println("ATZ");

  if ( FACTORYRESET_ENABLE )
  {
    /* Perform a factory reset to make sure everything is in a known state */
    Serial.println(F("Performing a factory reset: "));
    if ( ! ble.factoryReset() ) {
      error(F("Couldn't factory reset"));
    }
  }

  if ( !ble.isVersionAtLeast(MINIMUM_FIRMWARE_VERSION) )
  {
    error( F("Callback requires at least 0.8.0") );
  }

  Serial.println( F("Adding Service 0x1234 with 2 chars 0x2345 & 0x6789") );
  ble.sendCommandCheckOK( F("AT+GATTADDSERVICE=uuid=0x1234") );
  ble.sendCommandWithIntReply( F("AT+GATTADDCHAR=UUID=0x2345,PROPERTIES=0x08,MIN_LEN=1,MAX_LEN=6,DATATYPE=string,DESCRIPTION=string,VALUE=abc"), &charid_string);
  ble.sendCommandWithIntReply( F("AT+GATTADDCHAR=UUID=0x6789,PROPERTIES=0x08,MIN_LEN=4,MAX_LEN=4,DATATYPE=INTEGER,DESCRIPTION=number,VALUE=0"), &charid_number);

  ble.reset();

  /* Disable command echo from Bluefruit */
  ble.echo(false);

  Serial.println("Requesting Bluefruit info:");
  /* Print Bluefruit information */
  ble.info();

  ble.println("AT+UARTFLOW=on");

  /* Set callbacks */
  ble.setConnectCallback(BLEConnected);
  ble.setDisconnectCallback(BLEDisconnected);
  ble.setBleUartRxCallback(BLEDataReceived);

  /* Only one BLE GATT function should be set, it is possible to set it
    multiple times for multiple Chars ID  */
  ble.setBleGattRxCallback(charid_string, BleGattRX);
  ble.setBleGattRxCallback(charid_number, BleGattRX);
}

/*
   Get Data from the iOS app.
*/
char* BluetoothController::readPacket(void) {
  char* toReturn = "OK";
  // Check for incoming characters from Bluefruit
  ble.println("AT+BLEUARTRX");
  ble.readline();
  if (strcmp(ble.buffer, "OK") == 0) {
    // no data
    return toReturn;
  }
  // Some data was found, its in the buffer
  toReturn = ble.buffer;
  ble.waitForOK();
  return toReturn;
}

/*
   Send data to the iOS app.
*/
void BluetoothController::writePacket(char* packet) {
  ble.print("AT+BLEUARTTX=");
  ble.println(packet);

  // check response stastus
  if (! ble.waitForOK() ) {
    Serial.println(F("Failed to send?"));
  }
}

void BluetoothController::updateBLE(int interval) {
  ble.update(interval);
}

//void loop(void)
//{
//  // Check for user input
//  char inputs[BUFSIZE+1];
//
//  if ( getUserInput(inputs, BUFSIZE) )
//  {
//    // Send characters to Bluefruit
//    Serial.print("[Send] ");
//    Serial.println(inputs);
//
//    ble.print("AT+BLEUARTTX=");
//    ble.println(inputs);
//
//    // check response stastus
//    if (! ble.waitForOK() ) {
//      Serial.println(F("Failed to send?"));
//    }
//  }
//
//  // Check for incoming characters from Bluefruit
//  ble.println("AT+BLEUARTRX");
//  ble.readline();
//  if (strcmp(ble.buffer, "OK") == 0) {
//    // no data
//    return;
//  }
//  // Some data was found, its in the buffer
//  Serial.print(F("[Recv] ")); Serial.println(ble.buffer);
//  ble.waitForOK();
//}
//
///**************************************************************************/
///*!
//    @brief  Checks for user input (via the Serial Monitor)
//*/
///**************************************************************************/
//bool getUserInput(char buffer[], uint8_t maxSize)
//{
//  // timeout in 100 milliseconds
//  TimeoutTimer timeout(100);
//
//  memset(buffer, 0, maxSize);
//  while( (!Serial.available()) && !timeout.expired() ) { delay(1); }
//
//  if ( timeout.expired() ) return false;
//
//  delay(2);
//  uint8_t count=0;
//  do
//  {
//    count += Serial.readBytes(buffer+count, maxSize);
//    delay(2);
//  } while( (count < maxSize) && (Serial.available()) );
//
//  return true;
//}
