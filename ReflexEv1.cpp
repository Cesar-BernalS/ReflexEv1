/* César Tadeo Bernal Saucedo
Regina Aguilar García
María Fernanda García Bushbeck
*/

#include <iostream>
#include <fstream>
#include <string>

using namespace std;

const int MAX = 1000000;

// Arreglos globales
string meses[MAX], horas[MAX], ips[MAX], puertos[MAX], razones[MAX];
int dias[MAX];
int mesInt[MAX], horasInt[MAX]; 

// Convertir datos a enteros
int mesAEntero(string mes) {
    if (mes == "Jan") return 1; if (mes == "Feb") return 2;
    if (mes == "Mar") return 3; if (mes == "Apr") return 4;
    if (mes == "May") return 5; if (mes == "Jun") return 6;
    if (mes == "Jul") return 7; if (mes == "Aug") return 8;
    if (mes == "Sep") return 9; if (mes == "Oct") return 10;
    if (mes == "Nov") return 11; if (mes == "Dec") return 12;
    return 0;
}

 // Convertir horas a segundos y a enteros para más facilidad
    int horaAEntero(string h) {
    int hh = stoi(h.substr(0,2));
    int mm = stoi(h.substr(3,2));
    int ss = stoi(h.substr(6,2));
    return hh*3600 + mm*60 + ss;
}

// Método para leer archivo
int leerArchivo(string nombreArchivo) {
    ifstream archivo(nombreArchivo);
    if (!archivo.is_open()) {
        cout << "No se pudo abrir el archivo." << endl;
        return 0;
    }

    int contador = 0;
    string linea;
    while (contador < MAX && (archivo >> meses[contador] >> dias[contador] >> horas[contador] >> ips[contador])) {
        // Separar puerto de IP
        int pos = ips[contador].find(':');
        if (pos != string::npos) {
            puertos[contador] = ips[contador].substr(pos + 1);
            ips[contador] = ips[contador].substr(0, pos);
        } else {
            puertos[contador] = "";
        }

        // Leer el resto de la línea como razón
        getline(archivo, razones[contador]);

        // Llenar auxiliares
        mesInt[contador]   = mesAEntero(meses[contador]);
        horasInt[contador] = horaAEntero(horas[contador]);

        contador++;
    }

    archivo.close();
    return contador; // cantidad de registros leídos
}

// Comparar los datos por índice
inline bool esMenorIdx(int i, int j){
    if (mesInt[i] != mesInt[j]) return mesInt[i] < mesInt[j];
    if (dias[i]   != dias[j])   return dias[i]   < dias[j];
    return horasInt[i] < horasInt[j];
}

void mergeIdx(int idx[], int temp[], int l, int m, int r){ 
    int i = l, j = m + 1, k = l;
    while (i <= m && j <= r) {
        if (esMenorIdx(idx[i], idx[j])) {
            temp[k++] = idx[i++];
        } else {
            temp[k++] = idx[j++];
        }
    }
    while (i <= m) {
        temp[k++] = idx[i++];
    }
    while (j <= r) {
        temp[k++] = idx[j++];
    }
    for (int t = l; t <= r; ++t) {
        idx[t] = temp[t];
    }
}

// Merge sort recursivo
void mergeSortIdxRec(int idx[], int temp[], int l, int r) {
    if (l >= r) return;
    int m = l + (r - l)/2;
    mergeSortIdxRec(idx, temp, l, m);
    mergeSortIdxRec(idx, temp, m+1, r);
    mergeIdx(idx, temp, l, m, r);
}

void ordenar(int idx[], int n) {
    static int temp[MAX];           
    mergeSortIdxRec(idx, temp, 0, n-1);
}

int main(){
    int n = leerArchivo("bitacora.txt");

    int idx[MAX];
    for (int i = 0; i < n; ++i) idx[i] = i;

    ordenar(idx, n);
    
    // Impresión del ordenamiento
    for (int k = 0; k < n; ++k) {
    int i = idx[k]; 
    cout << meses[i] << " " 
        << dias[i] << " " 
        << horas[i] << " "
        << ips[i] << ":" << puertos[i] << " "
        << razones[i] << "\n";
}
    return 0;
}