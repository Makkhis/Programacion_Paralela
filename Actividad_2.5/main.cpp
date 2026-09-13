#include <iostream>
#include <iomanip>
#include <omp.h>
#include <random>
#include <ctime>
#include <utility>

using namespace std;

class Ordenamientos {
public:

    static void llenarArregloParalelo(long long* arr, int n, long long minVal, long long maxVal) {
        #pragma omp parallel for schedule(static)
        for (int i = 0; i < n; i++) {
            thread_local mt19937 generador((unsigned int)time(NULL) + omp_get_thread_num() * 7919);
            uniform_int_distribution<long long> distribucion(minVal, maxVal);

            arr[i] = distribucion(generador);
        }
    }

    static void burbujaSecuencial(long long* arr, int n) {
        for (int i = 0; i < n - 1; i++) {
            for (int j = 0; j < n - 1 - i; j++) {
                if (arr[j] > arr[j + 1]) {
                    swap(arr[j], arr[j + 1]);
                }
            }
        }
    }

    static void burbujaParaleloOddEven(long long* arr, int n) {
        #pragma omp parallel
        {
            #pragma omp single
            {
                cout << "  (Ordenamiento paralelo iniciado con " << omp_get_num_threads() << " hilos)\n";
            }

            for (int fase = 0; fase < n; fase++) {
                if (fase % 2 == 0) {
                    #pragma omp for
                    for (int i = 0; i < n - 1; i += 2) {
                        if (arr[i] > arr[i + 1]) swap(arr[i], arr[i + 1]);
                    }
                } else {
                    #pragma omp for
                    for (int i = 1; i < n - 1; i += 2) {
                        if (arr[i] > arr[i + 1]) swap(arr[i], arr[i + 1]);
                    }
                }
            }
        }
    }

    static void fusionar(long long* arr, long long* temp, int inicio, int medio, int fin) {
        int i = inicio, j = medio + 1, k = inicio;

        while (i <= medio && j <= fin) {
            if (arr[i] <= arr[j]) temp[k++] = arr[i++];
            else temp[k++] = arr[j++];
        }
        while (i <= medio) temp[k++] = arr[i++];
        while (j <= fin) temp[k++] = arr[j++];

        for (int x = inicio; x <= fin; x++) arr[x] = temp[x];
    }

    static void mergeSortSecuencialRec(long long* arr, long long* temp, int inicio, int fin) {
        if (inicio >= fin) return;

        int medio = inicio + (fin - inicio) / 2;
        mergeSortSecuencialRec(arr, temp, inicio, medio);
        mergeSortSecuencialRec(arr, temp, medio + 1, fin);
        fusionar(arr, temp, inicio, medio, fin);
    }

    static void mergeSortSecuencial(long long* arr, int n) {
        long long* temp = new long long[n];
        mergeSortSecuencialRec(arr, temp, 0, n - 1);
        delete[] temp;
    }

    static const int UMBRAL_TAREA = 5000;

    static void mergeSortParaleloRec(long long* arr, long long* temp, int inicio, int fin) {
        if (inicio >= fin) return;

        if (fin - inicio < UMBRAL_TAREA) {
            mergeSortSecuencialRec(arr, temp, inicio, fin);
            return;
        }

        int medio = inicio + (fin - inicio) / 2;

        #pragma omp task shared(arr, temp)
        mergeSortParaleloRec(arr, temp, inicio, medio);

        #pragma omp task shared(arr, temp)
        mergeSortParaleloRec(arr, temp, medio + 1, fin);

        #pragma omp taskwait
        fusionar(arr, temp, inicio, medio, fin);
    }

    static void mergeSortParalelo(long long* arr, int n) {
        long long* temp = new long long[n];

        #pragma omp parallel
        {
            #pragma omp single
            {
                cout << "  (Ordenamiento paralelo iniciado con " << omp_get_num_threads() << " hilos)\n";
                mergeSortParaleloRec(arr, temp, 0, n - 1);
            }
        }

        delete[] temp;
    }

    static bool estaOrdenado(long long* arr, int n) {
        for (int i = 0; i < n - 1; i++) {
            if (arr[i] > arr[i + 1]) return false;
        }
        return true;
    }
};

const int LIMITE_IMPRESION = 200;

void imprimirArreglo(const string& nombre, long long* arr, int n) {
    cout << nombre << ": [ ";
    for (int i = 0; i < n; i++) cout << arr[i] << " ";
    cout << "]" << endl;
}


long long* copiarArreglo(long long* original, int n) {
    long long* copia = new long long[n];
    for (int i = 0; i < n; i++) copia[i] = original[i];
    return copia;
}

void mostrarTiempo(const string& etiqueta, double tiempo) {
    cout << fixed << setprecision(6);
    cout << etiqueta << ": " << tiempo << " segundos\n";
}

int main() {
    cout << "Avila Martinez Marco | Larios Hernandez Carlos Alberto | Macias Renteria Dante Yael | Salcedo Ramos Luis Gael\n";

    long long* original = nullptr;
    int tamano = 0;

    double tLlenado = -1;
    double tIterSec = -1, tIterPar = -1;
    double tRecSec = -1, tRecPar = -1;

    int opcion;

    do {
        cout << "\n===== MENU ORDENAMIENTOS (OpenMP) =====\n";
        cout << "1. Llenar arreglo\n";
        cout << "2. Ordenamiento iterativo SECUENCIAL (Burbuja)\n";
        cout << "3. Ordenamiento iterativo PARALELO (Burbuja Odd-Even)\n";
        cout << "4. Ordenamiento recursivo SECUENCIAL (Merge Sort)\n";
        cout << "5. Ordenamiento recursivo PARALELO (Merge Sort con tareas)\n";
        cout << "6. Mostrar comparativa de tiempos y speedup\n";
        cout << "7. Salir\n";
        cout << "Seleccione una opcion: ";
        cin >> opcion;

        switch (opcion) {
            case 1: {
                cout << "Ingrese el tamano del arreglo: ";
                cin >> tamano;

                long long minVal, maxVal;
                cout << "Ingrese el valor minimo del rango: ";
                cin >> minVal;
                cout << "Ingrese el valor maximo del rango: ";
                cin >> maxVal;

                if (tamano <= 0 || minVal > maxVal) {
                    cout << "Datos invalidos.\n";
                    break;
                }

                delete[] original;
                original = new long long[tamano];

                tLlenado = -1; tIterSec = -1; tIterPar = -1; tRecSec = -1; tRecPar = -1;

                double inicio = omp_get_wtime();
                Ordenamientos::llenarArregloParalelo(original, tamano, minVal, maxVal);
                double fin = omp_get_wtime();
                tLlenado = fin - inicio;

                cout << "Arreglo generado (tamano = " << tamano << ", rango [" << minVal << ", " << maxVal << "]).\n";
                mostrarTiempo("Tiempo de llenado en paralelo", tLlenado);

                if (tamano <= LIMITE_IMPRESION) {
                    imprimirArreglo("Arreglo desordenado", original, tamano);
                } else {
                    cout << "El arreglo es demasiado grande para mostrarse en consola.\n";
                }
                break;
            }

            case 2: {
                if (original == nullptr) { cout << "Primero debe llenar el arreglo (opcion 1).\n"; break; }

                long long* trabajo = copiarArreglo(original, tamano);

                double inicio = omp_get_wtime();
                Ordenamientos::burbujaSecuencial(trabajo, tamano);
                double fin = omp_get_wtime();
                tIterSec = fin - inicio;

                cout << "Ordenado correctamente: " << (Ordenamientos::estaOrdenado(trabajo, tamano) ? "SI" : "NO") << endl;
                mostrarTiempo("Tiempo (burbuja secuencial)", tIterSec);
                if (tamano <= LIMITE_IMPRESION) imprimirArreglo("Arreglo ordenado", trabajo, tamano);

                delete[] trabajo;
                break;
            }

            case 3: {
                if (original == nullptr) { cout << "Primero debe llenar el arreglo (opcion 1).\n"; break; }

                long long* trabajo = copiarArreglo(original, tamano);

                double inicio = omp_get_wtime();
                Ordenamientos::burbujaParaleloOddEven(trabajo, tamano);
                double fin = omp_get_wtime();
                tIterPar = fin - inicio;

                cout << "Ordenado correctamente: " << (Ordenamientos::estaOrdenado(trabajo, tamano) ? "SI" : "NO") << endl;
                mostrarTiempo("Tiempo (burbuja paralelo Odd-Even)", tIterPar);
                if (tamano <= LIMITE_IMPRESION) imprimirArreglo("Arreglo ordenado", trabajo, tamano);

                delete[] trabajo;
                break;
            }

            case 4: {
                if (original == nullptr) { cout << "Primero debe llenar el arreglo (opcion 1).\n"; break; }

                long long* trabajo = copiarArreglo(original, tamano);

                double inicio = omp_get_wtime();
                Ordenamientos::mergeSortSecuencial(trabajo, tamano);
                double fin = omp_get_wtime();
                tRecSec = fin - inicio;

                cout << "Ordenado correctamente: " << (Ordenamientos::estaOrdenado(trabajo, tamano) ? "SI" : "NO") << endl;
                mostrarTiempo("Tiempo (merge sort secuencial)", tRecSec);
                if (tamano <= LIMITE_IMPRESION) imprimirArreglo("Arreglo ordenado", trabajo, tamano);

                delete[] trabajo;
                break;
            }

            case 5: {
                if (original == nullptr) { cout << "Primero debe llenar el arreglo (opcion 1).\n"; break; }

                long long* trabajo = copiarArreglo(original, tamano);

                double inicio = omp_get_wtime();
                Ordenamientos::mergeSortParalelo(trabajo, tamano);
                double fin = omp_get_wtime();
                tRecPar = fin - inicio;

                cout << "Ordenado correctamente: " << (Ordenamientos::estaOrdenado(trabajo, tamano) ? "SI" : "NO") << endl;
                mostrarTiempo("Tiempo (merge sort paralelo con tareas)", tRecPar);
                if (tamano <= LIMITE_IMPRESION) imprimirArreglo("Arreglo ordenado", trabajo, tamano);

                delete[] trabajo;
                break;
            }

            case 6: {
                cout << "\n===== COMPARATIVA DE TIEMPOS =====\n";
                if (tLlenado >= 0) mostrarTiempo("Llenado", tLlenado);

                if (tIterSec >= 0) mostrarTiempo("Burbuja secuencial", tIterSec);
                if (tIterPar >= 0) mostrarTiempo("Burbuja paralelo", tIterPar);
                if (tIterSec > 0 && tIterPar > 0) {
                    cout << fixed << setprecision(3);
                    cout << "Speedup burbuja (secuencial / paralelo): " << (tIterSec / tIterPar) << "x\n";
                }

                if (tRecSec >= 0) mostrarTiempo("Merge sort secuencial", tRecSec);
                if (tRecPar >= 0) mostrarTiempo("Merge sort paralelo", tRecPar);
                if (tRecSec > 0 && tRecPar > 0) {
                    cout << fixed << setprecision(3);
                    cout << "Speedup merge sort (secuencial / paralelo): " << (tRecSec / tRecPar) << "x\n";
                }

                if (tIterSec < 0 && tRecSec < 0) {
                    cout << "Aun no se ha ejecutado ningun ordenamiento.\n";
                }
                break;
            }

            case 7:
                cout << "Saliendo del programa...\n";
                break;

            default:
                cout << "Opcion invalida, intente de nuevo.\n";
        }

    } while (opcion != 7);

    delete[] original;

    cout << "Avila Martinez Marco | Larios Hernandez Carlos Alberto | Macias Renteria Dante Yael | Salcedo Ramos Luis Gael\n";
    return 0;
}
