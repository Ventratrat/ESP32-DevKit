#include <WiFi.h>
#include <HTTPClient.h>

// Replace with your network credentials
const char* ssid = "Your SSID";
const char* password = "Your Password";

// Google search URL
const char* googleSearch = "http://www.google.com/search?q=";

void setup() {
  Serial.begin(115200);

  // Initialize WiFi
  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println();
  Serial.print("Connected to WiFi. IP Address: ");
  Serial.println(WiFi.localIP());

  // Perform a Google search
  googleWebSearch("Search Term");
}

void loop() {
  // Nothing to do here
}

void googleWebSearch(String name) {
  if(WiFi.status() == WL_CONNECTED) {
    WiFiClient wifiClient;
    HTTPClient http;
    name.replace(" ", "+");
    String searchURL = String(googleSearch) + name;
    http.begin(wifiClient, searchURL);
    int httpCode = http.GET();

    if(httpCode > 0) {
      Serial.printf("HTTP Request succeeded, code: %d\n", httpCode);
      String payload = http.getString();
      Serial.println(payload.substring(0, 500)); // Print the first 500 characters of the response
    } else {
      Serial.printf("HTTP Request failed, error: %d\n", httpCode);
    }
    http.end(); //Close connection
  } else {
    Serial.println("WiFi is not connected");
  }
}
