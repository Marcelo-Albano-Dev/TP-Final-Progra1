#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/transaccion.h"
#include "../include/videojuego.h"

#define MARGEN_GANANCIA 1.30f

void menuTransacciones()
{
    int idBuscado;
    int opcion;
    do
    {
        printf("\n==== MENU DE TRANSACCIONES ====\n");
        printf("1. Listado de transacciones.\n");
        printf("2. Ver transaccion especifica.\n");
        printf("3. Registrar venta.\n");
        printf("0. Volver al menu principal.\n");
        scanf("%d", &opcion);

        switch (opcion)
        {
            case 0:
                printf("Volver al menu\n");
                break;
            case 1:
                listadoTransacciones("transacciones.bin");
                break;
            case 2:
                printf("Ingrese el ID de la transaccion a buscar: ");
                scanf("%d", &idBuscado);
                verTransaccionDetalle("transacciones.bin", idBuscado);
                break;
            case 3:
                procesarCarritoCompras("transacciones.bin");
                break;
            default:
                printf("Opcion no valida. Seleccione una opcion valida.\n");
                break;
        }
    } while (opcion != 0);
}

//* =========================== MUESTRAS DE TRANSACCIONES ======================== *//

void listadoTransacciones(char archivoTrans[])
{
    FILE *buffer = fopen(archivoTrans, "rb");
    if (buffer == NULL)
    {
        printf("Error al abrir el archivo.\n");
        return;
    }

    Transaccion aux;
    while (fread(&aux, sizeof(Transaccion), 1, buffer))
    {
        printf("================\n");
        printf("ID Transaccion: %d | Id videojuego: %d | Fecha: %d/%d/%d\n",
               aux.idTransaccion, aux.idJuegoVendido, aux.fecha.dia, aux.fecha.mes, aux.fecha.anio);
        printf("================\n");
    }
    fclose(buffer);
}

void verTransaccionDetalle(char archivoTrans[], int idBuscado)
{
    FILE *buffer = fopen(archivoTrans, "rb");
    if (buffer == NULL)
    {
        printf("Error al abrir el archivo de transacciones.\n");
        return;
    }

    Transaccion aux;
    while (fread(&aux, sizeof(Transaccion), 1, buffer) == 1)
    {
        if (aux.idTransaccion == idBuscado)
        {
            mostrarTrans(aux);
            fclose(buffer);
            return;
        }
    }
    fclose(buffer);
    printf("La transaccion no fue encontrada.\n");
}

void mostrarTrans(Transaccion trans)
{
    printf("\n========= DETALLES DE TRANSACCION =========\n");
    printf("Id: %d\n", trans.idTransaccion);
    printf("Fecha: %d/%d/%d\n", trans.fecha.dia, trans.fecha.mes, trans.fecha.anio);
    printf("ID del juego vendido: %d\n", trans.idJuegoVendido);
    printf("Cantidad vendida: %d\n", trans.cantidadVendida);
    printf("Precio unitario: $%.2f\n", trans.precioUnitarioVenta);
    printf("Ganancia total: $%.2f\n", trans.gananciaTotal);
    printf("DNI Cliente: %s\n", trans.dniCliente);
    printf("DNI Empleado: %s\n", trans.dniEmpleado);
    printf("===========================================\n");
}

//* =========================== PERSISTENCIA Y AUXILIARES =========================== *//

long generarIdTransaccion(char archivoTransaccion[])
{
    FILE *buffer = fopen(archivoTransaccion, "rb");
    if (buffer == NULL) return 1;

    fseek(buffer, 0, SEEK_END);
    long bytes = ftell(buffer);
    fclose(buffer);

    if (bytes <= 0) return 1;
    return (bytes / (long)sizeof(Transaccion)) + 1;
}

int registrarTransaccion(char archivoTransacciones[], Transaccion trans, int idJuego, int cantidadVendida)
{
    if (actualizarStockVideojuego(idJuego, -cantidadVendida) == 0)
    {
        return -3; // Fallo al descontar stock
    }

    FILE *buffer = fopen(archivoTransacciones, "ab");
    if (buffer == NULL) return -1;

    if (fwrite(&trans, sizeof(Transaccion), 1, buffer) != 1)
    {
        fclose(buffer);
        return -1;
    }
    fclose(buffer);
    return 1;
}

//* =========================== CARRITO DE COMPRAS =========================== *//

int buscarIndiceEnCarrito(ItemCarrito *carrito, int cantItems, int idBuscado)
{
    for (int i = 0; i < cantItems; i++)
    {
        if (carrito[i].idJuego == idBuscado) return i;
    }
    return -1;
}

ItemCarrito* redimensionCarrito(ItemCarrito *carrito, int *cantItems, ItemCarrito nuevoItem)
{
    ItemCarrito *temp = realloc(carrito, (*cantItems + 1) * sizeof(ItemCarrito));
    if (temp == NULL)
    {
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

ItemCarrito* cargarItemsCarrito(int *cantItems)
{
    ItemCarrito *carrito = NULL;
    *cantItems = 0;
    char seguir = 's';

    while (seguir == 's' || seguir == 'S')
    {
        int idBuscado, cantDeseada;
        Videojuego comprado;

        printf("\nIngrese el ID del videojuego: ");
        scanf("%d", &idBuscado);

        int idx = buscarIndiceEnCarrito(carrito, *cantItems, idBuscado);
        int yaCargados = (idx != -1) ? carrito[idx].cantidad : 0;

        if (buscarVideojuegoPorId(idBuscado, &comprado) == 0 || (comprado.stock - yaCargados) <= 0)
        {
            printf("Juego inexistente o sin stock disponible.\n");
        }
        else
        {
            int stockDisp = comprado.stock - yaCargados;
            printf("Juego: %s | Stock: %d | Precio: $%.2f\n", comprado.titulo, stockDisp, comprado.precioAdquisicion * MARGEN_GANANCIA);
            printf("Cuantas unidades desea llevar?: ");
            scanf("%d", &cantDeseada);

            if (cantDeseada <= 0 || cantDeseada > stockDisp)
            {
                printf("Cantidad invalida.\n");
            }
            else if (idx != -1)
            {
                carrito[idx].cantidad += cantDeseada;
                printf("Unidades sumadas al carrito.\n");
            }
            else
            {
                ItemCarrito nuevo = {idBuscado, cantDeseada, comprado.precioAdquisicion};
                carrito = redimensionCarrito(carrito, cantItems, nuevo);
                if (carrito == NULL) return NULL;
                printf("Producto agregado al carrito.\n");
            }
        }

        printf("\nDesea agregar otro producto? (s/n): ");
        scanf(" %c", &seguir);
    }

    return carrito;
}

int procesarCarritoCompras(char archivoTransacciones[])
{
    int cantItems = 0;
    ItemCarrito *carrito = cargarItemsCarrito(&cantItems);

    if (carrito == NULL || cantItems == 0)
    {
        printf("Carrito vacio. Operacion cancelada.\n");
        return 0;
    }

    Transaccion base;
    printf("\n=== DATOS DE FACTURACION ===\n");
    printf("DNI del cliente: ");
    scanf(" %14s", base.dniCliente);
    printf("DNI del empleado: ");
    scanf(" %14s", base.dniEmpleado);

    printf("Fecha (Dia Mes Anio): ");
    scanf("%d %d %d", &base.fecha.dia, &base.fecha.mes, &base.fecha.anio);
    while (base.fecha.dia < 1 || base.fecha.dia > 31 ||
           base.fecha.mes < 1 || base.fecha.mes > 12 ||
           base.fecha.anio < 2000 || base.fecha.anio > 2100)
    {
        printf("Fecha invalida. Reingrese (Dia Mes Anio): ");
        scanf("%d %d %d", &base.fecha.dia, &base.fecha.mes, &base.fecha.anio);
    }

    int idBase = (int)generarIdTransaccion(archivoTransacciones);

    for (int i = 0; i < cantItems; i++)
    {
        Transaccion trans = base;
        trans.idTransaccion = idBase + i;
        trans.idJuegoVendido = carrito[i].idJuego;
        trans.cantidadVendida = carrito[i].cantidad;
        trans.precioUnitarioVenta = carrito[i].precioAdquisicion * MARGEN_GANANCIA;
        trans.gananciaTotal = (trans.precioUnitarioVenta - carrito[i].precioAdquisicion) * trans.cantidadVendida;

        if (registrarTransaccion(archivoTransacciones, trans, trans.idJuegoVendido, trans.cantidadVendida) != 1)
        {
            printf("Error al registrar el juego ID %d.\n", trans.idJuegoVendido);
        }
    }

    free(carrito);
    printf("\nVenta finalizada, %d productos registrados.\n", cantItems);
    return 1;
}
