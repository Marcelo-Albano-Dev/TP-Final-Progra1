#ifndef ESTRUCTURAS_H
#define ESTRUCTURAS_H

#define ARCH_PERSONAS "data/personas.dat"
#define ARCH_USUARIOS "data/usuarios.dat"
#define ARCH_VIDEOJUEGOS "data/videojuegos.dat"
#define ARCH_TRANSACCIONES "data/transacciones.dat"

typedef struct stPersona {
    char dni[15];
    char nombre[50];
    char telefono[20];
    char direccion[30];
    char rol[15]; // "cliente", "empleado", etc.
} Persona;

typedef struct stUsuario {
    char usuario[20];
    char contrasenia[20];
    Persona persona; // El usuario del sistema es una persona (empleado).
} Usuario;

typedef struct stVideojuego {
    int idJuego;
    char titulo[50];
    char plataforma[20]; // Ejemplo: "PS5", "Xbox Series X", "PC", "Switch".
    char genero[20];
    int stock;
    float precioAdquisicion;
} Videojuego;

typedef struct stFecha {
    int dia;
    int mes;
    int anio;
} Fecha;

typedef struct stTransaccion {
    int idTransaccion;
    Fecha fecha;
    int idJuegoVendido;
    int cantidadVendida;
    float precioUnitarioVenta;
    float gananciaTotal;
    char dniCliente[15];
    char dniEmpleado[15];
} Transaccion;

#endif
