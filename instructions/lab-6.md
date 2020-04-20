# Bežične senzorske mreže - Lab 6

### FESB, smjer 110/111/112/114/120, akademska godina 2019/2020

## Što je MQTT protokol

**MQTT** je protokol za komunikaciju koji je razvijen u IBMu. Radi na principu **publish/subscribe** te je kreiran je za kao jednostavan protokol za upotrebu. Dizajniran je kako bi se minimizirala propusnost mreže i zahtjevi resursa uređaja. Jedna od bitnih značajki protokola su osiguranje pouzdanosti i određenog stupnja isporuke poruke. Zbog toga je postao jako rasprostranjen u primjeni i popularan. Najviše se koristi u IoT (eng. *Internet of Things* – Internet stvari) uređajima i M2M (eng. *Machine to Machine*) komunikaciji gdje je potreban protokol koji ne povećava potrošnju energije.

![Hive-MQ](https://user-images.githubusercontent.com/8695815/79661765-ae80e280-81b4-11ea-8c07-7c0b4c21246e.png)
izvor: [hivemq](https://www.hivemq.com/blog/mqtt-essentials-part2-publish-subscribe/)

## Publish/subscribe

Koncept *publish/subscribe* (poznat i kao ``pub/sub``) predstavlja alternativu tradicionalnoj klijent-server arhitekturi. U modelu klijent-sever klijent komunicira izravno s krajnjom točkom. S druge strane model ``pub/sub`` razdvaja klijenta koji šalje poruku (*publish*) od klijenta ili klijenata koji primaju poruke (*subscribe*). Izdavači (*publisher*) i pretplatnici (*subscriber*) nikada izravno ne komuniciraju. Veza između njih se upravlja trećom komponentom (brokerom). Zadatak brokera je filtrirati sve dolazne poruke i pravilno ih distribuirati pretplatnicima.

## Topic

MQTT koristi *topic* (temu) kako bi odredio koja poruka ide kojem klijentu (pretplatniku). Tema je hijerarhijski strukturiran niz koji se može koristiti za filtriranje i usmjeravanje poruka. Tema se sastoji od jedne ili više razina tema. Svaka razina teme odvojena tzv. znakom *slash*. U nastavku je dan primjer teme:

## Quality of Service - QoS

Kod MQTT protokola jedan od bitnijih faktora je *Quiality of Service* (**QoS**). QoS ugovor između pošiljatelja poruke i primatelja poruke koji definira garanciju isporuke za određenu poruku. Postoje 3 razine QoS u MQTT-u:

1. Najviše odjednom (0)
2. Barem jednom (1)
3. Točno jednom (2)

# Zadatak

U sklopu današnje vježbe student će korištenjem Python programskog okruženja slati senzorske podatke na MQTT broker te će se odgovarajućim klijentom pretplatiti na navedene podatke. Naime, u prethodnoj vježbi studenti su podatke od temperaturi i vlažnosti sa jednog senzora ispisivali na serijskom portu na računalu. Cilj današnje vježbe je navedene senzorske podatke koji su poslani putem serijskog porta na računalo prosljeđivati putem MQTT protokola prema brokeru koji će prosljeđivati te podatke studentu na mobitel ili na web aplikaciju na računalu.

### Uputstva za instalaciju i pokretanje aplikacije

Prije korištenja dolje navedenog kod provjerite je li instaliran Python na vašem računalu. Nadalje, za realizaciju ove vježbe potrebno je instalirati ``Mosquitto Python`` modul pod nazivom ``Paho MQTT client``, tako da jednostavno utipkate sljedeću naredbu u CLI (Command Line):

`pip install paho-mqtt`

Sada ćemo kreirati jednostavnu python skriptu koja će slati podatke sa serijskog porta prema javno distupnom brokeru. Otvorite text editor (npr. ``Visual Studio Code``), sačuvajte datoteku sa imenom koja ima ekstenziju `.py`, primjerice `sensor.py`. Nakon toga u datoteku kopirajte ispod naveden kod:

```python
import serial
import paho.mqtt.client as mqtt

BROKER = "mqtt.eclipse.org"
CLIENTID = "MQTTExample"
TOPIC_ONE = "A507/sensors/temperature"
TOPIC_TWO = "A507/sensors/humidity"
COMPORT = "COM9" # please replace this with your port number
QOS = 1

import time

mqttc = mqtt.Client(CLIENTID)
mqttc.connect(BROKER)

ser = serial.Serial(COMPORT, 115200, timeout=5) 
while True:
	message = ser.readline()
	print (message)
	if "T:" in message:
		string, temp = message.split(" ")
		mqttc.publish(TOPIC_ONE, payload=temp, qos=QOS, retain=False)
	if "H:" in message:
		string, hum = message.split(" ")
		mqttc.publish(TOPIC_TWO, payload=hum, qos=QOS, retain=False)
	time.sleep(0.01)
mqttc.disconnect()
time.sleep(1)
```

Primjetite da bi se poruka uspješno poslala prema brokeru, ona putem serijskog porta mora biti poslana u formatu npr. **"T: 25.3"** ili **"H: 33"**. Prema tome, prilagodite kod na Arduino Receiveru kako bi slao podatke prema serijskom portu u navedenom formatu.

Nakon što su podaci uspješno poslani prema MQTT brokeru, potrebno je iste vizualizirati. Za realizaciju navedenog primjera, možete instalirati aplikaciju [mqtt-explorer](https://mqtt-explorer.com/). 