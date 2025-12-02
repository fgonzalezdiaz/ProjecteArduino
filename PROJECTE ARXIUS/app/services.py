from app import crud
# services.py
from sqlalchemy.orm import Session
from app import crud
from fastapi import FastAPI, HTTPException
from app.mqtt_client import create_mqtt_client
import threading
mqtt_client = None
def get_all_users(db: Session):
    return crud.get_usuaris(db)


def get_connection():
    global mqtt_client
    try:
        if mqtt_client is None:
            mqtt_client = create_mqtt_client()
            
            # Arrancamos el loop en un hilo para no bloquear FastAPI
            thread = threading.Thread(target=mqtt_client.loop_start)
            thread.daemon = True
            thread.start()
        
        return {"message": "Conexión MQTT iniciada correctamente"}
    except Exception as e:
        raise HTTPException(status_code=500, detail=f"Error conectando MQTT: {e}")
    
def mqtt_get_message():
    return mqtt_client.get_last_message()

def mqtt_post_message(topic : str, message : str):
    if mqtt_client.publish_message(topic, message):
        return "Missatge enviat correctament"
    else:
        return "Missatge no enviat"

    
def mqtt_get_user_id(id : str, db: Session):
    user = crud.get_usuari(db, id)
    return bool(user)

    