#include <SPI.h>
#include <Wire.h>
#include <EEPROM.h>
#include <Servo.h>
#include <dht.h>

#include <LinxArduinoMega2560.h>
#include <LinxSerialListener.h>
dht dht;

#define DHT11_PIN 2
// Create A Pointer To The LINX Device Object We Instantiate In Setup()
LinxArduinoMega2560* LinxDevice;

// Initialize LINX Device And Listener
int DHT(unsigned char numInputBytes, unsigned char* input, unsigned char* numResponseBytes, unsigned char* response);
int SetDelay(unsigned char numInputBytes, unsigned char* input, unsigned char* numResponseBytes, unsigned char* response);

unsigned int delayDuration;

void setup()
{
  // Instantiate The LINX Device
  LinxDevice = new LinxArduinoMega2560();
  LinxSerialConnection.Start(LinxDevice, 0);
  LinxSerialConnection.AttachCustomCommand(1, SetDelay); 
  // The LINX Listener Is Pre Instantiated, Call Start And Pass A Pointer To The LINX Device And The UART Channel To Listen On
  
  LinxSerialConnection.AttachCustomCommand(0, DHT); 
  
}

void loop()
{
  // Listen For New Packets From LabVIEW
  LinxSerialConnection.CheckForCommands();
  
}

int DHT(unsigned char numInputBytes, unsigned char* input, unsigned char* numResponseBytes, unsigned char* response)
{
  int chk = dht.read11(DHT11_PIN);
  response[0] = (unsigned char)dht.humidity;
  response[1] = (unsigned char)dht.temperature;
  *numResponseBytes = 2;

  delay(delayDuration);
  
  return 0;
}

int SetDelay(unsigned char numInputBytes, unsigned char* input, unsigned char* numResponseBytes, unsigned char* response)
{
  if(numInputBytes >= 2) {
    delayDuration = (unsigned int)input[0] | ((unsigned int)input[1] << 8);
  }
  *numResponseBytes = 0;
  
  return 0;
}
