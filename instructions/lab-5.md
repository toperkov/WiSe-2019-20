# Bežične senzorske mreže - Lab 5

### FESB, smjer 110/111/112/114/120, akademska godina 2019/2020

## Kako štediti bateriju?

![low_energy](https://media.giphy.com/media/3ohhwGIssJMuuEUqxq/giphy.gif)

Bežični senzorski uređaji su u suštini mali, baterijski napajani senzorski čvorovi kojima je potrebno osigurati autonomiju rada, bez zamjene baterija i to u trajanju što duljeg vremenskog perioda. Ovakav zahtjev predstavlja poseban izazov s obzirom da krajnji senzorski čvorovi nisu opremljeni ikakvim sustavom za punjenje baterija, ponajviše jer je cilj smanjiti njihovu kompleksnost i veličinu, a time povećati njihovu pouzdanost.

U sklopu danjašnje vježbe student će naučiti na koji način može smanjiti potrošnju bežičnog senzorskog uređaja s ciljem uštede kapaciteta baterije, a time i osiguranja autonomije baterijski napajanog senzorskog čvora.

## Baterija

Kapacitet baterije se mjeri u miliampersatima (*mAh*), a to je mjera koja nam u osnovi kaže koliko sati baterija može kontinuirano davati struje. Primjerice, baterija od *800 mAh* teoretski može dati *800 mA* u periodu od jednog sata. Samim time, ako poznajete prosječnu potrošnju vašeg senzorskog čvora, možete procijeniti prosječno trajanje života vašeg uređaja jednostavnim računanjem sljedeće jednadžbe:

```
Kapacitet baterije (u mAh) / Prosječna potrošnja struje (u mA) = Prosječno trajanje života u satima
```

Primjerice, ako je prosječna potrošnja vašeg senzora *150 mA* dok baterija ima kapacitet od *800 mAh* jednostavnom računicom dolazimo do sljedećeg rezultata:

```
800 mAh / 150 mA = 5.33 sati 
```

Međutim, ukoliko želimo da naš senzorski uređaj ima autonomiju rada od primjerice **godine dana**, gore navedene pretpostavke za prosječnu potrošnju kao i za kapacitet baterije nam ne idu u prilog. Najjednostavniji način za postizanje autonomije rada senzorskog uređaja bi bilo povećanja kapaciteta baterije. Međutim, da bismo to realizirali, trebala bi nam baterija od otprilike **1500 Ah** (za usporedbu, **akumulatori u automobilu imaju kapacitet od 120 Ah**), što ergonomski nije moguće. Prema tome, potrebno je poraditi i na smanjenju potrošnje senzorskog uređaja.

Ako primjetite, senzorski čvor većinu svog vremena ne radi ništa. Samim time, ako se u periodu neaktivnosti senzorskog čvora ne mjere nikakva stanja senzora i na šalju podaci preko radio kanala, tada je senzorski čvor u osnovi neaktivan, te je potrebno u tom periodu maksimalno smanjiti njegovu potrošnju. Također, ako želimo smanjiti ukupnu veličinu senzorskog uređaja, potrebno je voditi računa o veličini, a samim time i kapacitetu baterije. U našem primjeru možemo govoriti o senzorskom uređaju čiji **kapacitet baterije ne prelazi otprilike 3000 mAh**. Ukoliko želimo osigurati autonomiju rada od primjerice godine dana, tada potrošnja uređaja treba biti u razini **nekoliko uA** u periodu neaktivnosti.

Na navedenom linku možete procijeniti trajanje života baterije ako unesete parametre o potrošnji u periodu aktivnosti i neaktivnosti uređaja. Procjenite kolike nam vrijednosti o potrošnji trebaju da bi omogućili bateriji od *3000 mAh* autonomiju rada od godine dana. Pri tome možete uzeti u obzir da uređaj radi mjerenje jednom u **10 minuta**.

http://oregonembedded.com/batterycalc.htm

Ubacite u vaš Arduino Uno R3 sljedeći kod i provjerite kolika je potrošnja Arduina:

```arduino
void setup () { }
void loop () { }
```

te ga usporedite sa potrošnjom u scenariju kada imate ovakav kod (**NAPOMENA:** potrebno je instalirati ``Low-Power`` biblioteku od Rocketscream-a):

```arduino
#include <LowPower.h>
void setup() {}
void loop() { LowPower.powerDown(SLEEP_FOREVER, ADC_OFF, BOD_OFF); }
```

Koji su najveći potrošači baterije? To su redom **mikrokontroleri**, **radio primopredajnik** te u manjoj mjeri **memorija i senzori**. Redom ćemo pokazati na primjerima kako jednostavnim intervencijama možemo smanjiti potrošnju senzorskog čvora. 

## Optimizacija potrošnje senzora


<img src="https://cloud.githubusercontent.com/assets/8695815/24080434/691c28cc-0c9f-11e7-8945-a862b272fff7.png" width="400px" height="400px" />

<!---![dht11bh1750_bb](https://cloud.githubusercontent.com/assets/8695815/24080414/ff06e742-0c9e-11e7-8ba6-984ff94f51fb.png)
-->

U primjeru s prošle vježbe, cilj je napraviti takvu senzorsku mrežu koja će mjeriti temperaturu/vlagu i osvjetljenje svako par minuta, dok će ostatak vremena između dva očitanja biti neaktivna. Pri tome smo upotrebljavali **DHT11/22** senzor za očitanje temperature/vlage, te smo koristili senzor **BH1750** za očitanje razine osvjetljenja. Međutim, ako pogledamo detaljnije datasheet naših senzora, primjetit ćete da senzori DHT11/22 u tzv. idle mode-u (kada ne radi očitanja) troši otprilike **50uA**, što je ipak previše za naš senzorski uređaj. Najjednostavniji način smanjenja potrošnje takvog senzora bi bilo **isključivanje** sa napajanja u periodu neaktivnosti senzora. To možemo napraviti jednostavno da se senzor napaja sa jednog od **digitalnih pin-ova** u periodu aktivnosti, te u periodu neaktivnosti softverski isključimo digitalni pin te mu uskratimo napajanje. Također, u periodu aktivnosti potrošnja takvog senzora je otprilike 1,5 mA. Kod BH1750 senzora potrošnja u periodu neaktivnosti je 1.0 uA što je čak povoljno za naš scenarij pa ga ne treba spajati na digitalni pin. Prema slici spojite senzore te testirajte kod koji je dan u prilogu ove vježbe. Primjetite da se kod razlikuje jedino u tome što prije početka mjerenja palimo digitalni pin:

```arduino
digitalWrite(4, HIGH);
```

## Optimizacija potrošnje mikrokontrolera

Na slici se nalazi tablica od koja govori o potrošnji **ATmega328P** procesora u ovisnosti o različitim modovima spavanja (koji pokreće Arduino Uno R3 i Pro Mini). Uočite da različiti modovi spavanja onemogućavaju razne interne satove i komponente kako bi se smanjila potrošnja energije. Također, interesira nas na koji način možemo probuditi procesor iz stanja mirovanja, kao što je putem **interrupta** ili **timer-a**. Na sljedećem linku možete naći detaljnnije objašnjenje koje komponente procesora možemo izgasiti da bi maksimalno smanjili potrošnju procesora: http://www.gammon.com.au/power

<img src="https://cloud.githubusercontent.com/assets/8695815/24080623/b15fb0e6-0ca3-11e7-8ca8-376df9508988.png" width="600px" />

U našem slučaju ćemo koristiti najnižu potrošnju energije u stanje mirovanja, tzv. *power-down mode*. Za potrebe izrade današnje vježbe ćemo koristiti *Lightweight low-Power* biblioteku za Arduino od Rocketscream-a koja podržava *power-down mode* pa je instalirajte u PlatformIO. (HINT: ``platformio lib search Low-Power`` za traženje biblioteke)

Kao što je detaljnije navedeno na linku http://www.gammon.com.au/power, najveći potrošač uz stavljanje procesora u power-down node je i omogućen ADC (*Analog to Digital Conversion*) te *brown-out* (BOD) funkcionalnost/detekcija. Jednostavnim pozivom naredbe u Arduinu isključujemo ADC i BOD te stavljamo procesor u power-down mode.

```arduino
LowPower.powerDown(SLEEP_FOREVER, ADC_OFF, BOD_OFF)
```

U ovom načinu rada gotovo sve komponente procesora su onemogućene, dok samo vanjski *interrupt* ili *timer* (kao što je watchdog timer ili *Real Time Clock - RTC*) mogu probuditi procesor. U sklopu današnje vježbe će se upotrebljavati *watchdog timer* za buđenje s *power-down sleep* stanja. *Watchdog timer* je komponenta koja se nalazi unutar AVR procesora koji resetira procesor ako se otkrije da je zaustavljena normalna egzekucija programa. Obično se *watchdog* koristi da bi se nepouzdani program ili program sa dosta grešaka izvodio s više stabilnosti. Međutim, moguće je koristiti *watchdog timer* za buđenje procesora iz *power-down sleep* stanja, umjesto da ga se koristi za resetiranje.

<img src="https://cloud.githubusercontent.com/assets/8695815/24080639/10eaa994-0ca4-11e7-8ec0-106f41727389.png" width="600px" />

Na slici možete primjetiti da postoje različiti periodi unutar kojih *watchdog timer* može probuditi naš procesor. Ako želimo što više držati procesor u *power-down modu*, tada on preko watchdog timer-a može biti maksimalno 8 s. Jednostavnom modifikacijom gore navedene naredbe postavljamo procesor u power-down mode na period od 8 sekundi.

```arduino
LowPower.powerDown(SLEEP_8S, ADC_OFF, BOD_OFF)
```

Vaš zadatak je da modificirate navedenu skriptu danu u vježbi na način da joj omogućite korištenjem gore navedenih uputa očitanje DHT11/22 i BH1750 senzora svako jednu minutu uz što manju potrošnju energije.

U nastavku su uz današnje upute dane još neke upute koje dodatno mogu smanjiti ukupnu potrošnju senzorskog čvora, a koje nažalost nećemo u sklopu vježbe pokriti:

<img src="https://cloud.githubusercontent.com/assets/8695815/24080649/330628be-0ca4-11e7-9428-91578d7aa339.jpg" width="600px" />

 - Pokretanje procesora na nižoj frekvenciji
 - Pokretanje procesor na nižem naponu
 - Isključite nepotrebne interne module u softvare-u (npr. SPI, I2C, serijski, ADC)
 - Isključite otkrivanje redukcije struje (BOD)
 - Isključite analogno-digitalne pretvarač (ADC)
 - Isključite watchdog timer
 - Stavite procesor u spavanje
 - Ne koristite neučinkovite regulatore napona - ako je moguće pokrenite ih izravno iz baterije
 - Nemojte koristiti indikatore/ekrane (LED indikator, LCD ekran)
 - Probudite procesor iz sna samo kada je to potrebno
 - Isključivanje (s MOSFET-om) vanjskih uređaja (npr. SD kartice, senzori temperature)

**ZAKLJUČAK:** Korištenjem navedenih uputa moguće je veoma jednostavno optimizirati potrošnju vašeg Arduino uređaja. Najveći potrošaći uz mikrokontroler su LED-ice, regulatori napona, ADC i BOD. Čak je i watchdog timer ponekad potrebno isključiti i zamijeniti ga sa preciznijim satovima, kao što je RTC (npr. DS3232), čija je potrošnja znano manja od watchdog-a. Uz to, moguće je koristiti i Arduino uređaje koji rade na manjem naponu (Arduino Pro Mini verzija 3V3), pa regulatori napona u tom slučaju nisu ni potrebni. Postoje i verzije Arduina koje rade i na manjoj frekveniciji (upravo spomenuti Arduino Pro Mini na 3V3 radi na frekvenciji od 8 MHz) pa i time dodatno dobivamo na kapacitetu baterije.

## Zadatak

Zadatak se sastoji od dva dijela.
1. U prvom dijelu zadatka prilagodite kod za čitanje temperature, vlažnosti i osvjetljenja kojeg ćete zatražiti od profesora na način da se senzorski čvor budi sa periodom od 8 sekundi korištenjem watchdog timera.
2. U drugom dijelu zadatka, prilagodite kod na način da senzorski čvor sada stavljate u *power-down* mode te ga budite putem vanjskog interrupta.

## Korisni linkovi:

[1] http://oregonembedded.com/batterycalc.htm  
[2] http://www.microchip.com/forums/m672837.aspx  
[3] https://github.com/rocketscream/Low-Power  
[4] http://www.gammon.com.au/power  
[5] http://www.home-automation-community.com/arduino-low-power-how-to-run-atmega328p-for-a-year-on-coin-cell-battery/  
[6] http://www.elechouse.com/elechouse/images/product/Digital%20light%20Sensor/bh1750fvi-e.pdf  
[7] https://www.sparkfun.com/datasheets/Sensors/Temperature/DHT22.pdf  
[8] https://learn.adafruit.com/low-power-wifi-datalogging/overview  
[9] https://edwardmallon.wordpress.com/2014/05/21/using-a-cheap-3-ds3231-rtc-at24c32-eeprom-from-ebay/  
[10] https://www.youtube.com/watch?v=urLSDi7SD8M  
[11] https://www.youtube.com/watch?v=iMC6eG24S9g  