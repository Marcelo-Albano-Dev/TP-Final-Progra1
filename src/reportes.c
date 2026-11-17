#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/reportes.h"
#include "../include/estructuras.h"
#define ARCHIVO_VIDEOJUEGOS "videojuegos.bin"
#define ARCHIVO_DEVOLUCIONES "devoluciones.bin"
#define MARGEN_GANACIA 1.30f
#define STOCK_CRITICO 5

///Structs----------------------------------------------------
typedef struct {
    int idAuditoria;
    int idJuego;
    float precioAdquisicionViejo;
    float precioAdquisicionNuevo;
    Fecha fechaModificacion;
} AuditoriaPrecio;

///Prototipado------------------------------------------------
//Reporte de recaudacion mensual
int consultarCambioPrecio(int idJuego,int mes, int anio);
void mostrarReporte();

//Venta de mayor ganancia
void ventaMayorGanancia();

//Reporte de Stock Critico
Videojuego *cargarStockCritico(int *cantidad);
void interCambiarVideojuegos(Videojuego *vj1, Videojuego *vj2);
int particionStock(Videojuego vector[],int inicio,int fin);
void quickSortStock(Videojuego vector[],int inicio,int fin);
void mostrarStockCritico();

//Auditoria
void listarAuditorias();

//Analisis de ganancias
float analisisGanancias(FILE *buffer,Fecha desde,Fecha hasta);
void reporteGananciasPorFecha();

//Funciones auxiliares
int compararFechas(Fecha f1,Fecha f2);
int buscarVideoJuego(int idJuego, Videojuego *resultado);

///"Main" para Reportes---------------------------------------
void menuReportes() {
///Variables--------------------------------------------------
    int opcion;

///Variables de Struct----------------------------------------
    AuditoriaPrecio Audi1;

    printf("\n[Modulo Reportes - En desarrollo]\n\n");
    printf("1.Reporte de recaudacion Mensual\n");
    printf("2.Reporte de venta de mayor ganancia\n");
    printf("3.Reporte de Stock critico\n");
    printf("4.Listar auditorias de precios\n");
    printf("5.Analisis de ganancias por fechas\n");
    printf("0.Salir del Modulo\n");

    do{
        scanf("%i",&opcion);
        switch(opcion){
case 1:
    mostrarReporte(stTransaccion.dniEmpleado);
break;
case 2:
    ventaMayorGanancia();
break;
case 3:
    mostrarStockCritico();
break;
case 4:
    listarAuditorias();
break;
case 5:
    reporteGananciasPorFecha();
break;
    default:
    printf("opcion invalida\n");
        }
    }while(opcion!=0);

}

///Funciones---------------------------------------------------


//Reporte de recaudacion mensual:
int consultarCambioPrecio(int idJuego,int mes, int anio){
    FILE *buffer=fopen(AUDITORIA_H,"rb");
    AuditoriaPrecio aux;
    if(archivo==NULL){
        return 0;
    }
    while(fread(&aux,sizeof(AuditoriaPrecio),1,buffer)==1){
        if(aux.idJuego==idJuego&&aux.fechaModificacion.mes==
           mes&&aux.fechaModificacion.anio==anio){
            fclose(buffer);
            return 1;
           }
    }
    fclose(buffer);
    return 0;
}

void mostrarReporte(){
    FILE *buffer=fopen(ARCH_TRANSACCIONES,"rb");
    Transaccion aux;
    int mes;
    int anio;
    int cambio;
    float recaudacion=0;
    if(buffer==NULL){
        printf("No existe el archivo de transacciones o hubo un error\n");
        return;
    }
    printf("Ingrese anio\n");
    scanf("%i",&anio);
    printf("Ingrese el mes\n");
    scanf("%i",&mes);
    if(mes<1||mes>12||anio<1){
        printf("Fecha incorrecta\n");
    fclose(buffer);
    return;
    }
    printf("Recaudacion mensual\n");
    printf("Perdiodo %i/%i\n",mes,anio);
    printf("Empleado encargado del reporte: %s\n",stEmpleado.dniEmpleado);
    while(fread(&aux,sizeof(Transaccion),1,archivo)==1){
        if(aux.fecha.mes==mes&&aux.fecha.anio==anio){
            recaudacion+=aux.gananciaTotal;
            cambio=consultarCambioPrecio(aux.idJuego,mes,anio);
            printf("Transaccion: %d,aux.idTransaccion");
            printf("Fecha: %d/%d/%d",aux.fecha.dia,
            aux.fecha.mes,aux.fecha.año);
            printf("Juego ID: %d",aux.idJuego);
            printf("Gancia: %.2f",aux.gananciaTotal);
            printf("Cambio de precio: %s\n",cambio);
        }
    }
    printf("Recaudacion Total: %.2f\n",recaudacion);
    fclose(buffer);
}

//Venta de mayor ganancia
//La idea seria mostrar el producto/juego que al vender, produce la mayor ganancia.
void ventaMayorGanancia(){
    FILE *buffer=fopen(ARCH_TRANSACCIONES, "rb");
    Transaccion aux;
    Transaccion mayor;
    Videojuego juego;
    int encontrado=0;
    if(buffer==NULL){
        printf("No existe el archivo de transacciones o hubo un error");
        return;
    }
    while(fread(&aux,sizeof(transaccion),1,archivo)==1){
        if(!encontrado||aux.gananciaTotal>mayor.gananciaTotal){
            mayor=aux;
            encontrado=1;
        }
    }
    fclose(buffer);
    if(!encontrado){
        printf("No hay transacciones registradas\n");
        return;
    }
    printf("Mayor ganancia:\n");
    printf("ID transaccion: %d\n",mayor.idJuegoVendido);
    printf("Ganancia: %.2f\n",mayor.gananciaTotal);
    printf("Fecha: %d/%d/%d\n",mayor.fecha.dia,
    mayor.fecha.mes,mayor.fecha.anio);

    if(buscarVideoJuego(mayor.idJuegoVendido,&juego)){
        printf("ID juego: %d\n",juego.idJuego);
        printf("Nombre del juego: %s\n",juego.nombre);
    }else{
        printf("No se encontro el videojuego en cuestion\n");
    }
}

//Stock critico

Videojuego *cargarStockCritico(int *cantidad){
    FILE *buffer=fopen(ARCH_VIDEOJUEGOS, "rb");
    Videojuego aux;
    Videojuego *vector=NULL;
    *cantidad=0;
    if(buffer==NULL){
        return;
    }
    while(fread(&aux,sizeof(Videojuego),1,buffer)==1){
        if(aux.stock<=STOCK_CRITICO){
            Videojuego *nuevo=
            realloc(vector,(*cantidad+1)*sizeof(Videojuego));
            if(nuevo==NULL){
                free(vector);
                fclose(buffer);
                *cantidad=0;
                return;
            }
            vector=nuevo;
            vector[*cantidad]=aux;
            (*cantidad)++;
        }
    }
    fclose(buffer);
    return vector;
}

//Intercambia dos posiciones para el vector
void interCambiarVideojuegos(Videojuego *vj1, Videojuego *vj2){
    Videojuego aux= *vj1;
    *vj1= *vj2;
    *vj2= aux;
}

//Para ordenar el stock en orden ascendente
int particionStock(Videojuego vector[],int inicio,int fin){
    int pivote=vector[fin].stock;
    int i=inicio-1;
    int j;
    for(j=inicio;j<fin;j++){
        if(vector[j].stock<=pivote){
            i++;
            interCambiarVideojuegos(&vector[i+1],&vector[fin]);
        }
    }
    interCambiarVideojuegos(&vector[i+1],&vector[fin]);
    return i+1;
}

//Para ordenar los juegos por stock ascendente
void quickSortStock(Videojuego vector[],int inicio,int fin){
    if(inicio<fin){
        int pivote=particionStock(vector,inicio,fin);
        quickSortStock(vector,inicio,pivote-1);
        quickSortStock(vector,pivote+1,fin);
    }
}

void mostrarStockCritico(){
    int cantidad=0;
    int i;
    Videojuego *vector=cargarStockCritico(&cantidad);
    if(cantidad==0){
        printf("No hay juegs con stock critico\n");
        free(vector);
        return;
    }
    quickSortStock(vector,0,cantidad-1);
    printf("Stock critico\n");
    printf("ID\nJuego\nStock\n");
    for(i=0;i<cantidad;i++){
        printf("%d\n%s\n%d\n",
        vector[i].idJuego,
        vector[i].nombre,
        vector[i].stock);
    }
    free(vector);
}

//Que producto/juego que sea consumido esta en bajas cantidades,
//que no puede bajar de 5 unidades restantes.
//Tendria que guardar fechas de forma recursiva

//Listar Auditorias
void listarAuditorias(){
    FILE *buffer=fopen(AUDITORIA_H,"rb");
    AuditoriaPrecio aux;
    if(buffer==NULL){
        printf("No existe el archivo auditoria\n");
        return;
    }
    printf("Auditoria de Precios\n");
    while(fread(&aux,sizeof(AuditoriaPrecio),1,archivo)==1){
    printf("ID Auditoria: %d\n",aux.idAuditoria);
    printf("ID Juego: %d\n",aux.idJuego);
    printf("Precio anterior: %.2f\n",aux.precioAdquisicionViejo);
    printf("Precio nuev: %.2f\n",aux.precioAdquisicionNuevo);
    printf("Fecha: %d\n%d\n%d\n",aux.fechaModificacion.dia,
           aux.fechaModificacion.mes,aux.fechaModificacion.anio);
    }
    fclose(buffer);
}

//Analisis-----------------------------------------------------
float analisisGanancias(FILE *buffer,Fecha desde,Fecha hasta){
    Transaccion aux;
    if(fread(&aux,sizeof(Transaccion),1,archivo)!=1){
        return 0;
    }
    if(compararFechas(aux.fecha,desde)>=0&&
       compararFechas(aux.fecha,hasta)<=0){
        return aux.gananciaTotal+analisisGanancias(buffer,
        desde,hasta);
       }
}

//Reporte de Ganancias
void reporteGananciasPorFecha(){
    FILE *buffer=fopen(ARCH_TRANSACCIONES,"rb");
    Fecha desde;
    Fecha hasta;
    float total;
    if(buffer==NULL){
        printf("No existe el archivo de transacciones\n");
        return;
    }
    printf("Fecha inicial:\n%d\n%d\n%d\n",
    &desde.dia,&desde.mes,&desde.anio);
    printf("Fecha final:\n%d\n%d\n%d\n",
    &hasta.dia,&hasta.mes,&hasta.anio);
    if(compararFechas(desde,hasta)>0){
        printf("El rango de fechas es incorrecto\n");
        fclose(buffer);
        return;
    }
    total=analisisGanancias(buffer,desde,hasta);
    printf("Analisis de Ganancias");
    printf("Desde:%d\n%d\n%d\n",desde.dia,desde.mes,desde.anio);
    printf("Hasta:%d\n%d\n%d\n",hasta.dia,hasta.mes,hasta.anio);
    printf("Ganancias acumuladas:%.2f\n",total);
    fclose(buffer);
}

//Funciones Auxiliares
int compararFechas(Fecha f1,Fecha f2){
    if(f1.anio<f2.anio)
        return -1;
    if(f1.anio>f2.anio)
        return 1;
    if(f1.mes<f2.mes)
        return -1;
    if(f1.mes>f2.mes)
        return 1;
    if(f1.dia<f2.dia)
        return -1;
    if(f1.dia>f2.dia)
        return 1;

    return 0;
}

int buscarVideoJuego(int idJuego, Videojuego *resultado){
    FILE*buffer=fopen(ARCHIVO_VIDEOJUEGOS,"rb");
    Videojuego aux;
    if(archivo==NULL){
        return 0;
    }
    while(fread(&aux,sizeof(Videojuego),1,archivo)==1){
        if(aux.idJuego==idJuego){
            *resultado=aux;
            fclose(archivo);
            return 1;
        }
    }
    fclose(archivo);
    return 0;
}


