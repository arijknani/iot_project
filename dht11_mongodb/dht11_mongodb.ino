#include <DHT.h>
#include <DHT_U.h>

#include <MongoDB.h>

#include <ESP8266WiFi.h>


#define DHTPIN D4// DHT11 data pin is connected to GPIO2 on ESP8266
#define DHTTYPE DHT11  // DHT11 sensor type

const char* ssid = "TOPNET_2588";
const char* password = "mmylbaf98f";
const char* uri = "your-mongodb-atlas-connection-string";
const char* database = "your-database-name";
const char* collection = "your-collection-name";

WiFiClient wifiClient;
MongoClient mongoClient(wifiClient, uri);
DHT dht(DHTPIN, DHTTYPE);

void setup() {
  Serial.begin(9600);

  // Connect to Wi-Fi network
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
  }

  // Initialize DHT11 sensor
  dht.begin();
}

void loop() {
  // Read temperature and humidity from DHT11 sensor
  float temperature = dht.readTemperature();
  float humidity = dht.readHumidity();

  // Check if any errors occurred while reading from the DHT11 sensor
  if (isnan(temperature) || isnan(humidity)) {
    Serial.println("Error reading from DHT11 sensor");
    return;
  }

  // Print the temperature and humidity values to the serial monitor
  Serial.print("Temperature: ");
  Serial.print(temperature);
  Serial.print(" °C, Humidity: ");
  Serial.print(humidity);
  Serial.println("%");

  // Insert data into MongoDB Atlas
  mongo::BSONObjBuilder builder;
  builder.append("temperature", temperature);
  builder.append("humidity", humidity);
  mongo::BSONObj bson = builder.obj();
  mongoClient.insert(database, collection, bson);

  delay(2000);
}