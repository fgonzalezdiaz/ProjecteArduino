from sqlalchemy.ext.declarative import declarative_base
from sqlalchemy import Column, BigInteger, Integer, String, Date, Time, ForeignKey
from sqlalchemy.orm import relationship

Base = declarative_base()

class Usuari(Base):
    __tablename__ = "usuaris"

    id = Column(BigInteger, primary_key=True, autoincrement=True)
    nom = Column(String(100), nullable=False)
    nivell = Column(Integer, nullable=False)

    asistencias = relationship("Asistencia", back_populates="usuari")

class Zona(Base):
    __tablename__ = "zones"

    id = Column(BigInteger, primary_key=True, autoincrement=True)
    nom = Column(String(100), nullable=False)
    nivell_acces = Column(Integer, nullable=False)

class Asistencia(Base):
    __tablename__ = "asistencia"

    id_usuari = Column(BigInteger, ForeignKey("usuaris.id"), primary_key=True)
    fecha = Column(Date, primary_key=True)
    hora_inici = Column(Time, nullable=False)
    hora_final = Column(Time, nullable=False)

    usuari = relationship("Usuari", back_populates="asistencias")
