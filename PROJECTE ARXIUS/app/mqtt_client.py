import ssl
import paho.mqtt.client as mqtt
import json

AWS_IOT_ENDPOINT = "a1k1syidbqlxng-ats.iot.us-east-1.amazonaws.com"
CLIENT_ID = "client_1"
PATH_TO_CERTIFICATE = "certificats/d8eb1dca03956f52a96d9a0f600d2f7fd10276d1ca2ebe4a2fd47e2c75d9cb86-certificate.pem.crt"
PATH_TO_PRIVATE_KEY = "certificats/d8eb1dca03956f52a96d9a0f600d2f7fd10276d1ca2ebe4a2fd47e2c75d9cb86-private.pem.key"
PATH_TO_ROOT_CA = "certificats/AmazonRootCA1.pem"
TOPIC = "test/topic"

mqtt_client = None
connection_status = "Desconectado"
last_message = None

def on_connect(client, userdata, flags, rc):
    global connection_status
    if rc == 0:
        connection_status = "Conectado"
        client.subscribe(TOPIC)
        print(f"Conectado y suscrito a {TOPIC}")
    else:
        connection_status = "Error de conexión"
        print(f"Error de conexión: código {rc}")

def on_disconnect(client, userdata, rc):
    global connection_status
    connection_status = "Desconectado"
    if rc != 0:
        print(f"Desconexión inesperada: {rc}")

def on_message(client, userdata, msg):
    global last_message
    # Decodifica el payload a string
    payload_str = msg.payload.decode()
    # Intenta parsear como JSON
    payload_json = json.loads(payload_str)
    # Extrae el valor del campo "mensaje"
    last_message = payload_json.get("mensaje", payload_str)
    print(last_message)

def create_mqtt_client():
    global mqtt_client
    mqtt_client = mqtt.Client(client_id=CLIENT_ID)
    mqtt_client.tls_set(ca_certs=PATH_TO_ROOT_CA,
                        certfile=PATH_TO_CERTIFICATE,
                        keyfile=PATH_TO_PRIVATE_KEY,
                        tls_version=ssl.PROTOCOL_TLSv1_2,
                        ciphers=None)
    mqtt_client.on_connect = on_connect
    mqtt_client.on_disconnect = on_disconnect
    mqtt_client.on_message = on_message
    mqtt_client.connect(AWS_IOT_ENDPOINT, port=8883)
    mqtt_client.loop_start()

def get_connection_status():
    return connection_status

def get_last_message():
    return last_message

def publish_message(topic: str, message: str):
    global mqtt_client
    if mqtt_client is None:
        return False
    try:
        # Convierte el mensaje a JSON si no lo es ya
        try:
            # Intenta parsearlo como JSON
            json.loads(message)
            json_message = message
        except json.JSONDecodeError:
            # Si no es JSON, lo convierte a JSON
            json_message = json.dumps({"mensaje": message})
        
        # Publica el mensaje en formato JSON
        mqtt_client.publish(topic, json_message, qos=1)
        print(f"Mensaje publicado en {topic}: {json_message}")
        return True
    except Exception as e:
        print(f"Error: {e}")
        return False

if __name__ == "__main__":
    create_mqtt_client()