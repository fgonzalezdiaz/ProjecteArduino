from typing import List
from sqlalchemy import text
from sqlalchemy.orm import Session
from app import database, models, schemas

# Usuari
def create_usuari(db: Session, usuari: schemas.UsuariCreate):
    db_usuari = models.Usuari(nom=usuari.nom, nivell=usuari.nivell)
    db.add(db_usuari)
    db.commit()
    db.refresh(db_usuari)
    return db_usuari


def get_usuaris(db: Session) -> List[str] :
    consulta = text("SELECT * FROM usuaris")
    result = db.execute(consulta)
    rows = result.fetchall()
    return [str(row) for row in rows]
