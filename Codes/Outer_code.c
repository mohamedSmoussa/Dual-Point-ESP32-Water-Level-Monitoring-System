#include <esp_now.h>
#include <WiFi.h>
#include <HardwareSerial.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

#define LED_PIN 2
#define RX_PIN 22
#define TX_PIN 23
#define TRIG_PIN 5
#define ECHO_PIN 18

#define SDA_PIN 17
#define SCL_PIN 16

HardwareSerial sim800l(1);
LiquidCrystal_I2C lcd(0x27, 16, 2);  // غير 0x27 لو عنوان I2C مختلف

typedef struct Message {
    float distance;
} Message;

Message receivedData;

float innerDistance = 0;
float outerDistance = 0;

// فلاج لتجنب إرسال SMS مكرر
bool outerAlertSent = false;
bool innerAlertSent = false;

float measureLocalDistance() {
    digitalWrite(TRIG_PIN, LOW);
    delayMicroseconds(2);
    digitalWrite(TRIG_PIN, HIGH);
    delayMicroseconds(10);
    digitalWrite(TRIG_PIN, LOW);

    long duration = pulseIn(ECHO_PIN, HIGH);
    float distance = duration * 0.034 / 2;
    return distance;
}

void sendSMS(float distance, const char* source) {
    Serial.println("Sending SMS...");
    sim800l.println("AT+CMGF=1");
    delay(1000);
    sim800l.println("AT+CMGS=\"+201012887746\"");
    delay(1000);
    sim800l.print("Alert! ");
    sim800l.print(source);
    sim800l.print(" Distance: ");
    sim800l.print(distance);
    sim800l.println(" cm detected.");
    delay(1000);
    sim800l.write(26);  // Ctrl+Z
    delay(3000);
    Serial.println("SMS Sent!");
}

void onDataRecv(const esp_now_recv_info_t *info, const uint8_t *incomingData, int len) {
    memcpy(&receivedData, incomingData, sizeof(receivedData));
    innerDistance = receivedData.distance;

    Serial.print("Received Distance: ");
    Serial.print(innerDistance);
    Serial.println(" cm");

    if (innerDistance < 10 && !innerAlertSent) {
        digitalWrite(LED_PIN, HIGH);
        sendSMS(innerDistance, "Inner");
        innerAlertSent = true;
        delay(500);
        digitalWrite(LED_PIN, LOW);
    } else if (innerDistance >= 10) {
        innerAlertSent = false;
    }
}

void displayDistances() {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Outer: ");
    lcd.print(outerDistance, 1);
    lcd.print("cm");

    lcd.setCursor(0, 1);
    lcd.print("Inner: ");
    lcd.print(innerDistance, 1);
    lcd.print("cm");

    delay(500); // إتاحة وقت للعرض
}

void setup() {
    Serial.begin(115200);
    WiFi.mode(WIFI_STA);
    pinMode(LED_PIN, OUTPUT);
    pinMode(TRIG_PIN, OUTPUT);
    pinMode(ECHO_PIN, INPUT);

    sim800l.begin(9600, SERIAL_8N1, RX_PIN, TX_PIN);
    delay(3000);
    Serial.println("SIM800L Initialized");

    Wire.begin(SDA_PIN, SCL_PIN);
    lcd.init();
    lcd.backlight();
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("System Ready");
    delay(1000);
    lcd.clear();

    if (esp_now_init() != ESP_OK) {
        Serial.println("ESP-NOW init failed!");
        return;
    }
    esp_now_register_recv_cb(onDataRecv);
}

void loop() {
    outerDistance = measureLocalDistance();
    Serial.print("Local Distance: ");
    Serial.print(outerDistance);
    Serial.println(" cm");

    if (outerDistance < 10 && !outerAlertSent) {
        digitalWrite(LED_PIN, HIGH);
        sendSMS(outerDistance, "Outer");
        outerAlertSent = true;
        delay(500);
        digitalWrite(LED_PIN, LOW);
    } else if (outerDistance >= 10) {
        outerAlertSent = false;
    }

    displayDistances();
    delay(500);
}