from sqlalchemy import create_engine
from sqlalchemy.orm import sessionmaker
import os
from sqlalchemy.orm import Session

# Puedes configurar la URL en una variable de entorno o ponerla directamente
DATABASE_URL = os.getenv("DATABASE_URL", "mysql+mysqlconnector://root:123456@localhost:3309/databaseitic")

# Crea el motor de conexión
engine = create_engine(DATABASE_URL)

# Crea una clase para las sesiones (maneja las transacciones)

def get_db():
    db = Session(engine)
    try:
        yield db
    finally:
        db.close()

