// Codes für                  -- A --         -- B --         -- C --
char switchCodes[2][3][13] = { { "111110FFFF0F", "11111F0FFF0F", "11111FF0FF0F" }, // AN
                               { "111110FFFFF0", "11111F0FFFF0", "11111FF0FFF0" }  // AUS
                             };


/**
 * Empfängt Daten vom 433 MHz Modul
 */
void radioReceive() {
  if (mySwitch.available()) {                    // Wenn Daten Empfangen wurden
    switch (mySwitch.getReceivedValue()) {
      case 16766228: D_ON();  break;
      case 16766225: D_OFF(); break;
      case 16766276: E_ON();  break;
      case 16766273: E_OFF(); break;
    }
    mySwitch.resetAvailable();
  }
}


/**
 * Sendet Daten vom 433 MHz Modul
 */
void radioSend(char device, char state) {
  mySwitch.sendTriState( switchCodes[state][device-1] );
}


/**
 * Sendet Daten vom 433 MHz Modul
 */
void radioSendAll(char state) {
  mySwitch.sendTriState( switchCodes[state][0] );
  mySwitch.sendTriState( switchCodes[state][1] );
  mySwitch.sendTriState( switchCodes[state][2] );
}
