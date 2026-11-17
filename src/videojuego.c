#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "../include/videojuego.h"

void limpiarBufferVideojuego() {
    while (getchar() != '\n');
}

void pasarAMayusculas(char destino[], char origen[]) {
    int i = 0;
    while (origen[i] != '\0') {
        destino[i] = toupper((unsigned char)origen[i]);
        i++;
    }
    destino[i] = '\0';
}

int validarFechaDevolucion(Fecha f) {
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

// MENU DE VIDEOJUEGOS
void menuVideojuegos() {
    int opc = 0;
    int idBusqueda = 0;

    do {
        system("cls || clear");
        printf("=== MENU DE GESTION Y DEVOLUCIONES ===\n");
        printf("1. Ingresar / agregar nuevo videojuego\n");
        printf("2. Modificar datos de un videojuego\n");
        printf("3. Dar de baja logica a un videojuego\n");
        printf("4. Ver lista de videojuegos\n");
        printf("5. Buscar videojuego por plataforma\n");
        printf("6. Ver detalle completo de un videojuego (Admin)\n");
        printf("7. Registrar devolucion de venta\n");
        printf("8. Ver historial de devoluciones\n");
        printf("0. VOLVER AL MENU PRINCIPAL\n");
        printf("[#] SELECCIONE UNA OPCION [#]: ");
        scanf("%d", &opc);
        limpiarBufferVideojuego();

        switch (opc) {
            case 1:
                cargarVideojuego();
                break;
            case 2:
                printf("\nIngrese el ID del videojuego a modificar: ");
                scanf("%d", &idBusqueda);
                limpiarBufferVideojuego();
                modificarVideojuego(idBusqueda);
                break;
            case 3:
                printf("\nIngrese el ID del videojuego a dar de baja: ");
                scanf("%d", &idBusqueda);
                limpiarBufferVideojuego();
                bajaLogicaVideojuego(idBusqueda);
                break;
            case 4:
                listarVideojuegosResumen();
                break;
            case 5:
                listarVideojuegosPorPlataforma();
                break;
            case 6:
                printf("\nIngrese el ID del videojuego: ");
                scanf("%d", &idBusqueda);
                limpiarBufferVideojuego();
                verDetalleVideojuego(idBusqueda);
                break;
            case 7:
                registrarDevolucion();
                break;
            case 8:
                listarDevoluciones();
                break;
            case 0:
                printf("\nVolviendo al menu principal...\n");
                break;
            default:
                printf("\n[!] Opcion no valida, seleccione una opcion valida.\n");
        }

        if (opc != 0) {
            printf("\nPresione ENTER para continuar...");
            getchar();
        }
    } while (opc != 0);
}

// P1: Obtiene el ultimo ID de videojuego guardado para autoincrementar
int obtenerUltimoIdVideojuego() {
    FILE *file = fopen(ARCH_VIDEOJUEGOS, "rb");
    if (file == NULL) {
        return 0;
    }

    Videojuego aux;
    int ultimoId = 0;

    fseek(file, 0, SEEK_END);
    if (ftell(file) > 0) {
        fseek(file, -(long)sizeof(Videojuego), SEEK_END);
        if (fread(&aux, sizeof(Videojuego), 1, file) == 1) {
            ultimoId = aux.idJuego;
        }
    }
    fclose(file);
    return ultimoId;
}

// Alta de un nuevo videojuego en el archivo binario
void cargarVideojuego() {
    FILE *file = fopen(ARCH_VIDEOJUEGOS, "ab");
    if (file == NULL) {
        printf("\n[!] ERROR | No se pudo abrir el archivo de videojuegos.\n");
        return;
    }

    Videojuego nuevoJuego;
    nuevoJuego.idJuego = obtenerUltimoIdVideojuego() + 1;

    printf("\n------------------------------------------\n");
    printf("=== Registrar un nuevo juego [ID asignado: %d] ===\n", nuevoJuego.idJuego);
    printf("------------------------------------------\n");

    printf("Ingrese el titulo del videojuego: ");
    fgets(nuevoJuego.titulo, sizeof(nuevoJuego.titulo), stdin);
    nuevoJuego.titulo[strcspn(nuevoJuego.titulo, "\n")] = '\0';

    printf("Ingrese plataforma [PC] - [XBOX] - [PLAYSTATION] - [NINTENDO]: ");
    fgets(nuevoJuego.plataforma, sizeof(nuevoJuego.plataforma), stdin);
    nuevoJuego.plataforma[strcspn(nuevoJuego.plataforma, "\n")] = '\0';

    printf("Genero del videojuego (ej. accion, puzzle, misterio, etc.): ");
    fgets(nuevoJuego.genero, sizeof(nuevoJuego.genero), stdin);
    nuevoJuego.genero[strcspn(nuevoJuego.genero, "\n")] = '\0';

    printf("Stock disponible: ");
    scanf("%d", &nuevoJuego.stock);
    limpiarBufferVideojuego();

    printf("Precio US$: ");
    scanf("%f", &nuevoJuego.precioAdquisicion);
    limpiarBufferVideojuego();

    fwrite(&nuevoJuego, sizeof(Videojuego), 1, file);
    fclose(file);

    printf("\n[!] Se ha guardado con exito.\n");
}

// Modifica los datos de un videojuego activo buscandolo por ID
void modificarVideojuego(int idJuego) {
    FILE *file = fopen(ARCH_VIDEOJUEGOS, "r+b");
    if (file == NULL) {
        printf("\n[!] ERROR | No hay videojuegos registrados todavia.\n");
        return;
    }

    Videojuego aux;
    int encontrado = 0;

    long pos = ftell(file);
    while (fread(&aux, sizeof(Videojuego), 1, file) == 1 && encontrado == 0) {
        if (aux.idJuego == idJuego && aux.stock >= 0) {
            encontrado = 1;
            printf("\nEditando juego: %s\n", aux.titulo);

            printf("Nuevo titulo: ");
            fgets(aux.titulo, sizeof(aux.titulo), stdin);
            aux.titulo[strcspn(aux.titulo, "\n")] = '\0';

            printf("Nueva plataforma: ");
            fgets(aux.plataforma, sizeof(aux.plataforma), stdin);
            aux.plataforma[strcspn(aux.plataforma, "\n")] = '\0';

            printf("Nuevo genero: ");
            fgets(aux.genero, sizeof(aux.genero), stdin);
            aux.genero[strcspn(aux.genero, "\n")] = '\0';

            printf("Nuevo stock: ");
            scanf("%d", &aux.stock);
            limpiarBufferVideojuego();

            printf("Nuevo precio US$: ");
            scanf("%f", &aux.precioAdquisicion);
            limpiarBufferVideojuego();

            fseek(file, pos, SEEK_SET);
            fwrite(&aux, sizeof(Videojuego), 1, file);
            fflush(file);
            printf("\n[!] Videojuego modificado con exito.\n");
        }
        pos = ftell(file);
    }

    if (encontrado == 0) {
        printf("\n[!] No se encontro ningun videojuego activo con el ID %d.\n", idJuego);
    }
    fclose(file);
}

// Realiza la baja logica poniendo el stock en -1 sin borrar el registro fisico
void bajaLogicaVideojuego(int idJuego) {
    FILE *file = fopen(ARCH_VIDEOJUEGOS, "r+b");
    if (file == NULL) {
        printf("\n[!] ERROR | No hay videojuegos registrados todavia.\n");
        return;
    }

    Videojuego aux;
    int encontrado = 0;
    char confirma;

    long pos = ftell(file);
    while (fread(&aux, sizeof(Videojuego), 1, file) == 1 && encontrado == 0) {
        if (aux.idJuego == idJuego && aux.stock >= 0) {
            encontrado = 1;
            printf("\nSeguro que desea dar de baja a %s? (s/n): ", aux.titulo);
            scanf("%c", &confirma);
            limpiarBufferVideojuego();

            if (confirma == 's' || confirma == 'S') {
                aux.stock = -1;
                fseek(file, pos, SEEK_SET);
                fwrite(&aux, sizeof(Videojuego), 1, file);
                fflush(file);
                printf("\n[!] Baja logica realizada con exito.\n");
            } else {
                printf("\nOperacion cancelada.\n");
            }
        }
        pos = ftell(file);
    }

    if (encontrado == 0) {
        printf("\n[!] No se encontro ningun videojuego activo con el ID %d.\n", idJuego);
    }
    fclose(file);
}

// D1: Lista todos los videojuegos activos en formato tabla
void listarVideojuegosResumen() {
    FILE *file = fopen(ARCH_VIDEOJUEGOS, "rb");
    if (file == NULL) {
        printf("\n[!] No hay videojuegos registrados todavia.\n");
        return;
    }

    Videojuego aux;
    int cantidad = 0;

    printf("\n%-5s | %-30s | %-15s | %-6s\n", "ID", "TITULO", "PLATAFORMA", "STOCK");
    printf("==================================================================\n");

    while (fread(&aux, sizeof(Videojuego), 1, file) == 1) {
        if (aux.stock >= 0) {
            printf("%-5d | %-30s | %-15s | %-6d\n", aux.idJuego, aux.titulo, aux.plataforma, aux.stock);
            cantidad++;
        }
    }

    if (cantidad == 0) {
        printf("No hay videojuegos activos para mostrar.\n");
    }
    fclose(file);
}

// D2: Filtra juegos con stock por plataforma usando arreglo dinamico (realloc)
void listarVideojuegosPorPlataforma() {
    FILE *file = fopen(ARCH_VIDEOJUEGOS, "rb");
    if (file == NULL) {
        printf("\n[!] ERROR | NO HAY VIDEOJUEGOS REGISTRADOS TODAVIA\n");
        return;
    }

    char plataformaBuscada[30];
    char platBuscadaMayus[30];
    char platJuegoMayus[20];

    printf("\nIngrese la plataforma deseada: ");
    fgets(plataformaBuscada, sizeof(plataformaBuscada), stdin);
    plataformaBuscada[strcspn(plataformaBuscada, "\n")] = '\0';
    pasarAMayusculas(platBuscadaMayus, plataformaBuscada);

    Videojuego aux;
    Videojuego *Juegosfiltrados = NULL;
    int cantidad = 0;

    while (fread(&aux, sizeof(Videojuego), 1, file) == 1) {
        pasarAMayusculas(platJuegoMayus, aux.plataforma);
        if (strcmp(platJuegoMayus, platBuscadaMayus) == 0 && aux.stock > 0) {
            Videojuego *temp = realloc(Juegosfiltrados, (cantidad + 1) * sizeof(Videojuego));
            if (temp == NULL) {
                printf("\n[!] ERROR | Sin memoria\n");
                free(Juegosfiltrados);
                fclose(file);
                return;
            }
            Juegosfiltrados = temp;
            Juegosfiltrados[cantidad] = aux;
            cantidad++;
        }
    }
    fclose(file);

    if (cantidad == 0) {
        printf("\nNo se encontraron juegos con stock para la plataforma [%s]\n", plataformaBuscada);
        return;
    }

    printf("\n=== JUEGOS DISPONIBLES PARA %s ===\n", plataformaBuscada);
    for (int i = 0; i < cantidad; i++) {
        float precioDeVenta = Juegosfiltrados[i].precioAdquisicion * MARGEN_GANANCIA;
        printf("[%d] %s - Precio de venta US$: %.2f - Stock: %d\n",
               Juegosfiltrados[i].idJuego, Juegosfiltrados[i].titulo, precioDeVenta, Juegosfiltrados[i].stock);
    }
    free(Juegosfiltrados);
}

// Busca un videojuego activo por ID y copia sus datos si recibe puntero
int buscarVideojuegoPorId(int idJuego, Videojuego *JuegoEncontrado) {
    FILE *file = fopen(ARCH_VIDEOJUEGOS, "rb");
    if (file == NULL) {
        return 0;
    }

    Videojuego aux;
    while (fread(&aux, sizeof(Videojuego), 1, file) == 1) {
        if (aux.idJuego == idJuego && aux.stock >= 0) {
            if (JuegoEncontrado != NULL) {
                *JuegoEncontrado = aux;
            }
            fclose(file);
            return 1;
        }
    }
    fclose(file);
    return 0;
}

// Muestra todos los campos de un videojuego incluyendo costo y precio sugerido
void verDetalleVideojuego(int idJuego) {
    Videojuego encontrado;
    if (buscarVideojuegoPorId(idJuego, &encontrado)) {
        printf("\n=== DETALLE ADMINISTRATIVO DEL VIDEOJUEGO ===\n");
        printf("ID Juego:              %d\n", encontrado.idJuego);
        printf("Titulo:                %s\n", encontrado.titulo);
        printf("Plataforma:            %s\n", encontrado.plataforma);
        printf("Genero:                %s\n", encontrado.genero);
        printf("Stock Actual:          %d unidades\n", encontrado.stock);
        printf("Precio Adquisicion:    $%.2f\n", encontrado.precioAdquisicion);
        printf("Precio Venta Sugerido: $%.2f\n", encontrado.precioAdquisicion * MARGEN_GANANCIA);
    } else {
        printf("\nNo se encontro ningun videojuego con el ID %d.\n", idJuego);
    }
}

// Actualiza el stock de un videojuego activo verificando que no quede negativo
int actualizarStockVideojuego(int idJuego, int cambioStock) {
    FILE *file = fopen(ARCH_VIDEOJUEGOS, "r+b");
    if (file == NULL) {
        return 0;
    }

    Videojuego aux;
    long pos = ftell(file);
    while (fread(&aux, sizeof(Videojuego), 1, file) == 1) {
        if (aux.idJuego == idJuego && aux.stock >= 0) {
            if (aux.stock + cambioStock < 0) {
                fclose(file);
                return 0;
            }
            aux.stock += cambioStock;
            fseek(file, pos, SEEK_SET);
            fwrite(&aux, sizeof(Videojuego), 1, file);
            fflush(file);
            fclose(file);
            return 1;
        }
        pos = ftell(file);
    }
    fclose(file);
    return 0;
}

// Obtiene el ultimo ID de devolucion registrado
int obtenerUltimoIdDevolucion() {
    FILE *file = fopen(ARCHIVO_DEVOLUCIONES, "rb");
    if (file == NULL) {
        return 0;
    }

    Devolucion aux;
    int ultimoId = 0;

    fseek(file, 0, SEEK_END);
    if (ftell(file) > 0) {
        fseek(file, -(long)sizeof(Devolucion), SEEK_END);
        if (fread(&aux, sizeof(Devolucion), 1, file) == 1) {
            ultimoId = aux.idDevolucion;
        }
    }
    fclose(file);
    return ultimoId;
}

// Verifica si una transaccion ya fue devuelta anteriormente
int transaccionYaDevuelta(int idTransaccion) {
    FILE *file = fopen(ARCHIVO_DEVOLUCIONES, "rb");
    if (file == NULL) {
        return 0;
    }

    Devolucion aux;
    while (fread(&aux, sizeof(Devolucion), 1, file) == 1) {
        if (aux.idTransaccionAnulada == idTransaccion) {
            fclose(file);
            return 1;
        }
    }
    fclose(file);
    return 0;
}

// Registra la devolucion de una venta y reintegra las unidades al stock
void registrarDevolucion() {
    int idTransaccion = 0;
    printf("\n=== REGISTRO DE DEVOLUCIONES ===\n");
    printf("Ingrese el ID de la transaccion a anular: ");
    scanf("%d", &idTransaccion);
    limpiarBufferVideojuego();

    if (transaccionYaDevuelta(idTransaccion)) {
        printf("\n[!] ERROR | LA TRANSACCION #%d YA FUE DEVUELTA ANTERIORMENTE\n", idTransaccion);
        return;
    }

    int idJuego = 0;
    int cantidad = 0;
    printf("Ingrese ID del juego asociado: ");
    scanf("%d", &idJuego);
    limpiarBufferVideojuego();

    printf("Ingrese cantidad de unidades a reintegrar: ");
    scanf("%d", &cantidad);
    limpiarBufferVideojuego();

    Devolucion nueva;
    nueva.idDevolucion = obtenerUltimoIdDevolucion() + 1;
    nueva.idTransaccionAnulada = idTransaccion;
    nueva.idJuego = idJuego;
    nueva.cantidadDevuelta = cantidad;

    do {
        printf("FECHA DE DEVOLUCION [DIA MES ANIO]: ");
        scanf("%d %d %d", &nueva.fechaDevolucion.dia, &nueva.fechaDevolucion.mes, &nueva.fechaDevolucion.anio);
        limpiarBufferVideojuego();
        if (!validarFechaDevolucion(nueva.fechaDevolucion)) {
            printf("[!] Fecha invalida. Verifique dia, mes y anio.\n");
        }
    } while (!validarFechaDevolucion(nueva.fechaDevolucion));

    printf("Motivo: ");
    fgets(nueva.motivo, sizeof(nueva.motivo), stdin);
    nueva.motivo[strcspn(nueva.motivo, "\n")] = '\0';

    if (actualizarStockVideojuego(nueva.idJuego, nueva.cantidadDevuelta)) {
        FILE *file = fopen(ARCHIVO_DEVOLUCIONES, "ab");
        if (file != NULL) {
            fwrite(&nueva, sizeof(Devolucion), 1, file);
            fclose(file);
            printf("\nDevolucion #%d registrada. Se han devuelto al stock (%d unidades).\n",
                   nueva.idDevolucion, nueva.cantidadDevuelta);
        }
    } else {
        printf("\n[!] ERROR | NO SE ENCONTRO EL VIDEOJUEGO ID %d PARA ACTUALIZAR EL STOCK\n", nueva.idJuego);
    }
}

// Muestra el historial completo de devoluciones registradas
void listarDevoluciones() {
    FILE *file = fopen(ARCHIVO_DEVOLUCIONES, "rb");
    if (file == NULL) {
        printf("\n[!] No hay devoluciones registradas.\n");
        return;
    }

    Devolucion aux;
    printf("\n=== HISTORIAL DE DEVOLUCIONES DE VIDEOJUEGOS ===\n");
    while (fread(&aux, sizeof(Devolucion), 1, file) == 1) {
        printf("DEVOLUCION #%d | Venta anulada #%d | ID de juego #%d | Cantidad %d | Fecha %d/%d/%d | Motivo: %s\n",
               aux.idDevolucion, aux.idTransaccionAnulada, aux.idJuego, aux.cantidadDevuelta,
               aux.fechaDevolucion.dia, aux.fechaDevolucion.mes, aux.fechaDevolucion.anio,
               aux.motivo);
    }
    fclose(file);
}
