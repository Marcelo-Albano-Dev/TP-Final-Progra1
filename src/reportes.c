#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/reportes.h"
#include "../include/videojuego.h"


void limpiarBufferReportes() {
    while (getchar() != '\n');
}

// Menu de Reportes
void menuReportes() {
    int opcion = 0;

    do {
        system("cls || clear");
        printf("====================================\n");
        printf("        MODULO DE REPORTES          \n");
        printf("====================================\n");
        printf("1. Reporte de recaudacion mensual\n");
        printf("2. Reporte de venta de mayor ganancia\n");
        printf("3. Reporte de stock critico (QuickSort)\n");
        printf("4. Listar historial de cambios de precio\n");
        printf("5. Analisis de ganancias por rango de fechas\n");
        printf("0. Salir del Modulo\n");
        printf("------------------------------------\n");
        printf("Ingrese una opcion: ");
        scanf("%d", &opcion);
        limpiarBufferReportes();

        switch (opcion) {
            case 1:
                mostrarReporteMensual();
                break;
            case 2:
                ventaMayorGanancia();
                break;
            case 3:
                mostrarStockCritico();
                break;
            case 4:
                listarReportesPrecio();
                break;
            case 5:
                reporteGananciasPorFecha();
                break;
            case 0:
                printf("\nVolviendo al menu principal...\n");
                break;
            default:
                printf("\nOpcion invalida.\n");
        }

        if (opcion != 0) {
            printf("\nPresione ENTER para continuar...");
            getchar();
        }
    } while (opcion != 0);
}

// Verifica si hubo modificacion de precio para un juego en un mes/anio dado
int consultarCambioPrecio(int idJuego, int mes, int anio) {
    FILE *buffer = fopen(ARCH_REPORTES_PRECIO, "rb");
    ReportesPrecio aux;

    if (buffer == NULL) {
        return 0;
    }

    while (fread(&aux, sizeof(ReportesPrecio), 1, buffer) == 1) {
        if (aux.idJuego == idJuego &&
            aux.fechaModificacion.mes == mes &&
            aux.fechaModificacion.anio == anio) {
            fclose(buffer);
            return 1;
        }
    }
    fclose(buffer);
    return 0;
}

// Reporte de recaudacion y ganancia neta filtrada por mes y anio
void mostrarReporteMensual() {
    FILE *buffer = fopen(ARCH_TRANSACCIONES, "rb");
    Transaccion aux;
    int mes, anio, cambio;
    float recaudacionTotal = 0.0f;
    float gananciaTotal = 0.0f;

    if (buffer == NULL) {
        printf("\nNo existe el archivo de transacciones todavia.\n");
        return;
    }

    printf("\nIngrese anio: ");
    scanf("%d", &anio);
    limpiarBufferReportes();

    printf("Ingrese el mes (1-12): ");
    scanf("%d", &mes);
    limpiarBufferReportes();

    if (mes < 1 || mes > 12 || anio < 1) {
        printf("\nFecha incorrecta.\n");
        fclose(buffer);
        return;
    }

    printf("\n=== RECAUDACION MENSUAL (%02d/%d) ===\n", mes, anio);

    while (fread(&aux, sizeof(Transaccion), 1, buffer) == 1) {
        if (aux.fecha.mes == mes && aux.fecha.anio == anio) {
            float venta = aux.precioUnitarioVenta * aux.cantidadVendida;
            float ganancia = (aux.precioUnitarioVenta - aux.gananciaTotal) * aux.cantidadVendida;

            recaudacionTotal += venta;
            gananciaTotal += ganancia;
            cambio = consultarCambioPrecio(aux.idJuegoVendido, mes, anio);

            printf("Transaccion #%d | Fecha: %02d/%02d/%d | Juego ID: %d | Ganancia: $%.2f | Cambio precio: %s\n",
                   aux.idTransaccion, aux.fecha.dia, aux.fecha.mes, aux.fecha.anio,
                   aux.idJuegoVendido, ganancia, cambio ? "SI" : "NO");
        }
    }

    printf("------------------------------------\n");
    printf("Recaudacion Bruta Total: $%.2f\n", recaudacionTotal);
    printf("Ganancia Neta Total:     $%.2f\n", gananciaTotal);
    fclose(buffer);
}

// Busca y muestra la transaccion individual que genero mayor ganancia neta
void ventaMayorGanancia() {
    FILE *buffer = fopen(ARCH_TRANSACCIONES, "rb");
    Transaccion aux;
    Transaccion mayor;
    Videojuego juego;
    float mayorGanancia = 0.0f;
    int encontrado = 0;

    if (buffer == NULL) {
        printf("\nNo existe el archivo de transacciones todavia.\n");
        return;
    }

    while (fread(&aux, sizeof(Transaccion), 1, buffer) == 1) {
        float gananciaActual = (aux.precioUnitarioVenta - aux.gananciaTotal) * aux.cantidadVendida;
        if (!encontrado || gananciaActual > mayorGanancia) {
            mayor = aux;
            mayorGanancia = gananciaActual;
            encontrado = 1;
        }
    }
    fclose(buffer);

    if (!encontrado) {
        printf("\nNo hay transacciones registradas.\n");
        return;
    }

    printf("\n=== VENTA DE MAYOR GANANCIA ===\n");
    printf("ID Transaccion: #%d\n", mayor.idTransaccion);
    printf("Ganancia Neta:  $%.2f\n", mayorGanancia);
    printf("Fecha:          %02d/%02d/%d\n", mayor.fecha.dia, mayor.fecha.mes, mayor.fecha.anio);

    if (buscarVideojuegoPorId(mayor.idJuegoVendido, &juego)) {
        printf("ID Juego:       %d\n", juego.idJuego);
        printf("Titulo:         %s\n", juego.titulo);
    } else {
        printf("ID Juego:       %d (No encontrado en el catalogo activo)\n", mayor.idJuego);
    }
}

// Carga en un arreglo dinamico los videojuegos activos con stock menor o igual al critico
Videojuego *cargarStockCritico(int *cantidad) {
    FILE *buffer = fopen(ARCH_VIDEOJUEGOS, "rb");
    Videojuego aux;
    Videojuego *vector = NULL;
    *cantidad = 0;

    if (buffer == NULL) {
        return NULL;
    }

    while (fread(&aux, sizeof(Videojuego), 1, buffer) == 1) {
        if (aux.stock >= 0 && aux.stock <= STOCK_CRITICO) {
            Videojuego *nuevo = realloc(vector, (*cantidad + 1) * sizeof(Videojuego));
            if (nuevo == NULL) {
                free(vector);
                fclose(buffer);
                *cantidad = 0;
                return NULL;
            }
            vector = nuevo;
            vector[*cantidad] = aux;
            (*cantidad)++;
        }
    }
    fclose(buffer);
    return vector;
}

// Intercambia dos estructuras Videojuego en memoria
void intercambiarVideojuegos(Videojuego *vj1, Videojuego *vj2) {
    Videojuego aux = *vj1;
    *vj1 = *vj2;
    *vj2 = aux;
}

// Particion para QuickSort por stock ascendente
int particionStock(Videojuego juegos[], int inicio, int fin) {
    int pivote = juegos[fin].stock;
    int i = inicio - 1;

    for (int j = inicio; j < fin; j++) {
        if (juegos[j].stock <= pivote) {
            i++;
            intercambiarVideojuegos(&juegos[i], &juegos[j]);
        }
    }
    intercambiarVideojuegos(&juegos[i + 1], &juegos[fin]);
    return i + 1;
}


// Ordena el arreglo de videojuegos por stock de menor a mayor usando QuickSort
void quickSortStock(Videojuego juegos[], int inicio, int fin) {
    if (inicio < fin) {
        int pivote = particionStock(juegos, inicio, fin);
        quickSortStock(juegos, inicio, pivote - 1);
        quickSortStock(juegos, pivote + 1, fin);
    }
}

// Lista los juegos en stock critico ordenados de menor a mayor disponibilidad
void mostrarStockCritico() {
    int cantidad = 0;
    Videojuego *vector = cargarStockCritico(&cantidad);

    if (cantidad == 0 || vector == NULL) {
        printf("\nNo hay juegos con stock critico (<= %d unidades).\n", STOCK_CRITICO);
        free(vector);
        return;
    }

    quickSortStock(vector, 0, cantidad - 1);

    printf("\n=== VIDEOJUEGOS CON STOCK CRITICO (<= %d) ===\n", STOCK_CRITICO);
    printf("%-5s | %-30s | %-6s\n", "ID", "TITULO", "STOCK");
    printf("-----------------------------------------------\n");
    for (int i = 0; i < cantidad; i++) {
        printf("%-5d | %-30s | %-6d\n", vector[i].idJuego, vector[i].titulo, vector[i].stock);
    }
    free(vector);
}

// Muestra el historial de modificaciones de precios registradas
void listarReportesPrecio() {
    FILE *buffer = fopen(ARCH_REPORTES_PRECIO, "rb");
    ReportesPrecio aux;

    if (buffer == NULL) {
        printf("\nNo hay reportes de cambios de precio registrados.\n");
        return;
    }

    printf("\n=== HISTORIAL DE CAMBIOS DE PRECIO ===\n");
    while (fread(&aux, sizeof(ReportesPrecio), 1, buffer) == 1) {
        printf("Reporte #%d | Juego ID: %d | Anterior: $%.2f | Nuevo: $%.2f | Fecha: %02d/%02d/%d\n",
               aux.idReportes, aux.idJuego, aux.precioAdquisicionViejo,
               aux.precioAdquisicionNuevo, aux.fechaModificacion.dia,
               aux.fechaModificacion.mes, aux.fechaModificacion.anio);
    }
    fclose(buffer);
}

// Suma recursivamente la ganancia neta de las transacciones dentro de un rango de fechas
float analisisGanancias(FILE *buffer, Fecha desde, Fecha hasta) {
    Transaccion aux;

    if (fread(&aux, sizeof(Transaccion), 1, buffer) != 1) {
        return 0.0f;
    }

    float gananciaActual = 0.0f;
    if (compararFechas(aux.fecha, desde) >= 0 && compararFechas(aux.fecha, hasta) <= 0) {
        gananciaActual = (aux.precioUnitarioVenta - aux.gananciaTotal) * aux.cantidadVendida;
    }

    return gananciaActual + analisisGanancias(buffer, desde, hasta);
}

// Solicita un rango de fechas y muestra las ganancias acumuladas en ese periodo
void reporteGananciasPorFecha() {
    FILE *buffer = fopen(ARCH_TRANSACCIONES, "rb");
    Fecha desde, hasta;
    float total;

    if (buffer == NULL) {
        printf("\nNo existe el archivo de transacciones todavia.\n");
        return;
    }

    printf("\nIngrese fecha inicial (DIA MES ANIO): ");
    scanf("%d %d %d", &desde.dia, &desde.mes, &desde.anio);
    limpiarBufferReportes();

    printf("Ingrese fecha final (DIA MES ANIO): ");
    scanf("%d %d %d", &hasta.dia, &hasta.mes, &hasta.anio);
    limpiarBufferReportes();

    if (compararFechas(desde, hasta) > 0) {
        printf("\nEl rango de fechas es incorrecto (la fecha inicial es posterior a la final).\n");
        fclose(buffer);
        return;
    }

    total = analisisGanancias(buffer, desde, hasta);

    printf("\n=== ANALISIS DE GANANCIAS POR PERIODO ===\n");
    printf("Desde: %02d/%02d/%d\n", desde.dia, desde.mes, desde.anio);
    printf("Hasta: %02d/%02d/%d\n", hasta.dia, hasta.mes, hasta.anio);
    printf("Ganancias netas acumuladas: $%.2f\n", total);
    fclose(buffer);
}

// Compara dos fechas (-1 si f1 < f2, 1 si f1 > f2, 0 si son iguales)
int compararFechas(Fecha f1, Fecha f2) {
    if (f1.anio < f2.anio) return -1;
    if (f1.anio > f2.anio) return 1;
    if (f1.mes < f2.mes) return -1;
    if (f1.mes > f2.mes) return 1;
    if (f1.dia < f2.dia) return -1;
    if (f1.dia > f2.dia) return 1;
    return 0;
}
