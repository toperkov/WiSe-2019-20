# Bežične senzorske mreže - Lab 4

### FESB, smjer 110/111/112/114/120, akademska godina 2019/2020

## Interupt

![trigger](https://media.giphy.com/media/d7wz7XI8RAikOv6iTI/giphy.gif)

### Što je Interrupt?

Interrupt je signal koji govori procesoru da odmah zaustavi na čemu trenutno radi i pokrene neku obradu visokog prioriteta. Ta se obrada visokog prioriteta naziva *Interrupt Handler* pri čemu se izvršava tzv. ISR funkcija (*Interrupts Service Routine*). Kao što mu ime kaže, *Interrupt* "prekida" normalan tok programa da bi izveo radnju (pokrenuo drugi blok koda, funkciju itd.) te se nakon izvršenja vrati se u normalan tok programa kada je akcija završena. Prekidi se primjerice mogu odnositi za događaje kao što su promjene *countera*, stanja promjene pin-a (iz LOW u HIGH ili iz HIGH u LOW), komunikacije preko Serial porta koja prima informacije ili analogno / digitalne pretvorbe, ovisno o tome što podržava mikrokontroler s kojim radite.

### Odakle dolazi interrupt?

Interrupti se mogu generirati iz više izvora:

1. **Vanjski Interrupt** zbog promjene stanja jednog od vanjskih interrupt pinova.
2. **Timer Interrupt** iz jednog od Arduinovih timera.
3. **Interrupt pin-ovi** promjenom stanja bilo kojeg u grupi pinova.


### Za što su oni dobri?


Pomoću interrupta, nije potrebno pisati petlju koda kojom cijelo vrijeme provjeravate je li se dogodila neka bitna promejna. Nije potrebno brinuti o potenicjalnoj sporoj reakciji ili primjerice propuštenim pritiscima tipki zbog dugotrajnih izvršenja podprograma. Procesor će automatski zaustaviti sve na čemu radi kada se *Interrupt* dogodi i pozvati tzv. *Interrupt Handler*. Potrebno je samo napisati pripadajuću funkciju koju pozvate u trnutku izvršenja Interrupta.

## Vanjski Interrupt

Broj vanjskih Interrupta koje posjeduju mikrokontroleri se razlikuje. Na primjer, Arduino boardovi, kao što su UNO-a ili Duemilanove, imaju samo dva interrupta koji se nalaze na digitalnim pinovima 2 i 3. Ostali boardovi poput Arduino Mega imaju 6, dok esp8266 ima oko 16 interruptova.

| BOARD |DIGITAL PINS USABLE FOR INTERRUPTS |
|----------|:-------------|
|UNO, Nano, Mini, other 328-based | 2, 3  |
|Uno WiFi Rev 2 | all digital pins |
|Mega, Mega2560, MegaADK | 2, 3, 18, 19, 20, 21 |
|Micro, Leonardo, other 32u4-based | 0, 1, 2, 3, 7 |
|Zero | all digital pins except 4 |
|MKR Family boards | 0, 1, 4, 5, 6, 7, 8, 9, A1, A2 |
|Due | all digital pins |
|101 | all digital pins (Only pins 2, 5, 7, 8, 10, 11, 12, 13 work with CHANGE) |


### Što je Interrupts Service Routine?

**Interrupts Service Routine (ISR)** naziva se i *Interrupt Handler*. Postoji mnogo različitih *Interrupt Handler*-a koji mogu obraditi više različitih vrsta prekida, poput jednostavnog primjera sata u računalu koji ima vlastiti Interrupt Handler, drugi primjer je tipkovnica.

Na slici iznad je dan primjer načina izvršenja koda, gdje se kod uredno izvršava dok se *Interrupt* ne pozove na *Line 3*, a zatim funkcija skoči na *ISR* i počne izvršavati *Line 5* i *Line 6*. Nakon izvršenja linija koda unutar *ISR*-a skače natrag na *Line 4* i nastavlja izvršavati kod. Ako se kod nalazi u petlji (npr. u ``loop()`` funkciji), vraća se na *Line 1*.

### Kako se aktiviraju Interrupti?

Postoji pet vrsta pokretanja Interrupta na Arduinu:

- **CHANGE:** Kada se signal promijeni čak i ako se signal poraste ili padne ili ako je signal u niskom stanju na 0 ili ako je signal u visokom stanju 5V.
- **RISING:** Na uzlaznom rubu signal koji ide od niskog do visokog stanja, odnosno signal koji aktivira od 0V do 5V.
- **FALLING:** Na padajućem rubu signal koji ide s visokog na niži, odnosno da se signal aktivira od 5V do 0V.
- **LOW:** Nisko je kontinuirani okidač (trigger) kad god je signal nizak, drugim riječima signal je na 0V.
- **HIGH:** Visoka je kontinuirani okidač (trigger) kad god je signal visok, drugim riječima signal je na 5v.

![Arduino-interrupts](https://user-images.githubusercontent.com/8695815/77925555-47eb6180-72a5-11ea-8d9a-2caf3d8609bc.jpg)

Rukovanje Interruptom se može interpretirati kao i bilo koja druga ``void`` funkcija. Ako napišete neku funkciju te je dodate *Interruptu*, ona će se pozvati kad god se aktivira taj *Interrupt signal*. Kad se vratite iz *Interrupt Handlera*, procesor nastavlja raditi na onome što je radio prije.

Kao što možemo vidjeti iz ove tablice, Arduino UNO ima samo 2 Interrupt pina, a oni su 2 i 3. Ovo se može upotrijebiti na način da se pozove sljedeća funkcija:

```arduino
attachInterrupt(digitalPinToInterrupt(pin), ISR function, Mode);
```

Ova funkcija gotovo uvijek ide u ``setup()`` funkciju. Kao što možete primjetiti, upotrebljavaju se 3 parametra:

- **Broj Interrupt pina:** Prvi parametar uzima broj interrupt pin-a kojeg želimo upotrbljvati. Službena dokumentacija Arduina preporučuje korištenje funkcije digitalPinToInterrupt(). Jednostavno unesite broj pina u funkciju digitalPinToInterrupt.
- **ISRFunction:** ISR je kratica za Interrupts Service Routine. Ovdje trebamo proslijediti naziv funkcije koja bi trebala sadržavati što će mikrokontroler učiniti kad vaš željeni Interrupt pin primi vrstu koju događaj / okidač nego što spominjete u sljedećem parametru (Način).
- **Mode:** Ovo govori mikrokontroleru kada treba pokrenuti Interrupt. Postoje 4 definirana načina. Oni su: **HIGH**, **LOW**, **RISING**, **FALLING**, **CHANGE**.

![LED-pushbutton-interrupt](https://user-images.githubusercontent.com/8695815/77940592-3744e680-72b9-11ea-9869-2fe7fca602d3.png)


1. Zadatak:

    Pokrenite [tinkercad](https://www.tinkercad.com/things/dyIVOteU7mx-pushbutton-led-interrupt-studenti) simulator te prilagodite kod tako na način da palite i gasite LED-icu preko Interrupta tritiskom na tipku.

## Timer Interrupt

Arduino Uno ima 3 timera: **Timer0**, **Timer1** i **Timer2**. Timer0 je već postavljen za generiranje interrupta jednom u milisekundi za ažuriranje brojača milisekundi ``millis()``. Budući da je to ono što tražimo, možemo iskoristiti Timer0 da generira interrupt za nas! Arduino timeri imaju brojne konfiguracijske registre. U našem primjeru postavit ćemo registar za usporedbu za Timer0 (ovaj registar je poznat kao ``OCR0A``) da generira još jedan interrupt negdje na sredini tog broja. Prilikom svakog *tick*-a, brojač vremena uspoređuje se s registrom za usporedbu, a kad se oni izjednače, generirat će se *Interrupt*.

![blink_led_example](https://user-images.githubusercontent.com/8695815/77923783-330dce80-72a3-11ea-8fdd-1ae68c8b626c.png)

1. Zadatak:

    Pokrenite [tinkercad](https://www.tinkercad.com/things/90DUKgIqxtd-led-blinking-studenti) simulator te omogućite LED-icama treperenje u različitim intervalima. 
    
    >HINT: Kreirajte klasu kojom ćete paliti i gasiti LED-ice.
    
2. Zadatak:

    Nakon toga, pokušajte kontrolirati treperenje LED-ica korištenjem **Timer Interrupta**. U nastavku je dan primjer korištenja Timer0 interrupta te implementacije ISR funkcije koja se poziva pri aktivaciji interrupta.


```arduino
// place in setup()
// Timer0 is already used for millis() - we'll just interrupt somewhere
// in the middle and call the "Compare A" function below
OCR0A = 0xAF;
TIMSK0 |= _BV(OCIE0A);
```

```arduino
// Interrupt is called once every millisecond
SIGNAL(TIMER0_COMPA_vect){
  unsigned long currentMillis = millis();

  led1.Update(currentMillis);
  led2.Update(currentMillis);
  led3.Update(currentMillis);
}
```

## Watchdog Timer - WDT

*Watchdog Timer* (WDT) je hardverski timer koji automatski generira resetiranje sustava ukoliko se dogodi pogreška u softveru ili hardveru. Watchdog Timer ima vlastiti oscilator koji radi na 1 MHz, te radi na naponu od 5V. Često se koristi za automatsko resetiranje Arduino-a koji ostane visiti zbog pogreške u softveru ili hardveru. Svi Arduino boardovi imaju hardver Watchdog Timer.

![Arduino-watchdog-timer](https://user-images.githubusercontent.com/8695815/77950600-0371bd00-72c9-11ea-86db-f19653c94bd5.png)


Glavni program obično ima petlju (*loop*) kroz koju stalno prolazi kroz obavljanje različitih funkcija. WDT se učitava s početnom vrijednošću većom od najgoreg vremenskog kašnjenja kroz glavnu programsku petlju. Svaki put kada prođe kroz glavnu petlju kôd resetira vremenski WDT-a. Ako se dogodi greška i glavni se program ne vrati na resetiranje vremena prije odbrojavanja na nulu, stvara se prekid radi resetiranja procesora. Korišten na ovaj način, zaštitni mjerač vremena može otkriti kvar na arduino programu bez nadzora i pokušati popraviti akciju s resetiranjem. Obično se nakon resetiranja može očitati i registar kako bi se utvrdilo je li WDT inicirao resetiranje ili je bio normalan reset. Na Arduinu se ovaj registar naziva *Watchdog Reset Flag Register (WDRF)*.

```arduino
wdt_enable(WDTO_2S);
```

WDT se može omogućiti s različitim postavkama vremena. Postavka vremena je vrijeme između resetiranja WDT-a i feeda. Vrijeme mora biti duže od vremena potrebnog da se petlja programa ponovno vrati. Može se postaviti najviše 8 sekundi i najmanje 15mSec. U nastavku je dana tablica koja vremena podržava WDT:


| THRESHOLD VALUE |CONSTANT NAME |SUPPORTED ON |
|----------|:-------------:|------:|
|15 ms | WDTO_15MS | ATMega 8, 168, 328, 1280, 2560 |
|30 ms | WDTO_30MS | ATMega 8, 168, 328, 1280, 2560 |
|60 ms | WDTO_60MS | ATMega 8, 168, 328, 1280, 2560 |
|120 ms | WDTO_120MS | ATMega 8, 168, 328, 1280, 2560 |
|250 ms | WDTO_250MS | ATMega 8, 168, 328, 1280, 2560 |
|500 ms | WDTO_500MS | ATMega 8, 168, 328, 1280, 2560 |
|1 s | WDTO_1S | ATMega 8, 168, 328, 1280, 2560 |
|2 s | WDTO_2S | ATMega 8, 168, 328, 1280, 2560 |
|4 s | WDTO_4S | ATMega 168, 328, 1280, 2560 |
|8 s | WDTO_8S | ATMega 168, 328, 1280, 2560 |

```arduino
wdt_reset();
```

Ova se funkcija mora pozvati na početku loop() petlje. S njom se poništava timer WDT-a. Ako petlja programa visi i ne može je resetirati, tada će WDT arduino i spriječiti problem s prekidom kontrolera zbog smetnji ili buke.

```arduino
#include <avr/wdt.h>
 
void setup(){
   //watchdog timer with 2 Seconds time out
   wdt_enable(WDTO_2S);
}
 
void loop(){
   //do stuff here
 
   wdt_reset();
}
```

1. Zadatak:

    Nadopunite Blink program koji je dan u nastavku sa podrškom za WDT te koristite ``WDTO_500MS``, odnosno 500 ms za reset.


```arduino
void setup() {
  // initialize digital pin LED_BUILTIN as an output.
  pinMode(LED_BUILTIN, OUTPUT);
}

// the loop function runs over and over again forever
void loop() {
  digitalWrite(LED_BUILTIN, HIGH);   // turn the LED on (HIGH is the voltage level)
  delay(1000);                       // wait for a second
  digitalWrite(LED_BUILTIN, LOW);    // turn the LED off by making the voltage LOW
  delay(1000);                       // wait for a second
}
```


## Reference

1. https://circuits4you.com/2018/01/24/tutorial-on-arduino-watchdog-timer-setup/
2. https://www.phippselectronics.com/introduction-arduino-interrupts/
3. https://circuitdigest.com/microcontroller-projects/arduino-interrupt-tutorial-with-examples
4. https://www.youtube.com/watch?v=YT3birSKLLU
5. https://www.youtube.com/watch?v=QtyOiTw0oQc
6. https://www.electronics-lab.com/project/using-interrupts-with-arduino/
7. https://microcontrollerslab.com/use-arduino-interrupts-examples/
8. https://learn.adafruit.com/multi-tasking-the-arduino-part-2/timers
9. https://www.allaboutcircuits.com/technical-articles/using-interrupts-on-arduino/
10. https://folk.uio.no/jeanra/Microelectronics/ArduinoWatchdog.html
11. https://www.instructables.com/id/Arduino-Pushing-the-Limits-Arduino-Timers/
12. https://forum.arduino.cc/index.php?topic=512462.0
13. https://embedds.com/using-watchdog-timer-in-your-projects/
14. https://arduino.stackexchange.com/questions/10230/arduino-watchdog-timer