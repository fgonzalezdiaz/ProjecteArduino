from typing import List
from sqlalchemy import text
from sqlalchemy.orm import Session
from . import database, models, schemas
from sqlalchemy import text
from datetime import datetime
from sqlalchemy import text
from datetime import datetime
from sqlalchemy.orm import Session
# Usuari
def create_usuari(db: Session, usuari: schemas.UsuariCreate):
    db_usuari = models.Usuari(nom=usuari.nom, nivell=usuari.nivell)
    db.add(db_usuari)
    db.commit()
    db.refresh(db_usuari)
    return db_usuari


def get_usuaris(db: Session) -> List[str] :
    consulta = text("SELECT * FROM usuaris")
    result = db.execute(consulta, {"id": id}).mappings().first()
    #result = db.execute(consulta)
    rows = result.fetchall()
    return [str(row) for row in rows]

def get_usuari(db: Session, id: str):
    consulta = text("SELECT * FROM usuaris WHERE id = :id") # EL ERROR ESTA AQUI
    result = db.execute(consulta, {"id": id})
    fila = result.first()   # o fetchall()
    if fila is None:
        return []
    return {
        "id": fila.id,
        "nom": fila.nom,
        "nivell": fila.nivell
    }


def marcar_presencia(db: Session, id_usuari: int):
    # Format: dia/mes/any HH:MM
    horari_actual = datetime.now().strftime("%d/%m/%Y %H:%M")

    consulta = text("""
        INSERT INTO present (id_usuari, horari)
        VALUES (:id_usuari, :horari)
        ON DUPLICATE KEY UPDATE id_usuari = id_usuari
    """)

    db.execute(consulta, {"id_usuari": id_usuari, "horari": horari_actual})
    db.commit()
    return True

