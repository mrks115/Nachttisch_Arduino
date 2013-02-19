/**
 * Spezielle Bytes
 */
#define MESSAGE_START  0x7F
#define MESSAGE_END    0x7E
#define MESSAGE_ESCAPE 0x7D
#define MESSAGE_XOR    0x20
#define MESSAGE_ACK    0xFF



/**
 * Finite-State-Machine Initialisierung
 */
#define STATE_WAIT_START 0  // Warten auf Start-Byte
#define STATE_RECEIVE    1  // Byte empfangen
#define STATE_XOR        2  // Byte nach Escape Byte empfangen
#define EVENT_START      0  // Start  Byte Empfangen
#define EVENT_END        1  // End    Byte Empfangen
#define EVENT_ESCAPE     2  // Escape Byte Empfangen
#define EVENT_OTHER      3  //        Byte Empfangen

byte currentState = STATE_WAIT_START; // Auf Start-Byte warten
char buffer[10];       // Maximale Länge der Nachrichten exkl. Start/End-Bytes
char bufferLength = 0; // Länge der Nachricht

typedef void (*action)(char c);



/**
 * State-Event-Matrix
 */
typedef struct {
  byte nextState;       
  action actionToDo; // function-pointer to the action that shall be released in current state
} stateElement;      // structure for the elements in the state-event matrix

stateElement stateMatrix[3][4] = {
   { {STATE_RECEIVE, taskClear}, {STATE_WAIT_START, taskIdle},  {STATE_WAIT_START, taskIdle}, {STATE_WAIT_START, taskIdle} },
   { {STATE_RECEIVE, taskIdle},    {STATE_WAIT_START, taskReady}, {STATE_XOR, taskIdle},        {STATE_RECEIVE, taskAdd} },
   { {STATE_RECEIVE, taskAddXor},  {STATE_RECEIVE, taskAddXor},   {STATE_RECEIVE, taskAddXor},  {STATE_RECEIVE, taskAddXor} }
};



/**
 * Aktuellen Task ausführen und nächsten Zustand holen
 */
void stateNext(byte e, char c) {
    //determine the State-Matrix-Element in dependany of current state and triggered event
    stateElement stateEvaluation = stateMatrix[currentState][e];
 
    // get next state
    currentState = stateEvaluation.nextState;
    
    // run task
    (*stateEvaluation.actionToDo)(c);
}



/**
 * Serielle Daten empfangen
 */
void receiveSerial() {
  char c, e;

  while (Serial3.available()) {
    c = Serial3.read();
    Serial2.print(c);
    
    
    switch (c) {
      case MESSAGE_START:  e = EVENT_START;  break;
      case MESSAGE_END:    e = EVENT_END;    break;
      case MESSAGE_ESCAPE: e = EVENT_ESCAPE; break;
      default:             e = EVENT_OTHER;
    }
    
    stateNext(e, c);
  }
}



/**
 * State-Event-Matrix-Tasks
 */

// Nichts tun
void taskIdle(char c) {}

// Zurücksetzen des Buffers
void taskClear(char c) {
  bufferLength = 0;
}

// Alle Bytes empfangen
void taskReady(char c) {
  evalCommand();
}

// Byte zu Buffer hinzufügen
void taskAdd(char c) {
  buffer[bufferLength++] = c;
}

// Byte mit XOR zu Buffer hinzufügen
void taskAddXor(char c) {
  buffer[bufferLength++] = c ^ MESSAGE_XOR;
}



/**
 * Befehl ausführen
 */
void evalCommand() {
    switch (buffer[0]) {
      case 0x00: ; break; // Get Light Sensor Value
      case 0x01: ; break; // Get Temperature Sensor Value
      case 0x02: ; break; // Get Humidity Sensor Value
      
      case 0x10: radioSend(1, 0); break; // Switch A Off
      case 0x11: radioSend(1, 1); break; // Switch A On
      case 0x12: radioSend(2, 0); break; // Switch B Off
      case 0x13: radioSend(2, 1); break; // Switch B On
      case 0x14: radioSend(3, 0); break; // Switch C Off
      case 0x15: radioSend(3, 1); break; // Switch C On
      case 0x16: radioSendAll(0); break; // Switch All Off
      case 0x17: radioSendAll(1); break; // Switch All On
    }
}



/**
 * Befehl weiterleiten an LED-Matrix
 */
void sendColorduino(char bytes[], char num) {
  Serial2.write(MESSAGE_START);
    
  for (char i = 0; i < num; i++) {
    if (bytes[i] == MESSAGE_START || bytes[i] == MESSAGE_END || bytes[i] == MESSAGE_ESCAPE) {
      Serial2.write(MESSAGE_ESCAPE);
      Serial2.write(MESSAGE_XOR ^ bytes[i]);
    } else {
      Serial2.write(bytes[i]);
    }
  }
    
  Serial2.write(MESSAGE_END);
}
