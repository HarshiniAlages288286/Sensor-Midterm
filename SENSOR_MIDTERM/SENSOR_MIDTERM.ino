#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <DHT.h>

#define DHTPIN D5        // Pin where the DHT22 sensor is connected
#define DHTTYPE DHT22    // Type of the DHT sensor being used
#define LDRPIN A0        // Pin where the LDR sensor is connected

DHT dht(DHTPIN, DHTTYPE);  // Initialize the DHT sensor
WiFiClient client;         // Create a WiFiClient object

String URL = "http://192.168.100.13/dht22_project/test_data.php";  // URL of your PHP script

const char* ssid = "Hardip75";
const char* password = "Rita006296";

int temperature = 0;
int humidity = 0;
int ldrValue = 0;

void setup() {
  Serial.begin(115200);
  dht.begin();  // Start the DHT sensor
  connectWiFi();
}

void loop() {
  if (WiFi.status() != WL_CONNECTED) {
    connectWiFi();
  }

  // Read DHT22 sensor data
  Load_DHT22_Data();

  // Read LDR sensor data
  readLDRData();

  // Prepare POST data
  String postData = "temperature=" + String(temperature) + "&humidity=" + String(humidity) + "&ldrValue=" + String(ldrValue);

  // Initialize HTTPClient object
  HTTPClient http;
  http.begin(client, URL);  // Specify the URL and pass the WiFiClient object to begin method
  http.addHeader("Content-Type", "application/x-www-form-urlencoded");

  // Send POST request
  int httpCode = http.POST(postData);
  String payload = "";

  if (httpCode > 0) {
    if (httpCode == HTTP_CODE_OK) {
      payload = http.getString();
      Serial.println(payload);
    } else {
      Serial.printf("[HTTP] POST... code: %d\n", httpCode);
    }
  } else {
    Serial.printf("[HTTP] POST... failed, error: %s\n", http.errorToString(httpCode).c_str());
  }

  http.end();  // Close connection

  // Print debug information
  Serial.print("URL : ");
  Serial.println(URL);
  Serial.print("Data: ");
  Serial.println(postData);
  Serial.print("httpCode: ");
  Serial.println(httpCode);
  Serial.print("payload : ");
  Serial.println(payload);
  Serial.println("--------------------------------------------------");

  delay(5000); // Delay before next loop iteration
}

void Load_DHT22_Data() {
  // Read temperature and humidity from DHT22 sensor
  temperature = dht.readTemperature();
  humidity = dht.readHumidity();

  // Check if any read failed and set values to 0
  if (isnan(temperature) || isnan(humidity)) {
    Serial.println("Failed to read from DHT sensor!");
    temperature = 0;
    humidity = 0;
  }

  // Print temperature and humidity to Serial monitor
  Serial.printf("Temperature: %d °C\n", temperature);
  Serial.printf("Humidity: %d %%\n", humidity);
}

void readLDRData() {
  // Read analog value from LDR sensor
  ldrValue = analogRead(LDRPIN);

  // Print LDR value to Serial monitor
  Serial.print("LDR Value: ");
  Serial.println(ldrValue);
}

void connectWiFi() {
  // Connect to WiFi network
  WiFi.mode(WIFI_OFF);
  delay(1000);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  Serial.println("Connecting to WiFi");

  // Wait until connected
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
}

/*
TRUNCATE TABLE dht22;
ALTER TABLE dht22 AUTO_INCREMENT = 1;

http://localhost/dht22_project/test_data.php
*/
