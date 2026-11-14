#include <stdio.h>
#include  <stdlib.h>
#include  <string.h>
#include "../include/videojuego.h"

#include <ctype.h>

//DEFINE'S
#define ARCHIVO_VIDEOJUEGOS "videojuegos.bin"
#define ARCHIVO_DEVOLUCIONES "devoluciones.bin"
#define MARGEN_GANACIA 1.30f
// MAIN DE VIDEOJUEGOS
void menuVideojuegos() {

    int opc = 0;
    int idBusqueda = 0;

    do {
        printf("=== MENU DE GESTION Y DEVOLUCINES [AyrtonC] === \n");
        printf("1. Ingresar / agregar nuevo videojuego \n");
        printf("2. Ver lista de videojuegos \n");
        printf("3. Buscar videojuego por plataforma\n");
        printf("4. Ver Detalle completo de un videojuego (PERMISOS DE ADMIN) \n");
        printf("5. Registrar devolucion de venta (Devolucion) \n");
        printf("6. Ver historial de devoluciones \n");
        printf("0. VOLVER AL MENU PRINCIPAL \n");
        printf("[#] SELECCIONES UNA OPCION [#]:  \n");
        scanf("%i",&opc);
        switch (opc) {
            case 1:
                cargarVideojuego();
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
                case 0:
                printf("volviendo al menu principal \n");
                break;
                default:
                printf("Opcion no valida, Seleccione una opcion valida \n");
        }
    }while(opc!=0);
}
// P1
int obtenerUltimoIdDevolicion() {
    FILE *file = fopen(ARCHIVO_VIDEOJUEGOS, "rb");
    if (file == NULL) {
        return 0;
    }
    Videojuego aux;
    int ultimoId = 0;

    fseek(file, 0, SEEK_END);
    if (ftell(file) > 0) {
        fseek(file, 0, - ((long)sizeof(Videojuego), SEEK_END));
            if (fread(&aux, sizeof(Videojuego), 1, file) == 1) {
                ultimoId = aux.idJuego;
            }
    }
    fclose(file);
    return ultimoId;
}

void cargarVideojuego () {
    FILE *file = fopen(ARCHIVO_VIDEOJUEGOS, "ab");

    if (file == NULL) {
        return;
    }

    Videojuego nuevoJuego;
    nuevoJuego.idJuego = obtenerUltimoIdDevolicion() + 1;

    printf("\n ------------------------------------------ \n");
    printf("\n === Registrar un nuevo juego [ID asignado: %d] === \n");
    printf("\n ------------------------------------------ \n");

    printf("Ingrese el titulo del videojuego: \n");
    fflush(stdin);
    fgets(nuevoJuego.titulo, sizeof(nuevoJuego.titulo), stdin);
    nuevoJuego.titulo[strcspn(nuevoJuego.titulo, "\n")] = '\0';

    printf("Ingrese plataforma [PC]- [XBOX]-[PLAYSTATION]-[NINTENDO]: ");
    fflush(stdin);
    fgets(nuevoJuego.plataforma, sizeof(nuevoJuego.plataforma),stdin);
    nuevoJuego.plataforma[strcspn(nuevoJuego.plataforma, "\n")] = '\0';

    printf("Genero del videojuego (ej. accion, puzzle, misterio): ");
    fflush(stdin);
    fgets(nuevoJuego.genero, sizeof(nuevoJuego.genero), stdin);

    printf("Stock disponible: ");
    scanf("%d",&nuevoJuego.stock);

    printf("Precio US$: ");
    scanf("%.2f",&nuevoJuego.precioAdquisicion);

    fwrite(&nuevoJuego, sizeof(Videojuego), 1, file);
    fclose(file);

    printf("[!] Se a guardado con exito ");
}
//P1

void listarVideojuegosResumen() {

}
//P2