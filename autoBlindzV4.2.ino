#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>


#include <ESP8266mDNS.h>
#include <WiFiUdp.h>
#include <ArduinoOTA.h>
#include <EEPROM.h>
#include "index.h" //Our HTML webpage contents
#include "settings.h"
//const char* ssid = "skyFibre"; //your WiFi Name
//const char* password = "s1k9y2fbr";  //Your Wifi Password
int ledPin = LED_BUILTIN;
//WiFiServer server(80);
ESP8266WebServer server(80);    //Webserver Object

// Lets setup the motor
int in1 = 5;
int in2 = 4;

//Setup hall sensor, you may need to change these values
int encoderPin = A0; // hall sensor pin
int hallCenter = 525;
int encoderTreshold = 360;
int hallMeasurement;

boolean rpm_read_flag = true;
String round_reset_direction;
int blinderCurrentPosition = 0;
int blinderUpPosition = 0;
int blinderDownPosition = 11;
int start_rst;
int targetPosition;
String lastUpdateDirection = "down";

boolean newJob = false;
boolean directionTurned;
boolean middleTurnReached = false;
int nextStep;

void trackPosition() {
  calculateHallPosition();
  if (abs(hallMeasurement - hallCenter) < 30) {
    middleTurnReached = true;
  }
  if (middleTurnReached == true) {
    if (hallMeasurement > (hallCenter + encoderTreshold)) {
      middleTurnReached = false;
      if (lastUpdateDirection == "down") {
        blinderCurrentPosition = blinderCurrentPosition + 1;
      } else {
        blinderCurrentPosition = blinderCurrentPosition - 1;
      }
    }
  }
}


void calculateHallPosition() {

  // Hall sensor reading can be noisy, lets make an average reading.
  int total = 0;
  for (int i = 0; i < 10; i++) {
    total += analogRead(encoderPin);
  }
  hallMeasurement = total / 10;
}



void blinder_up() {
  digitalWrite(in1, LOW);
  digitalWrite(in2, HIGH);
  lastUpdateDirection = "up";
}

void blinder_down() {
  digitalWrite(in1, HIGH);
  digitalWrite(in2, LOW);
  lastUpdateDirection = "down";
}

void blinder_stop() {
  digitalWrite(in1, LOW);
  digitalWrite(in2, LOW);
  EEPROM.write(0, blinderCurrentPosition);
  EEPROM.commit();
}
void handleRoot() {
  String s = MAIN_page; //Read HTML contents
  server.send(200, "text/html", s); //Send web page
}

void handleGetCurrentPosition() {
  String valueString = "Current position: " + String(blinderCurrentPosition);
  server.send(200, "text/html", (String)blinderCurrentPosition); //Send ADC value only to client ajax request
}
void handleGetTargetPosition() {
  String valueString = "Current position: " + String(targetPosition);
  server.send(200, "text/html", (String)targetPosition); //Send ADC value only to client ajax request
}
void handleBlindsUp() {
  server.send(200, "text/html", "Blinds going Up"); //Send ADC value only to client ajax request
  targetPosition = 0;
  //Send ADC value only to client ajax request

}
void handleBlindsDown() {
  targetPosition = blinderDownPosition;
  String valueString = "Target set to " + String(targetPosition);
  server.send(200, "text/html", valueString); //Send ADC value only to client ajax request

}
void handleSetUpPos() {
  EEPROM.write(0, 0);
  EEPROM.commit();
  blinderCurrentPosition = EEPROM.read(0);
  targetPosition = blinderCurrentPosition;
  String valueString = "Current position set as UP: " + String(blinderCurrentPosition);
  server.send(200, "text/html", valueString); //Send ADC value only to client ajax request

}
void handleSetDownPos() {

  EEPROM.write(1, blinderCurrentPosition);
  EEPROM.commit();
  blinderDownPosition = EEPROM.read(1);
  targetPosition = blinderCurrentPosition;
  String valueString = "Current position set as DOWN: " + String(blinderDownPosition);
  server.send(200, "text/html", valueString); //Send ADC value only to client ajax request

}
void handleStepUp() {
  targetPosition = blinderCurrentPosition - 1;

}
void handleStepDown() {
  targetPosition = blinderCurrentPosition + 1;

}

void handleSetTargetPosition() {
  targetPosition = server.arg("pos").toInt();
  String valueString = "Target set to " + String(targetPosition);
  server.send(200, "text/html", valueString ); //Send ADC value only to client ajax request
}

void handleStop() {
  targetPosition = blinderCurrentPosition;
  String valueString = "Stopped";
  server.send(200, "text/html", valueString ); //Send ADC value only to client ajax request
}





void setup() {
  Serial.begin(115200);
  delay(10);
  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, LOW);
  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  WiFi.config(ip, gateway, subnet);
  ArduinoOTA.setHostname(hostName);
  ArduinoOTA.onStart([]() {
    Serial.println("Start");
  });
  ArduinoOTA.onEnd([]() {
    Serial.println("\nEnd");
  });
  ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
    Serial.printf("Progress: %u%%\r", (progress / (total / 100)));
  });
  ArduinoOTA.onError([](ota_error_t error) {
    Serial.printf("Error[%u]: ", error);
    if (error == OTA_AUTH_ERROR) Serial.println("Auth Failed");
    else if (error == OTA_BEGIN_ERROR) Serial.println("Begin Failed");
    else if (error == OTA_CONNECT_ERROR) Serial.println("Connect Failed");
    else if (error == OTA_RECEIVE_ERROR) Serial.println("Receive Failed");
    else if (error == OTA_END_ERROR) Serial.println("End Failed");
  });
  ArduinoOTA.begin();

  Serial.println("");
  Serial.println("WiFi connected");

  // Handle http commands
  server.on("/", handleRoot);
  server.on("/blindsUp", handleBlindsUp);
  server.on("/blindsDown", handleBlindsDown);
  server.on("/getTargetPosition", handleGetTargetPosition);
  server.on("/getCurrentPosition", handleGetCurrentPosition);
  server.on("/setUp", handleSetUpPos);
  server.on("/setDown", handleSetDownPos);
  server.on("/stepUp", handleStepUp);
  server.on("/stepDown", handleStepDown);
  server.on("/setTargetPosition", handleSetTargetPosition);
  server.on("/stop", handleStop);

  server.begin();
  Serial.println("Server started");
  // Hostname defaults to esp8266-[ChipID]


  Serial.print("Use this URL to connect: ");
  Serial.print("http://");
  Serial.print(WiFi.localIP());
  Serial.println("/");
  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);
  pinMode(encoderPin, INPUT);
  // Reset the current position as base
  EEPROM.begin(4);

  blinderCurrentPosition = EEPROM.read(0);
  blinderDownPosition = EEPROM.read(1);
  targetPosition = blinderCurrentPosition;
}



void loop() {

  unsigned long previousMillis = millis();
  ArduinoOTA.handle();
  //WiFiClient client = server.available();
  server.handleClient();
  if (targetPosition != blinderCurrentPosition) {
    processMovement();
  } else {
    blinder_stop();
  }
}

void processMovement() {
  trackPosition();
  if (blinderCurrentPosition > targetPosition) {
    blinder_up();
  } else if (blinderCurrentPosition < targetPosition) {
    blinder_down();
  }
  else {
    blinder_stop();
  }
}
