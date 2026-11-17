#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/transaccion.h"
#include "../include/videojuego.h"
#include "../include/persona.h"

int esFechaValida(Fecha f) {
    if (f.anio < 2000 || f.anio > 2100) return 0;
    if (f.mes < 1 || f.mes > 12) return 0;
    if (f.dia < 1) return 0;

    int diasMes[] = {0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    if ((f.anio % 4 == 0 && f.anio % 100 != 0) || (f.anio % 400 == 0)) {
        diasMes[2] = 29;
    }

    if (f.dia > diasMes[f.mes]) return 0;
    return 1;
}

void menuTransacciones() {
    int idBuscado;
    int opcion;
    do {
        system("cls || clear");
        printf("==== MENU DE TRANSACCIONES ====\n");
        printf("1. Listado de transacciones.\n");
        printf("2. Ver transaccion especifica.\n");
        printf("3. Registrar venta.\n");
        printf("0. Volver al menu principal.\n");
        printf("Seleccione una opcion: ");
        scanf("%d", &opcion);
        while (getchar() != '\n');

        switch (opcion) {
            case 0:
                printf("Volver al menu\n");
                break;
            case 1:
                listadoTransacciones(ARCH_TRANSACCIONES);
                break;
            case 2:
                printf("Ingrese el ID de la transaccion a buscar: ");
                scanf("%d", &idBuscado);
                while (getchar() != '\n');
                verTransaccionDetalle(ARCH_TRANSACCIONES, idBuscado);
                break;
            case 3:
                procesarCarritoCompras(ARCH_TRANSACCIONES);
                break;
            default:
                printf("Opcion no valida, seleccione una opcion valida.\n");
                break;
        }

        if (opcion != 0) {
            printf("\nPresione ENTER para continuar...");
            getchar();
        }
    } while (opcion != 0);
}

//* =========================== MUESTRAS DE TRANSACCIONES ======================== *//
void listadoTransacciones(char archivoTrans[]) {
    FILE *buffer = fopen(archivoTrans, "rb");
    if (buffer == NULL) {
        printf("No hay transacciones registradas todavia.\n");
    } else {
        Transaccion aux;
        while (fread(&aux, sizeof(Transaccion), 1, buffer) == 1) {
            printf("================================================\n");
            printf("ID Transaccion: #%d | ID Videojuego: %d | Cant: %d | Fecha: %02d/%02d/%d\n",
                   aux.idTransaccion, aux.idJuegoVendido, aux.cantidadVendida,
                   aux.fecha.dia, aux.fecha.mes, aux.fecha.anio);
        }
        printf("================================================\n");
        fclose(buffer);
    }
}

void verTransaccionDetalle(char archivoTrans[], int idBuscado) {
    FILE *buffer = fopen(archivoTrans, "rb");
    if (buffer == NULL) {
        printf("Error al abrir el archivo de transacciones.\n");
    } else {
        Transaccion aux;
        while (fread(&aux, sizeof(Transaccion), 1, buffer) == 1) {
            if (aux.idTransaccion == idBuscado) {
                mostrarTrans(aux);
                fclose(buffer);
                return;
            }
        }
        fclose(buffer);
        printf("La transaccion #%d no fue encontrada.\n", idBuscado);
    }
}

void mostrarTrans(Transaccion trans) {
    printf("\n========= DETALLES DE TRANSACCION =========\n");
    printf("ID Transaccion:   #%d\n", trans.idTransaccion);
    printf("Fecha:            %02d/%02d/%d\n", trans.fecha.dia, trans.fecha.mes, trans.fecha.anio);
    printf("ID Juego vendido: %d\n", trans.idJuegoVendido);
    printf("Cantidad vendida: %d\n", trans.cantidadVendida);
    printf("Precio unitario:  $%.2f\n", trans.precioUnitarioVenta);
    printf("Ganancia neta:    $%.2f\n", trans.gananciaTotal);
    printf("DNI Cliente:      %s\n", trans.dniCliente);
    printf("DNI Empleado:     %s\n", trans.dniEmpleado);
    printf("===========================================\n");
}

//* FUNCION AUXILIAR PARA GENERAR UN ID *//
long generarIdTransaccion(char archivoTransaccion[]) {
    FILE *buffer = fopen(archivoTransaccion, "rb");
    if (buffer == NULL) {
        return 0;
    }
    fseek(buffer, 0, SEEK_END);
    long bytes = ftell(buffer);
    fclose(buffer);

    if (bytes <= 0) {
        return 0;
    }
    return bytes / (long)sizeof(Transaccion);
}

//* PARA REGISTRAR UNA TRANSACCION EN EL ARCHIVO DE TRANSACCIONES *//
int registrarTransaccion(char archivoTransacciones[], Transaccion trans, int idJuego, int cantidadVendida) {
    FILE *buffer = fopen(archivoTransacciones, "ab");
    if (buffer == NULL) {
        return -1; // Fallo al abrir archivo
    }

    if (fwrite(&trans, sizeof(Transaccion), 1, buffer) != 1) {
        fclose(buffer);
        return -1;
    }
    fclose(buffer);

    if (actualizarStockVideojuego(idJuego, -cantidadVendida) == 0) {
        return -3; // Fallo al descontar stock
    }

    return 1; // Registro exitoso
}

//* AUXILIAR PARA USAR EN EL CARRITO *//
int buscarIndiceEnCarrito(ItemCarrito *carrito, int cantItems, int idBuscado) {
    for (int i = 0; i < cantItems; i++) {
        if (carrito[i].idJuego == idBuscado) {
            return i;
        }
    }
    return -1;
}

//* FUNCION AUXILIAR PARA REDIMENSIONAR UN ARREGLO *//
ItemCarrito* redimensionCarrito(ItemCarrito *carrito, int *cantItems, ItemCarrito nuevoItem) {
    ItemCarrito *temp = realloc(carrito, (*cantItems + 1) * sizeof(ItemCarrito));
    if (temp == NULL) {
        printf("Error critico de memoria al redimensionar el carrito.\n");
        free(carrito);
        *cantItems = 0;
        return NULL;
    }

    carrito = temp;
    carrito[*cantItems] = nuevoItem;
    (*cantItems)++;
    return carrito;
}

//* CARRITO DE COMPRAS *//
ItemCarrito* cargarItemsCarrito(int *cantItems) {
    ItemCarrito *carrito = NULL;
    *cantItems = 0;
    char seguir = 's';

    while (seguir == 's' || seguir == 'S') {
        int idBuscado, cantDeseada;
        Videojuego comprado;

        printf("\nIngrese el ID del videojuego: ");
        scanf("%d", &idBuscado);
        while (getchar() != '\n');

        int idx = buscarIndiceEnCarrito(carrito, *cantItems, idBuscado);
        int yaCargados = (idx != -1) ? carrito[idx].cantidad : 0;

        if (buscarVideojuegoPorId(idBuscado, &comprado) == 0 || (comprado.stock - yaCargados) <= 0) {
            printf("Juego inexistente o sin stock disponible.\n");
        } else {
            int stockDisp = comprado.stock - yaCargados;
            printf("Juego: %s | Stock: %d | Precio lista: $%.2f\n",
                   comprado.titulo, stockDisp, comprado.precioAdquisicion * MARGEN_GANANCIA);
            printf("Cuantas unidades desea llevar?: ");
            scanf("%d", &cantDeseada);
            while (getchar() != '\n');

            if (cantDeseada <= 0 || cantDeseada > stockDisp) {
                printf("Cantidad invalida.\n");
            } else if (idx != -1) {
                carrito[idx].cantidad += cantDeseada;
                printf("Unidades sumadas al carrito.\n");
            } else {
                ItemCarrito nuevo = {idBuscado, cantDeseada, comprado.precioAdquisicion};
                carrito = redimensionCarrito(carrito, cantItems, nuevo);
                if (carrito == NULL) return NULL;
                printf("Producto agregado al carrito.\n");
            }
        }

        printf("\nDesea agregar otro producto? (s/n): ");
        scanf(" %c", &seguir);
        while (getchar() != '\n');
    }

    return carrito;
}

int procesarCarritoCompras(char archivoTransacciones[]) {
    int cantItems = 0;
    ItemCarrito *carrito = cargarItemsCarrito(&cantItems);

    if (carrito == NULL || cantItems == 0) {
        printf("Carrito vacio. Operacion cancelada.\n");
        return 0;
    }

    Transaccion base;
    Persona clienteAux, empleadoAux;

    printf("\n=== DATOS DE FACTURACION ===\n");
    printf("DNI del cliente: ");
    fgets(base.dniCliente, sizeof(base.dniCliente), stdin);
    base.dniCliente[strcspn(base.dniCliente, "\n")] = '\0';

    if (buscarPersonaPorDni(base.dniCliente, &clienteAux) == 0) {
        printf("Error: No existe un cliente activo con DNI %s. Registrelo primero.\n", base.dniCliente);
        free(carrito);
        return 0;
    }

    printf("DNI del empleado: ");
    fgets(base.dniEmpleado, sizeof(base.dniEmpleado), stdin);
    base.dniEmpleado[strcspn(base.dniEmpleado, "\n")] = '\0';

    if (buscarPersonaPorDni(base.dniEmpleado, &empleadoAux) == 0 || strcmp(empleadoAux.rol, "cliente") == 0) {
        printf("Error: El DNI %s no corresponde a un empleado activo.\n", base.dniEmpleado);
        free(carrito);
        return 0;
    }

    do {
        printf("Fecha (Dia Mes Anio): ");
        scanf("%d %d %d", &base.fecha.dia, &base.fecha.mes, &base.fecha.anio);
        while (getchar() != '\n');
        if (!esFechaValida(base.fecha)) {
            printf("Fecha invalida (verifique dias del mes/anio). Intente nuevamente.\n");
        }
    } while (!esFechaValida(base.fecha));

    // Consulta descuento VIP acumulado del cliente
    float descVip = calcularDescuentoVipCliente(base.dniCliente);
    if (descVip > 0.0f) {
        printf("\n[!] Beneficio VIP aplicado para %s: %.0f%% de descuento!\n",
               clienteAux.nombre, descVip * 100);
    }

    int idBase = (int)generarIdTransaccion(archivoTransacciones);
    float totalTicket = 0.0f;

    for (int i = 0; i < cantItems; i++) {
        Transaccion trans = base;
        trans.idTransaccion = idBase + i + 1;
        trans.idJuegoVendido = carrito[i].idJuego;
        trans.cantidadVendida = carrito[i].cantidad;
        trans.precioUnitarioVenta = (carrito[i].precioAdquisicion * MARGEN_GANANCIA) * (1.0f - descVip);
        trans.gananciaTotal = (trans.precioUnitarioVenta - carrito[i].precioAdquisicion) * trans.cantidadVendida;

        totalTicket += trans.precioUnitarioVenta * trans.cantidadVendida;

        if (registrarTransaccion(archivoTransacciones, trans, trans.idJuegoVendido, trans.cantidadVendida) != 1) {
            printf("Error al registrar el juego ID %d.\n", trans.idJuegoVendido);
        }
    }

    free(carrito);
    printf("\nVenta finalizada! %d productos registrados. Total cobrado: $%.2f\n", cantItems, totalTicket);
    return 1;
}
