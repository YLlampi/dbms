#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <algorithm>

#include "TextTable.h"

using namespace std;

struct Atributo {
    string nombre;
    string tipo;
};

struct RegistroVariable {
    vector<string> valores;
};

struct TablaMetadata {
    string nombre;
    int proximaId;
};

struct Tabla {
    TablaMetadata metadata;
    vector<Atributo> atributos;
};

void escribirRegistroVariable(const RegistroVariable& registro, ofstream& archivo) {
    int numAtributos = registro.valores.size();
    archivo.write(reinterpret_cast<const char*>(&numAtributos), sizeof(int));
    for (const auto& valor : registro.valores) {
        int longitudValor = valor.length();
        archivo.write(reinterpret_cast<const char*>(&longitudValor), sizeof(int));
        archivo.write(valor.c_str(), longitudValor);
    }
}

RegistroVariable leerRegistroVariable(ifstream& archivo) {
    RegistroVariable registro;
    try {
        int numAtributos;
        archivo.read(reinterpret_cast<char*>(&numAtributos), sizeof(int));
        for (int i = 0; i < numAtributos; ++i) {
            int longitudValor;
            archivo.read(reinterpret_cast<char*>(&longitudValor), sizeof(int));
            char* valorBuffer = new char[longitudValor + 1];
            archivo.read(valorBuffer, longitudValor);
            valorBuffer[longitudValor] = '\0';
            registro.valores.push_back(valorBuffer);
            delete[] valorBuffer;
        }
    } catch (...) {
        // cout << "[ERROR] - lectura de registro variable" << endl;
    }
    return registro;
}

void almacenarRegistroVariable(const RegistroVariable& registro, const string& archivoNombre) {
    ofstream archivo(archivoNombre, ios::binary | ios::app);
    if (archivo.is_open()) {
        escribirRegistroVariable(registro, archivo);
        archivo.close();
        cout << "Registro almacenado con éxito." << endl;
    } else {
        cout << "No se pudo abrir el archivo." << endl;
    }
}

void recuperarRegistrosVariables(const string& archivoNombre) {
    ifstream archivo(archivoNombre, ios::binary);
    if (archivo.is_open()) {
        RegistroVariable registro;
        while (!archivo.eof()) {
            registro = leerRegistroVariable(archivo);
            if (!archivo.eof()) {
                for (const auto& valor : registro.valores) {
                    cout << valor << " ";
                }
                cout << endl;
            }
        }
        archivo.close();
    } else {
        cout << "No se pudo abrir el archivo." << endl;
    }
}

void eliminarRegistroVariable(const string& nombre, const string& archivoNombre) {
    ifstream archivoEntrada(archivoNombre, ios::binary);
    ofstream archivoTemporal("temp.bin", ios::binary);
    if (archivoEntrada.is_open() && archivoTemporal.is_open()) {
        RegistroVariable registro;
        bool encontrado = false;
        while (!archivoEntrada.eof()) {
            registro = leerRegistroVariable(archivoEntrada);
            if (!archivoEntrada.eof()) {
                if (registro.valores[0] != nombre) {
                    escribirRegistroVariable(registro, archivoTemporal);
                } else {
                    encontrado = true;
                }
            }
        }
        archivoEntrada.close();
        archivoTemporal.close();
        remove(archivoNombre.c_str());
        rename("temp.bin", archivoNombre.c_str());
        if (encontrado) {
            cout << "Registro eliminado con éxito." << endl;
        } else {
            cout << "No se encontró el registro." << endl;
        }
    } else {
        cout << "No se pudo abrir el archivo." << endl;
    }
}

void actualizarRegistroVariable(const string& nombre, const vector<string>& nuevosValores, const string& archivoNombre) {
    ifstream archivoEntrada(archivoNombre, ios::binary);
    ofstream archivoTemporal("temp.bin", ios::binary);
    if (archivoEntrada.is_open() && archivoTemporal.is_open()) {
        RegistroVariable registro;
        bool encontrado = false;
        while (!archivoEntrada.eof()) {
            registro = leerRegistroVariable(archivoEntrada);
            if (!archivoEntrada.eof()) {
                if (registro.valores[0] != nombre) {
                    escribirRegistroVariable(registro, archivoTemporal);
                } else {
                    registro.valores = nuevosValores;
                    escribirRegistroVariable(registro, archivoTemporal);
                    encontrado = true;
                }
            }
        }
        archivoEntrada.close();
        archivoTemporal.close();
        remove(archivoNombre.c_str());
        rename("temp.bin", archivoNombre.c_str());
        if (encontrado) {
            cout << "Registro actualizado con éxito." << endl;
        } else {
            cout << "No se encontró el registro." << endl;
        }
    } else {
        cout << "No se pudo abrir el archivo." << endl;
    }
}

void buscarRegistroVariable(const string& nombre, const string& archivoNombre) {
    ifstream archivo(archivoNombre, ios::binary);
    if (archivo.is_open()) {
        RegistroVariable registro;
        bool encontrado = false;
        while (!archivo.eof()) {
            registro = leerRegistroVariable(archivo);
            if (!archivo.eof()) {
                if (registro.valores[0] == nombre) {
                    cout << "Registro encontrado:" << endl;
                    for (const auto& valor : registro.valores) {
                        cout << valor << " ";
                    }
                    cout << endl;
                    encontrado = true;
                    break;
                }
            }
        }
        archivo.close();
        if (!encontrado) {
            cout << "No se encontró el registro." << endl;
        }
    } else {
        cout << "No se pudo abrir el archivo." << endl;
    }
}

void crearTablaNueva(vector<Tabla>& tablas) {
    Tabla nuevaTabla;
    cout << "Ingrese el nombre de la nueva tabla: ";
    cin >> nuevaTabla.metadata.nombre;

    int numAtributos;
    cout << "Ingrese el número de atributos de la tabla: ";
    cin >> numAtributos;

    // Atributo idAtributo;
    // idAtributo.nombre = "id";
    // idAtributo.tipo = "int";
    // nuevaTabla.atributos.push_back(idAtributo);

    for (int i = 0; i < numAtributos; ++i) {
        Atributo atributo;
        cout << "Ingrese el nombre del atributo " << i + 1 << ": ";
        cin >> atributo.nombre;
        cout << "Ingrese el tipo del atributo " << i + 1 << ": ";
        cin >> atributo.tipo;
        nuevaTabla.atributos.push_back(atributo);
    }

    nuevaTabla.metadata.proximaId = 1;
    tablas.push_back(nuevaTabla);
    cout << "Tabla creada con éxito." << endl;
}

void insertarDatosCabecera(vector<Tabla>& tablas, const string& archivoNombre, const vector<string>& cabeceras) {
    Tabla nuevaTabla;
    // cout << "Ingrese el nombre de la nueva tabla: ";
    nuevaTabla.metadata.nombre = archivoNombre;

    // int numAtributos;
    // cout << "Ingrese el número de atributos de la tabla: ";
    // cin >> numAtributos;

    // Atributo idAtributo;
    // idAtributo.nombre = "id";
    // idAtributo.tipo = "int";
    // nuevaTabla.atributos.push_back(idAtributo);

    for (int i = 0; i < cabeceras.size(); ++i) {
        Atributo atributo;
        // cout << "Ingrese el nombre del atributo " << i + 1 << ": ";
        // cin >> atributo.nombre;
        // cout << "Ingrese el tipo del atributo " << i + 1 << ": ";
        // cin >> atributo.tipo;
        std::string nombre = cabeceras[i];
        atributo.nombre = nombre;
        atributo.tipo = "string";
        nuevaTabla.atributos.push_back(atributo);
    }

    nuevaTabla.metadata.proximaId = 1;
    tablas.push_back(nuevaTabla);
    cout << "Tabla creada con éxito." << endl;
}

Tabla* obtenerTablaPorNombre(const string& nombre, vector<Tabla>& tablas) {
    for (auto& tabla : tablas) {
        if (tabla.metadata.nombre == nombre) {
            return &tabla;
        }
    }
    return nullptr;
}

void almacenarRegistroVariable(const RegistroVariable& registro, const Tabla& tabla, const string& archivoNombre) {
    ofstream archivo(archivoNombre, ios::binary | ios::app);
    if (archivo.is_open()) {
        escribirRegistroVariable(registro, archivo);
        archivo.close();
        cout << "Registro almacenado con éxito." << endl;
    } else {
        cout << "No se pudo abrir el archivo." << endl;
    }
}

void recuperarRegistrosVariables(const Tabla& tabla, const string& archivoNombre) {
    ifstream archivo(archivoNombre, ios::binary);
    if (archivo.is_open()) {        
        // Verificar si el archivo está vacío
        archivo.seekg(0, ios::end);
        if (archivo.tellg() == 0) {
            cout << "El archivo está vacío." << endl;
            archivo.close();
            return;
        }

        // Regresar al inicio del archivo
        TextTable t('-', '|', '+');
        
        archivo.seekg(0, ios::beg);

        RegistroVariable registro;
        while (!archivo.eof()) {
            registro = leerRegistroVariable(archivo);
            if (!archivo.eof()) {
                for (const auto& valor : registro.valores) {
                    // cout << valor << " ";
                    t.add(" " + valor + " ");
                }
                t.endOfRow();
                cout << endl;
            }
        }

        std::cout << t << std::endl;
        archivo.close();
    } else {
        cout << "No se pudo abrir el archivo." << endl;
    }
}

void eliminarRegistroVariable(const string& nombre, const Tabla& tabla, const string& archivoNombre) {
    ifstream archivoEntrada(archivoNombre, ios::binary);
    ofstream archivoTemporal("temp.bin", ios::binary);
    if (archivoEntrada.is_open() && archivoTemporal.is_open()) {
        RegistroVariable registro;
        bool encontrado = false;
        while (!archivoEntrada.eof()) {
            registro = leerRegistroVariable(archivoEntrada);
            if (!archivoEntrada.eof()) {
                if (registro.valores[0] != nombre) {
                    escribirRegistroVariable(registro, archivoTemporal);
                } else {
                    encontrado = true;
                }
            }
        }
        archivoEntrada.close();
        archivoTemporal.close();
        remove(archivoNombre.c_str());
        rename("temp.bin", archivoNombre.c_str());
        if (encontrado) {
            cout << "Registro eliminado con éxito." << endl;
        } else {
            cout << "No se encontró el registro." << endl;
        }
    } else {
        cout << "No se pudo abrir el archivo." << endl;
    }
}

void actualizarRegistroVariable(const string& nombre, const vector<string>& nuevosValores, const Tabla& tabla, const string& archivoNombre) {
    ifstream archivoEntrada(archivoNombre, ios::binary);
    ofstream archivoTemporal("temp.bin", ios::binary);
    if (archivoEntrada.is_open() && archivoTemporal.is_open()) {
        RegistroVariable registro;
        bool encontrado = false;
        while (!archivoEntrada.eof()) {
            registro = leerRegistroVariable(archivoEntrada);
            if (!archivoEntrada.eof()) {
                if (registro.valores[0] != nombre) {
                    escribirRegistroVariable(registro, archivoTemporal);
                } else {
                    registro.valores = nuevosValores;
                    escribirRegistroVariable(registro, archivoTemporal);
                    encontrado = true;
                }
            }
        }
        archivoEntrada.close();
        archivoTemporal.close();
        remove(archivoNombre.c_str());
        rename("temp.bin", archivoNombre.c_str());
        if (encontrado) {
            cout << "Registro actualizado con éxito." << endl;
        } else {
            cout << "No se encontró el registro." << endl;
        }
    } else {
        cout << "No se pudo abrir el archivo." << endl;
    }
}

void buscarRegistroVariable(const string& nombre, const Tabla& tabla, const string& archivoNombre) {
    ifstream archivo(archivoNombre, ios::binary);
    if (archivo.is_open()) {
        RegistroVariable registro;
        bool encontrado = false;
        while (!archivo.eof()) {
            registro = leerRegistroVariable(archivo);
            if (!archivo.eof()) {
                if (registro.valores[0] == nombre) {
                    cout << "Registro encontrado:" << endl;
                    TextTable t('-', '|', '+');
                    for (const auto& valor : registro.valores) {
                        // cout << valor << " ";
                        t.add(" " + valor + " ");
                    }
                    t.endOfRow();
                    // cout << endl;
                    encontrado = true;
                    std::cout << t << std::endl;
                    break;
                }
            }
        }
        archivo.close();
        if (!encontrado) {
            cout << "No se encontró el registro." << endl;
        }
    } else {
        cout << "No se pudo abrir el archivo." << endl;
    }
}

void buscarRegistroWhere(const Tabla& tabla, const string& archivoNombre, int posicion, string& name) {
    ifstream archivo(archivoNombre, ios::binary);
    if (archivo.is_open()) {        
        // Verificar si el archivo está vacío
        archivo.seekg(0, ios::end);
        if (archivo.tellg() == 0) {
            cout << "El archivo está vacío." << endl;
            archivo.close();
            return;
        }

        // Regresar al inicio del archivo
        TextTable t('-', '|', '+');
        
        archivo.seekg(0, ios::beg);

        RegistroVariable registro;
        while (!archivo.eof()) {
            registro = leerRegistroVariable(archivo);
            if (!archivo.eof()) {
                if(registro.valores[posicion] == name) {
                    for (const auto& valor : registro.valores) {
                    // cout << valor << " ";
                    t.add(" " + valor + " ");
                    }
                    t.endOfRow();
                    cout << endl;
                }
            }
        }

        std::cout << t << std::endl;
        archivo.close();
    } else {
        cout << "No se pudo abrir el archivo." << endl;
    }
}

void crearTablaCsv(vector<Tabla>& tablas, const string& archivoNombre) {
    std::ifstream inputFile(archivoNombre);

    std::string line;
    
    // Crear tabla con las cabeceras
    std::getline(inputFile, line);
    std::stringstream ss(line);
    std::string cabecera;
    std::vector<string> cabeceras;
    while (std::getline(ss, cabecera, ',')) {
        cabeceras.push_back(cabecera);
    }
    insertarDatosCabecera(tablas, archivoNombre, cabeceras);

    while (std::getline(inputFile, line)) {
        std::stringstream ss(line);
        std::string value;
        std::vector<std::string> values;

        while (std::getline(ss, value, ',')) {
            values.push_back(value);
        }
        // Insertar Data a la tabla
        Tabla* tabla = obtenerTablaPorNombre(archivoNombre, tablas);
        if (tabla) {
            RegistroVariable registro;
            int i = 0;
            for (const auto& atributo : tabla->atributos) {
                string valor;
                // cout << "Ingrese el valor para el atributo " << atributo.nombre << ": ";
                // cin >> valor;
                registro.valores.push_back(values[i]);
                i++;
            }
            almacenarRegistroVariable(registro, *tabla, tabla->metadata.nombre + ".bin");
        } else {
            cout << "No se encontró la tabla." << endl;
        }
    }

    inputFile.close();
    std::cout << "Archivo importado correctamente..." << std::endl;
}


int main() {
    vector<Tabla> tablas;
    int opcion;

    do {
        cout << "Menu de opciones:" << endl;
        cout << "1. Crear tabla desde csv" << endl;
        cout << "2. Crear tabla nueva" << endl;
        cout << "3. Insertar registro" << endl;
        cout << "4. Recuperar registros" << endl;
        cout << "5. Eliminar registro" << endl;
        cout << "6. Actualizar registro" << endl;
        cout << "7. Buscar registro" << endl;
        cout << "8. Buscar registro (WHERE)" << endl;
        cout << "9. Salir" << endl;
        cout << "Ingrese una opción: ";
        cin >> opcion;

        switch (opcion) {
            case 1: {
                string archivoNombre;
                cout << "Ingrese nombre del archivo(csv): ";
                cin>>archivoNombre;
                crearTablaCsv(tablas, archivoNombre);
                break;
            }
            case 2:
                crearTablaNueva(tablas);
                break;
            case 3: {
                string nombreTabla;
                cout << "Ingrese el nombre de la tabla: ";
                cin >> nombreTabla;
                Tabla* tabla = obtenerTablaPorNombre(nombreTabla, tablas);
                if (tabla) {
                    RegistroVariable registro;
                    for (const auto& atributo : tabla->atributos) {
                        string valor;
                        cout << "Ingrese el valor para el atributo " << atributo.nombre << ": ";
                        cin >> valor;
                        registro.valores.push_back(valor);
                    }
                    almacenarRegistroVariable(registro, *tabla, tabla->metadata.nombre + ".bin");
                } else {
                    cout << "No se encontró la tabla." << endl;
                }
                break;
            }
            case 4: {
                string nombreTabla;
                cout << "Ingrese el nombre de la tabla: ";
                cin >> nombreTabla;
                Tabla* tabla = obtenerTablaPorNombre(nombreTabla, tablas);
                if (tabla) {
                    recuperarRegistrosVariables(*tabla, tabla->metadata.nombre + ".bin");
                } else {
                    cout << "No se encontró la tabla." << endl;
                }
                break;
            }
            case 5: {
                string nombreTabla;
                cout << "Ingrese el nombre de la tabla: ";
                cin >> nombreTabla;
                Tabla* tabla = obtenerTablaPorNombre(nombreTabla, tablas);
                if (tabla) {
                    string nombreEliminar;
                    cout << "Ingrese el nombre del registro a eliminar: ";
                    cin >> nombreEliminar;
                    eliminarRegistroVariable(nombreEliminar, *tabla, tabla->metadata.nombre + ".bin");
                } else {
                    cout << "No se encontró la tabla." << endl;
                }
                break;
            }
            case 6: {
                string nombreTabla;
                cout << "Ingrese el nombre de la tabla: ";
                cin >> nombreTabla;
                Tabla* tabla = obtenerTablaPorNombre(nombreTabla, tablas);
                if (tabla) {
                    string nombreActualizar;
                    cout << "Ingrese el id del registro a actualizar: ";
                    cin >> nombreActualizar;
                    RegistroVariable registro;
                    for (const auto& atributo : tabla->atributos) {
                        string valor;
                        cout << "Ingrese el nuevo valor para el atributo " << atributo.nombre << ": ";
                        cin >> valor;
                        registro.valores.push_back(valor);
                    }
                    actualizarRegistroVariable(nombreActualizar, registro.valores, *tabla, tabla->metadata.nombre + ".bin");
                } else {
                    cout << "No se encontró la tabla." << endl;
                }
                break;
            }
            case 7: {
                string nombreTabla;
                cout << "Ingrese el nombre de la tabla: ";
                cin >> nombreTabla;
                Tabla* tabla = obtenerTablaPorNombre(nombreTabla, tablas);
                if (tabla) {
                    string nombreBuscar;
                    cout << "Ingrese el id del registro a buscar: ";
                    cin >> nombreBuscar;
                    buscarRegistroVariable(nombreBuscar, *tabla, tabla->metadata.nombre + ".bin");
                } else {
                    cout << "No se encontró la tabla." << endl;
                }
                break;
            }
            case 8:{
                string nombreTabla;
                cout << "Ingrese el nombre de la tabla: ";
                cin >> nombreTabla;
                Tabla* tabla = obtenerTablaPorNombre(nombreTabla, tablas);
                if (tabla) {
                    string atributoWhere;
                    cout << "Ingrese nombre del atributo: ";
                    cin>>atributoWhere;
                    auto vec = tabla->atributos;
                    auto it = std::find(vec.begin(), vec.end(), [&](const Atributo& atributo) {
                               return atributo.nombre == atributoWhere;
                           });

                    if (it != vec.end()) {
                        int posicion = std::distance(vec.begin(), it);

                        string nombreBuscar;
                        cout << "Atributo debe ser igual a: ";
                        cin >> nombreBuscar;
                        buscarRegistroWhere(*tabla, tabla->metadata.nombre + ".bin", posicion, atributoWhere);
                        
                    } else {
                        std::cout << "Atributo no encontrado." << std::endl;
                        break;
                    }
                    /*
                    string nombreBuscar;
                    cout << "Ingrese el id del registro a buscar: ";
                    cin >> nombreBuscar;
                    buscarRegistroVariable(nombreBuscar, *tabla, tabla->metadata.nombre + ".bin");
                    */
                } else {
                    cout << "No se encontró la tabla." << endl;
                }
                

                break;
            }
            case 9:
                cout << "Saliendo del Programa..!" << endl;
                break;
            default:
                cout << "Opción inválida. Intente nuevamente." << endl;
                break;
        }
    } while (opcion != 9);

    return 0;
}