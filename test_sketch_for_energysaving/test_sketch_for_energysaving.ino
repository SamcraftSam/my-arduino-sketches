#include <avr/wdt.h>
#include <avr/sleep.h>
volatile bool f = 0;
int idleTime = 0;
void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
}

void loop() {
  while (idleTime <= 4000){
    wdt_enable(WDTO_2S); //Задаем интервал сторожевого таймера (2с)
    WDTCSR |= (1 << WDIE); //Устанавливаем бит WDIE регистра WDTCSR для разрешения прерываний от сторожевого таймера
    set_sleep_mode(SLEEP_MODE_PWR_DOWN); 
    sleep_mode(); // Переводим МК в спящий режим
    idleTime = idleTime + 2000;
  }
  digitalWrite(LED_BUILTIN, HIGH);
  delay(2000);
  digitalWrite(LED_BUILTIN, LOW);
  idleTime = 0;
}

ISR (WDT_vect) {
  wdt_disable();
  //f = !f;
}
