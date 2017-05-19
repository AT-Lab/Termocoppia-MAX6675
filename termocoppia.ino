#include "max6675.h"

#define ktcSO   2
#define ktcCS   13
#define ktcCLK  3

MAX6675 ktc(ktcCLK, ktcCS, ktcSO);

#define a       9
#define b       11
#define c       12
#define d       7
#define e       8
#define f       6
#define g       10

#define mig   0
#define cen   1
#define dec   4
#define uni   5

byte seven_seg_digits[][7] = { { 1, 1, 1, 1, 1, 1, 0 }, // = 0
  { 0, 1, 1, 0, 0, 0, 0 }, // = 1
  { 1, 1, 0, 1, 1, 0, 1 }, // = 2
  { 1, 1, 1, 1, 0, 0, 1 }, // = 3
  { 0, 1, 1, 0, 0, 1, 1 }, // = 4
  { 1, 0, 1, 1, 0, 1, 1 }, // = 5
  { 1, 0, 1, 1, 1, 1, 1 }, // = 6
  { 1, 1, 1, 0, 0, 0, 0 }, // = 7
  { 1, 1, 1, 1, 1, 1, 1 }, // = 8
  { 1, 1, 1, 0, 0, 1, 1 }, // = 9
  { 1, 1, 1, 0, 1, 1, 1 }, // = A
  { 0, 0, 1, 1, 1, 1, 1 }, // = B
  { 1, 0, 0, 1, 1, 1, 0 }, // = C
  { 0, 1, 1, 1, 1, 0, 1 }, // = D
  { 1, 0, 0, 1, 1, 1, 1 }, // = E
  { 1, 0, 0, 0, 1, 1, 1 }, // = F
  { 0, 0, 0, 0, 0, 0, 0 }, // = NULL
  { 0, 0, 0, 0, 0, 0, 1 }  // = -
};

byte sevSeg[7] = {a, b, c, d, e, f, g};

byte digitPin[4] = {uni, dec, cen, mig};

int num = 0;

void setup() {
  pinMode(uni, OUTPUT);
  pinMode(dec, OUTPUT);
  pinMode(cen, OUTPUT);
  pinMode(mig, OUTPUT);
  for (int j = 0; j < (sizeof(sevSeg) / sizeof(sevSeg[0])); j++) pinMode(sevSeg[j], OUTPUT);
  delay(200);
  Serial.begin(9600);
  delay(500);
}

void loop() {
  Serial.print(num++);
  Serial.print(": \t");
  
  float temp = ktc.readCelsius();
  
  Serial.print("Deg C = ");
  Serial.print(temp);
  Serial.print("\n");
  
  uint8_t thousends = temp / 1000;
  temp = temp - thousends * 1000;

  uint8_t hundreds = temp / 100;
  temp = temp - hundreds * 100;

  uint8_t tens = temp / 10;
  uint8_t ones = temp - tens * 10;
  
  if (thousends == 0) {
    thousends = 16;
    if (hundreds == 0) {
      hundreds = 16;
      if (tens == 0) tens = 16;
    }
  }
  
  uint8_t temperature[4] = {ones, tens, hundreds, thousends};

  for (uint8_t y = 0; y < 50; y++) {
    for (byte digit = 0; digit < 4; digit++) {
      digitalWrite(digitPin[digit], LOW);
      sevenSegWrite(temperature[digit]);
      delay(2);
      digitalWrite(digitPin[digit], HIGH);
    }
  }
  
}

void sevenSegWrite(byte num) {
  for (byte segCount = 0; segCount < 7; ++segCount) {
    digitalWrite(sevSeg[segCount], seven_seg_digits[num][segCount]);
  }
}
