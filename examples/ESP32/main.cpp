

#include "certs.h"
#include "secrets.h"

#ifndef DEVICE_NAME 
#error "Please include the correct certs.h file."
#endif

#include "shadow.pb.h"


#include <Arduino.h>
#include <WiFi.h>
#include <WiFiClientSecure.h>

// NOTE: Import everything else before Golain.h
#include "golain.h"




Golain golain(GOLAIN_CERT_CA, GOLAIN_DEVICE_CERT, GOLAIN_PRIVATE_KEY);




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
    if(current.on != last.on){
      Serial.println("ON status changed");
    }
    Serial.println("device shadow updated form web");

}

const char* WIFI_SSID     = SECRET_SSID;
const char* WIFI_PASSWORD = SECRET_PASS;

void setup() {
    Serial.begin(115200);

    // Fill in your Wi-Fi credentials here to connect to the internet:
    WiFi.begin(SECRET_SSID, SECRET_PASS);

    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }

    Serial.println("Connected to WiFi");

    golain.setDeviceShadowCallback(&onDeviceShadowUpdate);

    

    golain.begin();

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
}

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
        GlobalShadow.Nurse = 10;
        golain.updateShadow();
    }

    delay(1000);

    
}