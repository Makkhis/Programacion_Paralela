#include <iostream>
#include <iomanip>
#include <omp.h>
#include <cstring>

using namespace std;

struct ResultadoBusqueda {
    bool encontrada;
    long long combinacionesRevisadas;
    double tiempo;
    long long indiceEncontrado;
    int hiloGanador;
    int numHilos;
};

class BusquedaClave {
public:
    static char* crearAlfabeto() {
        char* alfabeto = new char[37];
        int idx = 0;
        for (char c = 'A'; c <= 'Z'; c++) alfabeto[idx++] = c;
        for (char c = '0'; c <= '9'; c++) alfabeto[idx++] = c;
        alfabeto[36] = '\0';
        return alfabeto;
    }

    static long long calcularEspacioBusqueda(int longitud, int baseAlfabeto) {
        long long total = 1;
        for (int i = 0; i < longitud; i++) {
            total *= baseAlfabeto;
        }
        return total;
    }

    static void indiceACombinacion(long long indice, int longitud, char* alfabeto, int baseAlfabeto, char* resultado) {
        for (int pos = longitud - 1; pos >= 0; pos--) {
            resultado[pos] = alfabeto[indice % baseAlfabeto];
            indice /= baseAlfabeto;
        }
        resultado[longitud] = '\0';
    }

    static bool validarClave(const string& clave, int longitudEsperada) {
        if (clave.empty()) {
            cout << "Error: la clave no puede estar vacia.\n";
            return false;
        }
        if ((int)clave.size() != longitudEsperada) {
            cout << "Error: la clave debe tener exactamente " << longitudEsperada << " caracteres (tiene " << clave.size() << ").\n";
            return false;
        }
        for (char c : clave) {
            bool esLetra = (c >= 'A' && c <= 'Z');
            bool esDigito = (c >= '0' && c <= '9');
            if (!esLetra && !esDigito) {
                cout << "Error: la clave solo puede contener letras MAYUSCULAS (A-Z) y digitos (0-9), sin espacios ni otros caracteres.\n";
                return false;
            }
        }
        return true;
    }

    static ResultadoBusqueda buscarSecuencial(const char* clave, int longitud, long long espacio, char* alfabeto, int baseAlfabeto) {
        ResultadoBusqueda r;
        r.encontrada = false;
        r.indiceEncontrado = -1;
        r.hiloGanador = -1;
        r.numHilos = 1;

        char* combinacion = new char[longitud + 1];
        long long revisadas = 0;

        double inicio = omp_get_wtime();

        for (long long i = 0; i < espacio; i++) {
            indiceACombinacion(i, longitud, alfabeto, baseAlfabeto, combinacion);
            revisadas++;

            if (strcmp(combinacion, clave) == 0) {
                r.encontrada = true;
                r.indiceEncontrado = i;
                break;
            }
        }

        double fin = omp_get_wtime();

        r.combinacionesRevisadas = revisadas;
        r.tiempo = fin - inicio;

        delete[] combinacion;
        return r;
    }

    static ResultadoBusqueda buscarParalelo(const char* clave, int longitud, long long espacio, char* alfabeto, int baseAlfabeto, int hilosSolicitados, bool mostrarDetalle) {
        if (hilosSolicitados > 0) {
            omp_set_num_threads(hilosSolicitados);
        }

        bool encontradaGlobal = false;
        long long indiceEncontrado = -1;
        int hiloGanador = -1;
        long long totalRevisadas = 0;
        int hilosReales = 0;

        double inicio = omp_get_wtime();

        #pragma omp parallel reduction(+:totalRevisadas)
        {
            int idHilo = omp_get_thread_num();
            int numHilos = omp_get_num_threads();

            #pragma omp single
            {
                hilosReales = numHilos;
            }

            long long base = espacio / numHilos;
            long long resto = espacio % numHilos;

            long long inicioHilo, finHilo; // [inicioHilo, finHilo)
            if (idHilo < resto) {
                inicioHilo = idHilo * (base + 1);
                finHilo = inicioHilo + (base + 1);
            } else {
                inicioHilo = resto * (base + 1) + (idHilo - resto) * base;
                finHilo = inicioHilo + base;
            }
            long long cantidadAsignada = finHilo - inicioHilo;

            if (mostrarDetalle) {
                #pragma omp critical(impresion)
                {
                    cout << "Hilo " << idHilo << " -> Inicio: " << inicioHilo
                         << " -> Fin: " << (finHilo - 1)
                         << " -> Cantidad: " << cantidadAsignada << "  [INICIA]" << endl;
                }
            }

            char* combinacionLocal = new char[longitud + 1];
            long long revisadasLocal = 0;
            bool encontroEsteHilo = false;

            for (long long i = inicioHilo; i < finHilo; i++) {
                bool otroHiloEncontro;
                #pragma omp atomic read
                otroHiloEncontro = encontradaGlobal;

                if (otroHiloEncontro) break;

                indiceACombinacion(i, longitud, alfabeto, baseAlfabeto, combinacionLocal);
                revisadasLocal++;

                if (strcmp(combinacionLocal, clave) == 0) {
                    #pragma omp critical(resultado)
                    {
                        if (!encontradaGlobal) {
                            encontradaGlobal = true;
                            indiceEncontrado = i;
                            hiloGanador = idHilo;
                        }
                    }
                    encontroEsteHilo = true;
                    break;
                }
            }

            totalRevisadas += revisadasLocal;

            if (mostrarDetalle) {
                #pragma omp critical(impresion)
                {
                    cout << "Hilo " << idHilo << " -> [FINALIZA] Reviso " << revisadasLocal
                         << " combinaciones. Encontro la clave: " << (encontroEsteHilo ? "SI" : "NO") << endl;
                }
            }

            delete[] combinacionLocal;
        }

        double fin = omp_get_wtime();

        ResultadoBusqueda r;
        r.encontrada = encontradaGlobal;
        r.indiceEncontrado = indiceEncontrado;
        r.hiloGanador = hiloGanador;
        r.combinacionesRevisadas = totalRevisadas;
        r.tiempo = fin - inicio;
        r.numHilos = hilosReales;
        return r;
    }
};

void mostrarTiempo(const string& etiqueta, double tiempo) {
    cout << fixed << setprecision(6);
    cout << etiqueta << ": " << tiempo << " segundos\n";
}

int main() {
    cout << "Avila Martinez Marco | Larios Hernandez Carlos Alberto | Macias Renteria Dante Yael | Salcedo Ramos Luis Gael\n";
    const int BASE_ALFABETO = 36;
    char* alfabeto = BusquedaClave::crearAlfabeto();

    int longitud = 0;
    long long espacio = 0;
    char* claveActual = nullptr;

    ResultadoBusqueda resSecuencial; resSecuencial.encontrada = false; resSecuencial.tiempo = -1;
    ResultadoBusqueda resParalelo;   resParalelo.encontrada = false;   resParalelo.tiempo = -1;

    int opcion;

    do {
        cout << "\n===== MENU BUSQUEDA EXHAUSTIVA (OpenMP) =====\n";
        cout << "1. Configurar longitud y clave de prueba\n";
        cout << "2. Ejecutar busqueda SECUENCIAL\n";
        cout << "3. Ejecutar busqueda PARALELA\n";
        cout << "4. Comparar resultados\n";
        cout << "5. Salir\n";
        cout << "Seleccione una opcion: ";
        cin >> opcion;

        switch (opcion) {
            case 1: {
                cout << "Ingrese la longitud de la clave de prueba: ";
                cin >> longitud;

                if (longitud <= 0) {
                    cout << "La longitud debe ser mayor que 0.\n";
                    break;
                }

                espacio = BusquedaClave::calcularEspacioBusqueda(longitud, BASE_ALFABETO);
                cout << "Espacio de busqueda: " << BASE_ALFABETO << "^" << longitud
                     << " = " << espacio << " combinaciones posibles.\n";

                delete[] claveActual;
                claveActual = new char[longitud + 1];

                string entrada;
                bool valida = false;
                do {
                    cout << "Ingrese la clave de prueba (" << longitud << " caracteres, A-Z y 0-9, en MAYUSCULAS): ";
                    cin >> entrada;
                    valida = BusquedaClave::validarClave(entrada, longitud);
                } while (!valida);

                strcpy(claveActual, entrada.c_str());

                resSecuencial.tiempo = -1;
                resParalelo.tiempo = -1;

                cout << "Clave de prueba configurada correctamente: " << claveActual << "\n";
                break;
            }

            case 2: {
                if (claveActual == nullptr) { cout << "Primero configure la longitud y la clave (opcion 1).\n"; break; }

                cout << "\n--- BUSQUEDA SECUENCIAL ---\n";
                cout << "Inicio de la busqueda...\n";

                resSecuencial = BusquedaClave::buscarSecuencial(claveActual, longitud, espacio, alfabeto, BASE_ALFABETO);

                cout << "Fin de la busqueda.\n";
                cout << "Combinaciones revisadas: " << resSecuencial.combinacionesRevisadas << "\n";

                if (resSecuencial.encontrada) {
                    char* combinacion = new char[longitud + 1];
                    BusquedaClave::indiceACombinacion(resSecuencial.indiceEncontrado, longitud, alfabeto, BASE_ALFABETO, combinacion);
                    cout << "Clave encontrada: " << combinacion << "\n";
                    delete[] combinacion;
                } else {
                    cout << "Clave NO encontrada dentro del espacio de busqueda.\n";
                }

                mostrarTiempo("Tiempo secuencial", resSecuencial.tiempo);
                break;
            }

            case 3: {
                if (claveActual == nullptr) { cout << "Primero configure la longitud y la clave (opcion 1).\n"; break; }

                int hilos;
                cout << "Numero de hilos a utilizar (0 = usar el maximo disponible): ";
                cin >> hilos;

                char mostrarDetalleChar;
                cout << "Desea ver el detalle de inicio/fin de cada hilo? (S/N): ";
                cin >> mostrarDetalleChar;
                bool mostrarDetalle = (mostrarDetalleChar == 'S' || mostrarDetalleChar == 's');

                cout << "\n--- BUSQUEDA PARALELA ---\n";
                cout << "Inicio de la busqueda...\n";

                resParalelo = BusquedaClave::buscarParalelo(claveActual, longitud, espacio, alfabeto, BASE_ALFABETO, hilos, mostrarDetalle);

                cout << "Fin de la busqueda.\n";
                cout << "Hilos utilizados: " << resParalelo.numHilos << "\n";
                cout << "Combinaciones revisadas (suma de todos los hilos): " << resParalelo.combinacionesRevisadas << "\n";

                if (resParalelo.encontrada) {
                    char* combinacion = new char[longitud + 1];
                    BusquedaClave::indiceACombinacion(resParalelo.indiceEncontrado, longitud, alfabeto, BASE_ALFABETO, combinacion);
                    cout << "Clave encontrada por el hilo: " << resParalelo.hiloGanador << "\n";
                    cout << "Combinacion encontrada: " << combinacion << "\n";
                    delete[] combinacion;
                } else {
                    cout << "Clave NO encontrada dentro del espacio de busqueda.\n";
                }

                mostrarTiempo("Tiempo paralelo", resParalelo.tiempo);
                break;
            }

            case 4: {
                cout << "\n===== COMPARATIVA DE RESULTADOS =====\n";
                if (claveActual != nullptr) cout << "Clave de prueba: " << claveActual << "  (longitud " << longitud << ")\n";
                cout << "Espacio de busqueda: " << espacio << " combinaciones\n";

                if (resSecuencial.tiempo >= 0) {
                    mostrarTiempo("Tiempo secuencial", resSecuencial.tiempo);
                } else {
                    cout << "Aun no se ha ejecutado la busqueda secuencial.\n";
                }

                if (resParalelo.tiempo >= 0) {
                    mostrarTiempo("Tiempo paralelo", resParalelo.tiempo);
                    cout << "Hilos utilizados: " << resParalelo.numHilos << "\n";
                    cout << "Hilo ganador: " << (resParalelo.encontrada ? to_string(resParalelo.hiloGanador) : "N/A") << "\n";
                } else {
                    cout << "Aun no se ha ejecutado la busqueda paralela.\n";
                }

                if (resSecuencial.tiempo > 0 && resParalelo.tiempo > 0) {
                    cout << fixed << setprecision(3);
                    cout << "Speedup (secuencial / paralelo): " << (resSecuencial.tiempo / resParalelo.tiempo) << "x\n";
                }
                break;
            }

            case 5:
                cout << "Saliendo del programa...\n";
                break;

            default:
                cout << "Opcion invalida, intente de nuevo.\n";
        }

    } while (opcion != 5);

    delete[] alfabeto;
    delete[] claveActual;

    cout << "Avila Martinez Marco | Larios Hernandez Carlos Alberto | Macias Renteria Dante Yael | Salcedo Ramos Luis Gael\n";
    return 0;
}
