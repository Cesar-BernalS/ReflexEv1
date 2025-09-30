// Autores: César Tadeo Bernal Sauceda, Regina Aguilar García, María Fernanda García Bushbeck
// Fecha: 29/09/2025
// Descripción: Programa que lee una bitácora, ordena los registros por fecha y permite buscar registros en un rango de fechas usando búsquedas binarias

#include <iostream>
#include <fstream>
#include <string>

using namespace std;

const int MAX = 20000;

// Arreglos globales para almacenar los datos de la bitácora
string meses[MAX], horas[MAX], ips[MAX], puertos[MAX], razones[MAX];
int dias[MAX];
int mesInt[MAX], horasInt[MAX]; 

// Convierte el nombre del mes a su equivalente entero (Jan=1, Dec=12)
// Parámetro: mes - nombre del mes en formato string
// Retorno: número entero del mes
// Complejidad: O(1)
int mesAEntero(string mes) {
    if (mes == "Jan") return 1; if (mes == "Feb") return 2;
    if (mes == "Mar") return 3; if (mes == "Apr") return 4;
    if (mes == "May") return 5; if (mes == "Jun") return 6;
    if (mes == "Jul") return 7; if (mes == "Aug") return 8;
    if (mes == "Sep") return 9; if (mes == "Oct") return 10;
    if (mes == "Nov") return 11; if (mes == "Dec") return 12;
    return 0;
}

// Convierte una hora en formato HH:MM:SS a segundos totales
// Parámetro: h - hora en formato string
// Retorno: número de segundos desde las 00:00:00
// Complejidad: O(1)
int horaAEntero(string h) {
    int hh = stoi(h.substr(0,2));
    int mm = stoi(h.substr(3,2));
    int ss = stoi(h.substr(6,2));
    return hh*3600 + mm*60 + ss;
}

// Lee el archivo de bitácora y almacena los datos en los arreglos globales
// Parámetro: nombreArchivo - ruta del archivo de bitácora
// Retorno: cantidad de registros leídos
// Complejidad: O(n)
int leerArchivo(string nombreArchivo) {
    ifstream archivo(nombreArchivo);
    if (!archivo.is_open()) {
        cout << "No se pudo abrir el archivo." << endl;
        return 0;
    }

    int contador = 0;
    string linea;
    while (contador < MAX && archivo >> meses[contador] >> dias[contador] >> horas[contador] >> ips[contador]) {
        int pos = ips[contador].find(':');
        if (pos != string::npos) {
            puertos[contador] = ips[contador].substr(pos + 1);
            ips[contador] = ips[contador].substr(0, pos);
        } else {
            puertos[contador] = "";
        }

        archivo.ignore(); // Ignorar salto de línea residual
        getline(archivo >> ws, razones[contador]);
        contador++;
    }

    archivo.close();
    return contador;
}

// Intercambia dos registros completos en los arreglos globales
// Parámetros: i, j - índices de los registros a intercambiar
// Retorno: ninguno
// Complejidad: O(1)
void swap(int i, int j) {
    int tempMes = mesInt[i]; mesInt[i] = mesInt[j]; mesInt[j] = tempMes;
    int tempDia = dias[i]; dias[i] = dias[j]; dias[j] = tempDia;
    int tempHora = horasInt[i]; horasInt[i] = horasInt[j]; horasInt[j] = tempHora;

    string tempMesStr = meses[i]; meses[i] = meses[j]; meses[j] = tempMesStr;
    string tempHoraStr = horas[i]; horas[i] = horas[j]; horas[j] = tempHoraStr;
    string tempIp = ips[i]; ips[i] = ips[j]; ips[j] = tempIp;
    string tempPuerto = puertos[i]; puertos[i] = puertos[j]; puertos[j] = tempPuerto;
    string tempRazon = razones[i]; razones[i] = razones[j]; razones[j] = tempRazon;
}

// Ordena los registros de la bitácora usando Insertion Sort por mes, día y hora
// Parámetro: n - número de registros
// Retorno: ninguno
// Complejidad: O(n^2)
void insertionSort(int n) {
    for(int i = 1; i < n; i++){
        for(int j = i-1; j >= 0; j--){
            if(mesInt[j+1] < mesInt[j] ||
            (mesInt[j+1] == mesInt[j] && dias[j+1] < dias[j]) ||
            (mesInt[j+1] == mesInt[j] && dias[j+1] == dias[j] && horasInt[j+1] < horasInt[j])) {
                swap(j, j+1);
            } else{
                break;
            }
        }
    }
}

// Convierte fecha completa en clave única de segundos para comparaciones
// Parámetros: mes, dia, horaSegundos
// Retorno: clave numérica
// Complejidad: O(1)
int fechaAClave(int mes, int dia, int horaSegundos) {
    return ((mes * 31 + dia) * 86400) + horaSegundos;
}

// Búsqueda binaria para encontrar el primer registro >= fechaClave
// Parámetros: n - número de registros, fechaClave - clave de la fecha a buscar
// Retorno: índice del primer registro que cumple la condición
// Complejidad: O(log n)
int busquedaBinariaInicio(int n, int fechaClave) {
    int izquierda = 0, derecha = n-1, res = n;
    while(izquierda <= derecha) {
        int medio = (izquierda + derecha)/2;
        int claveMedio = fechaAClave(mesInt[medio], dias[medio], horasInt[medio]);
        if(claveMedio >= fechaClave) {
            res = medio;
            derecha = medio - 1;
        } else {
            izquierda = medio + 1;
        }
    }
    return res;
}

// Búsqueda binaria para encontrar el último registro <= fechaClave
// Parámetros: n - número de registros, fechaClave - clave de la fecha a buscar
// Retorno: índice del último registro que cumple la condición
// Complejidad: O(log n)
int busquedaBinariaFin(int n, int fechaClave) {
    int izquierda = 0, derecha = n-1, res = -1;
    while(izquierda <= derecha) {
        int medio = (izquierda + derecha)/2;
        int claveMedio = fechaAClave(mesInt[medio], dias[medio], horasInt[medio]);
        if(claveMedio <= fechaClave) {
            res = medio;
            izquierda = medio + 1;
        } else {
            derecha = medio - 1;
        }
    }
    return res;
}

// Función principal
int main() {
    int n = leerArchivo("bitacora.txt");
    if(n == 0) return 0;

    // Convertir meses y horas a enteros
    for(int i = 0; i < n; i++){
        mesInt[i] = mesAEntero(meses[i]);
        horasInt[i] = horaAEntero(horas[i]);
    }

    // Ordenar bitácora
    insertionSort(n);

    // Pedir fechas al usuario
    string mesInicio, mesFin;
    int diaInicio, diaFin;

    cout << "Ingrese la fecha de inicio (ejemplo: Aug 4):\n";
    cin >> mesInicio >> diaInicio;

    cout << "Ingrese la fecha de fin (ejemplo: Sep 21):\n";
    cin >> mesFin >> diaFin;

    int mesInicioInt = mesAEntero(mesInicio);
    int mesFinInt = mesAEntero(mesFin);
    if(mesInicioInt == 0 || mesFinInt == 0){
        cout << "Error: mes inválido.\n";
        return 0;
    }

    // Claves completas con hora fija
    int fechaInicioClave = fechaAClave(mesInicioInt, diaInicio, 0);        // 00:00:00
    int fechaFinClave    = fechaAClave(mesFinInt, diaFin, 86399);          // 23:59:59

    if(fechaInicioClave > fechaFinClave){
        cout << "Error: la fecha de inicio es mayor que la de fin.\n";
        return 0;
    }

    // Búsqueda binaria
    int inicioIdx = busquedaBinariaInicio(n, fechaInicioClave);
    int finIdx    = busquedaBinariaFin(n, fechaFinClave);

    // Validación de rango
    if(inicioIdx == n || finIdx == -1 || inicioIdx > finIdx){
        cout << "No hay registros en el rango ingresado.\n";
        return 0;
    }

    // Guardar resultados en archivo
    ofstream archivoSalida("resultado.txt");
    cout << "\nRegistros en el rango:\n";
    for(int i = inicioIdx; i <= finIdx; i++){
        cout << meses[i] << " " << dias[i] << " " << horas[i] 
            << " " << ips[i] << ":" << puertos[i] << " " << razones[i] << "\n";
        archivoSalida << meses[i] << " " << dias[i] << " " << horas[i] 
                    << " " << ips[i] << ":" << puertos[i] << " " << razones[i] << "\n";
    }
    archivoSalida.close();
    cout << "\nResultados guardados en 'resultado.txt'\n";

    return 0;
}
