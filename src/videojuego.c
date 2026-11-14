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
                listarVideojuegosResumen();
                break;
                case 3:
                listarVideojuegosPorPlataforma();
                break;
                case 4:
                printf("Ingrese el ID del videojuego: ");
                scanf("%d", &idBusqueda);
                verDetalleVideojuego(idBusqueda);
                break;
                case 5:
                registrarDevolucion();
                break;
                case 6:
                listarDevoluciones();
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
        fseek(file, -(long)sizeof(Videojuego), SEEK_END);
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
    printf("\n === Registrar un nuevo juego [ID asignado: %d] === \n",nuevoJuego.idJuego);
    printf("\n ------------------------------------------ \n");

    printf("Ingrese el titulo del videojuego: \n");
    fflush(stdin);
    fgets(nuevoJuego.titulo, sizeof(nuevoJuego.titulo), stdin);
    nuevoJuego.titulo[strcspn(nuevoJuego.titulo, "\n")] = '\0';

    printf("Ingrese plataforma [PC]- [XBOX]-[PLAYSTATION]-[NINTENDO]: ");
    fflush(stdin);
    fgets(nuevoJuego.plataforma, sizeof(nuevoJuego.plataforma),stdin);
    nuevoJuego.plataforma[strcspn(nuevoJuego.plataforma, "\n")] = '\0';

    printf("Genero del videojuego (ej. accion, puzzle, misterio,etc.): ");
    fflush(stdin);
    fgets(nuevoJuego.genero, sizeof(nuevoJuego.genero), stdin);
    nuevoJuego.genero[strcspn(nuevoJuego.genero, "\n")] = '\0';

    printf("Stock disponible: ");
    scanf("%d",&nuevoJuego.stock);

    printf("Precio US$: ");
    scanf("%f",&nuevoJuego.precioAdquisicion);

    fwrite(&nuevoJuego, sizeof(Videojuego), 1, file);
    fclose(file);

    printf("[!] Se a guardado con exito ");
}
//D1

void listarVideojuegosResumen() {
FILE *file = fopen(ARCHIVO_VIDEOJUEGOS, "rb");
    if (file == NULL) {
        return;
    }

    Videojuego aux;

printf("\n%-5s | %-30s | %-12s | %-6s\n", "ID", "TITULO", "PLATAFORMA", "STOCK");
    printf (" ================================================================");

    while (fread(&aux, sizeof(Videojuego), 1, file) == 1) {
        printf("%-5d | %-30s | %-12s | %-6d \n", aux.idJuego, aux.titulo, aux.plataforma, aux.stock);

    }
fclose(file);
}
//D2

void listarVideojuegosPorPlataforma() {

FILE *file = fopen(ARCHIVO_VIDEOJUEGOS, "rb");
    if (file == NULL) {
        printf("\n [!] ERROR | NO HAY VIDEOJUEGOS REGISTRADOS TODAVIA\n");
        return;
    }

    char plataformaBuscada[30];
    printf("\n Ingrese la plataorma deseada: ");
    fflush(stdin);
    fgets(plataformaBuscada, sizeof(plataformaBuscada), stdin);
    plataformaBuscada[strcspn(plataformaBuscada, "\n")] = '\0';

    Videojuego aux;
    Videojuego *Juegosfiltrados = NULL;
    int cantidad = 0;

    while (fread(&aux, sizeof(Videojuego), 1, file) == 1) {
        if (strcmp(aux.plataforma, plataformaBuscada) == 0 && aux.stock > 0) {
Videojuego *temp = realloc(Juegosfiltrados, (cantidad + 1) * sizeof(Videojuego));
        if (temp == NULL) {
            printf("\n [!] ERROR | Sin memoria \n");
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
        printf("No se encontraron juegos con stock para la plataforma [%s] \n", plataformaBuscada);
        return;
    }
    printf("\n === JUEGOS DISPONIBLES PARA %s ==== \n", plataformaBuscada);
    for (int i = 0 ; i < cantidad ; i++) {
        float precioDeVenta = Juegosfiltrados[i].precioAdquisicion * MARGEN_GANACIA;
        printf("[%d] %s - Precio de venta US$: %.2f - Stock: %d \n"), Juegosfiltrados[i].idJuego, Juegosfiltrados[i].titulo,precioDeVenta,Juegosfiltrados[i].stock;
    }
    free(Juegosfiltrados);
}

int buscarVideojuegoPorId (int idJuego, Videojuego *JuegoEncontrado) {
    FILE *file = fopen(ARCHIVO_VIDEOJUEGOS, "rb");
    if (file == NULL) {
        return 0;
    }
    Videojuego aux;
    while (fread(&aux, sizeof(Videojuego), 1, file) == 1) {
        if (aux.idJuego == idJuego) {
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
void verDetalleVideojuego (int idJuego) {
    Videojuego encontrado;
    if (buscarVideojuegoPorId (idJuego, &encontrado)) {
        printf("\n=== DETALLE ADMINISTRATIVO DEL VIDEOJUEGO ===\n");
        printf("ID Juego:           %d\n", encontrado.idJuego);
        printf("Titulo:             %s\n", encontrado.titulo);
        printf("Plataforma:         %s\n", encontrado.plataforma);
        printf("Genero:             %s\n", encontrado.genero);
        printf("Stock Actual:       %d unidades\n", encontrado.stock);
        printf("Precio Adquisicion: $%.2f\n", encontrado.precioAdquisicion);
        printf("Precio Venta Sugerido:  $%.2f\n", encontrado.precioAdquisicion * MARGEN_GANACIA);
    } else {
        printf("No se encontro ningun videojuego con el ID %d.\n", idJuego);
    }
}

int actualizarStockVideojuegos (int idJuego, int cambioStock) {
    FILE *file = fopen(ARCHIVO_VIDEOJUEGOS, "r+b");
    if (file == NULL) {
        return 0;
    }

    Videojuego aux;
    while (fread(&aux, sizeof(Videojuego), 1, file) == 1) {
        if (aux.idJuego == idJuego) {
            if (aux.stock + cambioStock < 0) {
                fclose(file);
                return 0;
            }
aux.stock += cambioStock;
            fseek(file, -((long) sizeof(Videojuego)), SEEK_CUR);
            fwrite(&aux, sizeof(Videojuego), 1, file);
            fclose(file);
            return 1;
        }
    }
    fclose(file);
    return 0;
}

int obtenerUltimoIdDevolucion () {
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

int transcionYaDevuelta (int idTransaccion) {
    FILE *file = fopen(ARCHIVO_DEVOLUCIONES, "rb");
    if (file == NULL) {
        return 0;
    }
    Devolucion aux;
    while (fread(&aux,sizeof(Devolucion), 1, file) == 1) {
        if (aux.idTransaccionAnulada == idTransaccion) {
            fclose(file);
            return 1;
        }
    }
    fclose(file);
    return 0;
}

void registrarDevoluciones () {
    int idTransaccion = 0;
    printf("\n === REGISTRO DE DEVOLUCIONES === \n");
    printf("Ingrese el id de la transaccion a anular: ");
    scanf("%d", &idTransaccion);

    if (transcionYaDevuelta(idTransaccion)) {
        printf("[!] ERROR | LA TRANSACCION #%d, YA FUE DEVUELTA ANTERIORMENTE \n",idTransaccion);
        return;
    }

    int idJuego = 0;
    int cantidad = 0;
    printf("Ingrese ID del juego asociado:");
    scanf("%d", &idJuego);
    printf("Ingrese cantidad de unidades a reintegrar: ");
    scanf("%d", &cantidad);

    Devolucion nueva;

    nueva.idDevolucion = obtenerUltimoIdDevolucion() + 1;
    nueva.idTransaccionAnulada = idTransaccion;
    nueva.idJuego = idJuego;
    nueva.cantidadDevuelta = cantidad;

    printf("FECHA DE DEVOLUCION [DIA | MES | AÑO ]: ");
    scanf("%d %d %d", &nueva.fechaDevolucion.dia, &nueva.fechaDevolucion.mes, &nueva.fechaDevolucion.anio);

    printf("Motivo: ");
    fflush(stdin);
    fgets(nueva.motivo, sizeof(nueva.motivo), stdin);
    nueva.motivo[strcspn(nueva.motivo, "\n")] = '\0';

    if (actualizarStockVideojuegos(nueva.idJuego, nueva.cantidadDevuelta)) {
        FILE *file = fopen(ARCHIVO_DEVOLUCIONES, "ab");
        if (file != NULL) {
            fwrite(&nueva, sizeof(Devolucion), 1, file);
            fclose(file);
            printf("Devolucion #%d Registrada. \n ha sido devuelto al stock (%d unidades)\n");
            nueva.idDevolucion, nueva.cantidadDevuelta;
        }
    }else {
        printf("[!] ERROR, NO SE ENCONTRO EL VIDEOJUEGO ID %d PARA ACTUALIZAR EL STOCK \n",nueva.idJuego);
    }
}

void listarDevoluciones () {
    FILE *file = fopen(ARCHIVO_DEVOLUCIONES, "rb");
    if (file == NULL) {
        printf("[!] No hay devoluciones registradas \n");
        return;
    }
    Devolucion aux;
    printf("\n === HISTORIAL DE DEVOLUCIONES DE VIDEOJUEGOS ===");
    while (fread(&aux, sizeof(Devolucion), 1, file) == 1) {
        printf("DEVOLUCION #%d | Venta anulada #%d | ID de juego #%d | Cantidad %d | Fecha %d/%d/%d | Motivo %s \n",
            aux.idDevolucion,aux.idTransaccionAnulada, aux.idJuego, aux.cantidadDevuelta,
            aux.fechaDevolucion.dia, aux.fechaDevolucion.mes, aux.fechaDevolucion.anio,
            aux.motivo);
    }
    fclose(file);
}