#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <Servo.h>
#include <Wire.h>
#include "SSD1306Wire.h"
#include "WebHtml.h"
#include <Ticker.h>
#include <cmath>

Ticker gripperTimer;

SSD1306Wire display(0x3c, SDA, SCL);

#define ServoPort1 D0
#define ServoPort2 D3
#define ServoPort3 D4
#define ServoPort4 D5
#define ServoPort5 D6
#define ServoPort6 D7
#define ServoPort7 D8

#define J1_LENGTH 125
#define J2_LENGTH 155

#define ARM_BASE_HEIGHT 85
#define ARM_SHOULDER_LENGTH 125
#define ARM_ELBOW_LENGTH 125
#define ARM_WRIST_LENGTH 155
#define MIN_SAFE_HEIGHT 20

const char* ssid = "ZES ARM";
const char* password = "12345678x";

unsigned long gripStartTime = 0;
bool isGripping = false;

Servo myservo1, myservo2, myservo3, myservo4, myservo5, myservo6, myservo7;
ESP8266WebServer server(80);

const int homePositions[7] = { 90, 130, 130, 50, 90, 85, 85 };

int currentPositions[7] = { 90, 130, 130, 50, 90, 85, 85 };

struct JointAngles {
  float theta1;
  float theta2;
  float theta3;
  float delta;
};

struct Point3D {
  float x;
  float y;
  float z;
};

bool calculateInverseKinematics(float x, float y, float z, JointAngles& angles) {
  float delta = atan2(z, x);

  float radial_distance = sqrt(x * x + z * z);
  x = radial_distance;

  float numerator = x * x + y * y - J1_LENGTH * J1_LENGTH - J2_LENGTH * J2_LENGTH;
  float denominator = 2 * J1_LENGTH * J2_LENGTH;

  if (abs(numerator / denominator) > 1) {
    return false;
  }

  float cos_theta2 = numerator / denominator;
  float theta2 = acos(cos_theta2);

  float theta1_part1 = atan2(y, x);
  float theta1_part2 = atan2(
    J2_LENGTH * sin(theta2),
    J1_LENGTH + J2_LENGTH * cos(theta2));
  float theta1 = theta1_part1 - theta1_part2;

  angles.theta1 = 90 - degrees(theta1);
  angles.theta2 = 180 - degrees(theta2);
  angles.delta = 90 - degrees(delta);

  angles.theta3 = 270 - angles.theta2 - angles.theta1;
  angles.theta3 = fmod(angles.theta3, 360.0);
  if (angles.theta3 > 180.0) {
    angles.theta3 -= 360.0;
  }
  if (angles.theta3 < 0.0) {
    angles.theta3 += 360.0;
  }
  angles.theta3 = abs(180 - angles.theta3);

  if (angles.theta1 < 0 || angles.theta1 > 180 || angles.theta2 < 30 || angles.theta2 > 180) {
    return false;
  }

  return true;
}

void displayInfo(String line1, String line2 = "", String line3 = "") {
  float currentHeight = calculateHeight(
    currentPositions[1],  // shoulder
    currentPositions[3],  // elbow
    currentPositions[4]   // wrist
  );

  Point3D currentPos = calculateForwardKinematics(
    currentPositions[1],  // shoulder
    currentPositions[3],  // elbow
    currentPositions[4],  // wrist
    currentPositions[0]   // base
  );

  display.clear();
  display.setFont(ArialMT_Plain_10);
  
  display.drawString(0, 0, line1);
  
  String secondLine = (line2 != "") ? line2 : 
                      "Height: " + String(currentHeight, 1) + "mm";
  display.drawString(0, 16, secondLine);
  
  String thirdLine = (line3 != "") ? line3 : 
                     "X:" + String(currentPos.x, 1) + 
                     " Y:" + String(currentPos.y, 1) + 
                     " Z:" + String(currentPos.z, 1);
  display.drawString(0, 32, thirdLine);

  display.setFont(ArialMT_Plain_10);
  String ipText = "IP: " + WiFi.softAPIP().toString();
  display.drawString(0, 54, ipText);
  
  display.display();
}

void handleIK() {
  if (!server.hasArg("x") || !server.hasArg("y") || !server.hasArg("z")) {
    server.send(400, "text/plain", "Missing coordinates");
    return;
  }

  float x = server.arg("x").toFloat();
  float y = server.arg("y").toFloat() - 90;
  float z = server.arg("z").toFloat();

  JointAngles angles;
  if (!calculateInverseKinematics(x, y, z, angles)) {
    server.send(400, "text/plain", "Position unreachable");
    return;
  }

  int targetAngles[4] = {
    (int)angles.delta,
    (int)angles.theta1,
    (int)angles.theta2,
    (int)angles.theta3
  };

  int currentAngles[4] = {
    currentPositions[0],
    currentPositions[1],
    currentPositions[3],
    currentPositions[4]
  };

  Servo* servos[5] = { &myservo1, &myservo2, &myservo3, &myservo4, &myservo5 };

  while (true) {
    bool allAtTarget = true;

    for (int i = 0; i < 4; i++) {
      if (currentAngles[i] < targetAngles[i]) {
        currentAngles[i]++;
        allAtTarget = false;
      } else if (currentAngles[i] > targetAngles[i]) {
        currentAngles[i]--;
        allAtTarget = false;
      }

      if (i == 1) {  // Shoulder servos
        servos[1]->write(currentAngles[i]);
        servos[2]->write(currentAngles[i]);
      } else {
        servos[i < 2 ? i : i + 1]->write(currentAngles[i]);
      }
    }

    if (allAtTarget) break;
    delay(40);
  }

  currentPositions[0] = targetAngles[0];  // Base
  currentPositions[1] = targetAngles[1];  // Shoulder 1
  currentPositions[2] = targetAngles[1];  // Shoulder 2
  currentPositions[3] = targetAngles[2];  // Elbow
  currentPositions[4] = targetAngles[3];  // Wrist

  displayInfo("Movement Completed");

  server.send(200, "text/plain", "Movement completed");
}

void handleCalculateIK() {
  if (!server.hasArg("x") || !server.hasArg("y") || !server.hasArg("z")) {
    server.send(400, "text/plain", "Missing coordinates");
    return;
  }

  float x = server.arg("x").toFloat();
  float y = server.arg("y").toFloat() - 90;
  float z = server.arg("z").toFloat();

  JointAngles angles;
  if (!calculateInverseKinematics(x, y, z, angles)) {
    server.send(400, "text/plain", "Position unreachable");
    return;
  }

  String errorMsg;
  if (!validateMovement(angles.theta1, angles.theta2, angles.theta3, errorMsg)) {
    server.send(400, "text/plain", errorMsg);
    return;
  }

  String response = "{\"delta\":" + String(angles.delta) + ",\"theta1\":" + String(angles.theta1) + ",\"theta2\":" + String(angles.theta2) + ",\"theta3\":" + String(angles.theta3) + "}";

  server.send(200, "application/json", response);
}

Point3D calculateForwardKinematics(float theta1, float theta2, float theta3, float delta) {
  float theta1_rad = radians(theta1 - 90);
  float theta2_rad = radians(theta2 - 180);
  float delta_rad = radians(delta - 90);

  float r = J1_LENGTH * cos(theta1_rad) + J2_LENGTH * cos(theta1_rad + theta2_rad);

  Point3D pos;
  pos.x = r * cos(delta_rad);
  pos.y = 90 + J1_LENGTH * sin(theta1_rad) + J2_LENGTH * sin(theta1_rad + theta2_rad);
  pos.z = r * sin(delta_rad);
  pos.y = abs(pos.y)+90;
  return pos;
}


float calculateHeight(int shoulderAngle, int elbowAngle, int wristAngle) {
  float calcShoulder = 180 - shoulderAngle;
  float calcElbow = 180 - elbowAngle;
  float calcWrist = wristAngle - 90;

  float shoulderRad = calcShoulder * PI / 180.0;
  float elbowRad = calcElbow * PI / 180.0;
  float wristRad = calcWrist * PI / 180.0;

  float height = ARM_BASE_HEIGHT;
  height += ARM_SHOULDER_LENGTH * sin(shoulderRad);
  height += ARM_ELBOW_LENGTH * sin(shoulderRad + elbowRad);
  height += ARM_WRIST_LENGTH * sin(shoulderRad + elbowRad + wristRad);

  return height;
}
bool validateMovement(int shoulderAngle, int elbowAngle, int wristAngle, String& message) {
  float height = calculateHeight(shoulderAngle, elbowAngle, wristAngle);

  if (height < MIN_SAFE_HEIGHT) {
    message = "Height: " + String(height, 1) + "mm";
    return false;
  }
  return true;
}

void moveServoSlowly(Servo& servo1, Servo& servo2, int currentPos1, int targetPos1, String servoName, bool isSynchronized = false) {
  if (currentPos1 == targetPos1) return;

  int currentShoulder = currentPositions[1];
  int currentElbow = currentPositions[3];
  int currentWrist = currentPositions[4];

  if (servoName.indexOf("Shoulder") >= 0) currentShoulder = targetPos1;
  else if (servoName.indexOf("Elbow") >= 0) currentElbow = targetPos1;
  else if (servoName.indexOf("Wrist") >= 0) currentWrist = targetPos1;

  String errorMessage;
  if (!validateMovement(currentShoulder, currentElbow, currentWrist, errorMessage)) {
    displayInfo("Movement blocked",
                "Too close to ground",
                errorMessage);
    return;
  }

  displayInfo("Moving " + servoName,
              "From: " + String(currentPos1),
              "To: " + String(targetPos1));

  int step = (currentPos1 < targetPos1) ? 1 : -1;

  for (int pos = currentPos1; pos != targetPos1; pos += step) {
    servo1.write(pos);
    if (isSynchronized) {
      servo2.write(pos);
    }
    delay(60);
  }

  servo1.write(targetPos1);
  if (isSynchronized) {
    servo2.write(targetPos1);
  }

  if (servoName.indexOf("Shoulder") >= 0) currentPositions[1] = targetPos1;
  else if (servoName.indexOf("Elbow") >= 0) currentPositions[3] = targetPos1;
  else if (servoName.indexOf("Wrist") >= 0) currentPositions[4] = targetPos1;
  else if (servoName.indexOf("Base Servo") >= 0) currentPositions[0] = targetPos1;

  float finalHeight = calculateHeight(
    currentPositions[1],
    currentPositions[3],
    currentPositions[4]);

  displayInfo(servoName + " moved");
}

void stopGripping() {
  myservo7.write(85);
  isGripping = false;
  gripperTimer.detach();
}

void handleServo() {
  if (server.hasArg("servoPOS1")) {
    String POS1 = server.arg("servoPOS1");
    int pos1 = POS1.toInt();

    if (pos1 != currentPositions[0]) {
      moveServoSlowly(myservo1, myservo2, currentPositions[0], pos1, "Base Servo", false);
    }
  }

  if (server.hasArg("servoPOS2")) {
    String POS2 = server.arg("servoPOS2");
    int pos2 = POS2.toInt();

    if (pos2 != currentPositions[1]) {
      moveServoSlowly(myservo2, myservo3, currentPositions[1], pos2, "Shoulder Servos", true);
      currentPositions[2] = pos2;
    }
  }

  if (server.hasArg("servoPOS3")) {
    String POS3 = server.arg("servoPOS3");
    int pos3 = POS3.toInt();
    if (pos3 < 30) {
      pos3 = 30;
    }
    if (pos3 != currentPositions[3]) {
      moveServoSlowly(myservo4, myservo2, currentPositions[3], pos3, "Elbow Servo", false);
    }
  }

  if (server.hasArg("servoPOS4")) {
    String POS4 = server.arg("servoPOS4");
    int pos4 = POS4.toInt();

    if (pos4 != currentPositions[4]) {
      moveServoSlowly(myservo5, myservo2, currentPositions[4], pos4, "Wrist Servo", false);
    }
  }

  if (server.hasArg("servoPOS5")) {
    String POS5 = server.arg("servoPOS5");
    int pos5 = POS5.toInt();
    myservo6.write(pos5);
    delay(15);
    myservo6.write(85);
  }

  if (server.hasArg("servoPOS6")) {
    String command = server.arg("servoPOS6");

    if (command == "close") {
      myservo7.write(140);
      delay(50);
      myservo7.write(85);
      isGripping = false;
    } else if (command == "open") {
      myservo7.write(80);
      delay(150);
      myservo7.write(85);
      isGripping = false;
    } else if (command == "grip") {
      myservo7.write(180);
      isGripping = true;
      gripperTimer.attach(10, stopGripping);
    }
  }
  server.send(200, "text/plane", "");
}

void handleRoot() {
  server.send(200, "text/html", MAIN_page);
}

void displayNames() {
  display.clear();
  display.setFont(ArialMT_Plain_10);

  display.setTextAlignment(TEXT_ALIGN_LEFT);
  display.drawString(4, 15, "Fatemeh Zolghadri");

  display.setTextAlignment(TEXT_ALIGN_CENTER);
  display.drawString(64, 28, "Soroush Ebrahimi");

  display.setTextAlignment(TEXT_ALIGN_RIGHT);
  display.drawString(124, 41, "Seyed Ali Sajjadi");

  display.setTextAlignment(TEXT_ALIGN_LEFT);

  display.drawLine(2, 2, 10, 2);
  display.drawLine(2, 2, 2, 10);
  display.drawLine(118, 2, 126, 2);
  display.drawLine(126, 2, 126, 10);
  display.drawLine(2, 54, 2, 62);
  display.drawLine(2, 62, 10, 62);
  display.drawLine(126, 54, 126, 62);
  display.drawLine(118, 62, 126, 62);

  display.display();
  delay(3000);
}

void setup() {
  Serial.begin(115200);
  delay(500);

  /////////////////////////////////////////
  display.init();
  display.flipScreenVertically();
  display.setContrast(255);
  display.clear();
  display.setFont(ArialMT_Plain_10);

  displayNames();

  displayInfo("Initializing Servos",
              "moving to home position");
  delay(100);
  /////////////////////////////////////////


  /////////////////////////////////////////
  myservo2.attach(ServoPort2, 500, 2400);
  myservo3.attach(ServoPort3, 500, 2400);
  myservo2.write(homePositions[1]);
  myservo3.write(homePositions[2]);
  delay(1500);

  myservo4.attach(ServoPort4, 500, 2400);
  myservo4.write(homePositions[3]);
  delay(1500);

  myservo5.attach(ServoPort5, 500, 2400);
  myservo5.write(homePositions[4]);
  delay(1500);

  myservo1.attach(ServoPort1, 500, 2400);
  myservo1.write(homePositions[0]);
  delay(1500);

  myservo6.attach(ServoPort6);
  myservo6.write(homePositions[5]);

  myservo7.attach(ServoPort7);
  myservo7.write(homePositions[6]);
  /////////////////////////////////////////

  WiFi.softAP(ssid, password);
  IPAddress apIP = WiFi.softAPIP();

  displayInfo("WiFi Access Point:",
              "SSID: " + String(ssid),
              "Password: " + String(password));

  Serial.println("Access Point Started");
  Serial.print("SSID: ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(apIP);

  server.on("/", handleRoot);
  server.on("/setPOS", handleServo);
  server.on("/moveToXYZ", handleIK);
  server.on("/calculateIK", handleCalculateIK);
  server.begin();
  Serial.println("HTTP server started");
}

void loop() {
  server.handleClient();
}