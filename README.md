# 🎮 Pixel Plus — Sistema de Gestión de Videojuegos

Trabajo Práctico Final de **Programación I** (UTN - Tecnicatura Universitaria en Programación).  
Sistema de consola desarrollado en **C** para la administración de inventario, clientes, empleados, ventas y reportes de una tienda de videojuegos mediante persistencia en archivos binarios.

---

## 👥 Integrantes y División de Módulos

| Integrante | Rama Git | Módulo Asignado | Responsabilidades Principales |
| :--- | :--- | :--- | :--- |
| **Marcelo** | `personas` | `persona.h` / `persona.c` | CRUD de Personas con bajas lógicas, gestión de Usuarios/Login y sistema de fidelización VIP recursivo. |
| **Ayrton** | `videojuegos` | `videojuego.h` / `videojuego.c` | Alta, modificación, bajas lógicas, control de stock y consultas del catálogo de videojuegos. |
| **Daniel** | `transacciones` | `transaccion.h` / `transaccion.c` | Registro de ventas, validación de stock/clientes, cálculo de ganancias y emisión de comprobantes. |
| **Tomi** | `reportes` | `reportes.h` / `reportes.c` | Estadísticas de ventas, rankings de videojuegos, filtros por fecha/plataforma y auditoría. |

---

## 🛠️ Requisitos Técnicos Implementados

* **Modularización:** Separación estricta de cabeceras (`include/`) e implementaciones (`src/`), manteniendo `main.c` como punto de entrada limpio.
* **Persistencia Binaria:** Almacenamiento en archivos `.dat` dentro de `data/` (`personas.dat`, `usuarios.dat`, `videojuegos.dat`, `transacciones.dat`).
* **Modificación *In-Place* y Bajas Lógicas:** Actualización directa de registros mediante `fseek` y `fwrite` sin eliminación física de datos históricos.
* **Recursividad:** Recorrido recursivo sobre punteros a archivo (`FILE *`) para el cálculo histórico de compras y beneficios VIP.

---

## 📁 Estructura del Proyecto

```text
TP-Final-Progra1/
├── include/               # Archivos de cabecera (.h)
│   ├── estructuras.h      # Estructuras base de la cátedra y rutas de archivos
│   ├── persona.h
│   ├── videojuego.h
│   ├── transaccion.h
│   ├── reportes.h
│   └── auditoria.h
├── src/                   # Código fuente (.c)
│   ├── main.c
│   ├── persona.c
│   ├── videojuego.c
│   ├── transaccion.c
│   ├── reportes.c
│   └── auditoria.c
├── data/                  # Archivos binarios (.dat) generados en ejecución
├── TP-Final-Progra1.cbp   # Archivo de proyecto para Code::Blocks
└── README.md