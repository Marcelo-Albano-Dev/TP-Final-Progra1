#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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
    Usuario sesionActual;
    int logueado = 0;

    inicializarAdminPorDefecto();

    do {
        while (logueado == 0) {
            system("cls || clear");
            printf("====================================\n");
            printf("      PIXEL PLUS - INICIO SESION    \n");
            printf("====================================\n");
            printf("1. Iniciar Sesion\n");
            printf("0. Salir\n");
            printf("------------------------------------\n");
            printf("Seleccione una opcion: ");
            opcion = -1;
            leido = scanf("%d", &opcion);
            limpiarBufferMain();
            if (leido != 1) {
                opcion = -1;
            }
            if (leido == EOF || opcion == 0) {
                printf("\nSaliendo del sistema...\n");
                return 0;
            }

            if (opcion == 1) {
                if (iniciarSesion(&sesionActual) == 1) {
                    logueado = 1;
                    printf("\nBienvenido/a %s [%s]\n", sesionActual.persona.nombre, sesionActual.persona.rol);
                } else {
                    printf("\n[!] Usuario o contrasenia incorrectos, o cuenta dada de baja.\n");
                }
                printf("Presione ENTER para continuar...");
                getchar();
            } else {
                printf("\nOpcion invalida.\n");
                printf("Presione ENTER para continuar...");
                getchar();
            }
        }

        system("cls || clear");
        printf("====================================\n");
        printf("      PIXEL PLUS - MENU PRINCIPAL   \n");
        printf("  Usuario: %s (%s)\n", sesionActual.persona.nombre, sesionActual.persona.rol);
        printf("====================================\n");
        printf("1. Gestion de Personas y Usuarios\n");
        printf("2. Gestion de Videojuegos\n");
        printf("3. Gestion de Transacciones\n");
        printf("4. Reportes (Solo Admin)\n");
        printf("5. Cerrar Sesion\n");
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
                if (strcmp(sesionActual.persona.rol, "admin") == 0) {
                    menuReportes();
                } else {
                    printf("\n[!] Acceso denegado: solo el administrador puede ver los reportes.\n");
                }
                break;
            case 5:
                logueado = 0;
                printf("\nSesion cerrada correctamente.\n");
                break;
            case 0:
                printf("\nSaliendo del sistema...\n");
                break;
            default:
                printf("\nOpcion invalida.\n");
        }

        if (opcion != 0 && opcion != 5) {
            printf("\nPresione ENTER para continuar...");
            getchar();
        }
    } while (opcion != 0);

    return 0;
}
