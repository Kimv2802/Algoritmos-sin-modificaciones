// Autores: Edna Martha Miranda Chávez
//          Sergio Fuenlabrada Velázquez
// P043- Agente Viajero

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <map>
#include <unordered_map>
#include <queue>
#include <algorithm>
#include <utility>
#include <limits>
#include <cctype>
#include <clocale>
#include <locale>

using namespace std;

using Distancia = long long;
constexpr Distancia INF = numeric_limits<Distancia>::max() / 4;
constexpr size_t MAX_RUTAS_A_MOSTRAR = 10000;
constexpr size_t MAX_CIUDADES_TSP = 18;

struct Ciudad {
    string nombre;
    map<int, Distancia> conexiones;
};

vector<Ciudad> capitales;
unordered_map<string, int> indicePorNombre;

string recortar(const string& texto) {
    const string espacios = " \t\r\n";
    const size_t inicio = texto.find_first_not_of(espacios);
    if (inicio == string::npos) return "";
    const size_t fin = texto.find_last_not_of(espacios);
    return texto.substr(inicio, fin - inicio + 1);
}

string normalizarNombre(string texto) {
    texto = recortar(texto);
    transform(texto.begin(), texto.end(), texto.begin(), [](unsigned char c) {
        return static_cast<char>(tolower(c));
    });
    return texto;
}

bool convertirEntero(const string& texto, long long& valor) {
    try {
        const string limpio = recortar(texto);
        size_t procesados = 0;
        valor = stoll(limpio, &procesados);
        return !limpio.empty() && procesados == limpio.size();
    } catch (...) {
        return false;
    }
}

void reconstruirIndiceNombres() {
    indicePorNombre.clear();
    for (size_t i = 0; i < capitales.size(); ++i) {
        indicePorNombre[normalizarNombre(capitales[i].nombre)] = static_cast<int>(i);
    }
}

bool cargarArchivo(const string& nombreArchivo) {
    ifstream archivo(nombreArchivo);
    if (!archivo) {
        cout << "No se pudo abrir el archivo: " << nombreArchivo << "\n";
        return false;
    }

    vector<Ciudad> datosTemporales;
    string linea;
    size_t numeroLinea = 0;

    while (getline(archivo, linea)) {
        ++numeroLinea;
        linea = recortar(linea);
        if (linea.empty()) continue;

        const size_t dosPuntos = linea.find(':');
        if (dosPuntos == string::npos) {
            cout << "Error en la línea " << numeroLinea << ": falta el carácter ':'.\n";
            return false;
        }

        Ciudad ciudad;
        ciudad.nombre = recortar(linea.substr(0, dosPuntos));
        if (ciudad.nombre.empty()) {
            cout << "Error en la línea " << numeroLinea << ": el nombre de la ciudad está vacío.\n";
            return false;
        }

        const string bloqueConexiones = recortar(linea.substr(dosPuntos + 1));
        if (!bloqueConexiones.empty()) {
            string token;
            stringstream flujo(bloqueConexiones);
            while (getline(flujo, token, ',')) {
                token = recortar(token);
                const size_t guion = token.find('-');
                if (guion == string::npos) {
                    cout << "Error en la línea " << numeroLinea << ": conexión inválida '" << token << "'.\n";
                    return false;
                }

                long long idDestino = -1;
                long long distancia = -1;
                if (!convertirEntero(token.substr(0, guion), idDestino) ||
                    !convertirEntero(token.substr(guion + 1), distancia) ||
                    idDestino < 0 || distancia <= 0 ||
                    idDestino > numeric_limits<int>::max()) {
                    cout << "Error en la línea " << numeroLinea << ": conexión inválida '" << token << "'.\n";
                    return false;
                }
                ciudad.conexiones[static_cast<int>(idDestino)] = distancia;
            }
        }
        datosTemporales.push_back(move(ciudad));
    }

    if (datosTemporales.empty()) {
        cout << "El archivo no contiene ciudades válidas.\n";
        return false;
    }

    unordered_map<string, int> nombresEncontrados;
    for (size_t i = 0; i < datosTemporales.size(); ++i) {
        const string clave = normalizarNombre(datosTemporales[i].nombre);
        if (nombresEncontrados.count(clave) != 0) {
            cout << "Nombre de ciudad duplicado: " << datosTemporales[i].nombre << "\n";
            return false;
        }
        nombresEncontrados[clave] = static_cast<int>(i);
    }

    for (size_t i = 0; i < datosTemporales.size(); ++i) {
        for (map<int, Distancia>::const_iterator it = datosTemporales[i].conexiones.begin(); 
             it != datosTemporales[i].conexiones.end(); ++it) {
            int destino = it->first;
            Distancia distancia = it->second;
            (void)distancia;
            if (destino < 0 || destino >= static_cast<int>(datosTemporales.size())) {
                cout << "La ciudad '" << datosTemporales[i].nombre 
                     << "' contiene un ID de destino inexistente: " << destino << "\n";
                return false;
            }
            if (destino == static_cast<int>(i)) {
                cout << "La ciudad '" << datosTemporales[i].nombre 
                     << "' contiene una conexión hacia sí misma.\n";
                return false;
            }
        }
    }

    capitales = move(datosTemporales);
    reconstruirIndiceNombres();
    cout << "Datos cargados correctamente: " << capitales.size() << " ciudades.\n";
    return true;
}

void leer() {
    string nombreArchivo;
    cout << "Ingrese el nombre del archivo, con o sin extensión .txt: ";
    getline(cin >> ws, nombreArchivo);
    nombreArchivo = recortar(nombreArchivo);
    if (nombreArchivo.size() < 4 || 
        normalizarNombre(nombreArchivo.substr(nombreArchivo.size() - 4)) != ".txt") {
        nombreArchivo += ".txt";
    }
    cargarArchivo(nombreArchivo);
}

bool verificarDatosCargados() {
    if (!capitales.empty()) return true;
    cout << "No hay datos cargados. Utilice primero la opción 1.\n";
    return false;
}

void mostrarArbol() {
    if (!verificarDatosCargados()) return;
    cout << "\n=== ÁRBOL DE RUTAS ENTRE CAPITALES ===\n";
    for (size_t i = 0; i < capitales.size(); ++i) {
        cout << "\n[" << i << "] " << capitales[i].nombre << "\n";
        if (capitales[i].conexiones.empty()) {
            cout << "  Sin conexiones registradas\n";
            continue;
        }
        size_t posicion = 0;
        for (map<int, Distancia>::const_iterator it = capitales[i].conexiones.begin();
             it != capitales[i].conexiones.end(); ++it) {
            ++posicion;
            int destino = it->first;
            Distancia distancia = it->second;
            cout << (posicion == capitales[i].conexiones.size() ? "  +-- " : "  +-- ")
                 << capitales[destino].nombre << " (" << distancia << " km)\n";
        }
    }
}

void listarCiudades() {
    if (!verificarDatosCargados()) return;
    cout << "\n=== CIUDADES Y CONEXIONES ===\n";
    for (size_t i = 0; i < capitales.size(); ++i) {
        cout << "\n[" << i << "] " << capitales[i].nombre << ":\n";
        if (capitales[i].conexiones.empty()) {
            cout << "  Sin conexiones registradas\n";
            continue;
        }
        for (map<int, Distancia>::const_iterator it = capitales[i].conexiones.begin();
             it != capitales[i].conexiones.end(); ++it) {
            int destino = it->first;
            Distancia distancia = it->second;
            cout << "  -> " << capitales[destino].nombre << " | " << distancia << " km\n";
        }
    }
}

int solicitarCiudad(const string& mensaje) {
    string nombre;
    cout << mensaje;
    getline(cin >> ws, nombre);
    const auto it = indicePorNombre.find(normalizarNombre(nombre));
    if (it == indicePorNombre.end()) {
        cout << "Ciudad no encontrada: " << nombre << "\n";
        return -1;
    }
    return it->second;
}

bool dijkstra(int origen, int destino, vector<int>& ruta, Distancia& distanciaTotal) {
    const size_t n = capitales.size();
    vector<Distancia> distancias(n, INF);
    vector<int> anterior(n, -1);
    using Estado = pair<Distancia, int>;
    priority_queue<Estado, vector<Estado>, greater<Estado> > pendientes;

    distancias[origen] = 0;
    pendientes.push(make_pair(0, origen));

    while (!pendientes.empty()) {
        const Estado actual = pendientes.top();
        pendientes.pop();
        Distancia distanciaActual = actual.first;
        int ciudadActual = actual.second;

        if (distanciaActual != distancias[ciudadActual]) continue;
        if (ciudadActual == destino) break;

        for (map<int, Distancia>::const_iterator it = capitales[ciudadActual].conexiones.begin();
             it != capitales[ciudadActual].conexiones.end(); ++it) {
            int vecino = it->first;
            Distancia peso = it->second;
            if (distanciaActual <= INF - peso &&
                distanciaActual + peso < distancias[vecino]) {
                distancias[vecino] = distanciaActual + peso;
                anterior[vecino] = ciudadActual;
                pendientes.push(make_pair(distancias[vecino], vecino));
            }
        }
    }

    if (distancias[destino] == INF) return false;
    
    ruta.clear();
    for (int actual = destino; actual != -1; actual = anterior[actual]) {
        ruta.push_back(actual);
    }
    reverse(ruta.begin(), ruta.end());
    distanciaTotal = distancias[destino];
    return true;
}

void imprimirRuta(const vector<int>& ruta) {
    for (size_t i = 0; i < ruta.size(); ++i) {
        cout << capitales[ruta[i]].nombre;
        if (i + 1 < ruta.size()) cout << " -> ";
    }
    cout << "\n";
}

void buscarRutaMasCorta() {
    if (!verificarDatosCargados()) return;
    const int origen = solicitarCiudad("Ingrese la capital de origen: ");
    if (origen < 0) return;
    const int destino = solicitarCiudad("Ingrese la capital de destino: ");
    if (destino < 0) return;

    if (origen == destino) {
        cout << "La ciudad de origen y destino es la misma. Distancia: 0 km.\n";
        return;
    }

    vector<int> ruta;
    Distancia distancia = 0;
    if (!dijkstra(origen, destino, ruta, distancia)) {
        cout << "No existe una ruta entre las ciudades indicadas.\n";
        return;
    }

    cout << "\n=== RUTA MÁS CORTA ===\n";
    imprimirRuta(ruta);
    cout << "Distancia total: " << distancia << " km\n";
}

void enumerarRutasDFS(int actual, int destino, vector<bool>& visitado,
                      vector<int>& rutaActual, Distancia distanciaActual,
                      vector<pair<Distancia, vector<int> > >& rutas,
                      bool& limiteAlcanzado) {
    if (rutas.size() >= MAX_RUTAS_A_MOSTRAR) {
        limiteAlcanzado = true;
        return;
    }

    visitado[actual] = true;
    rutaActual.push_back(actual);

    if (actual == destino) {
        rutas.push_back(make_pair(distanciaActual, rutaActual));
    } else {
        for (map<int, Distancia>::const_iterator it = capitales[actual].conexiones.begin();
             it != capitales[actual].conexiones.end(); ++it) {
            int vecino = it->first;
            Distancia peso = it->second;
            if (!visitado[vecino] && distanciaActual <= INF - peso) {
                enumerarRutasDFS(vecino, destino, visitado, rutaActual,
                                 distanciaActual + peso, rutas, limiteAlcanzado);
                if (limiteAlcanzado) break;
            }
        }
    }

    rutaActual.pop_back();
    visitado[actual] = false;
}

void buscarTodasLasRutas() {
    if (!verificarDatosCargados()) return;
    const int origen = solicitarCiudad("Ingrese la capital de origen: ");
    if (origen < 0) return;
    const int destino = solicitarCiudad("Ingrese la capital de destino: ");
    if (destino < 0) return;

    vector<bool> visitado(capitales.size(), false);
    vector<int> rutaActual;
    vector<pair<Distancia, vector<int> > > rutas;
    bool limiteAlcanzado = false;

    enumerarRutasDFS(origen, destino, visitado, rutaActual, 0, rutas, limiteAlcanzado);

    if (rutas.empty()) {
        cout << "No se encontraron rutas simples entre las ciudades indicadas.\n";
        return;
    }

    sort(rutas.begin(), rutas.end());

    cout << "\n=== RUTAS SIMPLES ENCONTRADAS ===\n";
    for (size_t i = 0; i < rutas.size(); ++i) {
        cout << "\nRuta " << i + 1 << " (" << rutas[i].first << " km):\n";
        imprimirRuta(rutas[i].second);
    }

    cout << "\nRuta mínima dentro de la enumeración: " << rutas.front().first << " km\n";
    imprimirRuta(rutas.front().second);
    cout << '\n';

    if (limiteAlcanzado) {
        cout << "Aviso: se alcanzó el límite de " << MAX_RUTAS_A_MOSTRAR
             << " rutas para evitar una explosión combinatoria.\n";
    }
}

bool resolverTSPHeldKarp(int origen, vector<int>& ciclo, Distancia& costoMinimo) {
    const size_t n = capitales.size();
    if (n < 2 || n > MAX_CIUDADES_TSP) return false;

    const size_t totalMascaras = static_cast<size_t>(1) << n;
    
    // Verificar overflow potencial en la multiplicación
    if (totalMascaras > numeric_limits<size_t>::max() / n) {
        return false;
    }
    
    vector<Distancia> dp(totalMascaras * n, INF);
    vector<int> padre(totalMascaras * n, -1);

    // Función lambda para calcular posición
    auto posicion = [n](size_t mascara, size_t ciudad) {
        return mascara * n + ciudad;
    };

    const size_t mascaraInicial = static_cast<size_t>(1) << origen;
    dp[posicion(mascaraInicial, origen)] = 0;

    for (size_t mascara = 0; mascara < totalMascaras; ++mascara) {
        if ((mascara & mascaraInicial) == 0) continue;

        for (size_t actual = 0; actual < n; ++actual) {
            const Distancia costoActual = dp[posicion(mascara, actual)];
            if (costoActual == INF) continue;

            for (map<int, Distancia>::const_iterator it = capitales[actual].conexiones.begin();
                 it != capitales[actual].conexiones.end(); ++it) {
                int siguiente = it->first;
                Distancia peso = it->second;
                
                const size_t bitSiguiente = static_cast<size_t>(1) << siguiente;
                if ((mascara & bitSiguiente) != 0) continue;

                const size_t nuevaMascara = mascara | bitSiguiente;
                const size_t posNueva = posicion(nuevaMascara, siguiente);

                if (costoActual <= INF - peso && costoActual + peso < dp[posNueva]) {
                    dp[posNueva] = costoActual + peso;
                    padre[posNueva] = static_cast<int>(actual);
                }
            }
        }
    }

    const size_t mascaraCompleta = totalMascaras - 1;
    costoMinimo = INF;
    int ultimaCiudad = -1;

    for (size_t actual = 0; actual < n; ++actual) {
        if (static_cast<int>(actual) == origen) continue;

        const map<int, Distancia>::const_iterator regreso = capitales[actual].conexiones.find(origen);
        if (regreso == capitales[actual].conexiones.end()) continue;

        const Distancia costoParcial = dp[posicion(mascaraCompleta, actual)];
        if (costoParcial != INF && costoParcial <= INF - regreso->second &&
            costoParcial + regreso->second < costoMinimo) {
            costoMinimo = costoParcial + regreso->second;
            ultimaCiudad = static_cast<int>(actual);
        }
    }

    if (ultimaCiudad == -1) return false;

    vector<int> inversa;
    size_t mascara = mascaraCompleta;
    int actual = ultimaCiudad;

    while (actual != origen) {
        inversa.push_back(actual);
        const int previo = padre[posicion(mascara, static_cast<size_t>(actual))];
        mascara ^= static_cast<size_t>(1) << actual;
        actual = previo;
        if (actual < 0) return false;
    }

    ciclo.clear();
    ciclo.push_back(origen);
    reverse(inversa.begin(), inversa.end());
    ciclo.insert(ciclo.end(), inversa.begin(), inversa.end());
    ciclo.push_back(origen);
    return true;
}

void resolverAgenteViajero() {
    if (!verificarDatosCargados()) return;

    if (capitales.size() < 2) {
        cout << "Se requieren al menos dos ciudades para resolver el TSP.\n";
        return;
    }

    if (capitales.size() > MAX_CIUDADES_TSP) {
        cout << "La solución exacta Held-Karp está limitada a " << MAX_CIUDADES_TSP
             << " ciudades por su complejidad O(n^2 * 2^n).\n";
        return;
    }

    const int origen = solicitarCiudad("Ingrese la ciudad inicial del recorrido: ");
    if (origen < 0) return;

    vector<int> ciclo;
    Distancia costo = 0;

    if (!resolverTSPHeldKarp(origen, ciclo, costo)) {
        cout << "No existe un ciclo hamiltoniano que visite todas las ciudades y regrese al origen.\n";
        return;
    }

    cout << "\n=== SOLUCIÓN EXACTA DEL AGENTE VIAJERO ===\n";
    imprimirRuta(ciclo);
    cout << "\nDistancia total mínima: " << costo << " km\n";
}

int obtenerOpcionMenu(int minimo, int maximo) {
    string entrada;
    while (true) {
        cout << "Seleccione una opción: ";
        getline(cin >> ws, entrada);
        long long opcion = 0;
        if (convertirEntero(entrada, opcion) && opcion >= minimo && opcion <= maximo) {
            return static_cast<int>(opcion);
        }
        cout << "Entrada inválida. Ingrese un número entre " << minimo << " y " << maximo << ".\n";
    }
}

int main() {
    system("chcp 65001 > nul");
    setlocale(LC_ALL, "es_MX.UTF-8");

    int opcion = -1;
    do {
        cout << "\n========== MENÚ PRINCIPAL ==========\n"
             << "1. Leer datos desde archivo\n"
             << "2. Mostrar árbol de rutas\n"
             << "3. Buscar la ruta más corta (Dijkstra)\n"
             << "4. Enumerar todas las rutas simples\n"
             << "5. Listar ciudades y conexiones\n"
             << "6. Resolver el agente viajero exacto (Held-Karp)\n"
             << "0. Salir\n"
             << "====================================\n";

        opcion = obtenerOpcionMenu(0, 6);

        switch (opcion) {
            case 1: leer(); break;
            case 2: mostrarArbol(); break;
            case 3: buscarRutaMasCorta(); break;
            case 4: buscarTodasLasRutas(); break;
            case 5: listarCiudades(); break;
            case 6: resolverAgenteViajero(); break;
            case 0: cout << "Programa finalizado.\n"; break;
        }
    } while (opcion != 0);

    return 0;
}
