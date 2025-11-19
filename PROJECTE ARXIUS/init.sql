use databaseitic;

CREATE TABLE IF NOT EXISTS usuaris (
    id BIGINT AUTO_INCREMENT PRIMARY KEY,
    nom VARCHAR(100) NOT NULL,
    nivell INT NOT NULL
);

CREATE TABLE IF NOT EXISTS zones (
    id BIGINT AUTO_INCREMENT PRIMARY KEY,
    nom VARCHAR(100) NOT NULL,
    nivell_acces INT NOT NULL
);

CREATE TABLE IF NOT EXISTS horari (
    id BIGINT AUTO_INCREMENT PRIMARY KEY,
    fecha DATE NOT NULL,
    hora_inici TIME NOT NULL,
    hora_final TIME NOT NULL,
    UNIQUE KEY unique_horario (fecha, hora_inici, hora_final)
);

CREATE TABLE IF NOT EXISTS present (
    id_usuari BIGINT,
    id_horari BIGINT,
    present BOOLEAN NOT NULL,
    PRIMARY KEY (id_usuari, id_horari),
    FOREIGN KEY (id_usuari) REFERENCES usuaris(id),
    FOREIGN KEY (id_horari) REFERENCES horari(id)
);