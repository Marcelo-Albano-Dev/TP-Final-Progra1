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
        printf("6. Registrar Usuario (Empleados)\n");
        printf("7. Ver Listado de Usuarios\n");
        printf("8. Probar Inicio de Sesion (Login)\n");
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
            case 6:
                altaUsuario();
                break;
            case 7:
                listarUsuarios();
                break;
            case 8:
                if (iniciarSesion(&usuarioActivo) == 1) {
                    printf("\nLogin exitoso! Bienvenido/a %s (Rol: %s)\n",
                           usuarioActivo.persona.nombre, usuarioActivo.persona.rol);
                } else {
                    printf("\nUsuario o contrasenia incorrectos.\n");
                }
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

// Crea un usuario administrador si el archivo de usuarios aun no existe:
void inicializarAdminPorDefecto() {
    FILE *arch = fopen(ARCH_USUARIOS, "rb");
    Usuario adminDefault;

    if (arch == NULL) {
        arch = fopen(ARCH_USUARIOS, "wb");
        if (arch != NULL) {
            strcpy(adminDefault.usuario, "admin");
            strcpy(adminDefault.contrasenia, "1234");
            strcpy(adminDefault.persona.dni, "00000000");
            strcpy(adminDefault.persona.nombre, "Administrador General");
            strcpy(adminDefault.persona.telefono, "000-0000");
            strcpy(adminDefault.persona.direccion, "Local Pixel Plus");
            strcpy(adminDefault.persona.rol, "admin");

            fwrite(&adminDefault, sizeof(Usuario), 1, arch);
            fclose(arch);
        }
    } else {
        fclose(arch);
    }
}

// Verifica si un nombre de usuario ya esta ocupado en el sistema
int existeNombreUsuario(char nombreUsuario[]) {
    FILE *arch = fopen(ARCH_USUARIOS, "rb");
    Usuario aux;
    int existe = 0;

    if (arch != NULL) {
        while (fread(&aux, sizeof(Usuario), 1, arch) == 1 && existe == 0) {
            if (strcmp(aux.usuario, nombreUsuario) == 0) {
                existe = 1;
            }
        }
        fclose(arch);
    }

    return existe;
}

// Vincula una persona existente (empleado o admin) a una nueva cuenta de usuario
void altaUsuario() {
    Usuario nuevo;
    Persona emp;
    char dniEmp[15];
    FILE *arch;

    printf("\n--- ALTA DE USUARIO DEL SISTEMA ---\n");
    printf("Ingrese DNI del empleado ya registrado: ");
    fgets(dniEmp, 15, stdin);
    dniEmp[strcspn(dniEmp, "\n")] = '\0';

    if (buscarPersonaPorDni(dniEmp, &emp) == 0) {
        printf("\nError: No existe una persona activa con ese DNI. Registrela primero.\n");
        return;
    }

    if (strcmp(emp.rol, "cliente") == 0) {
        printf("\nError: Un cliente no puede tener cuenta de usuario del sistema.\n");
        return;
    }

    nuevo.persona = emp;

    printf("Nombre de usuario: ");
    fgets(nuevo.usuario, 20, stdin);
    nuevo.usuario[strcspn(nuevo.usuario, "\n")] = '\0';

    if (existeNombreUsuario(nuevo.usuario) == 1) {
        printf("\nError: Ese nombre de usuario ya existe.\n");
        return;
    }

    printf("Contrasenia: ");
    fgets(nuevo.contrasenia, 20, stdin);
    nuevo.contrasenia[strcspn(nuevo.contrasenia, "\n")] = '\0';

    arch = fopen(ARCH_USUARIOS, "ab");
    if (arch != NULL) {
        fwrite(&nuevo, sizeof(Usuario), 1, arch);
        fclose(arch);
        printf("\nUsuario creado con exito para %s!\n", nuevo.persona.nombre);
    } else {
        printf("\nError al guardar en el archivo de usuarios.\n");
    }
}

// Muestra los usuarios registrados junto con los datos de su persona asociada
void listarUsuarios() {
    FILE *arch = fopen(ARCH_USUARIOS, "rb");
    Usuario aux;

    if (arch == NULL) {
        printf("\nNo hay usuarios registrados.\n");
        return;
    }

    printf("\n--- LISTADO DE USUARIOS ---\n");
    while (fread(&aux, sizeof(Usuario), 1, arch) == 1) {
        printf("Usuario: %s | Nombre: %s | DNI: %s | Rol: %s\n",
               aux.usuario, aux.persona.nombre, aux.persona.dni, aux.persona.rol);
    }

    fclose(arch);
}

// Valida credenciales contra el archivo binario y carga el Usuario por puntero
int iniciarSesion(Usuario *usuarioLogueado) {
    FILE *arch = fopen(ARCH_USUARIOS, "rb");
    Usuario aux;
    char userIngresado[20];
    char passIngresada[20];
    int acceso = 0;

    if (arch == NULL) {
        printf("\nError: No se encuentra la base de usuarios.\n");
        return 0;
    }

    printf("\n--- INICIO DE SESION ---\n");
    printf("Usuario: ");
    fgets(userIngresado, 20, stdin);
    userIngresado[strcspn(userIngresado, "\n")] = '\0';

    printf("Contrasenia: ");
    fgets(passIngresada, 20, stdin);
    passIngresada[strcspn(passIngresada, "\n")] = '\0';

    while (fread(&aux, sizeof(Usuario), 1, arch) == 1 && acceso == 0) {
        if (strcmp(aux.usuario, userIngresado) == 0 &&
            strcmp(aux.contrasenia, passIngresada) == 0) {
            *usuarioLogueado = aux;
            acceso = 1;
        }
    }

    fclose(arch);
    return acceso;
}

