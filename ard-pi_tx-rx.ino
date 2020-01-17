//Message structure: <message>

const byte numChars = 64;
char receivedChars[numChars];
int id, spin, dir1, vel1, dir2, vel2;
boolean newData = false;

byte ledPin = 13;   // the onboard LED

//===============

void setup() {
    Serial.begin(115200);

    pinMode(ledPin, OUTPUT);
    digitalWrite(ledPin, HIGH);
    delay(200);
    digitalWrite(ledPin, LOW);
    delay(200);
    digitalWrite(ledPin, HIGH);

    Serial.println("<Arduino is ready>");

    
}

//===============

void loop() {
    rxMessage();
    if(newData)
    {
      Serial.println(receivedChars);
      char* token = strtok(receivedChars, " ");
      id = atoi(token);
      spin = atoi(strtok(0, " "));
      dir1 = atoi(strtok(0, " "));
      vel1 = atoi(strtok(0, " "));
      dir2 = atoi(strtok(0, " "));
      vel2 = atoi(strtok(0, " "));
    }
    returnMessage();
    
}

//===============
// TODO
// add message timeout
// add parsing of values x
//===============
void rxMessage() {
    static boolean rxInProgress = false;
    static byte index = 0;
    char startMarker = '<';
    char endMarker = '>';
    char receivedString;

    while (Serial.available() > 0 && newData == false) {
        receivedString = Serial.read();
        if (rxInProgress == true) {
            if (receivedString != endMarker) {
                receivedChars[index] = receivedString;
                index++;
                if (index >= numChars) {
                    index = numChars - 1;
                }
            }
            else {
                receivedChars[index] = '\0'; // terminate the string
                rxInProgress = false;
                index = 0;
                newData = true;
            }
        }

        else if (receivedString == startMarker) {
            rxInProgress = true;
        }
    }
}

//===============

void returnMessage() {
    if (newData == true) {
        Serial.print("<Ack message id: ");
        Serial.print(id);
        Serial.print("   ");
        Serial.print(millis());
        Serial.print('>');
            // change the state of the LED everytime a reply is sent
        digitalWrite(ledPin, ! digitalRead(ledPin));
        newData = false;
        delay(1000);
    }
}

//===============
