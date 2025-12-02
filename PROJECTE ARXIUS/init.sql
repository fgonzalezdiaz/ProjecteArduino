use databaseitic;

CREATE TABLE IF NOT EXISTS usuaris (
    id varchar(30) PRIMARY KEY,
    nom VARCHAR(100) NOT NULL,
    nivell INT NOT NULL
);

CREATE TABLE IF NOT EXISTS zones (
    id BIGINT AUTO_INCREMENT PRIMARY KEY,
    nom VARCHAR(100) NOT NULL,
    nivell_acces INT NOT NULL
);

CREATE TABLE IF NOT EXISTS present (
    id_usuari varchar(30),
    horari varchar(18),
    PRIMARY KEY (id_usuari, horari),
    FOREIGN KEY (id_usuari) REFERENCES usuaris(id)
);
