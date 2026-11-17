# 🎮 Pixel Plus — Sistema de Gestión de Videojuegos

Trabajo Práctico Final de **Programación I** (UTN - Tecnicatura Universitaria en Programación).  
Sistema de consola desarrollado en **C** para la administración de inventario, clientes, empleados, ventas, devoluciones y reportes de una tienda de videojuegos mediante persistencia en archivos binarios.

---

## 👥 Integrantes y División de Módulos

| Integrante | Rama Git | Módulo Asignado | Responsabilidades Principales |
| :--- | :--- | :--- | :--- |
| **Marcelo** | `personas` | `persona.h` / `persona.c` | CRUD de Personas con bajas lógicas, gestión de Usuarios/Login y sistema de fidelización VIP recursivo. |
| **Ayrton** | `videojuegos` | `videojuego.h` / `videojuego.c` | Alta, modificación (con registro histórico de precios), bajas lógicas, control de stock y devoluciones. |
| **Daniel** | `transacciones` | `transaccion.h` / `transaccion.c` | Carrito de compras dinámico, registro de ventas, validación de stock/clientes y aplicación de descuentos VIP. |
| **Tomi** | `reportes` | `reportes.h` / `reportes.c` | Estadísticas mensuales, venta de mayor ganancia, ordenamiento de stock crítico con QuickSort y análisis recursivo por fechas. |

---

## 🛠️ Requisitos Técnicos Implementados

* **Modularización:** Separación estricta de cabeceras (`include/`) e implementaciones (`src/`), manteniendo `main.c` como punto de entrada y control de sesión.
* **Control de Acceso y Roles:** Gestión de inicio y cierre de sesión en memoria (`Usuario`), restringiendo el acceso al módulo de reportes exclusivamente al rol `admin`.
* **Persistencia Binaria:** Almacenamiento en archivos `.dat` dentro de `data/` (`personas.dat`, `usuarios.dat`, `videojuegos.dat`, `transacciones.dat`, `devoluciones.dat` y `reportes_precio.dat`).
* **Modificación *In-Place* y Bajas Lógicas:** Actualización directa en posición exacta mediante `ftell`, `fseek` (`SEEK_SET`), `fwrite` y `fflush` sin eliminación física de registros históricos.
* **Memoria Dinámica y Ordenamiento:** Uso de `realloc` para carrito de compras, filtros por plataforma y arreglo de stock crítico ordenado mediante **QuickSort**.
* **Recursividad:** Recorrido recursivo sobre punteros a archivo (`FILE *`) tanto para el cálculo histórico de compras VIP como para el análisis de ganancias por rango de fechas.

---

## 🔐 Credenciales de Prueba Precargadas

* **Administrador:** `admin` / `1234` (Acceso total a todos los módulos)
* **Empleado 1:** `cvega` / `pixel123`
* **Empleado 2:** `esoto` / `tienda456`

---

## 📁 Estructura del Proyecto

```text
TP-Final-Progra1/
├── include/               # Archivos de cabecera (.h)
│   ├── estructuras.h      # Estructuras base de la cátedra y rutas de archivos
│   ├── persona.h
│   ├── videojuego.h
│   ├── transaccion.h
│   └── reportes.h
├── src/                   # Código fuente (.c)
│   ├── main.c
│   ├── persona.c
│   ├── videojuego.c
│   ├── transaccion.c
│   └── reportes.c
├── data/                  # Archivos binarios (.dat) con datos de prueba
├── TP-Final-Progra1.cbp   # Archivo de proyecto para Code::Blocks
└── README.md