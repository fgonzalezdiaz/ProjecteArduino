# schemas.py
from pydantic import BaseModel
from datetime import date, time

class UsuariBase(BaseModel):
    nom: str
    nivell: int

class UsuariCreate(UsuariBase):
    pass

class UsuariRead(UsuariBase):
    id: int

    class Config:
        orm_mode = True

class ZonaBase(BaseModel):
    nom: str
    nivell_acces: int

class ZonaCreate(ZonaBase):
    pass

class ZonaRead(ZonaBase):
    id: int

    class Config:
        orm_mode = True

class AsistenciaBase(BaseModel):
    id_usuari: int
    fecha: date
    hora_inici: time
    hora_final: time

class AsistenciaCreate(AsistenciaBase):
    pass

class AsistenciaRead(AsistenciaBase):

    class Config:
        orm_mode = True
