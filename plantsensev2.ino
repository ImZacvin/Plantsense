/*Plant watering system with new blynk update
   https://srituhobby.com
*/
//Include the library files
#include <NewPing.h>
#define BLYNK_PRINT Serial
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <DHT.h>

//Initialize the LCD display

char auth[] = "UAJu7TAAuQHkn4oNQocS2d1cl3rMFzUP";//Enter your Auth token
char ssid[] = "sukses";//Enter your WIFI name
char pass[] = "suksesssss";//Enter your WIFI password

BlynkTimer timer;
bool Relay = 0;

//Define component pins
#define soilSensor A0
#define tempDHT11 D4
#define DHTTYPE DHT11
#define TRIGGER_PIN D2
#define ECHO_PIN D1
#define MAX_DISTANCE 20
#define waterPump D3

DHT dht(tempDHT11, DHTTYPE);

NewPing sonar(TRIGGER_PIN, ECHO_PIN, MAX_DISTANCE);

void setup() {
  Serial.begin(9600);
  pinMode(waterPump, OUTPUT);
  digitalWrite(waterPump, HIGH);
  dht.begin();

  Blynk.begin(auth, ssid, pass, "blynk.cloud", 80);

  //Call the function
  timer.setInterval(100L, soilMoistureSensor);
  timer.setInterval(100L, waterDepthSensor);
  timer.setInterval(100L, tempSensorDHT11);
}

//Get the button value
BLYNK_WRITE(V1) {
  Relay = param.asInt();

  if (Relay == 1) {
    digitalWrite(waterPump, LOW);

  } else {
    digitalWrite(waterPump, HIGH);

  }
}

//Get the soil moisture values
void soilMoistureSensor() {
  int value = analogRead(soilSensor);
  value = map(value, 0, 1024, 0, 100);
  value = (value - 100) * -1;

  Blynk.virtualWrite(V0, value);

  Serial.println(value);

}

void waterDepthSensor() {
  double jarak = sonar.ping_cm();

  Blynk.virtualWrite(V4, jarak);
  Serial.println(jarak);

}

void tempSensorDHT11() {
  double h = dht.readHumidity();
  double t = dht.readTemperature();

  Blynk.virtualWrite(V2, h);
  Blynk.virtualWrite(V3, t);

  Serial.println(h);
  Serial.println(t);


}

void loop() {
  Blynk.run();//Run the Blynk library
  timer.run();//Run the Blynk timer
}
