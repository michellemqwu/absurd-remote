/*
  Dweet.io GET client for ArduinoHttpClient library
  Connects to dweet.io once every ten seconds,
  sends a GET request and a request body. Uses SSL

  Shows how to use Strings to assemble path and parse content
  from response. dweet.io expects:
  https://dweet.io/get/latest/dweet/for/thingName

  For more on dweet.io, see https://dweet.io/play/

  created 15 Feb 2016
  updated 22 Jan 2019
  by Tom Igoe

  this example is in the public domain
*/
#include <ArduinoHttpClient.h>
#include <WiFiNINA.h>

#include "arduino_secrets.h"
///////please enter your sensitive data in the Secret tab/arduino_secrets.h
/////// WiFi Settings ///////
char ssid[] = SECRET_SSID;
char pass[] = SECRET_PASS;

const char serverAddress[] = "dweet.io";  // server address
int port = 80;
// String dweetName = "al7215mmw451absurd"; // use your own thing name here
const char path[] = "/dweet/quietly/for/al7215mmw451absurd";

WiFiClient wifi;
HttpClient client = HttpClient(wifi, serverAddress, port);
int status = WL_IDLE_STATUS;

bool pinched = false;

void setup() {

  Serial.begin(9600);
  pinMode(2, INPUT);   // set the yellow LED pin to be an output
  pinMode(3, INPUT);   // set the red LED pin to be an output

  connectWifi();
}

void post(int value) {
  // assemble the path for the POST message:
  String postPath = "/dweet/for/al7215mmw451absurd";
  String contentType = "application/json";

  String postData = "{\"sensorValue\":\"";
  postData += value;
  postData += "\"}";

  Serial.println("making POST request");

  // send the POST request
  client.post(postPath, contentType, postData);
  // read the status code and body of the response
  int statusCode = client.responseStatusCode();
  String response = client.responseBody();

  Serial.print("Status code: ");
  Serial.println(statusCode);
  Serial.print("Response: ");
  Serial.println(response);
}

void loop() {
  int sensorValue = analogRead(A0);
  // Serial.println(sensorValue);

  if (sensorValue > 200 && !pinched) {
    pinched = true;
    post(0);
  } else if (sensorValue <= 200 && pinched) {
    pinched = false;
    post(1);
  }
  delay(20);
}


void connectWifi() {
  // while (!Serial);
  while ( status != WL_CONNECTED) {
    Serial.print("Attempting to connect to Network named: ");
    Serial.println(ssid);     // print the network name (SSID);

    // Connect to WPA/WPA2 network:
    status = WiFi.begin(ssid, pass);
  }

  // print the SSID of the network you're attached to:
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());

  // print your WiFi shield's IP address:
  IPAddress ip = WiFi.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);
}