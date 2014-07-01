
void rgb_led(uint8_t r, uint8_t g, uint8_t b) {
  analogWrite(rgb[0], r);
  analogWrite(rgb[1], g);
  analogWrite(rgb[2], b);
}

void led_blink(uint16_t sp) {
  if(millis()-last_ledblink >= sp) {
    digitalWrite(led, HIGH);
    delay(100);
    digitalWrite(led, LOW);
    last_ledblink = millis();
  }
}



