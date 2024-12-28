#include <M5Core2.h>
#include <WiFi.h>
#include <NTPClient.h>
#include <WiFiUdp.h>

// Wi-Fi credentials
const char* ssid = "Your_SSID";
const char* password = "Your_PASSWORD";

// NTP server settings
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org", 3600, 60000); // Adjust timezone offset (e.g., 3600 for GMT+1)

void setup() {
  M5.begin();
  M5.Lcd.clear(TFT_WHITE); // Set background color to white
  M5.Lcd.setTextColor(TFT_DARKGREY); // Set text color to dark gray
  
  // Connect to Wi-Fi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    M5.Lcd.print(".");
  }

  // Start NTP client
  timeClient.begin();
}

void loop() {
  timeClient.update();

  // Get current time
  String formattedTime = timeClient.getFormattedTime();

  // Get current date in DD. MM. YY format
  int rawTime = timeClient.getEpochTime();
  struct tm* timeInfo = gmtime((time_t*)&rawTime);
  char dateBuffer[20];
  strftime(dateBuffer, sizeof(dateBuffer), "%d. %m. %y", timeInfo); // Format: DD. MM. YY

  // Clear screen
  M5.Lcd.fillScreen(TFT_WHITE);

  // Display time (larger, centered at the top)
  M5.Lcd.setTextSize(6); // Bigger font for time
  int timeWidth = M5.Lcd.textWidth(formattedTime);
  int timeX = (M5.Lcd.width() - timeWidth) / 2; // Center horizontally
  M5.Lcd.setCursor(timeX, 20); // Position at the top
  M5.Lcd.print(formattedTime);

  // Display date (centered at the bottom)
  M5.Lcd.setTextSize(3); // Medium font for date
  int dateWidth = M5.Lcd.textWidth(dateBuffer);
  int dateX = (M5.Lcd.width() - dateWidth) / 2; // Center horizontally
  int dateY = M5.Lcd.height() - 40; // Position 40px above the bottom
  M5.Lcd.setCursor(dateX, dateY);
  M5.Lcd.print(dateBuffer);

  delay(1000); // Update every second
}
