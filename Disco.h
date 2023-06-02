//
// Created by yamil on 5/23/23.
//

#ifndef DBMS_DISCO_H
#define DBMS_DISCO_H

#include <iostream>
#include <vector>

class Sector {
private:
    std::string data;
    Sector *next;
public:
    Sector() = default;

    explicit Sector(int bytesData) {
        data.resize(bytesData);
    }

    std::string getData() {
        if (!this->data.empty()) {
            return data;
        }
        return "null";
    }

    void setData(const std::string &data) {
        Sector::data = data;
    }

    Sector *getNext() const {
        return next;
    }

    void setNext(Sector *next) {
        this->next = next;
    }
};

class Bloque {
private:
    std::vector<Sector *> sectores;
public:
    Bloque() = default;

    explicit Bloque(int numeroSectores) {
        this->sectores.resize(numeroSectores);
    }
};

class Pista {
private:
    int numeroSectores;
    std::vector<Sector *> sectores;
public:
    Pista() = default;

    explicit Pista(int numeroSectores) {
        this->numeroSectores = numeroSectores;
    }

    void add(Sector *sector) {
        this->sectores.push_back(sector);
    }

    const std::vector<Sector *> &getSectores() const {
        return sectores;
    }

    void setSectores(const std::vector<Sector *> &sectores) {
        Pista::sectores = sectores;
    }
};

class Superficie {
private:
    std::vector<Pista> pistas;
public:
    Superficie() = default;

    explicit Superficie(int numeroPistas) {
        this->pistas.resize(numeroPistas);
    }

    const std::vector<Pista> &getPistas() const {
        return pistas;
    }

    void setPistas(const std::vector<Pista> &pistas) {
        Superficie::pistas = pistas;
    }
};

class Plato {
private:
    std::vector<Superficie> superficies;
public:
    Plato() = default;

    const std::vector<Superficie> &getSuperficies() const {
        return superficies;
    }

    void setSuperficies(const std::vector<Superficie> &superficies) {
        Plato::superficies = superficies;
    }
};

class Disco {
private:
    std::vector<Plato> platos;
    int numPlatos;
    int numSuperficies;
    int numPistas;
    int numBloques;
    int numSectores;
    int numBytes;
public:
    Disco() = default;

    Disco(int numPlatos, int numSuperficies, int numPistas, int numBloques, int numSectores, int numBytes) : numPlatos(
            numPlatos), numSuperficies(numSuperficies), numPistas(numPistas), numBloques(numBloques), numSectores(
            numSectores), numBytes(numBytes) {}

    explicit Disco(int numeroPlatos) {
        this->platos.resize(numeroPlatos);
    }

    const std::vector<Plato> &getPlatos() const {
        return platos;
    }

    void setPlatos(const std::vector<Plato> &platos) {
        Disco::platos = platos;
    }


};

#endif //DBMS_DISCO_H
