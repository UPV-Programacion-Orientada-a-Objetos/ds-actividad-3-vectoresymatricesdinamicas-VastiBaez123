#include <iostream>

const int maxprods = 5; 
const int maxmatprim = 4; 
const float costobasemp = 2.0f; 

int numsemanas = 4;
float** planproduccion = nullptr; 
float* costosunitarios = nullptr; 
float* invmateriaprima = nullptr;

// Arreglo estático de requerimientos (5x4)
int vecrequerimientos[maxprods][maxmatprim] = {
    // MP1, MP2, MP3, MP4
    { 3, 1, 0, 0 },  
    { 2, 0, 1, 0 },  
    { 1, 2, 0, 1 },  
    { 0, 1, 3, 0 },  
    { 4, 0, 0, 1 }   
};


void iniciarSistema(){
    using namespace std; 
    
    cout << "inializacion de memoria" << endl;

    costosunitarios = new float[maxprods]; 
    cout << "-Vector Costos asignado" << endl;
    
    invmateriaprima = new float[maxmatprim];
    cout << "-Vector Materia Prima asignado" << endl;
    
    cout << "-Iniciando Matriz Planificacion (" << maxprods << "x" << numsemanas << ")" << endl;
    
    planproduccion = new float*[maxprods]; 
    
    for (int i = 0; i < maxprods; ++i) {
        planproduccion[i] = new float[numsemanas]; 
    }
    
    cout << "Inicialización de estructuras dinámicas completa" << endl;
}


void meterDatosPrueba() {
    using namespace std;
    cout << "Cargando datos de prueba" << endl;

    for (int i = 0; i < maxprods; ++i) {
        costosunitarios[i] = 10.0f + i * 5.0f; 
    }
    for (int i = 0; i < maxmatprim; ++i) {
        invmateriaprima[i] = 1000.0f + i * 500.0f; 
    }
    
    for (int i = 0; i < maxprods; ++i) {
        for (int j = 0; j < numsemanas; ++j) {
            planproduccion[i][j] = 100.0f + i * 10.0f + j * 5.0f;
        }
    }
    cout << "Datos de prueba cargados" << endl;
}


void agregarSemana() {
    using namespace std;
    int nuevaColumna = numsemanas + 1;
    float** matTemporal = nullptr;

    cout << "\nRedimensionando matriz de planificacion" << endl;
    
    matTemporal = new float*[maxprods]; 
    for (int i = 0; i < maxprods; ++i) {
        matTemporal[i] = new float[nuevaColumna];
    }
    
    for (int i = 0; i < maxprods; ++i) {
        for (int j = 0; j < numsemanas; ++j) {
            matTemporal[i][j] = planproduccion[i][j]; 
        }
        matTemporal[i][numsemanas] = 0.0f; 
    }

    for (int i = 0; i < maxprods; ++i) {
        delete[] planproduccion[i]; 
    }
    delete[] planproduccion;
    
    planproduccion = matTemporal; 
    numsemanas = nuevaColumna;
    
    cout << "Agregando Semana " << numsemanas - 1 << "..." << endl;
    cout << "Matriz redimensionada a " << maxprods << "x" << numsemanas << " Memoria gestionada!" << endl;
}


void calculaConsumoMP(int semana, float* consumoMP) {
    for (int k = 0; k < maxmatprim; ++k) {
        consumoMP[k] = 0.0f;
    }

    for (int i = 0; i < maxprods; ++i) {
        float produccion = planproduccion[i][semana];

        for (int k = 0; k < maxmatprim; ++k) {
            consumoMP[k] += produccion * vecrequerimientos[i][k];
        }
    }
}


void calculaCOGS(int semana) {
    using namespace std;
    float costoTotal = 0.0f;
    float totalUnidades = 0.0f;
    float valorInvFinal = 0.0f;

    for (int i = 0; i < maxprods; ++i) {
        float unidades = planproduccion[i][semana];
        float costoUnitario = costosunitarios[i];
        
        costoTotal += unidades * costoUnitario;
        totalUnidades += unidades;
    }
    
    float consumoMP[maxmatprim]; 
    calculaConsumoMP(semana, consumoMP);
    
    cout << "\nReporte de Costos (Semana " << semana << ")" << endl;
    cout << "Producción Total (Semana " << semana << "): " << totalUnidades << " unidades." << endl;
    cout << "Costo Total de Producción (COGS): $" << costoTotal << endl;
    
    cout << "Consumo de Materia Prima: ";
    for (int k = 0; k < maxmatprim; ++k) {
        float restante = invmateriaprima[k] - consumoMP[k];
        if (restante < 0) restante = 0; 
        valorInvFinal += restante * costobasemp;
        cout << "MP-" << k << " (" << consumoMP[k] << " uni) ";
    }
    cout << endl;
    
    cout << "Valor del inventario final: $" << valorInvFinal << endl;
}


void liberarMemoria() {
    using namespace std;
    cout << "\nLiberando memoria anti fugas" << endl;

    if (costosunitarios != nullptr) {
        delete[] costosunitarios;
        costosunitarios = nullptr; 
        cout << "Vector de Costos liberado" << endl;
    }

    if (invmateriaprima != nullptr) {
        delete[] invmateriaprima;
        invmateriaprima = nullptr;
        cout << "Vector de Materia Prima liberado" << endl;
    }

    if (planproduccion != nullptr) {
        cout << " Liberando matriz " << endl;
        
        for (int i = 0; i < maxprods; ++i) {
            delete[] planproduccion[i]; 
        }

        delete[] planproduccion;
        
        planproduccion = nullptr;
        cout << "  - Matriz de planificacion liberada" << endl;
    }

    cout << "Liberación de memoria completa" << endl;
}


int main() {
    using namespace std;
    int opcion = 0;

    iniciarSistema(); 
    meterDatosPrueba();

    do {
        cout << "\nSistema de Planificación y Costos" << endl;
        cout << "1. Ver Plan de Producción" << endl;
        cout << "2. Agregar Nueva Semana " << endl;
        cout << "3. Modificar Producción" << endl;
        cout << "4. Calcular COGS y Final Inventory" << endl;
        cout << "5. Salir" << endl;
        cout << "Opción seleccionada: ";
        if (!(cin >> opcion)) { 
            cin.clear();
            cin.ignore(10000, '\n');
            opcion = 0;
        }

        if (opcion == 1) {
            cout << "\n[Matriz Planificacion (" << maxprods << "x" << numsemanas << ")]" << endl;
            cout << "       ";
            for(int j = 0; j < numsemanas; ++j) cout << "Sem " << j << " ";
            cout << endl;
            for (int i = 0; i < maxprods; ++i) {
                cout << "Prod " << i << ": ";
                for (int j = 0; j < numsemanas; ++j) {
                    cout << planproduccion[i][j] << " ";
                }
                cout << endl;
            }
        } else if (opcion == 2) {
            agregarSemana(); 
        } else if (opcion == 3) {
            int prod, sem;
            float cant;
            cout << "Producto (0-" << maxprods - 1 << "): "; cin >> prod;
            cout << "Semana (0-" << numsemanas - 1 << "): "; cin >> sem;
            
            if (prod >= 0 && prod < maxprods && sem >= 0 && sem < numsemanas) {
                cout << "Cantidad a producir (actual: " << planproduccion[prod][sem] << "): "; cin >> cant;
                planproduccion[prod][sem] = cant;
                cout << "Producción actualizada." << endl;
            } else {
                cout << "error dato fuera de los límites." << endl;
            }

        } else if (opcion == 4) {
            int sem;
            cout << "Semana para el cálculo de costos (0-" << numsemanas - 1 << "): "; cin >> sem;
            if (sem >= 0 && sem < numsemanas) {
                calculaCOGS(sem); 
            } else {
                cout << "error semana fuera de los límites" << endl;
            }
        } else if (opcion != 5) {
            cout << "Opcion no valida" << endl;
        }

    } while (opcion != 5); 

    liberarMemoria();
    cout << "\nChao chao buen trabajo" << endl;
    return 0;
}
