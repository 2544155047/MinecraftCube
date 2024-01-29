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
WebServer server(80);
const char* serverIndex = "<form method='POST' action='/update' enctype='multipart/form-data'><input type='file' name='update'><input type='submit' value='Update'></form>";

void setup() {
    Serial.begin(115200);
    Serial.println("Booting Sketch...");
    dht.begin();
    Serial.println("DHT is Ready");
    WiFi.mode(WIFI_AP_STA);
    WiFi.begin(ssid, password);
    if (WiFi.waitForConnectResult() == WL_CONNECTED) {
        MDNS.begin(host);
        server.on("/", HTTP_GET, []() {
        server.sendHeader("Connection", "close");
        server.send(200, "text/html", serverIndex);
        });
        server.on("/update", HTTP_POST, []() {
        server.sendHeader("Connection", "close");
        server.send(200, "text/plain", (Update.hasError()) ? "FAIL" : "OK");
        ESP.restart();
        }, []() {
        HTTPUpload& upload = server.upload();
        if (upload.status == UPLOAD_FILE_START) {
            Serial.setDebugOutput(true);
            Serial.printf("Update: %s\n", upload.filename.c_str());
            if (!Update.begin()) { //start with max available size
            Update.printError(Serial);
            }
        } else if (upload.status == UPLOAD_FILE_WRITE) {
            if (Update.write(upload.buf, upload.currentSize) != upload.currentSize) {
            Update.printError(Serial);
            }
        } else if (upload.status == UPLOAD_FILE_END) {
            if (Update.end(true)) { //true to set the size to the current progress
            Serial.printf("Update Success: %u\nRebooting...\n", upload.totalSize);
            } else {
            Update.printError(Serial);
            }
            Serial.setDebugOutput(false);
        } else {
            Serial.printf("Update Failed Unexpectedly (likely broken connection): status=%d\n", upload.status);
        }
        });
        server.begin();
        MDNS.addService("http", "tcp", 80);

        Serial.printf("Ready! Open http://%s.local in your browser\n", host);
    } else {
        Serial.println("WiFi Failed");
    }

}

void loop() {
    server.handleClient();
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