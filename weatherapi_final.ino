#include <WiFi.h>
#include <ArduinoJson.h>
#include <HTTPClient.h>

// WiFi credentials
const char* ssid = "";
const char* password = "";

// OpenWeather API
String URL = "";      //Replace with your URL
String ApiKey = "";  // Replace with your API key

// Coordinates
String lat = ""; // Mention latitude
String lon = ""; // Mention longitude

void setup() {
  Serial.begin(115200);

  WiFi.begin(ssid, password);

  Serial.print("Connecting to WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("\nWiFi connected!");
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());
}

void loop() {
  if (WiFi.status() == WL_CONNECTED) {

    HTTPClient http;

    // Build API request
    String fullURL = URL + "lat=" + lat + "&lon=" + lon + "&units=metric&appid=" + ApiKey;
    http.begin(fullURL.c_str());

    int httpCode = http.GET();

    Serial.print("\nHTTP Code: ");
    Serial.println(httpCode);

    if (httpCode > 0) {

      String JSON_Data = http.getString();

      // Parse JSON
      DynamicJsonDocument doc(2048);
      DeserializationError error = deserializeJson(doc, JSON_Data);

      if (error) {
        Serial.print("JSON Error: ");
        Serial.println(error.c_str());
        return;
      }

      // Extract data
      const char* city = doc["name"];
      const char* country = doc["sys"]["country"];
      const char* description = doc["weather"][0]["description"];
      float temp = doc["main"]["temp"];
      float humidity = doc["main"]["humidity"];

      // Print output
      Serial.println("\n------ WEATHER DATA ------");

      Serial.print("Location: ");
      Serial.print(city);
      Serial.print(", ");
      Serial.println(country);

      Serial.print("Condition: ");
      Serial.println(description);

      Serial.print("Temperature: ");
      Serial.print(temp);
      Serial.println(" °C");

      Serial.print("Humidity: ");
      Serial.print(humidity);
      Serial.println(" %");

      // Rain prediction
      String weatherStr = String(description);
      weatherStr.toLowerCase();

      if (weatherStr.indexOf("rain") >= 0 || weatherStr.indexOf("drizzle") >= 0) {
        Serial.println("Prediction: Rain likely");
      } else {
        Serial.println("Prediction: No rain");
      }

      Serial.println("--------------------------");

    } else {
      Serial.println("Error fetching data!");
    }

    http.end();
  } else {
    Serial.println("WiFi Disconnected!");
  }

  delay(30000); // Update every 30 seconds
}
