#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include <Servo.h>

Servo myServo;
//L298n
int ena = 5;

int in1 = 2;
int in2 = 4;
int in3 = 7;
int in4 = 8;

int enb = 6;

int throttleValue = 0;
//Servo myESC;
int angle = 90;
RF24 radio(9, 10);  // CE, CSN
const byte address[6] = "nrF01";
void setup() {
  Serial.begin(9600);
  myServo.attach(3);
  //myESC.attach(5, 900, 2000); // ESC D5 pinine bağlandı
  //myESC.writeMicroseconds(900); // ESC başlangıç (armlama) sinyali
  radio.begin();
  radio.openReadingPipe(0, address);
  radio.setPALevel(RF24_PA_LOW);
  radio.startListening();
}
void loop() {
  if (radio.available()) {
    char text[32] = "";  //print only 32 characters
    int xL, yL, xR, yR;
    radio.read(&text, sizeof(text));
    //Serial.println(text);
    sscanf(text, "L-X:%d Y:%d R-X:%d Y:%d", &xL, &yL, &xR, &yR);

    // Artık değişkenlerde rakamlar var
    //Serial.print("xL = "); Serial.print(xL);
    //Serial.print(" yL = "); Serial.print(yL);
    //Serial.print(" xR = "); Serial.print(xR);
    //Serial.print(" yR = "); Serial.println(yR);
    angle =190 -( 9 * (10 - xR));
    myServo.write(angle);
    
    throttleValue = 255*yL/10;
    throttleValue = (yL<0)? -throttleValue : throttleValue;
    analogWrite(ena,throttleValue);
    analogWrite(enb,throttleValue);

    if (yL < 0) {
      digitalWrite(in1, LOW);
      digitalWrite(in2, HIGH);
      digitalWrite(in3, LOW);
      digitalWrite(in4, HIGH);
    } else {
      digitalWrite(in1, HIGH);
      digitalWrite(in2, LOW);
      digitalWrite(in3, HIGH);
      digitalWrite(in4, LOW);
    }
    //int escSignal = map(yL, 0, 10, 1000, 2000); // yR değerini ESC sinyaline çevirir
    //myESC.writeMicroseconds(escSignal);
    //Serial.println(text);
  }
}