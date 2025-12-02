from typing import List
from sqlalchemy import text
from sqlalchemy.orm import Session
from . import database, models, schemas

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