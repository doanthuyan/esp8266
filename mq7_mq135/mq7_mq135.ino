#include <TroykaMQ.h>
#include <SoftwareSerial.h>
#define MQ135_Ro 160
#define MQ7_Ro 7.2

#define PIN_MQ7         A0
#define PIN_MQ7_HEATER  11

MQ7 mq7(PIN_MQ7, PIN_MQ7_HEATER);
#define PIN_MQ135  A1
MQ135 mq135(PIN_MQ135);

#define COM_RX 7
#define COM_TX 8
SoftwareSerial ArduinoUno(COM_RX,COM_TX);

void setup()
{
  Serial.begin(115200);
  ArduinoUno.begin(9600);
  mq7.cycleHeat();
  mq135.calibrate(MQ135_Ro);
  Serial.print("CO2 Ro = ");
  Serial.println(mq135.getRo());
}

void loop()
{
  float co,co2;
  String dataStr;
  if (!mq7.isCalibrated() && mq7.atHeatCycleEnd()) {
    mq7.calibrate(MQ7_Ro);
    Serial.print("CO Ro = ");
    Serial.println(mq7.getRo());
    mq7.cycleHeat();
  }
  if (mq7.isCalibrated() && mq7.atHeatCycleEnd()) {
    Serial.print("CO Ratio: ");
    Serial.print(mq7.readRatio());
    Serial.print(" CarbonMonoxide: ");
    co = mq7.readCarbonMonoxide();
    Serial.print(co);
    Serial.println(" ppm ");
    delay(100);
    mq7.cycleHeat();
  }
  if (!mq135.isCalibrated() && mq135.heatingCompleted()) {
    mq135.calibrate();
    Serial.print("Ro = ");
    Serial.println(mq135.getRo());
  }
  if (mq135.isCalibrated() && mq135.heatingCompleted()) {
    Serial.print("Ratio: ");
    Serial.print(mq135.readRatio());
    
    Serial.print("\tCO2: ");
    co2 = mq135.readCO2();
    Serial.print(co2);
    Serial.println(" ppm");
    delay(100);
  }
  dataStr = String("CO:") + String(co) + String("CO2:") + String(co2);
}
