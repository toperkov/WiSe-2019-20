// SimpleRx - the slave or the receiver

#include <Arduino.h>
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

#define CE_PIN 9
#define CSN_PIN 10

const byte address[][6] = {"1node", "2node", "3node", "4node", "5node", "6node"};

RF24 radio(CE_PIN, CSN_PIN);

struct SensorData
{
  float temp;
  float hum;
  uint16_t lightLevel;
};

SensorData dataReceived;

bool newData = false;

void getData();
void showData();

void setup()
{

  Serial.begin(115200);

  Serial.println(F("SimpleRx Starting"));
  radio.begin();
  radio.setDataRate(RF24_250KBPS);
  radio.setChannel(111);
  radio.setPALevel(RF24_PA_MAX);
  radio.openReadingPipe(1, address[0]);
  radio.openReadingPipe(2, address[1]);
  radio.openReadingPipe(3, address[2]);
  radio.openReadingPipe(4, address[3]);
  radio.openReadingPipe(5, address[4]);
  radio.openReadingPipe(6, address[5]);
  radio.startListening();
}

void loop()
{
  getData();
  showData();
}

void getData()
{
  uint8_t pipeNum;
  if (radio.available(&pipeNum))
  {
    Serial.print(F("Got data on pipe: "));
    Serial.println(pipeNum);
    radio.read(&dataReceived, sizeof(dataReceived));
    newData = true;
  }
}

void showData()
{
  if (newData == true)
  {
    Serial.println("Received data");
    Serial.print("Humidity: ");
    Serial.println(dataReceived.hum);
    Serial.print("Temperature: ");
    Serial.println(dataReceived.temp);
    Serial.print("Light: ");
    Serial.println(dataReceived.lightLevel);
    newData = false;
  }
}