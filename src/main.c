#include <stdio.h>
#include <stdlib.h>

#include "../include/estructuras.h"
#include "../include/persona.h"
#include "../include/videojuego.h"
#include "../include/transaccion.h"
#include "../include/reportes.h"

static void limpiarBufferMain(void) {
    int c;
    while ((c = getchar()) != '\n' && c != EOF) {
    }
}

int main() {
    int opcion;
    int leido;

    do {
        system("cls || clear");
        printf("====================================\n");
        printf("      PIXEL PLUS - MENU PRINCIPAL   \n");
        printf("====================================\n");
        printf("1. Gestion de Personas y Usuarios\n");
        printf("2. Gestion de Videojuegos\n");
        printf("3. Gestion de Transacciones\n");
        printf("4. Reportes\n");
        printf("0. Salir\n");
        printf("------------------------------------\n");
        printf("Seleccione una opcion: ");
        opcion = -1;
        leido = scanf("%d", &opcion);
        limpiarBufferMain();
        if (leido != 1) {
            opcion = -1;
        }
        if (leido == EOF) {
            break;
        }

        switch (opcion) {
            case 1:
                menuPersonas();
                break;
            case 2:
                menuVideojuegos();
                break;
            case 3:
                menuTransacciones();
                break;
            case 4:
                menuReportes();
                break;
            case 0:
                printf("\nSaliendo del sistema...\n");
                break;
            default:
                printf("\nOpcion invalida.\n");
        }

        if (opcion != 0) {
            printf("\nPresione ENTER para continuar...");
            getchar();
        }
    } while (opcion != 0);

    return 0;
}
