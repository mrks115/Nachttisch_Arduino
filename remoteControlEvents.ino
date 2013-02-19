/**
 * D AN wurde gedrückt
 */
void D_ON() {
  Serial.println("D AN");
  char array[5] = { 0x21, 0xFF, 0xFF, 0xFF, 0xFF };
  sendColorduino(array, 5);
}

/**
 * D AUS wurde gedrückt
 */
void D_OFF() {
  Serial.println("D AUS");
  char array[4] = { 0x21, 0, 0, 0 };
  sendColorduino(array, 4);
}

/**
 * E AN wurde gedrückt
 */
void E_ON() {
  Serial.println("E AN");
  char array[2] = { 0x22, 50 };
  sendColorduino(array, 2);
}

/**
 * E AUS wurde gedrückt
 */
void E_OFF() {
  Serial.println("E AUS");
  char array[6] = { 0x24, 0xFF, 0xFF, 0xFF, 0x03, 50 };
  sendColorduino(array, 6);
}


