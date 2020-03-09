![arduinoblinkgif](https://cloud.githubusercontent.com/assets/8695815/23480241/8e5f41e0-fec7-11e6-8439-0f596aec5f54.gif)


# Bežične senzorske mreže - Lab 1

### FESB, smjer 110/111/112/114/120, akademska godina 2018/2019

Cilj ove vježbe je upoznavanje studenta sa PlatformIO IDE programskim okruženjem, kao i Arduino mikrokontroler platformom. Student će u sklopu današnje vježbe realizirati nekoliko osnovnih primjera te ih testirati na Arduino platformi. Od hardvera će nam biti potrebni:
- [Arduino Uno](https://www.adafruit.com/products/50)
- [USB A-B kabel](https://www.adafruit.com/products/62)
- Zelena, crvena ili plava LED-ica

## Upute za spajanje komponenti

1. Da biste realizirali prvi projekt jednostavno spojite komponente kako su navedene na slici. Primjetit ćete da je kod LED diode jedna žica dulja od druge. Upravo ta dulja žica treba biti povezana na PIN 13 kao što je prikazano na slici, dok je kraća žica povezana na GND (eng. Ground) PIN.

![arduinounomali](https://cloud.githubusercontent.com/assets/8695815/23481290/3e0e4fd4-fecb-11e6-8d90-3b66f4cdfd7a.PNG)

Primjer Arduino simulatora možete naći na [Autodesk Circuits module](https://circuits.io/circuits/4142024-simple-blink) stranici na kojoj možete kreirati jednostavne diagrame za vaše Arduino projekte.

## Kreiranje prvog projekta u PlatformIO IDE okruženju

1. Da biste kreirali novi projekt u PlatformIO IDE-u kliknite na Menu: ``PlatformIO > New Project``

2. Nakon toga će vam se pojaviti prozor u kojem ćete trebati odabrati platformu s kojom ćete raditi (možete ih više odabrati). Klikom na ``Board`` odeberite `Arduino Uno` platformu. Ispod toga će vam ponuditi ``Framework``, te će odabrati ``Arduino``. Nakon toga odaberite direktorij u kojem ćete sačuvati projekt (npr. 'C:/Users/Student/Desktop/Ivan/Blink') te kliknite na `Finish`. Ukoliko je sve bilo uspješno, trebao bi se u lijevom dijelu prozora pojaviti panel sa direktorijima i dokumentima.

3. Nakon toga ćete kreirati vaš prvi projekt. Desnim klikom na `scr` direktorij kliknite na `New File` te ga nazovite npr. `Blink.cpp`. U njega kopirajte sljedeći kod:

```arduino
#include <Arduino.h>

// Pin 13 has an LED connected on most Arduino boards.
// give it a name:
int led = 13;

// the setup routine runs once when you press reset:
void setup() {
  // initialize the digital pin as an output.
  pinMode(led, OUTPUT);
}

// the loop routine runs over and over again forever:
void loop() {
  digitalWrite(led, HIGH);   // turn the LED on (HIGH is the voltage level)
  delay(1000);               // wait for a second
  digitalWrite(led, LOW);    // turn the LED off by making the voltage LOW
  delay(1000);               // wait for a second
}
```

4. Da biste provjerili ispravnost koda kliknite na `Build`. Ukoliko je sve ispravno, trebao bi se pojaviti *success* prikazan zelenom bojom u Build panelu.

5. Nakon toga, klikom na `Upload` učitajte firmware na Arduino Uno.

## Kreiranje drugog projekta u PlatformIO IDE okruženju

U sklopu ove vježbe ćete realizirati očitavanje svjetla iz prostorije korištenjem Arduina i fotodioide koje ća vam pripremiti profesori.

1. Za realizaciju ove vježbe ćete spojiti fotodiodu kao što je prikazano na slici ispod. Primjetite da je jedan pin fotodiode povezan na Analogni ulaz `A0`, dok je drugi pin spojen na `GND`.

![photodiode](https://user-images.githubusercontent.com/8695815/54277548-73af8280-4590-11e9-9749-f9f810062f31.jpg)


2. Slično kao u prethodnom primjeru kreirajte novi projekt te kreirajte novu datoteku pod nazivom npr. `AnalogRead.cpp`. Za realizaciju vježbe ćemo koristiti već postojeći kod [AnalogReadSerial](https://www.arduino.cc/en/Tutorial/AnalogReadSerial). Jedino preinaku koju smo napravili je brzina čitanja/slanja podataka na serijski port te `delay` između dva čitanja (10 ms).

```arduino
/*
  AnalogReadSerial

  Reads an analog input on pin 0, prints the result to the Serial Monitor.
  Graphical representation is available using Serial Plotter (Tools > Serial Plotter menu).
  Attach the center pin of a potentiometer to pin A0, and the outside pins to +5V and ground.

  This example code is in the public domain.

  http://www.arduino.cc/en/Tutorial/AnalogReadSerial
*/

#include <Arduino.h>

// the setup routine runs once when you press reset:
void setup() {
  // initialize serial communication at 115200 bits per second:
  Serial.begin(115200);
}

// the loop routine runs over and over again forever:
void loop() {
  // read the input on analog pin 0:
  int sensorValue = analogRead(A0);
  // print out the value you read:
  Serial.println(sensorValue);
  delay(10);        // delay in between reads for stability
}
```

3. Nakon toga, klikom na `Upload` učitajte firmware na Arduino Uno. U datoteci `platformio.ini` postavite ``Baud rate`` na ``115200`` tako da upišete ``monitor_speed = 115200``.

3. Klikom ``Serial Monitor`` će vam se početi ispisivati vrijednost na Serijski port od Arduino Uno-a. Vaš zadatak je da testirate rad fotodiode u uvjetima različitog osvjetljenja te vidite kako se to reflektira na rad senzora.

## Kreiranje trećeg projekta u PlatformIO IDE okruženju - očitavanje temperature i vlažnosti preko DHT22 senzora

Uspješnom realizacijom ove vježbe ćete moći očitavati temperaturu i vlažnost na serijskom portu vašeg Arduino uređaja. Za realizaciju ove vježbe vam je potreban DHT22 senzor kojeg ćete povezati kako je to prikazano na slici ispod.

![DHT22](https://user-images.githubusercontent.com/8695815/54339289-b0cd5080-4634-11e9-898d-3fa84a18f2ff.png)


Prije nego što spojite Arduino na računalo, potrebno je instalirati biblioteke koje podržavaju rad senzora. Dakle, nakon što ste kreirali novi projekt kreirali (praznu) datoteku npr. ``TempHumSensor.cpp``, potrebno je instalirati biblioteke koje su nam potrebne za rad sa DHT22 senzorom. Da biste to realizirali, pratite korake koji su navedeni na slikama ispod.

Klikom na ``PIO Home`` će vam se pojaviti prozor kao što je prikazano na slici. Nakon toga kliknite na ikonu ``Libraries`` (Korak 1). U prozoru za pretraživanje upišite ``DHT`` (Korak 2) te pritisnite ikonu za pretraživanje. Nakon rezultata pretrage će vam se pojaviti ``DHT Sensor library`` te kliknite na njega (Korak 3).

![DHT_install](https://user-images.githubusercontent.com/8695815/76229726-ee9e8e00-6222-11ea-836a-7582c357de11.png)


Nakon toga kliknite na ikonu install (Korak 4) da biste instalirali biblioteku. U nastavku ispod možete naći primjer rada sa DHT22 senzorom (Korak 5). Kopirajte taj kod te ga pohranite u datoteci vašeg prethodno kreiranog projekta (npr. ``TempHumSensor.cpp``). 

![DHT_install2](https://user-images.githubusercontent.com/8695815/54339324-cb9fc500-4634-11e9-92af-fd75dace3985.png)


Nakon što ste instalirali ``DHT Sensor library``, ponovite isti postupak za instalaciju biblioteke ``Adafruit Unified Sensor``. Ukoliko ste sve korake uspješno odradili, trebali biste moći učitati kod u Arduino. Nakon toga otvorite ``Serial Monitor`` te komentirajte što vidite.