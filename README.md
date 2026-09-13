
Gemini
Nuevo chat
Buscar chats
Estudiantes
Imágenes
Biblioteca
Nuevo cuaderno
Subir Code::Blocks a GitHub Desktop
Guía para Exposición Gerontológica
Cómo borrar cola de impresora
Memoria interna de MX Brio
Tarea de Demografía: Investigación y Formato
Resumen y Reflexión de Desarrollo Organizacional
Guía rápida para práctica de MPI y OpenMP
Historia de la Gastronomía
Línea de tiempo gastronómica
Algoritmos Bioinspirados: Hormigas y PSO
Añadir borde superior en Pygame
Definición de Vulnerabilidad en Ciberseguridad
Pistas para cita temática Ghibli y Japonesa
DBCA Template Definition Explained
Conversación con Gemini
tengo una carpeta con otras dos carpetas que son unas actividades que hice en codeblocks y quiero hacer un repositorio en github desde la aplicacion de github desktop como puedo hacerlo?

Puedes subir tu carpeta principal con sus dos subcarpetas a GitHub siguiendo estos pasos dentro de GitHub Desktop:

1. Agregar la carpeta principal a la aplicación

Abre GitHub Desktop y arrastra la carpeta que contiene tus dos actividades directamente sobre la ventana de la aplicación.

Alternativamente, ve al menú superior en File > Add Local Repository... y selecciona la carpeta principal.

La aplicación mostrará una advertencia indicando que la carpeta no es un repositorio de Git. Haz clic en el enlace azul create a repository.

2. Configurar el repositorio

Name: Asigna un nombre para tu proyecto en GitHub.

Git Ignore: Se recomienda seleccionar la plantilla C++. Esto evitará que se suban archivos compilados u objetos (.exe, .o, .depend) creados por Code::Blocks.

Haz clic en Create Repository.

3. Registrar los archivos (Commit)

En la esquina inferior izquierda (pestaña Changes), escribe un título breve en la casilla Summary (por ejemplo: Primer commit: Actividades CodeBlocks).

Haz clic en el botón Commit to main (o master).

4. Publicar en GitHub

Haz clic en el botón Publish repository ubicado en la barra de herramientas superior.

Marca o desmarca la opción Keep this code private según si prefieres que tu repositorio sea privado o público.

Haz clic en Publish Repository.

no me deja agregar lo que me dijiste de primer commit pero creo que ya se hizo un commit inicial y se subiuo todo

Así es, el commit inicial se generó automáticamente al crear el repositorio (como muestra el historial abajo: Initial commit). Como no hay cambios nuevos pendientes en la carpeta, la sección para hacer un nuevo commit se bloquea.

Para subir las carpetas a tu cuenta de GitHub en línea, solo falta publicarlo:

En la barra superior de GitHub Desktop, haz clic en el botón Publish repository (o presiona Ctrl + P).

Si quieres que el proyecto sea público, desmarca la casilla Keep this code private.

Haz clic en el botón azul Publish Repository.

Con este paso, la carpeta principal y las subcarpetas de tus actividades quedarán subidas a tu perfil de GitHub.

Agreguen un README que incluya:

Nombre de la práctica.

Integrantes.

Algoritmos seleccionados.

Directivas OpenMP utilizadas.

Instrucciones básicas para compilar y ejecutar., estos son los codigos esta es el 2.5 #include <iostream>

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

cout << " (Ordenamiento paralelo iniciado con " << omp_get_num_threads() << " hilos)\n";

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

cout << " (Ordenamiento paralelo iniciado con " << omp_get_num_threads() << " hilos)\n";

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

} y esta es del 2.6 #include <iostream>

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

<< " -> Cantidad: " << cantidadAsignada << " [INICIA]" << endl;

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

ResultadoBusqueda resParalelo; resParalelo.encontrada = false; resParalelo.tiempo = -1;



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

if (claveActual != nullptr) cout << "Clave de prueba: " << claveActual << " (longitud " << longitud << ")\n";

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

¿Qué respuesta es más útil?
Tu selección ayudará a Gemini a ofrecer respuestas más útiles. Es posible que la respuesta tarde unos segundos en cargarse.
Opción A
Aquí tienes el contenido formateado en Markdown para tu archivo README.md. Puedes copiar este texto y guardarlo en un archivo llamado README.md dentro de la carpeta principal de tu repositorio.

Markdown
# Práctica de Programación Paralela con OpenMP

Este repositorio contiene las implementaciones en C++ con OpenMP correspondientes a la **Actividad 2.5 (Algoritmos de Ordenamiento Paralelo)** y la **Actividad 2.6 (Búsqueda Exhaustiva Paralela)**.

---

## 👥 Integrantes

* **Ávila Martínez Marco**
* **Larios Hernández Carlos Alberto**
* **Macías Renteria Dante Yael**
* **Salcedo Ramos Luis Gael**

---

## 🧠 Algoritmos Seleccionados

### Actividad 2.5: Algoritmos de Ordenamiento
1. **Llenado Paralelo de Arreglos:** Generación paralela de números aleatorios con distribuciones independientes por hilo.
2. **Burbuja (Bubble Sort):**
   * **Secuencial:** Algoritmo de burbuja tradicional de orden $O(n^2)$.
   * **Paralelo (Odd-Even Sort / Par-Impar):** Descomposición del intercambio de burbuja en fases pares e impares ejecutadas concurrentemente por múltiples hilos.
3. **Mezcla (Merge Sort):**
   * **Secuencial:** Algoritmo recursivo $O(n \log n)$.
   * **Paralelo con Tareas (Task-based Merge Sort):** Creación dinámica de tareas OpenMP (`#pragma omp task`) para procesar sub-arreglos de manera paralela hasta alcanzar un umbral óptimo de tamaño (*cutoff*).

### Actividad 2.6: Búsqueda Exhaustiva (Fuerza Bruta)
1. **Búsqueda Secuencial de Claves:** Recorrido secuencial del espacio de búsqueda $36^N$ (combinación de letras mayúsculas A-Z y dígitos 0-9).
2. **Búsqueda Paralela con Cancelación Pronta:**
   * División equitativa del rango del espacio de búsqueda entre hilos.
   * Monitoreo atómico del estado global de hallazgo para detener la exploración de otros hilos al encontrar la clave objetivo.

---

## ⚡ Directivas y Funciones de OpenMP Utilizadas

| Directiva / Función | Descripción y Uso en el Código |
| :--- | :--- |
| `#pragma omp parallel` | Inicia una región paralela compartida entre múltiples hilos. |
| `#pragma omp parallel for` | Paraleliza el bucle `for` de llenado de arreglos distribuyendo las iteraciones. |
| `#pragma omp for` | Distribuye las iteraciones de las fases pares/impares entre los hilos del equipo. |
| `#pragma omp single` | Asegura que un bloque de código (como mensajes iniciales o el lanzamiento del Merge Sort) se ejecute por un único hilo. |
| `#pragma omp task` | Crea tareas independientes para la ordenación de las sub-listas de Merge Sort. |
| `#pragma omp taskwait` | Sincroniza las tareas hijo antes de realizar la fusión (*merge*) del sub-arreglo. |
| `#pragma omp reduction(+:variable)` | Suma acumulativa del total de combinaciones revisadas por cada hilo. |
| `#pragma omp critical(nombre)` | Exclusión mutua para imprimir el estado de los hilos y registrar al hilo ganador sin condiciones de carrera. |
| `#pragma omp atomic read` | Lectura atómica para verificar si otro hilo ya encontró la clave objetivo. |
| `omp_get_wtime()` | Medición precisa del tiempo de ejecución en segundos. |
| `omp_get_thread_num()` | Obtención del identificador del hilo actual. |
| `omp_get_num_threads()` | Obtención del número total de hilos activos. |
| `omp_set_num_threads()` | Configuración dinámica de la cantidad de hilos a utilizar. |

---

## 🛠️ Instrucciones de Compilación y Ejecución

### Requisitos Previos
* Compilador de C++ compatible con soporte para **OpenMP** (ej. `g++` / GCC, MinGW, Clang o MSVC).

---

### Opción A: Desde Terminal / Línea de Comandos (GCC / G++)

#### 1. Compilar y ejecutar la Actividad 2.5 (Ordenamientos)
```bash
g++ -fopenmp -O2 Actividad2_5.cpp -o Actividad2_5
./Actividad2_5
2. Compilar y ejecutar la Actividad 2.6 (Búsqueda Exhaustiva)
Bash
g++ -fopenmp -O2 Actividad2_6.cpp -o Actividad2_6
./Actividad2_6
Nota: En Windows (CMD o PowerShell con MinGW), sustituye ./Actividad2_x por Actividad2_x.exe.

Opción B: Desde Code::Blocks
Abre el proyecto o archivo .cpp en Code::Blocks.

Ve al menú superior: Project > Build options...

En la pestaña Compiler settings > Other compiler options, agrega:

Plaintext
-fopenmp
En la pestaña Linker settings > Other linker options, agrega:

Plaintext
-fopenmp
Haz clic en OK y presiona Build and run (F9).

Opción B
Python
readme_content = """# Prácticas de Programación Paralela con OpenMP

Este repositorio contiene las soluciones para las prácticas de programación paralela utilizando **C++** y la biblioteca **OpenMP**, enfocado en la comparación de rendimiento (tiempo de ejecución y *speedup*) entre implementaciones secuenciales y paralelas.

---

## 👥 Integrantes del Equipo

* **Ávila Martínez Marco**
* **Larios Hernández Carlos Alberto**
* **Macías Rentería Dante Yael**
* **Salcedo Ramos Luis Gael**

---

## 📁 Estructura del Repositorio

El repositorio se divide en dos actividades / prácticas principales:

### 1. Práctica 2.5: Algoritmos de Ordenamiento Paralelos
Comparativa de rendimiento entre algoritmos de ordenamiento iterativos y recursivos en sus versiones secuenciales y paralelas.

* **Generación de Datos en Paralelo:** Llenado distribuido de un arreglo aleatorio utilizando generadores de números aleatorios *thread-local*.
* **Burbuja Secuencial vs. Burbuja Paralela (Odd-Even Sort):** Algoritmo de ordenamiento por transposición par-impar paralelizado por fases.
* **Merge Sort Secuencial vs. Merge Sort Paralelo con Tareas:** Implementación recursiva paralelizada mediante tareas de OpenMP (`#pragma omp task`) con un umbral de secuencialización para evitar el sobrecosto (*overhead*) del sistema de hilos.

### 2. Práctica 2.6: Búsqueda Exhaustiva de Claves Alfanuméricas (Fuerza Bruta)
Implementación y benchmarking de una búsqueda por fuerza bruta sobre un espacio de claves alfanuméricas en base 36 (`A-Z`, `0-9`).

* **Búsqueda Secuencial:** Recorrido iterativo simple de todo el espacio de combinaciones.
* **Búsqueda Paralela con Parada Temprana:** División equilibrada del espacio de búsqueda entre los hilos asignados, incorporando detección de hallazgo mediante lecturas atómicas para detener el resto de los hilos de manera anticipada.

---

## ⚡ Directivas y Funciones de OpenMP Utilizadas

A lo largo de ambos programas se emplean las siguientes directivas, cláusulas y funciones de la API de OpenMP:

| Directiva / Función | Descripción y Uso en las Prácticas |
| :--- | :--- |
| `#pragma omp parallel` | Define una región paralela donde se crea un equipo de hilos. |
| `#pragma omp parallel for` | Paraleliza un bucle `for` dividiendo sus iteraciones entre los hilos (utilizado en el llenado del arreglo). |
| `#pragma omp for` | Distribuye las iteraciones de los bucles de ordenamiento par e impar entre hilos dentro de un bloque paralelo existente. |
| `#pragma omp single` | Garantiza que un bloque de código sea ejecutado por un solo hilo (usado para iniciar tareas e imprimir avisos). |
| `#pragma omp task` | Crea una tarea asíncrona dentro del proceso de ordenamiento recursivo (Merge Sort). |
| `#pragma omp taskwait` | Sincroniza la ejecución de las tareas hijas antes de fusionar subarreglos en Merge Sort. |
| `#pragma omp atomic read` | Realiza lecturas atómicas de variables compartidas (`encontradaGlobal`) para la parada temprana sin bloqueo. |
| `#pragma omp critical` / `critical(nombre)` | Garantiza exclusión mutua para evitar condiciones de carrera al guardar el hilo ganador o imprimir en pantalla. |
| `reduction(+:variable)` | Acumula de forma segura el total de combinaciones revisadas por todos los hilos en la búsqueda paralela. |
| `omp_get_wtime()` | Mide el tiempo de reloj (*wall-clock time*) para calcular la duración y el *speedup*. |
| `omp_get_thread_num()` / `omp_get_num_threads()` | Obtiene el ID del hilo actual y el número total de hilos activos. |
| `omp_set_num_threads()` | Establece dinámicamente la cantidad de hilos solicitada por el usuario. |

---

## 🛠️ Instrucciones de Compilación y Ejecución

### Requisitos Previos
* Compilador de C++ con soporte para OpenMP (por ejemplo, `g++` de GCC, MinGW en Windows, o Clang).
* Entorno de Desarrollo opcional (Code::Blocks, VS Code, CLion, etc.).

---

### Opción 1: Compilación desde la Línea de Comandos (Terminal / CMD / PowerShell)

Navega a la carpeta correspondiente de cada práctica y ejecuta los siguientes comandos:

#### Práctica 2.5 (Ordenamientos)
```bash
# Compilar
g++ -O2 -fopenmp practica2_5.cpp -o practica2_5

# Ejecutar en Linux / macOS
./practica2_5

# Ejecutar en Windows
practica2_5.exe
Práctica 2.6 (Búsqueda Exhaustiva)
Bash
# Compilar
g++ -O2 -fopenmp practica2_6.cpp -o practica2_6

# Ejecutar en Linux / macOS
./practica2_6

# Ejecutar en Windows
practica2_6.exe
Nota: La bandera -fopenmp es indispensable para activar las directivas de paralelismo del compilador.

Opción 2: Configuración en Code::Blocks
Si estás utilizando el IDE Code::Blocks:

Abre el proyecto o archivo .cpp deseado.

Ve al menú superior Project > Build options...

En la pestaña Compiler settings > Other compiler options, agrega:

Plaintext
-fopenmp
En la pestaña Linker settings > Other linker options, agrega:

Plaintext
-fopenmp
(En algunas distribuciones de Linux/GCC puede requerirse -lgomp).

Haz clic en OK, compila y ejecuta el proyecto (F9).

📊 Muestra de Funcionalidades
Ambos programas cuentan con un menú interactivo por consola que permite:

Configurar parámetros de entrada (tamaños de arreglo, rangos, claves y longitudes).

Ejecutar pruebas de rendimiento secuenciales vs. paralelas.

Consultar tablas o resúmenes de Speedup (T 
secuencial
​
 /T 
paralelo
​
 ).
"""

with open("README.md", "w", encoding="utf-8") as f:
f.write(readme_content)

print("README.md generado correctamente.")


```text?code_stdout&code_event_index=1
README.md generado correctamente.

Your Markdown file is ready

Ícono de MD
README
 MD 
A continuación tienes el contenido formateado para tu archivo README.md. Puedes descargar el archivo generado o copiar y pegar directamente este texto en un archivo llamado README.md dentro de la carpeta principal de tu repositorio:

Markdown
# Prácticas de Programación Paralela con OpenMP

Este repositorio contiene las soluciones para las prácticas de programación paralela utilizando **C++** y la biblioteca **OpenMP**, enfocado en la comparación de rendimiento (tiempo de ejecución y *speedup*) entre implementaciones secuenciales y paralelas.

---

## 👥 Integrantes del Equipo

* **Ávila Martínez Marco**
* **Larios Hernández Carlos Alberto**
* **Macías Rentería Dante Yael**
* **Salcedo Ramos Luis Gael**

---

## 📁 Estructura del Repositorio

El repositorio se divide en dos actividades / prácticas principales:

### 1. Práctica 2.5: Algoritmos de Ordenamiento Paralelos
Comparativa de rendimiento entre algoritmos de ordenamiento iterativos y recursivos en sus versiones secuenciales y paralelas.

* **Generación de Datos en Paralelo:** Llenado distribuido de un arreglo aleatorio utilizando generadores de números aleatorios *thread-local*.
* **Burbuja Secuencial vs. Burbuja Paralela (Odd-Even Sort):** Algoritmo de ordenamiento por transposición par-impar paralelizado por fases.
* **Merge Sort Secuencial vs. Merge Sort Paralelo con Tareas:** Implementación recursiva paralelizada mediante tareas de OpenMP (`#pragma omp task`) con un umbral de secuencialización para evitar el sobrecosto (*overhead*) del sistema de hilos.

### 2. Práctica 2.6: Búsqueda Exhaustiva de Claves Alfanuméricas (Fuerza Bruta)
Implementación y benchmarking de una búsqueda por fuerza bruta sobre un espacio de claves alfanuméricas en base 36 (`A-Z`, `0-9`).

* **Búsqueda Secuencial:** Recorrido iterativo simple de todo el espacio de combinaciones.
* **Búsqueda Paralela con Parada Temprana:** División equilibrada del espacio de búsqueda entre los hilos asignados, incorporando detección de hallazgo mediante lecturas atómicas para detener el resto de los hilos de manera anticipada.

---

## ⚡ Directivas y Funciones de OpenMP Utilizadas

A lo largo de ambos programas se emplean las siguientes directivas, cláusulas y funciones de la API de OpenMP:

| Directiva / Función | Descripción y Uso en las Prácticas |
| :--- | :--- |
| `#pragma omp parallel` | Define una región paralela donde se crea un equipo de hilos. |
| `#pragma omp parallel for` | Paraleliza un bucle `for` dividiendo sus iteraciones entre los hilos (utilizado en el llenado del arreglo). |
| `#pragma omp for` | Distribuye las iteraciones de los bucles de ordenamiento par e impar entre hilos dentro de un bloque paralelo existente. |
| `#pragma omp single` | Garantiza que un bloque de código sea ejecutado por un solo hilo (usado para iniciar tareas e imprimir avisos). |
| `#pragma omp task` | Crea una tarea asíncrona dentro del proceso de ordenamiento recursivo (Merge Sort). |
| `#pragma omp taskwait` | Sincroniza la ejecución de las tareas hijas antes de fusionar subarreglos en Merge Sort. |
| `#pragma omp atomic read` | Realiza lecturas atómicas de variables compartidas (`encontradaGlobal`) para la parada temprana sin bloqueo. |
| `#pragma omp critical` / `critical(nombre)` | Garantiza exclusión mutua para evitar condiciones de carrera al guardar el hilo ganador o imprimir en pantalla. |
| `reduction(+:variable)` | Acumula de forma segura el total de combinaciones revisadas por todos los hilos en la búsqueda paralela. |
| `omp_get_wtime()` | Mide el tiempo de reloj (*wall-clock time*) para calcular la duración y el *speedup*. |
| `omp_get_thread_num()` / `omp_get_num_threads()` | Obtiene el ID del hilo actual y el número total de hilos activos. |
| `omp_set_num_threads()` | Establece dinámicamente la cantidad de hilos solicitada por el usuario. |

---

## 🛠️ Instrucciones de Compilación y Ejecución

### Requisitos Previos
* Compilador de C++ con soporte para OpenMP (por ejemplo, `g++` de GCC, MinGW en Windows, o Clang).
* Entorno de Desarrollo opcional (Code::Blocks, VS Code, CLion, etc.).

---

### Opción 1: Compilación desde la Línea de Comandos (Terminal / CMD / PowerShell)

Navega a la carpeta correspondiente de cada práctica y ejecuta los siguientes comandos:

#### Práctica 2.5 (Ordenamientos)
```bash
# Compilar
g++ -O2 -fopenmp practica2_5.cpp -o practica2_5

# Ejecutar en Linux / macOS
./practica2_5

# Ejecutar en Windows
practica2_5.exe
Práctica 2.6 (Búsqueda Exhaustiva)
Bash
# Compilar
g++ -O2 -fopenmp practica2_6.cpp -o practica2_6

# Ejecutar en Linux / macOS
./practica2_6

# Ejecutar en Windows
practica2_6.exe
Nota: La bandera -fopenmp es indispensable para activar las directivas de paralelismo del compilador.

Opción 2: Configuración en Code::Blocks
Si estás utilizando el IDE Code::Blocks:

Abre el proyecto o archivo .cpp deseado.

Ve al menú superior Project > Build options...

En la pestaña Compiler settings > Other compiler options, agrega:

Plaintext
-fopenmp
En la pestaña Linker settings > Other linker options, agrega:

Plaintext
-fopenmp
(En algunas distribuciones de Linux/GCC puede requerirse -lgomp).

Haz clic en OK, compila y ejecuta el proyecto (F9).

📊 Muestra de Funcionalidades
Ambos programas cuentan con un menú interactivo por consola que permite:

Configurar parámetros de entrada (tamaños de arreglo, rangos, claves y longitudes).

Ejecutar pruebas de rendimiento secuenciales vs. paralelas.

Consultar tablas o resúmenes de Speedup (T 
secuencial
​
 /T 
paralelo
​
 ).


---

### ¿Cómo agregarlo a tu repositorio desde GitHub Desktop?
1. Guarda el archivo como `README.md` dentro de la carpeta principal de tu proyecto.
2. Abre **GitHub Desktop**; verás que detecta automáticamente el nuevo archivo en la pestaña **Changes**.
3. En la casilla *Summary*, escribe por ejemplo: `Agregar README.md`.
4. Haz clic en **Commit to main** (o master).
5. Presiona **Push origin** (o *Publish*) para subir el cambio a GitHub.



Gemini es una IA y puede cometer errores.

Analizando
# Prácticas de Programación Paralela con OpenMP

Este repositorio contiene las soluciones para las prácticas de programación paralela utilizando **C++** y la biblioteca **OpenMP**, enfocado en la comparación de rendimiento (tiempo de ejecución y *speedup*) entre implementaciones secuenciales y paralelas.

---

## 👥 Integrantes del Equipo

* **Ávila Martínez Marco**
* **Larios Hernández Carlos Alberto**
* **Macías Rentería Dante Yael**
* **Salcedo Ramos Luis Gael**

---

## 📁 Estructura del Repositorio

El repositorio se divide en dos actividades / prácticas principales:

### 1. Práctica 2.5: Algoritmos de Ordenamiento Paralelos
Comparativa de rendimiento entre algoritmos de ordenamiento iterativos y recursivos en sus versiones secuenciales y paralelas.

* **Generación de Datos en Paralelo:** Llenado distribuido de un arreglo aleatorio utilizando generadores de números aleatorios *thread-local*.
* **Burbuja Secuencial vs. Burbuja Paralela (Odd-Even Sort):** Algoritmo de ordenamiento por transposición par-impar paralelizado por fases.
* **Merge Sort Secuencial vs. Merge Sort Paralelo con Tareas:** Implementación recursiva paralelizada mediante tareas de OpenMP (`#pragma omp task`) con un umbral de secuencialización para evitar el sobrecosto (*overhead*) del sistema de hilos.

### 2. Práctica 2.6: Búsqueda Exhaustiva de Claves Alfanuméricas (Fuerza Bruta)
Implementación y benchmarking de una búsqueda por fuerza bruta sobre un espacio de claves alfanuméricas en base 36 (`A-Z`, `0-9`).

* **Búsqueda Secuencial:** Recorrido iterativo simple de todo el espacio de combinaciones.
* **Búsqueda Paralela con Parada Temprana:** División equilibrada del espacio de búsqueda entre los hilos asignados, incorporando detección de hallazgo mediante lecturas atómicas para detener el resto de los hilos de manera anticipada.

---

## ⚡ Directivas y Funciones de OpenMP Utilizadas

A lo largo de ambos programas se emplean las siguientes directivas, cláusulas y funciones de la API de OpenMP:

| Directiva / Función | Descripción y Uso en las Prácticas |
| :--- | :--- |
| `#pragma omp parallel` | Define una región paralela donde se crea un equipo de hilos. |
| `#pragma omp parallel for` | Paraleliza un bucle `for` dividiendo sus iteraciones entre los hilos (utilizado en el llenado del arreglo). |
| `#pragma omp for` | Distribuye las iteraciones de los bucles de ordenamiento par e impar entre hilos dentro de un bloque paralelo existente. |
| `#pragma omp single` | Garantiza que un bloque de código sea ejecutado por un solo hilo (usado para iniciar tareas e imprimir avisos). |
| `#pragma omp task` | Crea una tarea asíncrona dentro del proceso de ordenamiento recursivo (Merge Sort). |
| `#pragma omp taskwait` | Sincroniza la ejecución de las tareas hijas antes de fusionar subarreglos en Merge Sort. |
| `#pragma omp atomic read` | Realiza lecturas atómicas de variables compartidas (`encontradaGlobal`) para la parada temprana sin bloqueo. |
| `#pragma omp critical` / `critical(nombre)` | Garantiza exclusión mutua para evitar condiciones de carrera al guardar el hilo ganador o imprimir en pantalla. |
| `reduction(+:variable)` | Acumula de forma segura el total de combinaciones revisadas por todos los hilos en la búsqueda paralela. |
| `omp_get_wtime()` | Mide el tiempo de reloj (*wall-clock time*) para calcular la duración y el *speedup*. |
| `omp_get_thread_num()` / `omp_get_num_threads()` | Obtiene el ID del hilo actual y el número total de hilos activos. |
| `omp_set_num_threads()` | Establece dinámicamente la cantidad de hilos solicitada por el usuario. |

---

## 🛠️ Instrucciones de Compilación y Ejecución

### Requisitos Previos
* Compilador de C++ con soporte para OpenMP (por ejemplo, `g++` de GCC, MinGW en Windows, o Clang).
* Entorno de Desarrollo opcional (Code::Blocks, VS Code, CLion, etc.).

---

### Opción 1: Compilación desde la Línea de Comandos (Terminal / CMD / PowerShell)

Navega a la carpeta correspondiente de cada práctica y ejecuta los siguientes comandos:

#### Práctica 2.5 (Ordenamientos)
```bash
# Compilar
g++ -O2 -fopenmp practica2_5.cpp -o practica2_5

# Ejecutar en Linux / macOS
./practica2_5

# Ejecutar en Windows
practica2_5.exe
```

#### Práctica 2.6 (Búsqueda Exhaustiva)
```bash
# Compilar
g++ -O2 -fopenmp practica2_6.cpp -o practica2_6

# Ejecutar en Linux / macOS
./practica2_6

# Ejecutar en Windows
practica2_6.exe
```

> **Nota:** La bandera `-fopenmp` es indispensable para activar las directivas de paralelismo del compilador.

---

### Opción 2: Configuración en Code::Blocks

Si estás utilizando el IDE **Code::Blocks**:

1. Abre el proyecto o archivo `.cpp` deseado.
2. Ve al menú superior **Project** > **Build options...**
3. En la pestaña **Compiler settings** > **Other compiler options**, agrega:
   ```text
   -fopenmp
   ```
4. En la pestaña **Linker settings** > **Other linker options**, agrega:
   ```text
   -fopenmp
   ```
   *(En algunas distribuciones de Linux/GCC puede requerirse `-lgomp`).*
5. Haz clic en **OK**, compila y ejecuta el proyecto (**F9**).

---

## 📊 Muestra de Funcionalidades

Ambos programas cuentan con un menú interactivo por consola que permite:
1. Configurar parámetros de entrada (tamaños de arreglo, rangos, claves y longitudes).
2. Ejecutar pruebas de rendimiento secuenciales vs. paralelas.
3. Consultar tablas o resúmenes de **Speedup** ($T_{secuencial} / T_{paralelo}$).
README.md
Mostrando README.md.
