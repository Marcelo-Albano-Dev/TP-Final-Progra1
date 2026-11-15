#include <stdio.h>
#include "../include/transaccion.h"
#include "videojuego.h"

void menuTransacciones() {
    printf("\n[Mlo]\n");
}
 

//* VER TODO EL ARCHIVO DE TRANSACCIONES *//
void listadoTransacciones(char archivoTrans[])
{
    FILE *buffer = fopen(archivoTrans, "rb");
    if(buffer == NULL)
    {
        printf("Error al abrir el archivo\n");
    }
    else
    {
        Transaccion aux;
        while(fread(&aux, sizeof(Transaccion), 1, buffer))
        {
            printf("================\n");
            printf("Id videojuego: %d, Fecha de venta: %d/%d/%d", aux.idJuegoVendido, aux.fecha.dia,aux.fecha.mes,aux.fecha.anio);
            printf("================\n");
        }
        fclose(buffer);
    }
}

//* VER UNA TRANSACCION ESPECIFICA *//
void verTransaccionDetalle(char archivoTrans[], int idBuscado)
{
    FILE *buffer = fopen(archivoTrans, "rb");
    if(buffer == NULL)
    {
        printf("Error al abrir el archivo de transacciones\n");
    }
    else
    {
       Transaccion aux;
        while(fread(&aux, sizeof(Transaccion), 1, buffer)==1)
        {
            if(aux.idTransaccion == idBuscado)
            {
                mostrarTrans(aux);
                fclose(buffer);
                return;
            }
        }
        fclose(buffer);
        printf("La transaccion no fue encontrada\n");
    }
}

void mostrarTrans(Transaccion trans)
{
    printf("Id: %d\n", trans.idTransaccion);
    
    printf("Fecha: %d/%d/%d\n", trans.fecha.dia, trans.fecha.mes, trans.fecha.anio);
    
    printf("ID del juego vendido: %d\n",trans.idJuegoVendido);
   
    printf("Cantidad vendida: %d\n", trans.cantidadVendida);
    
    printf("Precio unitario: %f\n", trans.precioUnitarioVenta);
    
    printf("Ganancia total: %f\n", trans.gananciaTotal);

    printf("%s\n", trans.dniCliente);

    printf("%s\n", trans.dniEmpleado);

}
