#include "MPU9250.h"

MPU9250 mpu;
const int ledPin = 13;
unsigned long startTime = 0;

void setup() {
    Serial.begin(115200);
    pinMode(ledPin, OUTPUT);
    Wire.begin();
    delay(2000);

    if (!mpu.setup(0x68)) {  // change to your own address
        while (1) {
            Serial.println("MPU connection failed. Please check your connection with `connection_check` example.");
            delay(5000);
        }
    }

    // calibrate anytime you want to
    Serial.println("Accel Gyro calibration will start in 5sec.");
    Serial.println("Please leave the device still on the flat plane.");
    mpu.verbose(true);
    delay(5000);
    mpu.calibrateAccelGyro();

    Serial.println("Mag calibration will start in 5sec.");
    Serial.println("Please Wave device in a figure eight until done.");
    delay(5000);
    mpu.calibrateMag();

    print_calibration();
    mpu.verbose(false);
    startTime = millis();
}

void loop() {
    if (mpu.update()) {
      /*if (millis() - startTime > 3000) {
      Serial.println("Data collected for 3 seconds. Stopped.");
      while (true) { // This will stop the loop from continuing
          digitalWrite(ledPin, LOW);    // set the LED off
        }
      }*/  
      
        static uint32_t prev_ms = millis();
        if (millis() > prev_ms + 5) {  // sampling rate = 200 Hz
            print_accel_gyro_mag_viz();
            //print_roll_pitch_yaw_formatted();
            prev_ms = millis();
        }
    }
    digitalWrite(ledPin, HIGH);   // set the LED on when sampling
}

void print_roll_pitch_yaw() {
    Serial.print("Yaw, Pitch, Roll: ");
    Serial.print(mpu.getYaw(), 2);
    Serial.print(", ");
    Serial.print(mpu.getPitch(), 2);
    Serial.print(", ");
    Serial.println(mpu.getRoll(), 2);
}

void print_calibration() {
    Serial.println("< calibration parameters >");
    Serial.println("accel bias [g]: ");
    Serial.print(mpu.getAccBiasX() * 1000.f / (float)MPU9250::CALIB_ACCEL_SENSITIVITY);
    Serial.print(", ");
    Serial.print(mpu.getAccBiasY() * 1000.f / (float)MPU9250::CALIB_ACCEL_SENSITIVITY);
    Serial.print(", ");
    Serial.print(mpu.getAccBiasZ() * 1000.f / (float)MPU9250::CALIB_ACCEL_SENSITIVITY);
    Serial.println();
    Serial.println("gyro bias [deg/s]: ");
    Serial.print(mpu.getGyroBiasX() / (float)MPU9250::CALIB_GYRO_SENSITIVITY);
    Serial.print(", ");
    Serial.print(mpu.getGyroBiasY() / (float)MPU9250::CALIB_GYRO_SENSITIVITY);
    Serial.print(", ");
    Serial.print(mpu.getGyroBiasZ() / (float)MPU9250::CALIB_GYRO_SENSITIVITY);
    Serial.println();
    Serial.println("mag bias [mG]: ");
    Serial.print(mpu.getMagBiasX());
    Serial.print(", ");
    Serial.print(mpu.getMagBiasY());
    Serial.print(", ");
    Serial.print(mpu.getMagBiasZ());
    Serial.println();
    Serial.println("mag scale []: ");
    Serial.print(mpu.getMagScaleX());
    Serial.print(", ");
    Serial.print(mpu.getMagScaleY());
    Serial.print(", ");
    Serial.print(mpu.getMagScaleZ());
    Serial.println();
}


// print to the format that is compatible with the viz code
// viz code url: https://github.com/thecountoftuscany/PyTeapot-Quaternion-Euler-cube-rotation/tree/master
void print_roll_pitch_yaw_formatted() {   
    Serial.print("y");
    Serial.print(mpu.getYaw(), 4);
    Serial.print("yp");
    Serial.print(mpu.getPitch(), 4);
    Serial.print("pr");
    Serial.print(mpu.getRoll(), 4);
    Serial.println("r");
}

// raw sensor reads output in format that is compatible with pyqtgraph viz code
// viz code see: https://github.com/Sanahm/MPU9250-Data-Visualization/tree/master; mpuScrollingPlot.py
void print_accel_gyro_mag_viz() {
  //accel
  Serial.print(mpu.getAccX()); Serial.print("\t");
  Serial.print(mpu.getAccY()); Serial.print("\t");
  Serial.print(mpu.getAccZ()); Serial.print("\t");
  //gyro
  Serial.print(mpu.getGyroX()); Serial.print("\t");
  Serial.print(mpu.getGyroY()); Serial.print("\t");
  Serial.print(mpu.getGyroZ()); Serial.print("\t");
  //mag
  Serial.print(mpu.getMagX()); Serial.print("\t");
  Serial.print(mpu.getMagY()); Serial.print("\t");
  Serial.print(mpu.getMagZ()); Serial.print("\n"); // new line in each sampling
}