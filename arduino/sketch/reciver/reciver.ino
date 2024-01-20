#include <WiFiManager.h> 

const int INTEGRATED_LED = 8;

void setup() {
    WiFi.mode(WIFI_STA);
    Serial.begin(115200);
    pinMode(INTEGRATED_LED, OUTPUT);
    digitalWrite(INTEGRATED_LED, HIGH);
    
    WiFiManager wm;
    //wm.resetSettings();
    bool res;
    res = wm.autoConnect("HydroSync - HR01MX","12345678");

    if(!res) {
        Serial.println("Fallo en la conexión");
        //ESP.restart();
    } 
    else {
        Serial.println("Conexión con éxito");  
    }

}

void loop() {
    digitalWrite(INTEGRATED_LED, LOW);
    delay(1000);
    digitalWrite(INTEGRATED_LED, HIGH);
    delay(1000);
}
