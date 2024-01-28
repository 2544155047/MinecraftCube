#include <Arduino.h>
#include<DHT.h>
#include<DHT_U.h>
#include <WiFi.h>
#include <WiFiClient.h>
#include <WebServer.h>
#include <ESPmDNS.h>
#include <Update.h>

const char* host = "esp32-webupdate";
const char* ssid = "HelloWorld";
const char* password = "FJPXPG123";

#define DHTPIN 2    // 定义DHT11模块连接的ESP32引脚（需要修改）
#define DHTTYPE DHT11   // 定义DHT11模块


float Humidity;   // 定义湿度变量
float Temperature;   // 定义温度变量

DHT dht(DHTPIN, DHTTYPE); // 创建DHT对象

void setup() {
    Serial.begin(115200);
    Serial.println("Booting Sketch...");
    dht.begin();
    Serial.println("DHT is Ready");

}

void getDHT(){
    delay(1000);
    Humidity = dht.readHumidity();   // 读取湿度
    Temperature = dht.readTemperature();   // 读取温度
    if (isnan(Humidity) || isnan(Temperature)) {   // 如果读取失败
    Serial.println("Failed to read from DHT sensor!");   // 输出错误信息
    }
    Serial.print("Humidity: ");   // 输出湿度信息
    Serial.println(Humidity);   // 输出湿度值
    Serial.print("Temperature: ");   // 输出温度信息
    Serial.print(Temperature);   // 输出温度值
    Serial.println(" °C ");

}