/*
   wESP32 Arduino BLE beacon scanner
   Based on Evandro Copercini Arduino port: https://github.com/moononournation/Arduino_BLE_Scanner/blob/master/Arduino_BLE_Scanner.ino
   Based on Neil Kolban example for IDF: https://github.com/nkolban/esp32-snippets/blob/master/cpp_utils/tests/BLE%20Tests/SampleScan.cpp
*/

#define POST_URL "http://192.168.1.131:5000/"
#define SCAN_TIME  10 // seconds
// Comment the follow line to disable serial message
#define SERIAL_PRINT

#include <Arduino.h>

#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEScan.h>
#include <BLEAdvertisedDevice.h>
#include <ETH.h>
#include <HTTPClient.h>

BLEScan *pBLEScan;

class MyAdvertisedDeviceCallbacks : public BLEAdvertisedDeviceCallbacks
{
    void onResult(BLEAdvertisedDevice advertisedDevice)
    {
#ifdef SERIAL_PRINT
      Serial.printf("Advertised Device: %s \n", advertisedDevice.toString().c_str());
#endif
    }
};

void setup()
{
  // Start the ethernet (Arduino-ESP32 3.x on rev 7 hardware)
  ETH.begin(ETH_PHY_RTL8201, 0, 16, 17, -1, ETH_CLOCK_GPIO0_IN);
  
#ifdef SERIAL_PRINT
  Serial.begin(115200);
  Serial.println("ESP32 BLE Scanner");
#endif

  // Init BLE, create and configure scanner
  BLEDevice::init("");
  pBLEScan = BLEDevice::getScan(); //create new scan
  pBLEScan->setAdvertisedDeviceCallbacks(new MyAdvertisedDeviceCallbacks());
  pBLEScan->setActiveScan(true); //active scan uses more power, but get results faster
  pBLEScan->setInterval(0x50);
  pBLEScan->setWindow(0x30);
}

void loop()
{
#ifdef SERIAL_PRINT
  Serial.printf("Start BLE scan for %d seconds...\n", SCAN_TIME);
#endif

  BLEScanResults *foundDevices = pBLEScan->start(SCAN_TIME);
  int count = foundDevices->getCount();
  String ss = "[";
  for (int i = 0; i < count; i++)
  {
    if (i > 0) {
      ss += ",";
    }
    BLEAdvertisedDevice d = foundDevices->getDevice(i);
    ss += "{\"address\":\"";
    ss += d.getAddress().toString().c_str();
    ss += "\",\"rssi\":";
    ss += d.getRSSI();

    if (d.haveName())
    {
      ss += ",\"name\":\"";
      ss += d.getName().c_str();
      ss += "\"";
    }

    if (d.haveAppearance())
    {
      ss += ",\"appearance\":";
      ss += d.getAppearance();
    }

    if (d.haveManufacturerData())
    {
      String md = d.getManufacturerData();
      uint8_t *mdp = (uint8_t*)md.c_str();
      char *pHex = BLEUtils::buildHexData(nullptr, mdp, md.length());
      ss += ",\"mfgdata\":\"";
      ss += pHex;
      ss += "\"";
      free(pHex);
    }

    if (d.haveServiceUUID())
    {
      ss += ",\"svcuuid\":\"";
      ss += d.getServiceUUID().toString().c_str();
      ss += "\"" ;
    }

    if (d.haveTXPower())
    {
      ss += ",\"txpower\":";
      ss += (int)d.getTXPower();
    }

    ss += "}";
  }
  ss += "]";

#ifdef SERIAL_PRINT
  Serial.println("Scan done!");
  Serial.println("[HTTP] begin...");
#endif

  // Configure URL where we will post the scanned beacons
  HTTPClient http;
  http.begin(POST_URL);
  http.addHeader("Content-Type", "application/json");

#ifdef SERIAL_PRINT
  Serial.println("[HTTP] POST");
#endif

  // Start connection and send HTTP header and POST body
  int httpCode = http.POST(ss);

  // httpCode will be negative on error
  if (httpCode > 0)
  {
    // HTTP header has been send and Server response header has been handled
#ifdef SERIAL_PRINT
    Serial.printf("[HTTP] POST... code: %d\n", httpCode);
    Serial.println(http.getString());
#endif
  }
  else
  {
#ifdef SERIAL_PRINT
    Serial.printf("[HTTP] POST... failed, error: %s\n", http.errorToString(httpCode).c_str());
#endif
  }

  http.end();
}
