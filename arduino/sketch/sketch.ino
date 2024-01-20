/*
Nombre del Proyecto: HydraSync - Transmitter
Descripción: Este código controla la lógcia del circuito que detecta el nivel de agua y lo envia a la BBDD
Autor: Alejandro Galicia.
Fecha de Creación: 19 de enero de 2024
*/

/*Librerias*/
#include <HTTPClient.h>
#include <WiFi.h>

/*Configuración de pines*/
const int OUT8_PIN = 5;  //Sensor al 90%
const int OUT7_PIN = 6;  //Sensor al 80%
const int OUT6_PIN = 7;  //Sensor al 70%
const int OUT5_PIN = 9;  //Sensor al 60%
const int OUT4_PIN = 10;  //Sensor al 50%
const int OUT3_PIN = 20;  //Sensor al 35%
const int OUT2_PIN = 21;  //Sensor al 20%
const int OUT1_PIN = 0;  //Sensor al 10%
const int PNP_PIN = 1;  //Sensor al 10%
const int INTEGRATED_LED = 8; // Led interno


/*Parametros del dispositivo*/
const String serialNumber = "HT01MX";
const String defaultTA = "xj0]-5J;gdu8lO8#";


/*Parametros de conexión WiFi*/
char* ssid = "Lancelot";
char* password = "GVRA_313352971";

/*Informacion que se manda a la BBDD*/
String tokenAuth = defaultTA;
String waterLevel = "";
String releState = "ON";
String systemMessage = "";

void setup() {
  pinMode(OUT8_PIN, INPUT_PULLUP);
  pinMode(OUT7_PIN, INPUT_PULLUP);
  pinMode(OUT6_PIN, INPUT_PULLUP);
  pinMode(OUT5_PIN, INPUT_PULLUP);
  pinMode(OUT4_PIN, INPUT_PULLUP);
  pinMode(OUT3_PIN, INPUT_PULLUP);
  pinMode(OUT2_PIN, INPUT_PULLUP);
  pinMode(OUT1_PIN, INPUT_PULLUP);
  pinMode(PNP_PIN, OUTPUT);
  pinMode(INTEGRATED_LED, OUTPUT);

  /*Iniciar comunicaciones*/
  Serial.begin(115200);
  WiFi.begin(ssid, password);
  delay(1000);
  /*Intentar conexión una vez cada segundo durante 15 segundos.
  Si la conexión es exitosa ejecuta el bucle principal, de lo
  contrario, manda al estado de error: E2-ERR-WIFI-CONNECTION*/
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
  }
  Serial.println("Hydratech conectada con exito a la IP: ");
  Serial.print(WiFi.localIP());
  digitalWrite(INTEGRATED_LED, LOW);
}

int iterador = 0;
void loop() {
  waterLevel = String(iterador += 1);
  systemMessage = String(iterador += 3);
  sendInfo(serialNumber, tokenAuth, waterLevel, releState, systemMessage, 1, 2500);
}