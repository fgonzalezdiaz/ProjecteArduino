from fastapi import Depends, FastAPI, HTTPException
from databases import Database
from typing import List
import os

from . import services
from . import models
from .mqtt_client import get_connection_status, create_mqtt_client, get_last_message, publish_message
from fastapi import FastAPI, Depends
from pydantic import BaseModel
from sqlalchemy.orm import Session
from .database import get_db



app = FastAPI()

# Modelo para publicar mensajes MQTT
class MQTTMessage(BaseModel):
    topic: str
    message: str

# Inicializar cliente MQTT al startup
@app.on_event("startup")
async def startup_event():
    try:
        create_mqtt_client()
        print("Cliente MQTT inicializado")
    except Exception as e:
        print(f"Error inicializando MQTT: {e}")

@app.get("/api")
async def read_root():
    return {"message": "API está corriendo"}

@app.get("/api/allUsers")
def read_all_users(db: Session = Depends(get_db)):
    if(db is None): return "Hola"    
    return services.get_all_users(db)


@app.get("/api/test-mqtt-connection")
def test_mqtt_connection():
    return services.get_connection()

@app.get("/api/mqtt/status")
def get_mqtt_status():
    status = get_connection_status()
    return {"status": status}

@app.get("/api/mqtt/message")
def get_mqtt_message():
    message = get_last_message()
    return {"message": message}

@app.post("/api/mqtt/publish")
def post_mqtt_message(data: MQTTMessage):
    success = publish_message(data.topic, data.message)
    return {"success": success, "topic": data.topic, "message": data.message}


