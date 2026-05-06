#include <iostream>
#include <vector>
using namespace std;

int main() {
    vector<int> vec = {10, 20, 30, 40, 50};

    cout << "Elementos del vector:" << endl;
    for (int i = 0; i < vec.size(); i++) {
        cout << vec[i] << " ";
    } cout << endl;

    // Modificacin
    vec[2] = 99;
    cout << "Nuevo valor en la posicin 2: " << vec[2] << endl;

    // Insercin
    vec.push_back(60);
    cout << "Nuevo tamao del vector: " << vec.size() << endl;

    // Eliminacin
    vec.pop_back();
    cout << "Tamao del vector despus de eliminar el ltimo elemento: " << vec.size() << endl;

    return 0;
}
