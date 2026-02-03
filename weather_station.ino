#include <WiFi.h>
#include <WebServer.h>
#include <DHT.h>

// Replace with your network credentials
const char* ssid = "Your_SSID";
const char* password = "Your_PASSWORD";

// DHT22 sensor settings
#define DHTPIN 4 // GPIO where the data pin is connected
#define DHTTYPE DHT22 // DHT 22 (AM2302)

// Initialize DHT sensor
DHT dht(DHTPIN, DHTTYPE);

// Create an instance of the server on port 80
WebServer server(80);

void setup() {
Serial.begin(115200);

// Initialize the DHT sensor
dht.begin();

// Connect to Wi-Fi network
WiFi.begin(ssid, password);
Serial.print("Connecting to Wi-Fi");
while (WiFi.status() != WL_CONNECTED) {
delay(1000);
Serial.print(".");
}

Serial.println("");
Serial.println("Connected to Wi-Fi");

Serial.print("IP address: ");
Serial.println(WiFi.localIP());

// Define routes
server.on("/", handleRoot);
server.onNotFound(handleNotFound);

// Start the server
server.begin();
Serial.println("HTTP server started");
}

void loop() {
// Handle client requests
server.handleClient();
}

void handleRoot() {
// Read sensor data
float temperature = dht.readTemperature();
float humidity = dht.readHumidity();

// Check if any reads failed and exit early (to try again)
if (isnan(temperature) || isnan(humidity)) {
Serial.println("Failed to read from DHT sensor!");
server.send(500, "text/plain", "Failed to read from DHT sensor!");
return;
}

// Prepare the HTML content
String html = "<html><head><title>ESP32 Weather Station</title></head><body>";
html += "<h1>ESP32 Weather Station</h1>";
html += "<p>Temperature: " + String(temperature) + " &deg;C</p>";
html += "<p>Humidity: " + String(humidity) + " %</p>";

html += "</body></html>";

// Send the HTML content
server.send(200, "text/html", html);
}

void handleNotFound() {
server.send(404, "text/plain", "404: Not Found");
}