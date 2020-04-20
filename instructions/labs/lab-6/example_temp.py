import serial
import json
import time
import random
import paho.mqtt.client as mqtt

BROKER = "mqtt.eclipse.org"
CLIENTID = "MQTTExample"
TOPIC = "A507/sensors/temperature"
QOS = 1

mqttc = mqtt.Client(CLIENTID)
mqttc.connect(BROKER)

while True:

	sensor_id = "Temperature"
	val = random.randint(10, 20)

	print("%s: %s" % (TOPIC, val))
	mqttc.publish(TOPIC, payload=val, qos=QOS, retain=False)
	time.sleep(5)

mqttc.disconnect()
time.sleep(1)