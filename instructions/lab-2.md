# Bežične senzorske mreže - Lab 2

### FESB, smjer 110/111/112/114/120, akademska godina 2019/2020

Low-level programiranje za ugradbene sustave je sasvim drugačije od programiranja za uređaje opće namjene, kao što su računala i mobiteli. Učinkovitost (u smislu brzine i prostora) je daleko važnije, jer su resursi na visokoj cijeni. Drugim rječima, veoma bitan naglasak se stavlja na **optimiziaciju** dijelova koda. Ova vježba je napravljena prema uputama na stranicama [Adafruit](https://learn.adafruit.com/memories-of-an-arduino/solving-memory-problems).

## Mikrokontroleri

Mikrokontroleri poput onih koji pokreću Arduino su dizajnirani za ugradbene aplikacije. Za razliku od računala opće namjene, ugradbeni procesor obično ima dobro definiran zadatak koji mora pouzdano i učinkovito obavljati - i uz minimalne troškove.

Najveća razlika između mikrokontrolera kojeg koristite na vašim vježbama i računala opće namjene je količina dostupne memorije. Arduino UNO ima samo 32KB flash memorije i 2KB SRAM-a što je otprilike **100.000 puta** manje od fizičke memorije od PC-a! (ne uključujući memoriju vanjskog diska)

Ponekad se može dogoditi da Arduino bez ikakvih razloga "poludi" ili se resetira sam po sebi, iako je kod 100% točan (uredno se kompajlira). U takvim slučajevima, jedan od mogućih uzroka je nedostatak slobodnog RAM-a (*Random Access Memory*). Drugim riječima, vaš mikrokontroler nema dovoljno slobodnog RAM-a za obavljanje traženog zadatka.

## Memorija Arduino mikrokontrolera

Memorija Arduina se sastoji od tri dijela:
 - Flash ili programska memorija
 - SRAM
 - EEPROM

<!---![learn_arduino_stack_operation](https://cloud.githubusercontent.com/assets/8695815/23830029/cf1517aa-0700-11e7-8b63-5432481523f4.gif)
-->
<img src="https://cloud.githubusercontent.com/assets/8695815/23830029/cf1517aa-0700-11e7-8b63-5432481523f4.gif" width="600px" height="600px" />

## SRAM

SRAM ili *Static Random Access Memory* je tip memorije u koji se može čitati i pisati prilikom izvršavanja programa. SRAM memorija ima višestruku ulogu prilikom izvršavanja programa:
 - **Statički Podaci** - Ovaj blok memorije SRAM-a je rezerviran prostor za sve globalne i statičke varijable iz svog programa.
 - **Heap** - koristi se za dinamički alocirane podatke kao što su ``malloc``.
 - **Stack** - upotrebljava se za lokalne varijable i za održavanje evidencije prekida (*interrupts*) i poziva funkcija. *Stack* raste od vrha memorije dolje prema dnu (prema *Heap-u*). Svaki interrupt, poziv funkcije i/ili lokalna varijabla uzrokuje rast Stack-a. Po povratku iz interrupta ili poziva funkcije oslobodit će se dio memorije koji taj interrupt ili funkcija upotrebljava.

## Usporedba Arduino memorije

U sljedećoj tablici možete vidjeti kapacitete memorije za nekoliko popularnih Arduino i Arduino kompatibilnih uređaja.

![learn_arduino_arduinochart](https://cloud.githubusercontent.com/assets/8695815/23830043/2fd4be2e-0701-11e7-875f-32ff3a364fd2.jpg)

## Mjerenje memorije Arduina

Jedan od načina dijagnoze problema memorije je detekcija zauzeća memorije.

### Flash memorija

Kompajler prilikom kompajliranja koda za vas izračuna zauzeće flash memorije tako da već unaprijed možete znati je li kod prevelik za ``upload`` na Arduino.

### SRAM memorija

Korištenje SRAM memorije je dinamičnije i stoga ju je (malo) teže mjeriti. ``free_ram()`` funkcija dana u nastavku je jedan od načina na koji možete to ralizirati. Korištenje SRAM-a je dinamično i s vremenom će se mijenjati. Dakle, važno je zvati ``free_ram()`` u različitim vremenima i iz različitih djelova vašeg koda da biste vidjeli kako se mijenja tijekom vremena izvršavanja koda.

```arduino
int freeRam() 
{
  extern int __heap_start, *__brkval; 
  int v; 
  return (int) &v - (__brkval == 0 ? (int) &__heap_start : (int) __brkval); 
}
```

U osnovi, funkcija ``free_ram()`` zapravo daje informaciju o slobodnom prostoru između Stack-a i Heap-a. Upravo taj prostor zaista treba pratiti ako pokušavate izbjeći rušenje programa.

## Optimizacija memorije Arduino programa

Prilikom kompajliranja svog programa, sam IDE će vam reći koliko je program velik (npr. PlatformIO). Ako ste dosegli ili prešli raspoloživi prostor, primjena nekih optimizacijskih mehanizama ga može vratiti unutar granica.

### Uklonite nepotrebne (mrtve) dijelove koda

Može se dogoditi da je rezultirajući kod nastao kao kombinacija koda iz više izvora, pa se lako može dogoditi da su dijelovi koda nekorišteni te ih se može ukloniti:
 - Nekorištene biblioteke
 - Nekorištene funkcije
 - Nekorištene varijable
 - Nedostupni dijelovi koda
 
## Optimizacija SRAM-a

Postoje razni načini na koji možete smanjiti korištenje SRAM memorije. Ovo su samo neki od njih.
### Eliminirajte nekorištene varijable

Može se ponekad dogoditi da se neke varijable uopće ne koriste pa ih jednostavno izbrišite.

### Smanjite nepotrebnu veličinu varijabli

Ne koristite *float* kada je *int* dovoljan. Nemojte koristiti *int* kada je *byte* dovoljan. Pokušajte koristiti najmanji tip podataka koji može pohraniti informaciju.

![learn_arduino_datatypes](https://cloud.githubusercontent.com/assets/8695815/23830238/5eeaa534-0706-11e7-9eae-bfd397b6aed7.jpg)

### Upotreba lokalnih varijabli

Globalne i statičke varijable se prve učitaju u SRAM te one guraju Heap gore prema Stacku. Ukoliko je moguće, incijalizirajte lokalne varijable unutar funkcija. Naime, prilikom izvršavanja funkcije alocira se dio Stack memorije. Unutar te memorije će biti sadržani:
 - svi parametri koji su dodijeljeni funkciji,
 - sve lokalne varijable koje su deklarirane u funkciji.  

Ovi se podaci koriste u funkciji te se prilikom izlaska iz funkcije dio memorije Stacka kojeg je funkcija koristila u potpunosti oslobađa!

### Upotreba PROGMEM za const podatke

U mnogim slučajevima, velika količina RAM-a je preuzeta od strane statičke memorije kao rezultat korištenja globalne varijable (kao što su *Strings* ili *Int*). Kada znate da se vrlo vjerojatno varijabla neće promijeniti, ona se jednostavno može pohraniti u tzv. PROGMEM (programsku memoriju). Kao jednostavan primjer je upotreba ``F()`` makro-a koji kaže kompajleru da se String pohrani u PROGMEM-u. 

Npr. ako zamijenite:

```arduino
Serial.println("Sram sram sram sram. Lovely sram! Wonderful sram! Sram sra-a-a-a-a-am sram sra-a-a-a-a-am sram. Lovely sram! Lovely sram! Lovely sram! Lovely sram! Lovely sram! Sram sram sram sram!");
```

sa 

```arduino
Serial.println(F("Sram sram sram sram. Lovely sram! Wonderful sram! Sram sra-a-a-a-a-am sram sra-a-a-a-a-am sram. Lovely sram! Lovely sram! Lovely sram! Lovely sram! Lovely sram! Sram sram sram sram!"));
```

sačuvat ćete 180 byte-ova SRAM memorije.

### Ne koristite rekurzivne funkcije

Na sljedećem primjeru ćete moći vidjeti jednostavno kako možete korištenjem rekurzivne funkcije napuniti Stack memoriju. Što mislite bi se trebalo dogoditi?

```arduino
// Pin 13 has an LED connected on most Arduino boards.
// give it a name:
int led = 13;
uint16_t a = 2;

#define DELAY 1000 // wait for a predefined delay (in milliseconds)

void powerUp();
void powerDown();
int freeRam();
void incVar();

// the setup routine runs once when you press reset:
void setup() {
  // initialize serial communication at 9600 bits per second:
  Serial.begin(9600);
  Serial.println("Program begins here!");
  // initialize the digital pin as an output.
  pinMode(led, OUTPUT);
}

// the loop routine runs over and over again forever:
void loop() {
  powerUp();
}

void powerUp() {
  Serial.print("Free SRAM: ");
  Serial.print(freeRam());
  Serial.println(" bytes");
  incVar();
  digitalWrite(led, HIGH);   // turn the LED on (HIGH is the voltage level)
  delay(DELAY);
  powerDown();
};

void powerDown() {
  Serial.print("Free SRAM: ");
  Serial.print(freeRam());
  Serial.print(" bytes");
  incVar();
  digitalWrite(led, LOW);    // turn the LED off by making the voltage LOW
  delay(DELAY);
  powerUp();

};

int freeRam ()
{
  extern int __heap_start, *__brkval;
  int v;
  return (int) &v - (__brkval == 0 ? (int) &__heap_start : (int) __brkval);
}

void incVar() {
  a = a + 2;
  Serial.print("a + 2 = ");
  Serial.println(a);
};
```

Ovu vježbu možete testirati preko servisa [tinkercad](www.tinkercad.com) u kojem možete razvijati i simulirati različite Arduino aplikacije.

### Zadatak 1

*Zatražite od profesora link na tinkercad simulaciju koji upotrebljava navedenu skriptu te komentirajte što se dogodilo.*

## Korištenje EEPROM-a

EEPROM je zgodan prostor za pohranu koji se može upotrebljavati za pohranu podataka kao što su kalibracija ili nekakve konstante ugađanja koji nisu praktične za pohranu u Flash memoriju. Za korištenje EEPROM-a morate uključiti biblioteku ``EEPROM.h``.

```arduino
#include <EEPROM.h>

// pin definitions
int buttonPin = 2;

// global variables
int lastButtonState = 1;
int counter;

void setup () {
 // setup pin modes
 pinMode(ledPin, OUTPUT);
 pinMode(buttonPin, INPUT_PULLUP);

 // initialise Serial port
 Serial.begin(9600);

 //assign counter the value of address 0
 counter = EEPROM.read(0);

 //write a 0 to address 0. This allows for consecutive resets to reset the counter
 //EEPROM.write(0,0);

}

void loop() {
 int buttonState = digitalRead(buttonPin);
 if (buttonState > 0){
      counter ++;
      EEPROM.write(0, counter);
      Serial.print("Counter: ");
      Serial.println(counter);
 }
 while (digitalRead(buttonPin) == 1){}
 buttonState = 0;
}
```


## Zadatak

1) Pitajte profesora za [tinkercad](www.tinkercad.com) simulaciju. *Vaš zadatak je razumijeti rad EEPROM-a testiranjem navedenog koda. Što mislite će se dogoditi sa varijablom ``counter`` ako resetiramo Arduino?*

2) *Nodogradite postojeći kod na način da omogućite promjenu stanja LED-ice pritiskom na tipku.*



# Zadatak

Skinite [``TempHumLight.ino``](https://github.com/toperkov/WiSe-2019-20/tree/master/instructions/labs/lab-2) datoteku koja se nalazi na GitHub repozitoriju te je stavite u ``scr`` direktorij u kojemu ćete raditi današnju vježbu (npr. ``C:\Users\A507\WiSe\toperkov\lab2\src``). Kao što ćete vidjeti, navedena skripta čita temperaturu i vlagu senzora (DHT22 u vašem primjeru) te razinu osvjetljenja (pomoću senzora BH1750). Prije pokretanja/kompajliranja skripte potrebno je instalirati biblioteke za DHT, Adafruit Sensor (već imate ove dvije instalirane iz prethodne vježbe) i BH1750.

<!---! Da biste to realizirali, u PlatformIO-u te upišite sljedeću naredbu:
``platformio lib search DHT``
Nakon toga će vam se prikazati popis biblioteka koje možete instalirati. Instalirajte biblioteku pod rednim brojem ``[19]`` tako da ćete utipkati:  
``platformio lib -g install 19`` (pod brojem 19 je Adafruit biblioteka za DHT senzor)  
Sličnu stvar ponovite i za senzor BH1750:  
``platformio lib search BH1750``  
``platformio lib -g install 439`` (redni broj biblioteke za senzor BH1750) 

Nakon toga povežite DHT i BH1750 senzore kako je prikazano na slici te testirajte navedeni kod -->

<!---![dht11bh1750_bb](https://cloud.githubusercontent.com/assets/8695815/23832209/8d2b52d4-0730-11e7-8dfc-4aa61bff21b7.png))
-->
<img src="https://cloud.githubusercontent.com/assets/8695815/23832209/8d2b52d4-0730-11e7-8dfc-4aa61bff21b7.png" width="400px" height="400px" />

*Vaš zadatak je da razumijete kod te ga optimizirate na način da zauzima što manje SRAM-a korištenjem gore navedenih uputa.*