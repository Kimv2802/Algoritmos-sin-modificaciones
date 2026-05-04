#include <iostream>
using namespace std;

// Función recursiva para calcular la suma de los primeros n números naturales
long long sumaNaturales(int n) {
    // Caso base: si n es 1, su suma es 1
    if (n == 1) {
        return 1;
    } else {
        // Llamada recursiva: suma de n más la suma de los (n-1) primeros números
        return n + sumaNaturales(n - 1);
    }
}

int main() {
    setlocale(LC_ALL, ""); // Permitir acentos y caracteres especiales en la consola
    int n;

    // 1. Solicitar el valor de n al usuario
    cout << "Ingrese el valor de n (número de términos a sumar): ";
    cin >> n;

    // 2. Validar que n sea un número positivo
    if (n <= 0) {
        cout << "El número debe ser mayor que cero." << endl;
        return 1; // Finaliza el programa con error
    }

    // 3. Calcular la suma utilizando recursividad
    long long resultado = sumaNaturales(n);

    // 4. Mostrar el resultado
    cout << "La suma de los primeros " << n << " números naturales es: " << resultado << endl;

    return 0;
}
