#include "certs.h"

#ifndef DEVICE_NAME
#error "Please include the correct certs.h file."
#endif

#define LED_BUILTIN 2

//uncomment below lines once you added the header files
//#include "shadow.pb.h"
//#include "<data-point_name>.pb.h"

#include <WiFi.h>
#include <WiFiClientSecure.h>

// NOTE: Import everything else before Golain.h
#include "Golain.h"


Golain golain(DEVICE_NAME, ROOT_TOPIC, GOLAIN_CERT_CA, GOLAIN_DEVICE_CERT, GOLAIN_PRIVATE_KEY);

GolainDataPoint IInputs;
sudo inputsData;

void onDeviceShadowUpdate(Shadow current, Shadow last) {
    Serial.println("Device shadow updated");

    // Control the LED based on the shadow state
    if (current.ledState) {
        digitalWrite(LED_BUILTIN, HIGH);
        delay(1000);
        Serial.println("LED is ON");
    } else {
        digitalWrite(LED_BUILTIN, LOW);
        delay(1000);
        Serial.println("LED is OFF");
    }
    // Serial output char data type given from the console
    if(current.has_macAddr){
      Serial.println(current.macAddr);
    }
}

void setup() {
    Serial.begin(115200);

    // Fill in your Wi-Fi credentials here to connect to the internet:
    WiFi.begin("<SSID>", "<PASSWORD>");

    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }

    Serial.println("Connected to WiFi");

    golain.setDeviceShadowCallback(&onDeviceShadowUpdate);

    Result r;
    r = golain.registerDataPoint("Inputs", &sudo_msg, sudo_size);

    if (r.err) {
        Serial.println("Failed to register data point: Inputs");
    } else {
        Serial.println("Registered data point: Inputs");
        IInputs = r.value;
    }

    golain.begin();
    pinMode(LED_BUILTIN, OUTPUT);
}

void loop() {

    // Call this function to keep the connection alive.
    golain.loop();

    if (millis() % 5000 == 0) {
        GlobalShadow.buttonPress = 1;
        golain.updateShadow();
    }

    delay(1000);

    inputsData.id6 = 10;

    // post-and-forget:
    golain.postData(IInputs, &inputsData);
//uncomment this and add respective functions to post data on respective data-points structure
// typedef struct {
//     int id;
//     int iaq;
//     int temperature;
//     int humidity;
//     int co2;
//     int voc;
// } SensorData;

    // Confirm post data:
    if (golain.postData(IInputs, &inputsData).err != GOLAIN_OK) {
        Serial.println("Failed to post data for Data Point: INPUTS");
    } else {
        Serial.println("Posted data for Data Point: INPUTS");
    }
}