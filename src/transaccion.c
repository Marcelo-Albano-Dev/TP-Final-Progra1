#include <stdio.h>
#include "../include/transaccion.h"
#include "videojuego.h"



void menuTransacciones() {
    
    int opcion;
    do {
        printf("=== MENU DE TRANSACCIONES Y REPORTES === \n");
        printf("1. Listado de transacciones.\n");
        printf("2. Ver transaccion especifica.\n");
        printf("3. Registrar venta.\n");
        printf("4.  \n");
        printf("5.  \n");
        printf("6.  \n");
        printf("0. Salir del programa.\n");
        
        scanf("%d",&opcion);
        switch (opcion) {
            case 0:
                
                break;
            case 1:
                
                break;
            case 2:

                break;
            case 3:
                break;
            case 4:
                break;
            case 5:
                break;
            case 6:
                break;
                default:
                printf("Opcion no valida, Seleccione una opcion valida \n");
                break;
        }
    }while(opcion!=0);
}
 

//* ===========================MUESTRAS DE TRANSACCIONES======================== *//
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



//* ===========================REGISTRO DE VENTA======================== *//

int cargaVenta(char archivoTransacciones[])
{
    //*================= VERIFICO SI EXISTE EL JUEGO =================*//
    Videojuego comprado;
    Transaccion trans;
    int idBuscado;
    printf("Ingrese el id del juego que quiere comprar\n");
    scanf("%d", &idBuscado);
    int verificacion = buscarVideojuegoPorId(idBuscado, &comprado); //SE GUARDA EL JUEGO A COMPRAR
    if(verificacion == 0 || comprado.stock<=0)
    {
        return 0; // EL JUEGO QUE SE SOLICITA NO SE ENCUENTRA EN LOS REGISTROS
    }
    
    
    //*================= CANTIDAD A COMPRAR =================*//
    printf("Cuantos desea comprar?\n");
    scanf("%d", &trans.cantidadVendida);
    if(trans.cantidadVendida <= 0 || trans.cantidadVendida > comprado.stock)
    {
        return -2; // CANTIDAD DESEADA INVALIDA O SIN STOCK SUFICIENTE
    }
    

    //*================= DATOS CLIENTE Y EMPLEADO =================*//
    printf("Ingrese el dni del cliente\n");
    scanf(" %s",trans.dniCliente);
    printf("Ingrese el dni del empleado que realiza la venta\n");
    scanf(" %s", trans.dniEmpleado);
    

    trans.idTransaccion = (int) generarIdTransaccion(archivoTransacciones);

    //*================= CARGA FECHA =================*//
    printf("Ingrese la fecha de la transaccion\n");
    printf("Dia: ");
    scanf("%d", &trans.fecha.dia);
    printf("Mes: ");
    scanf("%d", &trans.fecha.mes);
    printf("Anio: ");
    scanf("%d", &trans.fecha.anio);
    
    
    //*================= CALCULO GANANCIA TOTAL =================*//
    trans.idJuegoVendido = comprado.idJuego;
    trans.precioUnitarioVenta = comprado.precioAdquisicion * 1.30f;
    trans.gananciaTotal = (trans.precioUnitarioVenta - comprado.precioAdquisicion) * trans.cantidadVendida;
    

    //*================= TRABAJO CON ARCHIVOS =================*//
    return registrarTransaccion(archivoTransacciones, trans, comprado.idJuego, trans.cantidadVendida);
}


long generarIdTransaccion(char archivoTransaccion[])
{
    FILE * buffer = fopen(archivoTransaccion, "rb");
    if(buffer == NULL)
    {
        return 0;
    }
    fseek(buffer, 0, SEEK_END);
    long bytes = ftell(buffer);
    if(bytes<0)
    {
        fclose(buffer);
        return 0;
    }
    fclose(buffer);
    return bytes / (long)sizeof(Transaccion);
}

//* REGISTRAR Y ACTUALIZAR LA TRANSACCION *//
int registrarTransaccion(char archivoTransacciones[], Transaccion trans, int idJuego, int cantidadVendida)
{
    FILE *buffer = fopen(archivoTransacciones, "ab");
    if(buffer == NULL)
    {
        return -1;
    }
    if(fwrite(&trans, sizeof(Transaccion),1,buffer) != 1)
    {
        fclose(buffer);
        return -1;
    }
    fclose(buffer);

    if(actualizarStockVideojuego(idJuego, -cantidadVendida) == 0)
    {
        return -3; //error al actualizar stock
    } 
    return 1; // REGISTRO DE COMPRA EXITOSO
}