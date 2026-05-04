#include <iostream>
using namespace std;

// Función recursiva para resolver las Torres de Hanói
void torresDeHanoi(int n, char origen, char destino, char auxiliar) {
    if (n == 1) {
        cout << "Mover disco 1 de " << origen << " a " << destino << endl;
        return;
    }
    torresDeHanoi(n - 1, origen, auxiliar, destino); // Paso 1
    cout << "Mover disco " << n << " de " << origen << " a " << destino << endl; // Paso 2
    torresDeHanoi(n - 1, auxiliar, destino, origen); // Paso 3
}

int main() {
    int n;
    cout << "Ingrese el número de discos: ";
    cin >> n;
    torresDeHanoi(n, 'A', 'C', 'B');
    return 0;
}

