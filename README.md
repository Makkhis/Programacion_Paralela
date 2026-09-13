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
