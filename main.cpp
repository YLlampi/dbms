#include <bits/stdc++.h>
#include "TextTable.h"
#include "Disco.h"

using namespace std;

const string BASE_PATH = "/home/yamil/CLionProjects/dbms/";
const string NAME_FILE = BASE_PATH + "new_titanic.csv";
const string NAME_DATA = BASE_PATH + "data.txt";
std::string DATA;
bool band = false;
bool bandDisk = false;

int platos;
const int SUPERFICIE = 2;
int pistas;
int bloques;
int sectores;
int bytesSector;
Disco miDisco;
std::vector<Sector *> sectoresDisco;
std::vector<string> dataBloques;

void cleanScreen() {
    std::cout << "Presiona Enter para volver al menu...";
    std::cin.ignore();
    std::cin.get();

    std::cout << "\033[2J\033[1;1H";
}

void readData() {
    std::ifstream inputFile(NAME_DATA);
    std::getline(inputFile, DATA);
    inputFile.close();
}

void inputFiles() {
    std::ifstream inputFile(NAME_FILE);
    std::ofstream outputFile(NAME_DATA);

    std::string line;
    std::getline(inputFile, line);

    while (std::getline(inputFile, line)) {
        std::stringstream ss(line);
        std::string value;
        std::vector<std::string> values;

        while (std::getline(ss, value, ',')) {
            values.push_back(value);
        }

        std::string concatenatedLine = "#";
        concatenatedLine += values[0];

        for (size_t i = 1; i < values.size(); ++i) {
            concatenatedLine += "," + values[i];
        }
        outputFile << concatenatedLine;
    }
    outputFile << "#";

    inputFile.close();
    outputFile.close();

    readData();
    std::cout << "Archivo importado correctamente..." << std::endl;
}

std::streamsize obtenerTamanoBytesFila(std::ifstream& archivo) {
    std::streampos inicioLinea = archivo.tellg();

    char c;
    std::streamsize tamanoFila = 0;

    while (archivo.get(c) && c != '\n') {
        tamanoFila++;
    }

    archivo.seekg(inicioLinea, std::ios::beg);

    return tamanoFila;
}

void getNumeroBitsPorRegistro() {
    /*
    TextTable t('-', '|', '+');

    t.add(" id ");
    t.add(" N de bits ");
    t.add(" N de bytes ");
    t.endOfRow();

    int cont = 0;
    int id = 0;
    for (int i = 1; i < DATA.size(); i++) {
        if (DATA[i] != ',' and DATA[i] != '#') {
            cont++;
        } else if (DATA[i] == '#') {
            int bits = cont * 8;
            t.add(" " + to_string(++id) + " ");
            t.add(" " + to_string(bits) + " ");
            t.add(" " + to_string(bits / 8) + " ");
            t.endOfRow();
            cont = 0;
        }
    }
    std::cout << t << std::endl;
     */

    TextTable t('-', '|', '+');

    t.add(" id ");
    t.add(" N de bytes ");
    t.add(" N de bits ");
    t.endOfRow();
    std::ifstream archivo(NAME_FILE, std::ios::binary);

    int _cont = 0;

    if (archivo) {
        while (archivo.peek() != EOF) {
            std::streamsize tamanoFila = obtenerTamanoBytesFila(archivo);
            t.add(" " + to_string(++_cont) + " ");
            t.add(" " + to_string(tamanoFila) + " ");
            t.add(" " + to_string(tamanoFila * 8) + " ");
            t.endOfRow();

            archivo.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        }

        archivo.close();
    } else {
        std::cout << "Error al abrir el archivo." << std::endl;
    }

    std::cout << t << std::endl;


}

std::streamsize getNumeroBitsFile() {
    std::ifstream archivo(NAME_FILE, std::ios::binary);

    std::streampos inicioArchivo = archivo.tellg();
    archivo.seekg(0, std::ios::end);

    std::streampos tamano = archivo.tellg();
    archivo.seekg(inicioArchivo, std::ios::beg);

    return tamano;
}


void getRegistro(int numeroRegistro) {
    TextTable t('-', '|', '+');

    t.add(" PassengerId ");
    t.add(" Survived ");
    t.add(" Pclass ");
    t.add(" Name ");
    t.add(" Sex ");
    t.add(" Age ");
    t.add(" SibSp ");
    t.add(" Parch ");
    t.add(" Ticket ");
    t.add(" Fare ");
    t.add(" Cabin ");
    t.add(" Embarked ");
    t.endOfRow();

    int aux1 = 0;
    int aux2 = 0;
    std::string texto;
    for (int i = 0; i < DATA.size(); i++) {
        if (DATA[i] == '#') aux2++;
        if (aux2 == numeroRegistro and DATA[i + 1] != '#') {
            texto += DATA[i + 1];
        }
    }

    std::vector<std::string> vectorStrings;
    std::stringstream ss(texto);
    std::string token;

    while (std::getline(ss, token, ',')) {
        vectorStrings.push_back(token);
    }

    for (auto &text: vectorStrings) {
        t.add(" " + text + " ");
    }
    t.endOfRow();
    std::cout << t << std::endl;

}

void setDisco() {
    std::cout << "N° Platos: ";
    std::cin >> platos;
    std::cout << "N° Pistas: ";
    std::cin >> pistas;
//    std::cout << "N° Bloques: ";
//    std::cin >> bloques;
    std::cout << "N° Sectores: ";
    std::cin >> sectores;
    std::cout << "N° bytes por sector: ";
    std::cin >> bytesSector;

//    std::vector<Sector *> sectoresDisco;
    std::string dataSector;
    std::string newData = DATA;

    newData.erase(std::remove(newData.begin(), newData.end(), '#'), newData.end());
    newData.erase(std::remove(newData.begin(), newData.end(), ','), newData.end());

    int i;
    int aux;
    for (i = 0; i < newData.size(); i += bytesSector) {
        dataSector = newData.substr(i, bytesSector);
        Sector *miSector = new Sector(bytesSector);
        miSector->setData(dataSector);
        sectoresDisco.push_back(miSector);
        aux = i;
    }

    for (int j = 0; j < sectoresDisco.size() - 1; j++) {
        sectoresDisco[j]->setNext(sectoresDisco[j + 1]);
    }
    int contSectores = 0;

    miDisco.setData(platos, SUPERFICIE, pistas, sectores, bytesSector);
    bandDisk = true;

    std::vector<Plato> platosDisco(platos);
    for (int m = 0; m < platos; m++) {
        std::vector<Superficie> superficiesDisco(SUPERFICIE);
        for (int j = 0; j < SUPERFICIE; j++) {
            std::vector<Pista> pistasDisco(pistas);
            for (int k = 0; k < pistas; k++) {
                for (int u = 0; u < sectores; u++) {
                    if (contSectores < sectoresDisco.size()) {
                        pistasDisco[k].add(sectoresDisco[contSectores++]);
                    } else {
                        pistasDisco[k].add(new Sector(0));
                    }
                }
            }
            superficiesDisco[j].setPistas(pistasDisco);
        }
        platosDisco[m].setSuperficies(superficiesDisco);
    }
    miDisco.setPlatos(platosDisco);

    for (int m = 0; m < miDisco.getPlatos().size(); m++) {
        auto getPlatos = miDisco.getPlatos();
        std::cout << "Plato: " << m + 1 << std::endl;

        for (int j = 0; j < getPlatos[m].getSuperficies().size(); j++) {
            std::cout << "\tSuperficie: " << j + 1 << std::endl;

            auto getSuperficies = getPlatos[m].getSuperficies();
            for (int k = 0; k < getSuperficies[j].getPistas().size(); k++) {
                std::cout << "\t\tPista: " << k + 1 << std::endl;

                auto getPistas = getSuperficies[j].getPistas();
                for (int u = 0; u < getPistas[k].getSectores().size(); u++) {
                    std::cout << "\t\t\tSector: " << u + 1 << std::endl;
                    std::cout << "\t\t\t\tData: " << getPistas[k].getSectores()[u]->getData();
                    std::cout << std::endl;
                }
            }
        }
    }
}

double convertBytesToGB(unsigned long long bytes) {
    const double gigabyte = 1024 * 1024 * 1024;
    return static_cast<double>(bytes) / gigabyte;
}

void printSizeDisk() {
    unsigned long long sizeBytes = platos * SUPERFICIE * pistas * bloques * sectores * bytesSector;
    double sizeGb = convertBytesToGB(sizeBytes);
    std::cout << "\tDisk Size" << std::endl;
    std::cout << "Bytes:\t" << sizeBytes << std::endl;
    std::cout << "GB:\t" << sizeGb << std::endl;
}

void getDataSector() {
    int _plato;
    int _superficie;
    int _pista;
    int _sector;

    std::cout << "N Plato: ";
    std::cin >> _plato;
    std::cout << "N Superficie: ";
    std::cin >> _superficie;
    std::cout << "N Pista: ";
    std::cin >> _pista;
    std::cout << "N Sector: ";
    std::cin >> _sector;

    try {
        std::cout
                << miDisco.getPlatos()[_plato - 1].getSuperficies()[_superficie - 1].getPistas()[_pista -
                                                                                                 1].getSectores()[
                        _sector - 1]->getData()
                << std::endl;
    } catch (...) {
        std::cout << "null" << std::endl;
    }

}

void asignarBloques() {
    int numSectores;
    std::cout << "Numero de Sectores por Bloque: ";
    std::cin >> numSectores;

    for (int i = 0; i < sectoresDisco.size() - 2; i += 3) {
        std::string _data =
                sectoresDisco[i]->getData() + sectoresDisco[i + 1]->getData() + sectoresDisco[i + 2]->getData();
        dataBloques.push_back(_data);
    }
}

void getDataBloque() {
    int numBloque;
    std::cout << "Data de Bloque N°: ";
    std::cin >> numBloque;

    std::cout << "\tData\n";
    try {
        std::cout << dataBloques[numBloque - 1] << std::endl;
    } catch (...) {
        std::cout << "null" << std::endl;
    }
}

void menu() {
    int option;

    do {
        std::cout << "=========== Database System ===========" << std::endl;
        std::cout << "================ MENU =================" << std::endl;
        std::cout << "1) Importar archivo." << std::endl;
        if (band) {
            std::cout << "2) Mostrar N° de bits por registro." << std::endl;
            std::cout << "3) Mostrar N° de bits del archivo." << std::endl;
            std::cout << "4) Mostrar registro (Especifico)." << std::endl;
            std::cout << "5) Insertar datos del disco." << std::endl;
            if (bandDisk) {
                std::cout << "6) Obtener tamaño del disco." << std::endl;
                std::cout << "7) Data de un Sector (Especifico)." << std::endl;
                std::cout << "8) Asignar Bloques." << std::endl;
                std::cout << "9) Mostrar Datos de Bloque (Especifico)." << std::endl;
            }
        }
        std::cout << "0) Salir del programa." << std::endl;
        std::cout << "Opcion: ";
        std::cin >> option;
//        std::cout << std::endl;
        std::cout << "\033[2J\033[1;1H";

        switch (option) {
            case 1:
                inputFiles();
                band = true;
                cleanScreen();
                break;
            case 2:
                std::cout << "Numero de bits por registro:\n";
                getNumeroBitsPorRegistro();
                cleanScreen();
                break;
            case 3:
                // int numeroBitsFile = getNumeroBitsFile();
                std::cout << "Numero de bits por File: " << getNumeroBitsFile() * 8 << " bits" << std::endl;
                std::cout << "Numero de bytes por File: " << getNumeroBitsFile() << " bytes" << std::endl;
                cleanScreen();
                break;
            case 4:
                int numeroRegistro;
                std::cout << "Imprimir Registro N°: ";
                std::cin >> numeroRegistro;

                std::cout << "Registro " << numeroRegistro << ":\n";
                getRegistro(numeroRegistro);
                cleanScreen();
                break;
            case 5:
                setDisco();
                cleanScreen();
                break;
            case 6:
                printSizeDisk();
                cleanScreen();
                break;
            case 7:
                getDataSector();
                cleanScreen();
                break;
            case 8:
                asignarBloques();
                cleanScreen();
                break;
            case 9:
                getDataBloque();
                cleanScreen();
                break;
            case 0:
                std::cout << "Saliendo del programa..." << std::endl;
                exit(1);
            default:
                std::cout << "\033[2J\033[1;1H";
        }
    } while (option != 0);
}

int main() {

    menu();

    return 0;
}