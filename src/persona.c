#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/persona.h"

void limpiarBufferPersona() {
    while (getchar() != '\n');
}

void menuPersonas() {
    int opcion;
    char dniBuscado[15];

    do {
        system("cls || clear");
        printf("====================================\n");
        printf("        GESTION DE PERSONAS         \n");
        printf("====================================\n");
        printf("1. Registrar nueva Persona\n");
        printf("2. Modificar datos de Persona\n");
        printf("3. Dar de baja a una Persona\n");
        printf("4. Ver Listado Resumen de Personas\n");
        printf("5. Buscar Persona por DNI (Detalle)\n");
        printf("0. Volver al Menu Principal\n");
        printf("------------------------------------\n");
        printf("Ingrese una opcion: ");
        scanf("%d", &opcion);
        limpiarBufferPersona();

        switch (opcion) {
            case 1:
                altaPersona();
                break;
            case 2:
                printf("\nIngrese el DNI de la persona a modificar: ");
                fgets(dniBuscado, sizeof(dniBuscado), stdin);
                dniBuscado[strcspn(dniBuscado, "\n")] = '\0';
                modificarPersonaPorDni(dniBuscado);
                break;
            case 3:
                printf("\nIngrese el DNI de la persona a dar de baja: ");
                fgets(dniBuscado, sizeof(dniBuscado), stdin);
                dniBuscado[strcspn(dniBuscado, "\n")] = '\0';
                bajaLogicaPersona(dniBuscado);
                break;
            case 4:
                listarPersonasResumen();
                break;
            case 5:
                printf("\nIngrese el DNI a buscar: ");
                fgets(dniBuscado, sizeof(dniBuscado), stdin);
                dniBuscado[strcspn(dniBuscado, "\n")] = '\0';
                mostrarPersonaDetallada(dniBuscado);
                break;
            case 0:
                break;
            default:
                printf("\nOpcion incorrecta. Intente nuevamente.\n");
        }

        if (opcion != 0) {
            printf("\nPresione ENTER para continuar...");
            getchar();
        }

    } while (opcion != 0);
}
// Verifica si un DNI ya se encuentra registrado en el archivo de personas
int existePersonaPorDni(char dni[]) {
    FILE *arch = fopen(ARCH_PERSONAS, "rb");
    Persona aux;
    int existe = 0;

    if (arch != NULL) {
        while (fread(&aux, sizeof(Persona), 1, arch) == 1 && existe == 0) {
            if (strcmp(aux.dni, dni) == 0) {
                existe = 1;
            }
        }
        fclose(arch);
    }

    return existe;
}
// Solicita los datos de una nueva persona, valida unicidad de DNI y persiste el registro
void altaPersona() {
    Persona nueva;
    FILE *arch;

    printf("\n--- ALTA DE NUEVA PERSONA ---\n");
    printf("Ingrese DNI: ");
    fgets(nueva.dni, sizeof(nueva.dni), stdin);
    nueva.dni[strcspn(nueva.dni, "\n")] = '\0';

    if (existePersonaPorDni(nueva.dni) == 1) {
        printf("\nError: Ya existe una persona con ese DNI en el sistema.\n");
        return;
    }

    printf("Ingrese Nombre y Apellido: ");
    fgets(nueva.nombre, sizeof(nueva.nombre), stdin);
    nueva.nombre[strcspn(nueva.nombre, "\n")] = '\0';

    printf("Ingrese Telefono: ");
    fgets(nueva.telefono, sizeof(nueva.telefono), stdin);
    nueva.telefono[strcspn(nueva.telefono, "\n")] = '\0';

    printf("Ingrese Direccion: ");
    fgets(nueva.direccion, sizeof(nueva.direccion), stdin);
    nueva.direccion[strcspn(nueva.direccion, "\n")] = '\0';

    printf("Ingrese Rol (cliente / empleado): ");
    fgets(nueva.rol, sizeof(nueva.rol), stdin);
    nueva.rol[strcspn(nueva.rol, "\n")] = '\0';

    arch = fopen(ARCH_PERSONAS, "ab");
    if (arch != NULL) {
        fwrite(&nueva, sizeof(Persona), 1, arch);
        fclose(arch);
        printf("\nPersona guardada con exito!\n");
    } else {
        printf("\nError al abrir el archivo de personas.\n");
    }
}
// Busca una persona activa por DNI y sobrescribe sus datos en el archivo binario
void modificarPersonaPorDni(char dni[]) {
    FILE *arch = fopen(ARCH_PERSONAS, "r+b");
    Persona aux;
    int encontrado = 0;

    if (arch == NULL) {
        printf("\nNo hay personas cargadas todavia.\n");
        return;
    }

    while (fread(&aux, sizeof(Persona), 1, arch) == 1 && encontrado == 0) {
        if (strcmp(aux.dni, dni) == 0 && strcmp(aux.rol, "baja") != 0) {
            encontrado = 1;

            printf("\nPersona encontrada: %s\n", aux.nombre);
            printf("Ingrese nuevo Nombre y Apellido: ");
            fgets(aux.nombre, sizeof(aux.nombre), stdin);
            aux.nombre[strcspn(aux.nombre, "\n")] = '\0';

            printf("Ingrese nuevo Telefono: ");
            fgets(aux.telefono, sizeof(aux.telefono), stdin);
            aux.telefono[strcspn(aux.telefono, "\n")] = '\0';

            printf("Ingrese nueva Direccion: ");
            fgets(aux.direccion, sizeof(aux.direccion), stdin);
            aux.direccion[strcspn(aux.direccion, "\n")] = '\0';

            printf("Ingrese nuevo Rol (cliente / empleado): ");
            fgets(aux.rol, sizeof(aux.rol), stdin);
            aux.rol[strcspn(aux.rol, "\n")] = '\0';

            // Retrocede el puntero del archivo un registro para actualizar la parte necesaria

            fseek(arch, -sizeof(Persona), SEEK_CUR);
            fwrite(&aux, sizeof(Persona), 1, arch);
            printf("\nDatos modificados correctamente!\n");
        }
    }

    if (encontrado == 0) {
        printf("\nNo se encontro ninguna persona activa con ese DNI.\n");
    }

    fclose(arch);
}
// Realiza la baja logica de una persona cambiando su rol a "baja" sin eliminar el registro fisico
void bajaLogicaPersona(char dni[]) {
    FILE *arch = fopen(ARCH_PERSONAS, "r+b");
    Persona aux;
    int encontrado = 0;
    char confirma;

    if (arch == NULL) {
        printf("\nNo hay personas cargadas todavia.\n");
        return;
    }

    while (fread(&aux, sizeof(Persona), 1, arch) == 1 && encontrado == 0) {
        if (strcmp(aux.dni, dni) == 0 && strcmp(aux.rol, "baja") != 0) {
            encontrado = 1;
            printf("\nSeguro que desea dar de baja a %s? (s/n): ", aux.nombre);
            scanf("%c", &confirma);
            limpiarBufferPersona();

            if (confirma == 's' || confirma == 'S') {
                strcpy(aux.rol, "baja");
                fseek(arch, (long)sizeof(Persona) * -1, SEEK_CUR);
                fwrite(&aux, sizeof(Persona), 1, arch);
                printf("\nBaja logica realizada con exito.\n");
            } else {
                printf("\nOperacion cancelada.\n");
            }
        }
    }

    if (encontrado == 0) {
        printf("\nNo se encontro ninguna persona activa con ese DNI.\n");
    }

    fclose(arch);
}
// Muestra un resumen (DNI, nombre y rol) de todas las personas que no fueron dadas de baja
void listarPersonasResumen() {
    FILE *arch = fopen(ARCH_PERSONAS, "rb");
    Persona aux;
    int cantidad = 0;

    if (arch == NULL) {
        printf("\nNo hay personas registradas.\n");
        return;
    }

    printf("\n--- LISTADO DE PERSONAS ACTIVAS ---\n");
    while (fread(&aux, sizeof(Persona), 1, arch) == 1) {
        if (strcmp(aux.rol, "baja") != 0) {
            printf("DNI: %s | Nombre: %s | Rol: %s\n", aux.dni, aux.nombre, aux.rol);
            cantidad++;
        }
    }

    if (cantidad == 0) {
        printf("No hay personas activas para mostrar.\n");
    }

    fclose(arch);
}
// Busca una persona por su DNI y despliega todos los campos de su estructura
void mostrarPersonaDetallada(char dni[]) {
    FILE *arch = fopen(ARCH_PERSONAS, "rb");
    Persona aux;
    int encontrado = 0;

    if (arch == NULL) {
        printf("\nNo hay personas registradas.\n");
        return;
    }

    while (fread(&aux, sizeof(Persona), 1, arch) == 1 && encontrado == 0) {
        if (strcmp(aux.dni, dni) == 0) {
            encontrado = 1;
            printf("\n--- DETALLE DE PERSONA ---\n");
            printf("DNI: %s\n", aux.dni);
            printf("Nombre: %s\n", aux.nombre);
            printf("Telefono: %s\n", aux.telefono);
            printf("Direccion: %s\n", aux.direccion);
            printf("Rol: %s\n", aux.rol);
        }
    }

    if (encontrado == 0) {
        printf("\nNo se encontro una persona con el DNI %s.\n", dni);
    }

    fclose(arch);
}
