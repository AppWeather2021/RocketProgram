#include <SD.h>
#include <SPI.h>
#include <Servo.h>

//#include <SoftwareSerial.h>
#include <VB_MPU9250.h>
#include <VB_BMP280.h>
#define SD_CS_PIN 4

Servo myservo;

VB_BMP280 barometer;
VB_MPU9250 imu;

bool imu_connection;
bool barometer_connection;
//SoftwareSerial HC12(0,1);   // Declare SoftwareSerial for HC12

void setup() {
    //HC12.begin(9600);
    Serial.begin(9600);
    setupSensors();
    myservo.attach(9);
    myservo.write(0); 
    if (!SD.begin(SD_CS_PIN)) {
      return;
    }
}

void loop() {
    if ((imu_connection && barometer_connection)) {
        imu.read();
        barometer.read();

        if(imu.ay > 0){
          myservo.write(90);
          delay(10);
          tone(A1 , 4000, 500);
        }/*
        if(imu.gx < -100){
          myservo.write(90); 
          tone(A1 , 4000, 500);
        }*/
        /*if (imu.ay > 0){
          Serial.println("YES " + String(imu.ay));
        }else{
          Serial.println("NO " + String(imu.ay));
        }*/
        File dataFile = SD.open("data_xyz.csv", FILE_WRITE);
        dataFile.print(imu.time);
        dataFile.print(";");
        dataFile.print(barometer.alti);
        dataFile.print(";");
        dataFile.println();
        dataFile.close();
        
        /*) + imu.ax + imu.ay + imu.az;*/
        /*HC12.print(imu.time);
        HC12.print("  ");*//*
        HC12.print(imu.ay);
        HC12.println();*/
    } else {
        setupSensors();
    };

    

}

void setupSensors() {
    imu_connection   = (imu.begin() == MPU_OK);
    barometer.start_altitude = 0;
    barometer_connection = barometer.begin();
}
