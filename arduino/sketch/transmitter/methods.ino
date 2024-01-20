unsigned int getSensorValue() {
  /*Lectura del sensor:
    1. Se activa el transistor (PNP_PIN) para permitir el paso del negativo del emisor
    al colector y este al GND del ULN2803 para cerrar el circuito y hacer funcionar
    el ULN2803, se agrega un delay de 100ms para evitar errores de lectura.

    2. Se inicia una lectura de los 8 pines OUT (ULN2803) y almacenarlos en la variable
    <sensor> que por default asigna un entero de 16 bits para almacenar el valor. Mediante 
    el uso de operadores lógicos se va generando un numero binario.

        a) Se inicializa la variable sensor = 0b0000000000000000;

        b) Se realiza la operacion OUT AND 1 (digitalRead(OUT_PIN) & 0x01)
        Es decir 0b000000000000000X & 0b0000000000000001 con la idea de
        solo operar el bit menos significativo de la lectura con 1:
                            
                                AND
                        | X | B | SALIDA |
                        |---|---|--------|
                        | 0 | 1 |   0    |
                        | 1 | 1 |   1    |
        
        c) El resultado de la operación AND se desplazará n lugares
        a la izquierda (<< n), de tal manera que cada OUT se asigne
        al bit que le correspondería en notación binaria:
        
                         0b00000000HGFEDCBA

        Tal que A = OUT1, B = OUT2, C = OUT3, D = OUT4, E = OUT5
        F = OUT6, G = OUT7

        d) Uno a uno, los resultados de la operación AND  despues de 
        desplazarsese a donde corresponda, operarán un OR con la variable sensor
        que inicialmente vale 0b0000000000000000 el valor se asignará (|=)
        a la variable sensor de vuelta.

                        0b0000000000000000;
                        0b00000000HGFEDCBA;

                               OR
                    | OUT | SENSOR| SALIDA |
                    |-----|-------|--------|
                    |  0  |   0   |   0    |
                    |  1  |   0   |   1    |

    3. Tras un retraso para obtener los valores de los OUT, se apaga el 
    ULN2803 (PNP_PIN) y con ello se desenergiza el integrado, ahorrando energia
    y aumentando la vida util de las terminales sumergibles del sensor.

    4. Debido a que este proceso regresa 0's cuando se cierra el circuito, y 1's
    cuando se encuentra abierto; Para comodidad del desarrollador se busca replicar
    la siguiente logica:

            "Si 0 es falso y 1 es verdadero, entonces 0 es vacio y 1 es lleno"
    
    Por lo tanto, invertiremos el valor de los bits de la variable sensor.

        a) Se aplicara un XOR a los bits de la variable sensor con los bits de 0xFF (255),
        y este se asignara a la misma variable:

            sensor = sensor ^ 0xFF; o lo que es lo mismo, sensor ^= 0xFF;

                    Sensor = 0b00000000HGFEDCBA;
                      0xFF = 0b0000000011111111;

                               XOR
                    | Sensor | 0xFF | Salida |
                    |--------|------|--------|
                    |   0    |  1   |   0    |
                    |   1    |  1   |   1    |

        b) Debido a que despues del bit 8, se seguira operando XOR y por consecuente 
        esto afectara al valor de la variable sensor. Aplicaremos una AND para que solo
        los primeros 8 bits (0xFF) se mantengan y el resto de los bits se fuerzen a 0.
        
            sensor = sensor & 0xFF; o lo que es lo mismo, sensor &= 0xFF;

                    Sensor = 0b00000000HGFEDCBA;
                      0xFF = 0b0000000011111111;

                               AND
                    | Sensor | 0xFF | Salida |
                    |--------|------|--------|
                    |   0    |  1   |   0    |
                    |   1    |  1   |   1    |

        De esta manera, la variable toma la siguiente forma: Sensor = 0HGFEDCBA. Tal que 
        A = OUT1, B = OUT2, C = OUT3, D = OUT4, E = OUT5, F = OUT6, G = OUT7, y estas a su
        vez representan el valor logico de la lectura (0 para circuito abierto, y 1 para
        circuito cerrado). */

  // Inicializar la variable que contendrá el binario del estado logico del deposito.
  unsigned int sensorValue = 0;

  // Iniciar la lectura y construccion del numero binario que representara el deposito.
  digitalWrite(PNP_PIN, HIGH);
  delay(100);
  sensorValue |= (digitalRead(OUT8_PIN) & 0x01) << 7;
  sensorValue |= (digitalRead(OUT7_PIN) & 0x01) << 6;
  sensorValue |= (digitalRead(OUT6_PIN) & 0x01) << 5;
  sensorValue |= (digitalRead(OUT5_PIN) & 0x01) << 4;
  sensorValue |= (digitalRead(OUT4_PIN) & 0x01) << 3;
  sensorValue |= (digitalRead(OUT3_PIN) & 0x01) << 2;
  sensorValue |= (digitalRead(OUT2_PIN) & 0x01) << 1;
  sensorValue |= (digitalRead(OUT1_PIN) & 0x01);
  delay(100);
  digitalWrite(PNP_PIN, LOW);


  // XOR con 0xFF PARA inviertir los bits de la variable sensor.
  sensorValue ^= 0xFF;

  // Máscara para mantener solo 8 bits de la variable sensor.
  sensorValue &= 0xFF;
  
  return sensorValue;
}

/*El argumento estadoSensor entra como un entero, tras una serie de operaciones
regresa en formato String el valor correspondiente en notación binaria.*/
String getWaterLevel(unsigned int sensorValue) {
  // Garantizar que el valor esté en el rango correcto con 8 bits
  sensorValue &= 0xFF;
  // Crear una cadena para almacenar la representación binaria
  String waterLevel = "";
  // Generar la representación binaria de 8 bits
  for (int i = 7; i >= 0; i--) {
    waterLevel += (sensorValue & (1 << i)) ? '1' : '0';
  }
  return waterLevel;
}

void sendInfo(String serialNumber, String tokenAuth, String waterLevel, String releState, String systemMessage, int blinking, int ms) {
  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;
    String sendData = "serialNumber_device=" + serialNumber + "&tokenAuth=" + tokenAuth + "&waterLevel=" + waterLevel + "&releState=" + releState + "&systemMessage=" + systemMessage;
    // Indicamos el destino y preparamos el header
    http.begin("https://iot.amberage.com.mx/php/test.php");
    http.addHeader("Content-Type", "application/x-www-form-urlencoded");

    // Enviamos el POST pasándole los datos que queremos enviar (esta función nos devuelve un código que guardamos en un int)
    int responseCode = http.POST(sendData);

    if (responseCode > 0) {
      Serial.println("Respuesta del servidor: Código HTTP " + String(responseCode));  // Print del código de respuesta
      String responseBody = http.getString();
      Serial.println(responseBody + "\n");
      http.end();

      //Respuesta visual desde el led integrado
      if (blinking <= 1) {
        digitalWrite(INTEGRATED_LED, HIGH);
        delay(100);
        digitalWrite(INTEGRATED_LED, LOW);
      } else {
        for (int i = 0; i < blinking; i++) {
          digitalWrite(INTEGRATED_LED, HIGH);
          delay(100);
          digitalWrite(INTEGRATED_LED, LOW);
          delay(100);
        }
      }
      delay(ms);  //Milisegundos es un parametro que determina cuanto tiempo se ha de esperar despues de enviar la información.
    } else {
      Serial.println("E2-ERR-WIFI-CONNECTION");
      delay(ms);
    }
  }
}
