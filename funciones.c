#include <stdio.h>
#include <string.h>
#include <stdlib.h>  // Para malloc y free
#include "funciones.h"

#define MAX_PRODUCTOS 5

int menu() {
    int opcion;
    do {
        printf("1. Crear factura\n");
        printf("2. Leer factura\n");
        printf("3. Actualizar factura\n");
        printf("4. Borrar factura\n");
        printf("5. Mostrar factura detallada\n");
        printf("6. Salir\n");
        printf(">> ");
        scanf("%d", &opcion);

        if (opcion < 1 || opcion > 6) {
            printf("Opcion no valida, intente de nuevo.\n");
        }
    } while (opcion < 1 || opcion > 6);

    return opcion;
}

void leerCadena(char *cadena, int num) {
    fflush(stdin);
    fgets(cadena, num, stdin);
    int len = strlen(cadena) - 1;
    if (cadena[len] == '\n') {
        cadena[len] = '\0';
    }
}

void savefactura(struct Factura *factura) {
    FILE *file = fopen("factura.dat", "ab+");
    if (file == NULL) {
        printf("Error al abrir el archivo para guardar la factura.\n");
        return;
    }

    fwrite(factura, sizeof(struct Factura), 1, file);
    printf("Factura guardada correctamente.\n");
    fclose(file);
}

void createFactura() {
    struct Factura factura;
    printf("Ingrese el nombre del cliente: ");
    leerCadena(factura.nombre, 20);

    do {
        printf("Ingrese la cedula del cliente (entero): ");
        if (scanf("%d", &factura.cedula) != 1) {
            printf("Error: debe ingresar un numero entero.\n");
            while (getchar() != '\n');
            continue;
        }
        break;
    } while (1);

    do {
        printf("Ingrese el numero de productos (maximo %d, entero): ", MAX_PRODUCTOS);
        if (scanf("%d", &factura.numProductos) != 1 || factura.numProductos < 1 || factura.numProductos > MAX_PRODUCTOS) {
            printf("Numero de productos invalido. Debe ser un entero entre 1 y %d.\n", MAX_PRODUCTOS);
            while (getchar() != '\n');
            continue;
        }
        break;
    } while (1);

    factura.total = 0;
    for (int i = 0; i < factura.numProductos; i++) {
        printf("Ingrese el nombre del producto: ");
        leerCadena(factura.productos[i].nombre, 50);

        do {
            printf("Ingrese la cantidad del producto (entero): ");
            if (scanf("%d", &factura.productos[i].cantidad) != 1 || factura.productos[i].cantidad <= 0) {
                printf("La cantidad debe ser un entero positivo.\n");
                while (getchar() != '\n');
                continue;
            }
            break;
        } while (1);

        do {
            printf("Ingrese el precio del producto: ");
            if (scanf("%f", &factura.productos[i].precio) != 1 || factura.productos[i].precio <= 0) {
                printf("El precio debe ser un valor positivo.\n");
                while (getchar() != '\n');
                continue;
            }
            break;
        } while (1);

        factura.total += factura.productos[i].cantidad * factura.productos[i].precio;
    }

    savefactura(&factura);
}

void readFactura() {
    FILE *file = fopen("factura.dat", "rb");
    if (file == NULL) {
        printf("Error al abrir el archivo para leer facturas.\n");
        return;
    }

    struct Factura factura;
    printf("Cedula\t\tNombre\t\tTotal\n");
    while (fread(&factura, sizeof(struct Factura), 1, file)) {
        printf("%d\t\t%s\t\t%.2f\n", factura.cedula, factura.nombre, factura.total);
    }

    fclose(file);
}

void updateFactura() {
    int cedula;
    printf("Ingrese la cedula de la factura a actualizar: ");
    scanf("%d", &cedula);

    FILE *file = fopen("factura.dat", "rb+");
    if (file == NULL) {
        printf("Error al abrir el archivo para actualizar facturas.\n");
        return;
    }

    struct Factura factura;
    int found = 0;

    while (fread(&factura, sizeof(struct Factura), 1, file)) {
        if (factura.cedula == cedula) {
            found = 1;
            printf("Factura encontrada. Ingrese los nuevos datos:\n");

            printf("Ingrese el nuevo nombre del cliente: ");
            leerCadena(factura.nombre, 20);

            do {
                printf("Ingrese el nuevo numero de productos (maximo %d, entero): ", MAX_PRODUCTOS);
                if (scanf("%d", &factura.numProductos) != 1 || factura.numProductos < 1 || factura.numProductos > MAX_PRODUCTOS) {
                    printf("Numero de productos invalido. Debe ser un entero entre 1 y %d.\n", MAX_PRODUCTOS);
                    while (getchar() != '\n');
                    continue;
                }
                break;
            } while (1);

            factura.total = 0;
            for (int i = 0; i < factura.numProductos; i++) {
                printf("Ingrese el nombre del producto: ");
                leerCadena(factura.productos[i].nombre, 50);

                do {
                    printf("Ingrese la cantidad del producto (entero): ");
                    if (scanf("%d", &factura.productos[i].cantidad) != 1 || factura.productos[i].cantidad <= 0) {
                        printf("La cantidad debe ser un entero positivo.\n");
                        while (getchar() != '\n');
                        continue;
                    }
                    break;
                } while (1);

                do {
                    printf("Ingrese el precio del producto: ");
                    if (scanf("%f", &factura.productos[i].precio) != 1 || factura.productos[i].precio <= 0) {
                        printf("El precio debe ser un valor positivo.\n");
                        while (getchar() != '\n');
                        continue;
                    }
                    break;
                } while (1);

                factura.total += factura.productos[i].cantidad * factura.productos[i].precio;
            }

            fseek(file, -sizeof(struct Factura), SEEK_CUR);
            fwrite(&factura, sizeof(struct Factura), 1, file);
            printf("Factura actualizada correctamente.\n");
            break;
        }
    }

    if (!found) {
        printf("No se encontro una factura con la cedula proporcionada.\n");
    }

    fclose(file);
}

void deleteFactura() {
    int cedula;
    printf("Ingrese la cedula de la factura a borrar: ");
    scanf("%d", &cedula);

    FILE *file = fopen("factura.dat", "rb+");
    if (file == NULL) {
        printf("Error al abrir el archivo para borrar facturas.\n");
        return;
    }

    struct Factura factura;
    int found = 0;
    long pos;

    while ((pos = ftell(file)) >= 0 && fread(&factura, sizeof(struct Factura), 1, file)) {
        if (factura.cedula == cedula) {
            found = 1;
            printf("Factura con cedula %d borrada.\n", cedula);
            break;
        }
    }

    if (found) {
        // Mover todas las facturas siguientes una posicion hacia atras
        struct Factura temp;
        while (fread(&temp, sizeof(struct Factura), 1, file)) {
            fseek(file, pos, SEEK_SET);
            fwrite(&temp, sizeof(struct Factura), 1, file);
            pos = ftell(file);
            fseek(file, pos, SEEK_SET);
        }
        fflush(file);
        ftruncate(fileno(file), pos);
    } else {
        printf("No se encontro una factura con la cedula proporcionada.\n");
    }

    fclose(file);
}

void mostrarFacturaDetallada() {
    int cedula;
    printf("Ingrese la cedula de la factura a mostrar: ");
    scanf("%d", &cedula);

    FILE *file = fopen("factura.dat", "rb");
    if (file == NULL) {
        printf("Error al abrir el archivo para mostrar la factura.\n");
        return;
    }

    struct Factura factura;
    int found = 0;

    while (fread(&factura, sizeof(struct Factura), 1, file)) {
        if (factura.cedula == cedula) {
            found = 1;
            printf("Factura detallada:\n");
            printf("Nombre del cliente: %s\n", factura.nombre);
            printf("Cedula: %d\n", factura.cedula);
            printf("Productos:\n");
            printf("%-20s %-10s %-10s %-10s\n", "Producto", "Cantidad", "Precio", "Subtotal");
            for (int i = 0; i < factura.numProductos; i++) {
                float subtotal = factura.productos[i].cantidad * factura.productos[i].precio;
                printf("%-20s %-10d %-10.2f %-10.2f\n", factura.productos[i].nombre, factura.productos[i].cantidad, factura.productos[i].precio, subtotal);
            }
            printf("Total: %.2f\n", factura.total);
            break;
        }
    }

    if (!found) {
        printf("No se encontro una factura con la cedula proporcionada.\n");
    }

    fclose(file);
}
