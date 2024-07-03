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

/************************************* Device Shadow Callback ************************************
** This function is called everytime your device shadow updates. You can use this to update your
** device state based on the shadow state.
** The current or the new incoming shadow from the cloud is passed as the first argument, 
** and the last shadow state is passed as the second argument.
**
** For Example:
** If you want to turn on an LED when the red variable in the shadow is set to true, you can do:
**
** void onDeviceShadowUpdate(Shadow current, Shadow last) {
**     if (current.red) {
**         Serial.println("Red is on");
**         TODO: Add logic to turn on LED
**     }
** }
** 
** NOTE: You can only access the shadow variables in this function. If you want to access the shadow
** variables outside this function, you can use the GlobalShadow object.
** Editing the current or the last shadow in this function will not update the shadow in the cloud.
**
** 
** To update the shadow in the cloud, set the new values in the GlobalShadow object, and call
** golain.updateShadow() function to send the new shadow to the cloud.
**
*************************************************************************************************/

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

    /************************************* Using GSM / 4G Connection ******************************
    ** Golain's SDK supports using TinyGSM to connect to the internet using a GSM / 4G connection.
    ** To use this feature, add this macro to the beginning of your code, before including golain.h:
    **
    ** #define CONFIG_GOLAIN_USE_GSM
    **
    ** Then, after initializing the TinyGSM modem, pass the modem object to the golain.begin() function.
    ** For Example:
    ** TinyGsm modem(SerialAT);
    ** golain.begin(&modem);
    /**********************************************************************************************/

void loop() {

    // Call this function to keep the connection alive.
    golain.loop();

    /************************************ Using Device Shadow ************************************
    ** You can use GlobalShadow to access the current state of the device. Update the GlobalShadow
    ** Variable and call golain.updateShadow() to update the shadow in the cloud from your device!
    **
    ** For Example:
    ** If you want to move your robot in a direction given by the `direction` variable in your
    ** shadow, you can do:
    ** 
    ** if (GlobalShadow.direction == "forward") {
    **     Serial.println("Robot Moving Forward");
    **     TODO: Add logic for controlling robot.
    ** }
    ** 
    ** And, if you want to update the state on cloud of whether a button is pressed or not:
    ** 
    ** if (buttonPressed) {
    **     Serial.println("Button Pressed");
    **     GlobalShadow.buttonPressed = true;
    **     golain.updateShadow();
    ** } else {
    **     Serial.println("Button Not Pressed");
    **     GlobalShadow.buttonPressed = false;
    **     golain.updateShadow();
    ** } 
    **  
    **********************************************************************************************/

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
