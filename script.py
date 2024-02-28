import paho.mqtt.client as mqtt

# MQTT broker details
broker_address = "broker.hivemq.com"
broker_port = 1883
topic = "sidhant/task"


# Callback function to handle connection established
def on_connect(client, userdata, flags, rc):
    print("Connected with result code " + str(rc))
    # Subscribe to the topic
    client.subscribe(topic)


# Callback function to handle received message
def on_message(client, userdata, msg):
    print("Received message: " + msg.payload.decode())


# Create an MQTT client instance
client = mqtt.Client()

# Assign callbacks
client.on_connect = on_connect
client.on_message = on_message

# Connect to the MQTT broker
client.connect(broker_address, broker_port, 60)

# Loop to maintain connection and process messages
client.loop_forever()
