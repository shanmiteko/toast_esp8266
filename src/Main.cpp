#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <TZ.h>
#include "LED.h"
#include "Certs.h"
#include "Fetch.h"

const char *STASSID = "";
const char *STAPSK = "";
const char *QYWXBOTKEY = "";

void wifiConnect(const char *ssid, const char *psk)
{
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, psk);

  Serial.printf("Connecting(%s)", ssid);
  while (WiFi.status() != WL_CONNECTED)
  {
    LED_BLINK(delay(100));
    delay(1000);
  }
  Serial.println();

  Serial.print("Connected, IP address: ");
  Serial.println(WiFi.localIP());
}

char *getLocalTime()
{
  time_t now = time(nullptr);
  struct tm timeinfo;
  localtime_r(&now, &timeinfo);
  return asctime(&timeinfo);
}

// Set time via NTP, as required for x.509 validation
void timeSync()
{
  configTime(TZ_Asia_Shanghai, "ntp.aliyun.com", "ntp1.aliyun.com");

  Serial.print("Waiting for NTP time sync");
  time_t now = time(nullptr);
  while (now < 3600)
  {
    LED_BLINK(delay(100));
    delay(500);
    now = time(nullptr);
  }
  Serial.println("");

  struct tm timeinfo;
  localtime_r(&now, &timeinfo);
  Serial.print("Current time: ");
  Serial.print(asctime(&timeinfo));
}

void toast(const String &text)
{
  Respond res = Fetch.POST(
      String("https://qyapi.weixin.qq.com/cgi-bin/webhook/send?key=") + QYWXBOTKEY,
      String("{\"msgtype\":\"text\",\"text\":{\"content\":\"") + text + "\"}}");
  if (!res.isOk())
  {
    Serial.print("\r\nServer Respose Code: ");
    Serial.println(res._status);
  }
}

const String sysInfo()
{
  String result;

  result += String(getLocalTime()) + "\n";
  result += "flashSize: " + String(ESP.getFlashChipSize()) + "\n";
  result += "freestack: " + String(ESP.getFreeContStack()) + "\n";
  result += "freeHeap: " + String(ESP.getFreeHeap());

  return result;
}

void setup()
{
  pinMode(LED_BUILTIN, OUTPUT);

  Serial.begin(115200);
  Serial.println();

  wifiConnect(STASSID, STAPSK);

  timeSync();

  toast(ESP.getFullVersion());
}

void loop()
{
  LED_BLINK(delay(500));
  toast(sysInfo());
  delay(10000);
}