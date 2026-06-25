// Autores: Edna Martha Miranda Chávez
//          Sergio Fuenlabrada Velázquez
// P044 - Problema de las 8 reinas

#include <chrono>
#include <clocale>
#include <iomanip>
#include <iostream>
#include <limits>
#include <string>
#include <vector>
#include <locale>

using namespace std;

constexpr int N_MINIMO = 4;
constexpr int N_MAXIMO = 14;

enum class ModoVisualizacion {
    TODAS = 1,
    CANTIDAD_ESPECIFICA = 2,
    SOLO_ESTADISTICAS = 3
};

struct Configuracion {
    int n = 8;
    ModoVisualizacion modo = ModoVisualizacion::TODAS;
    unsigned long long limiteMostrar = 0;
};

struct Estadisticas {
    unsigned long long solucionesEncontradas = 0;
    unsigned long long solucionesMostradas = 0;
    unsigned long long posicionesEvaluadas = 0;
    unsigned long long posicionesDescartadas = 0;
    unsigned long long retrocesos = 0;
};

int leerEnteroEnRango(const string& mensaje, int minimo, int maximo) {
    int valor;
    while (true) {
        cout << mensaje;
        if (cin >> valor && valor >= minimo && valor <= maximo) {
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            return valor;
        }
        cout << "Entrada inválida. Capture un número entero entre "
             << minimo << " y " << maximo << ".\n";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
}

Configuracion solicitarConfiguracion() {
    Configuracion configuracion;
    cout << "=========================================================\n";
    cout << "       PROBLEMA DE LAS N-REINAS (BACKTRACKING)          \n";
    cout << "=========================================================\n\n";

    configuracion.n = leerEnteroEnRango("Ingrese el número de reinas (N) [" + to_string(N_MINIMO) + " - " + to_string(N_MAXIMO) + "]: ", N_MINIMO, N_MAXIMO);

    cout << "\nModos de visualización disponibles:\n"
         << "1. Mostrar todas las soluciones gráficamente\n"
         << "2. Mostrar un límite específico de soluciones\n"
         << "3. No mostrar soluciones (Ver solo estadísticas de rendimiento)\n";

    int opcModo = leerEnteroEnRango("Seleccione el modo: ", 1, 3);
    configuracion.modo = static_cast<ModoVisualizacion>(opcModo);

    if (configuracion.modo == ModoVisualizacion::CANTIDAD_ESPECIFICA) {
        configuracion.limiteMostrar = leerEnteroEnRango("Ingrese la cantidad máxima de soluciones a renderizar: ", 1, 1000000);
    }

    return configuracion;
}

void mostrarTablero(const vector<int>& columnaPorFila, int n, unsigned long long idSolucion) {
    cout << "\nSolución #" << idSolucion << ":\n";
    cout << "  ";
    for (int j = 0; j < n; j++) cout << "---";
    cout << "\n";

    for (int i = 0; i < n; i++) {
        cout << "| ";
        for (int j = 0; j < n; j++) {
            if (columnaPorFila[i] == j) {
                cout << "R "; // Reina
            } else {
                cout << ". "; // Casilla vacía
            }
        }
        cout << "|\n";
    }

    cout << "  ";
    for (int j = 0; j < n; j++) cout << "---";
    cout << "\n";
}

void colocarReinas(int fila, 
                    Configuracion& configuracion, 
                    Estadisticas& estadisticas,
                    vector<int>& columnaPorFila, 
                    vector<bool>& columnasOcupadas,
                    vector<bool>& diagonalesPrincipalesOcupadas, 
                    vector<bool>& diagonalesSecundariasOcupadas) {
    
    int n = configuracion.n;

    if (fila == n) {
        estadisticas.solucionesEncontradas++;
        
        bool renderizar = false;
        if (configuracion.modo == ModoVisualizacion::TODAS) {
            renderizar = true;
        } else if (configuracion.modo == ModoVisualizacion::CANTIDAD_ESPECIFICA && 
                   estadisticas.solucionesMostradas < configuracion.limiteMostrar) {
            renderizar = true;
        }

        if (renderizar) {
            estadisticas.solucionesMostradas++;
            mostrarTablero(columnaPorFila, n, estadisticas.solucionesEncontradas);
        }
        return;
    }

    for (int col = 0; col < n; col++) {
        estadisticas.posicionesEvaluadas++;
        
        int diagPrincipalIdx = fila - col + (n - 1);
        int diagSecundariaIdx = fila + col;

        if (columnasOcupadas[col] || 
            diagonalesPrincipalesOcupadas[diagPrincipalIdx] || 
            diagonalesSecundariasOcupadas[diagSecundariaIdx]) {
            estadisticas.posicionesDescartadas++;
            continue;
        }

        // Colocación tentativa de la reina
        columnaPorFila[fila] = col;
        columnasOcupadas[col] = true;
        diagonalesPrincipalesOcupadas[diagPrincipalIdx] = true;
        diagonalesSecundariasOcupadas[diagSecundariaIdx] = true;

        // Llamada recursiva para la siguiente fila
        colocarReinas(fila + 1, configuracion, estadisticas, columnaPorFila, 
                      columnasOcupadas, diagonalesPrincipalesOcupadas, diagonalesSecundariasOcupadas);

        // Backtracking (Deshacer movimientos)
        columnasOcupadas[col] = false;
        diagonalesPrincipalesOcupadas[diagPrincipalIdx] = false;
        diagonalesSecundariasOcupadas[diagSecundariaIdx] = false;
        estadisticas.retrocesos++;
    }
}

int main() {
    system("chcp 65001 > nul");
    setlocale(LC_ALL, "es_MX.UTF-8");

    const Configuracion configuracion = solicitarConfiguracion();
    Estadisticas estadisticas;

    vector<int> columnaPorFila(configuracion.n, -1);
    vector<bool> columnasOcupadas(configuracion.n, false);
    vector<bool> diagonalesPrincipalesOcupadas(2 * configuracion.n - 1, false);
    vector<bool> diagonalesSecundariasOcupadas(2 * configuracion.n - 1, false);

    cout << "\nIniciando búsqueda mediante backtracking optimizado...\n";

    const auto inicio = chrono::steady_clock::now();

    // Iniciar el algoritmo desde la fila 0
    Configuracion configMutable = configuracion; 
    colocarReinas(0, configMutable, estadisticas, columnaPorFila, 
                  columnasOcupadas, diagonalesPrincipalesOcupadas, diagonalesSecundariasOcupadas);

    const auto fin = chrono::steady_clock::now();
    const auto tiempoMilisegundos = chrono::duration_cast<chrono::milliseconds>(fin - inicio).count();

    cout << "\n=========================================================\n";
    cout << "               REPORTES Y ESTADÍSTICAS                   \n";
    cout << "=========================================================\n";
    cout << "Total de soluciones encontradas: " << estadisticas.solucionesEncontradas << "\n";
    cout << "Total de soluciones representadas: " << estadisticas.solucionesMostradas << "\n";
    cout << "Posiciones evaluadas: " << estadisticas.posicionesEvaluadas << "\n";
    cout << "Posiciones descartadas: " << estadisticas.posicionesDescartadas << "\n";
    cout << "Operaciones de retroceso: " << estadisticas.retrocesos << "\n";
    
    cout << fixed << setprecision(3);
    cout << "Tiempo de ejecución: " << tiempoMilisegundos << " ms\n";
    cout << "Complejidad temporal teórica: O(n!)\n";
    cout << "Complejidad espacial: O(n)\n";
    cout << "=========================================================\n";

    return 0;
}
