#define BLYNK_TEMPLATE_ID "TMPL3xJ35hsCr"
#define BLYNK_TEMPLATE_NAME "Smart Irrigation System"
#define BLYNK_AUTH_TOKEN "23Zm-HmfSqlDAgbo8weikcOA4JHeRVx3"

// Include the library files
#include <LiquidCrystal_PCF8574.h>
#include <Wire.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>

#define sensor 33
#define relay 4


// Initialize the LCD display
LiquidCrystal_PCF8574 lcd(0x37);

BlynkTimer timer;

// Enter your Auth token
char auth[] = "23Zm-HmfSqlDAgbo8weikcOA4JHeRVx3";

// Enter your WIFI SSID and password
char ssid[] = "dint knw";
char pass[] = "varshini";

void setup() {
  // Debug console
  Serial.begin(115200);

  // Connect to Blynk cloud
  Blynk.begin(auth, ssid, pass, "blynk.cloud", 80);

  // Initialize LCD
  lcd.begin(16, 2);  // Set LCD to 16x2 mode
  lcd.setBacklight(HIGH);  // Enable backlight
  
  // Display system loading message
  lcd.setCursor(1, 0);
  lcd.print("System Loading");
  for (int a = 0; a <= 15; a++) {
    lcd.setCursor(a, 1);
    lcd.print(".");
    delay(200);
  }
  lcd.clear();

  // Set timer to check soil moisture every 2 seconds
  timer.setInterval(2000L, soilMoisture);
}

// Get soil moisture sensor values
void soilMoisture() {
  int value = analogRead(sensor);
  value = map(value, 0, 4095, 0, 100);
  value = (value - 100) * -1;

  // Send data to Blynk
  Blynk.virtualWrite(V0, value);

  // Print moisture level to Serial Monitor
  Serial.print("Soil Moisture: ");
  Serial.print(value);
  Serial.println("%");

  // Display moisture level on LCD
  lcd.setCursor(0, 0);
  lcd.print("Moisture: ");
  lcd.print(value);
  lcd.print("%   ");
}

// Blynk button to control motor
BLYNK_WRITE(V1) {
  bool Relay = param.asInt();
  digitalWrite(relay, Relay ? LOW : HIGH);

  // Update LCD display only if state changes
  lcd.setCursor(0, 1);
  lcd.print(Relay ? "Motor is ON " : "Motor is OFF");
}

void loop() {
  Blynk.run();  // Run the Blynk library
  timer.run();  // Run the timer
}
