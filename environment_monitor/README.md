# esp8266
Codes for ESP8266

How to connect ESP8266 V12 to PC via an Arduino Uno:

Arduino-(Reset)->Arduino-(GND): Change ArduinoUno mode to UART
Arduino-(RX)->Esp-(RX)
Arduino-(TX)->Esp -(TX)
Esp -(VCC,CH_PD)->Arduino power out 3.3V
Esp -(GND,GPI-15)->Arduino GND

Note:
-	Debuging:Esp-(GPIO0) -> GND
-	Run standalone: Esp-(GPIO0) remove out GND
- To re-upload code to ESP, un-plug then re-plug GPIO0
- GPIO4: 5, GPIO5: 4 (they are label incorrectly)
