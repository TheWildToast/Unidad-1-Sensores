
void setup() {
  Serial.begin(115200);
}
bool iguales(char *contraCorrecta, char *contraMetida)
{
  int dig = 0;
  for (int i = 0; i < 6; i++) {
    if (contraCorrecta[i] == contraMetida[i])
    {
      dig = dig + 1;
    }
  }
  if (dig == 6)
  {
    return true;
  }
  else
  {
    return false;
  }
}

void toggle() {
  static uint32_t previousMillis = 0;
  static const uint32_t interval = 500;
  static bool taskInit = false;
  static const uint8_t ledPin =  3;
  static uint8_t ledState = LOW;
  
  if (taskInit == false) {
    pinMode(ledPin, OUTPUT);
    taskInit = true;
  }
  uint32_t currentMillis = millis();
  if ( (currentMillis - previousMillis) >= interval) {
    previousMillis = currentMillis;
    if (ledState == LOW) {
      ledState = HIGH;
    } else {
      ledState = LOW;
    }
    digitalWrite(ledPin, ledState);
  }
}
void bomba() {
  static unsigned long previousMillis = 0;
  static unsigned long intervalo = 1000;
  static char correcta[6] = {'d', 'u', 'd', 'd', 'u', 'a'};
  static char ingresada[6];
  static int counter = 0;
  static int estado = 1;
  static int regresiva = 20;
  unsigned long currentMillis = millis();


  switch (estado) {
    case 1: {
        if (Serial.available() > 0)
        {
          char dato = Serial.read();
          if ((char)dato == 'd' && regresiva > 10)
          {
            regresiva--;
            Serial.println(regresiva);
          }
          else if ((char)dato == 'u' && regresiva < 60)
          {
            regresiva++;
            Serial.println(regresiva);
          }
          else if ((char)dato == 'a')
          {
            estado = 2;
            previousMillis = millis();
            counter = 0;

          }
        }

        break;
      }
    case 2: {
        if (Serial.available() > 0) {
          ingresada[counter] = Serial.read();

          
          counter = counter + 1;
          if (counter == 6) {
            if (iguales(correcta, ingresada))
            {
              estado = 1;
              regresiva = 20;
              Serial.println("Bomba no exploto");
            }
            counter = 0;
          }
        }
        if ( (millis() - previousMillis) >= intervalo)
        {
          previousMillis = millis();
          if (regresiva > 0)
          {
            Serial.println(regresiva);
            regresiva = regresiva - 1;
          }
          else
          {
            estado = 1;
            regresiva = 20;
            Serial.println("Bomba Exploto");
          }
        }

        break;
      }
    default:
      Serial.println("Error");
      break;
  }
}
void loop() {
  bomba();
  toggle();
}
