#include <iostream>
#include <iomanip>
#include <chrono>
#include <ctime>
#include <string>

// ---------- Declaraciones (arriba de main) ----------
void imprimeLunes(int dia);
void imprimeJuevesAntesMediodia(int dia, int hora, int minuto);
void chequeaMartesOLuegoDeMediodia(int dia, int hora, int minuto);

void procesaArray(bool arr[], std::size_t n);           // versión bool
void procesaArray(int arr[], std::size_t n);            // versión int (punteros)

const char* nombreDia(int idx);

// ---------- main ----------
int main() {
    // 0=Lunes, 1=Martes, 2=Miércoles, 3=Jueves, 4=Viernes, 5=Sábado, 6=Domingo
    int diaActual = 3; // <-- CAMBIA AQUÍ para probar (ej. 0=Lunes, 3=Jueves, etc.)

    // Hora actual del sistema:
    int hora = 0, minuto = 0;
    {
        auto now   = std::chrono::system_clock::now();
        std::time_t tnow = std::chrono::system_clock::to_time_t(now);
        std::tm local{};
    #if defined(_WIN32)
        localtime_s(&local, &tnow);
    #else
        local = *std::localtime(&tnow);
    #endif
        hora   = local.tm_hour;   // 0..23
        minuto = local.tm_min;    // 0..59
    }

    // --- Para pruebas: puedes activar hora manual sin tocar la lógica ---
    bool usarHoraManual = false;      // <-- pon true para usar la hora manual
    int horaManual = 11, minutoManual = 30; // ej. 11:30 AM
    if (usarHoraManual) { hora = horaManual; minuto = minutoManual; }

    std::cout << "Dia actual: " << nombreDia(diaActual)
              << " | Hora: " << std::setw(2) << std::setfill('0') << hora
              << ":" << std::setw(2) << minuto << "\n\n";

    // 2) Si es lunes -> "es lunes"
    imprimeLunes(diaActual);

    // 3) Si es jueves antes de 12:00 -> "es jueves antes del mediodia"
    imprimeJuevesAntesMediodia(diaActual, hora, minuto);

    // 4) Función: si es martes -> "es martes" ; si no y es > 12:00 -> "no es martes pero es después de las 12 PM"
    chequeaMartesOLuegoDeMediodia(diaActual, hora, minuto);

    // 5) For sobre 7 días
    std::cout << "\n--- Ciclo de dias ---\n";
    for (int i = 0; i < 7; ++i) {
        if (i == diaActual) {
            std::cout << "Hoy SI es " << nombreDia(i) << "\n";
        } else {
            std::cout << "Hoy no es " << nombreDia(i) << "\n";
        }
    }

    // --------- Arrays y sobrecarga ---------
    std::cout << "\n--- Arrays ---\n";

    // 1) Array de 5 booleanos (contenido inicial irrelevante)
    bool banderas[5] = {true, true, true, true, true};
    procesaArray(banderas, 5); // coloca pares=false, impares=true
    std::cout << "bool[5] tras procesar (pares=false, impares=true): ";
    for (std::size_t i = 0; i < 5; ++i) {
        std::cout << (banderas[i] ? "true" : "false") << (i+1<5?" ":"");
    }
    std::cout << "\n";

    // 1.3) Array de 10 enteros inicializado 0..9
    int numeros[10];
    for (int i = 0; i < 10; ++i) numeros[i] = i;

    // 1.4-1.8) Sobrecarga para int[] con punteros, imprimir direcciones y valor %2
    std::cout << "\nDirecciones y valores (int%2) usando SOLO aritmetica de punteros:\n";
    procesaArray(numeros, 10);

    return 0;
}

// ---------- Definiciones (después de main) ----------
void imprimeLunes(int dia) {
    // 0 = Lunes
    if (dia == 0) {
        std::cout << "es lunes\n";
    }
}

void imprimeJuevesAntesMediodia(int dia, int hora, int minuto) {
    // 3 = Jueves ; "antes del mediodia" => antes de 12:00
    bool antesDeMediodia = (hora < 12); // 11:59 o menos
    if (dia == 3 && antesDeMediodia) {
        std::cout << "es jueves antes del mediodia\n";
    }
}

void chequeaMartesOLuegoDeMediodia(int dia, int hora, int minuto) {
    // 1 = Martes ; "después de las 12 PM" => hora >= 12:00
    if (dia == 1) {
        std::cout << "es martes\n";
    } else {
        bool despuesDeMediodia = (hora >= 12);
        if (despuesDeMediodia) {
            std::cout << "no es martes pero es despues de las 12 PM\n";
        }
    }
}

// --- Sobrecarga para bool[] ---
void procesaArray(bool arr[], std::size_t n) {
    for (std::size_t i = 0; i < n; ++i) {
        arr[i] = (i % 2 == 1); // pares=false, impares=true
    }
}

// --- Sobrecarga para int[] (SIN usar '[]', SOLO punteros) ---
// Requisitos:
//  - No modificar el puntero parametro (usar auxiliar).
//  - Reemplazar cada valor por valor%2.
//  - Imprimir direccion de cada elemento y el valor resultante.
void procesaArray(int arr[], std::size_t n) {
    int* aux = arr; // puntero auxiliar (no modificamos 'arr')
    for (std::size_t i = 0; i < n; ++i) {
        // *aux es el valor; (const void*)aux imprime la direccion
        *aux = (*aux) % 2;
        std::cout << (const void*)aux << " -> " << *aux << "\n";
        ++aux; // avanzar a la siguiente direccion de memoria
    }
}

const char* nombreDia(int idx) {
    static const char* nombres[7] = {
        "Lunes","Martes","Miercoles","Jueves","Viernes","Sabado","Domingo"
    };
    if (idx < 0 || idx > 6) return "Desconocido";
    return nombres[idx];
}
