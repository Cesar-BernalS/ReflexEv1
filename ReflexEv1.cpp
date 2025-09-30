#include <iostream>
#include <fstream>
#include <string>

using namespace std;

const int MAX = 1000000;
string meses[MAX], horas[MAX], ips[MAX], puertos[MAX], razones[MAX];
int dias[MAX];

int mesAEntero(string mes) {
    if (mes == "Jan") return 1; if (mes == "Feb") return 2;
    if (mes == "Mar") return 3; if (mes == "Apr") return 4;
    if (mes == "May") return 5; if (mes == "Jun") return 6;
    if (mes == "Jul") return 7; if (mes == "Aug") return 8;
    if (mes == "Sep") return 9; if (mes == "Oct") return 10;
    if (mes == "Nov") return 11; if (mes == "Dec") return 12;
    return 0;
}

int horaAEntero(string h) {
    int hh = stoi(h.substr(0,2));
    int mm = stoi(h.substr(3,2));
    int ss = stoi(h.substr(6,2));
    return hh*3600 + mm*60 + ss;
}

int leerArchivo(string nombreArchivo) {
    ifstream archivo(nombreArchivo);
    if (!archivo.is_open()) {
        cout << "No se pudo abrir el archivo." << endl;
        return 0;
    }

    int contador = 0;
    string linea;
    while (contador < MAX && archivo >> meses[contador] >> dias[contador] >> horas[contador] >> ips[contador]) {
        // Separar puerto de IP
        int pos = ips[contador].find(':');
        string ip_solo = ips[contador].substr(0, pos);
        puertos[contador] = ips[contador].substr(pos + 1);

        // Leer el resto de la línea como razón
        getline(archivo, razones[contador]);

        contador++;
    }

    archivo.close();
    return contador; // cantidad de registros leídos
}


int main() {
    int n = leerArchivo("bitacora.txt");

    // Convertir meses a enteros
    int mesInt[MAX];
    for (int i = 0; i < n; i++) {
        mesInt[i] = mesAEntero(meses[i]);
    }

    // Convertir horas a segundos y a enteros para más facilidad
    int horasInt[MAX];
    for (int i = 0; i < n; i++) {
        horasInt[i] = horaAEntero(horas[i]);
    }

    return 0;
}