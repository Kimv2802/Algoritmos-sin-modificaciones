#include <iostream>
#include <cstdio> // Librería necesaria para las funciones de estilo C

using namespace std;

int main() {
    // Abre el archivo usando la función clásica fopen
    FILE *archivo = fopen("num10.txt", "r");
    int valor;

    // Validación de apertura (puntero nulo)
    if (!archivo) {
        cerr << "Error al abrir el archivo.\n";
        return 1;
    }

    // fscanf devuelve el número de elementos leídos correctamente. 
    // Esperamos que sea 1 en cada ciclo.
    while (fscanf(archivo, "%d", &valor) == 1) {
        cout << "Leido: " << valor << endl;
    }

    // Es obligatorio cerrar el flujo con fclose
    fclose(archivo);
    return 0;
}
