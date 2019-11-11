#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#define OLED_RESET 4

Adafruit_SSD1306 display(OLED_RESET);

byte second, minute, hour;

bool state = 1;

byte minuteTimer = 1;
byte hourTimer = 0;

byte decToBcd(byte val) {
  return ( (val / 10 * 16) + (val % 10) );
}

byte bcdToDec(byte val) {
  return ( (val / 16 * 10) + (val % 16) );
}

void setDateDs1307() {
  second = 0;
  minute = 0;
  hour = 0;
  Wire.beginTransmission(0x68);
  Wire.write(decToBcd(0));
  Wire.write(decToBcd(second));
  Wire.write(decToBcd(minute));
  Wire.write(decToBcd(hour));
  Wire.endTransmission();
}

String getDateDs1307() {
  String output = "";
  Wire.beginTransmission(0x68);
  Wire.write(decToBcd(0));
  Wire.endTransmission();
  Wire.requestFrom(0x68, 7);
  second = bcdToDec(Wire.read() & 0x7f);
  minute = bcdToDec(Wire.read());
  hour = bcdToDec(Wire.read() & 0x3f);
  output = String(hour, DEC) + String(":") + String(minute, DEC) + String(":") + String(second, DEC);
  return output;
}

long switch_previousMillis_0 = 0;
bool previousState_0 = 0;

bool IsPressed_0(int switch_input_pin) {
  bool switchPressed = false;
  unsigned long currentMillis = millis();
  unsigned int currentState = digitalRead(switch_input_pin);
  if (currentMillis - switch_previousMillis_0 > 80) {
    if (currentState == 1 && previousState_0 == 0) {
      switchPressed = true;
      previousState_0 = currentState;
    }
    if (currentMillis - switch_previousMillis_0 > 150) {
      previousState_0 = 0;
      switch_previousMillis_0 = currentMillis;
    }
  }
  return (switchPressed);
}

long switch_previousMillis_1 = 0;
bool previousState_1 = 0;

bool IsPressed_1(int switch_input_pin) {
  bool switchPressed = false;
  unsigned long currentMillis = millis();
  unsigned int currentState = digitalRead(switch_input_pin);
  if (currentMillis - switch_previousMillis_1 > 80) {
    if (currentState == 1 && previousState_1 == 0) {
      switchPressed = true;
      previousState_1 = currentState;
    }
    if (currentMillis - switch_previousMillis_1 > 150) {
      previousState_1 = 0;
      switch_previousMillis_1 = currentMillis;
    }
  }
  return (switchPressed);
}

long switch_previousMillis_2 = 0;
bool previousState_2 = 0;

bool IsPressed_2(int switch_input_pin) {
  bool switchPressed = false;
  unsigned long currentMillis = millis();
  unsigned int currentState = digitalRead(switch_input_pin);
  if (currentMillis - switch_previousMillis_2 > 50) {
    if (currentState == 1 && previousState_2 == 0) {
      switchPressed = true;
      previousState_2 = currentState;
    }
    if (currentMillis - switch_previousMillis_2 > 100) {
      previousState_2 = 0;
      switch_previousMillis_2 = currentMillis;
    }
  }
  return (switchPressed);
}

void setup() {
  pinMode(5, INPUT);
  pinMode(6, INPUT);
  pinMode(7, INPUT);
  pinMode(8, OUTPUT);
  Wire.begin();
  Serial.begin(19200);
  setDateDs1307();
  display.begin(SSD1306_SWITCHCAPVCC, 0x3c);
  display.clearDisplay();
  display.setTextSize(4);
  display.setTextColor(WHITE);
  display.setCursor(5, 0);
  display.println("PUNCH");
  display.display();
  delay(1000);
}
void loop() {
  if (IsPressed_0(5)) {
    setDateDs1307();
    hourTimer = 0;
    minuteTimer = 1;
    state = 1;
  }
  if (IsPressed_1(6)) {
    if (hourTimer >= 24) {
      hourTimer = 0;
    } else {
      hourTimer++;
    }
  }
  if (IsPressed_2(7)) {
    if (minuteTimer >= 59) {
      minuteTimer = 0;
    } else {
      minuteTimer++;
    }
  }
  if (hour == hourTimer && minute == minuteTimer) {
    state = 0;
  }
  digitalWrite(8, state);
  display.clearDisplay();
  display.setTextSize(2);
  display.setTextColor(WHITE);
  display.setCursor(16, 2);
  display.print("SET ");
  display.print(hourTimer);
  display.print(":");
  display.println(minuteTimer);
  display.setCursor(24, 18);
  display.println(getDateDs1307());
  display.display();
}
