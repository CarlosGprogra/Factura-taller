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
        printf("5. Salir\n");
        printf(">> ");
        scanf("%d", &opcion);

        if (opcion < 1 || opcion > 5) {
            printf("Opcion no valida, intente de nuevo.\n");
        }
    } while (opcion < 1 || opcion > 5);

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

    printf("Ingrese la cedula del cliente: ");
    scanf("%d", &factura.cedula);

    do {
        printf("Ingrese el numero de productos (maximo %d): ", MAX_PRODUCTOS);
        scanf("%d", &factura.numProductos);
        if (factura.numProductos < 1 || factura.numProductos > MAX_PRODUCTOS) {
            printf("Numero de productos invalido. Debe ser entre 1 y %d.\n", MAX_PRODUCTOS);
        }
    } while (factura.numProductos < 1 || factura.numProductos > MAX_PRODUCTOS);

    factura.total = 0;
    for (int i = 0; i < factura.numProductos; i++) {
        printf("Ingrese el nombre del producto: ");
        leerCadena(factura.productos[i].nombre, 50);

        do {
            printf("Ingrese la cantidad del producto: ");
            scanf("%d", &factura.productos[i].cantidad);
            if (factura.productos[i].cantidad <= 0) {
                printf("La cantidad debe ser un valor positivo.\n");
            }
        } while (factura.productos[i].cantidad <= 0);

        do {
            printf("Ingrese el precio del producto: ");
            scanf("%f", &factura.productos[i].precio);
            if (factura.productos[i].precio <= 0) {
                printf("El precio debe ser un valor positivo.\n");
            }
        } while (factura.productos[i].precio <= 0);

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

    // Leer todo el archivo y almacenarlo en memoria
    fseek(file, 0, SEEK_END);
    long fileSize = ftell(file);
    fseek(file, 0, SEEK_SET);
    
    int numFacturas = fileSize / sizeof(struct Factura);
    struct Factura *facturas = malloc(fileSize);

    fread(facturas, sizeof(struct Factura), numFacturas, file);
    
    // Buscar y actualizar la factura correspondiente
    for (int i = 0; i < numFacturas; i++) {
        if (facturas[i].cedula == cedula) {
            found = 1;
            printf("Factura encontrada. Ingrese los nuevos datos:\n");

            printf("Ingrese el nuevo nombre del cliente: ");
            leerCadena(facturas[i].nombre, 20);

            do {
                printf("Ingrese el nuevo numero de productos (maximo %d): ", MAX_PRODUCTOS);
                scanf("%d", &facturas[i].numProductos);
                if (facturas[i].numProductos < 1 || facturas[i].numProductos > MAX_PRODUCTOS) {
                    printf("Numero de productos invalido. Debe ser entre 1 y %d.\n", MAX_PRODUCTOS);
                }
            } while (facturas[i].numProductos < 1 || facturas[i].numProductos > MAX_PRODUCTOS);

            facturas[i].total = 0;
            for (int j = 0; j < facturas[i].numProductos; j++) {
                printf("Ingrese el nombre del producto: ");
                leerCadena(facturas[i].productos[j].nombre, 50);

                do {
                    printf("Ingrese la cantidad del producto: ");
                    scanf("%d", &facturas[i].productos[j].cantidad);
                    if (facturas[i].productos[j].cantidad <= 0) {
                        printf("La cantidad debe ser un valor positivo.\n");
                    }
                } while (facturas[i].productos[j].cantidad <= 0);

                do {
                    printf("Ingrese el precio del producto: ");
                    scanf("%f", &facturas[i].productos[j].precio);
                    if (facturas[i].productos[j].precio <= 0) {
                        printf("El precio debe ser un valor positivo.\n");
                    }
                } while (facturas[i].productos[j].precio <= 0);

                facturas[i].total += facturas[i].productos[j].cantidad * facturas[i].productos[j].precio;
            }
            break;
        }
    }

    if (!found) {
        printf("No se encontro una factura con la cedula proporcionada.\n");
        free(facturas);
        fclose(file);
        return;
    }

    // Volver a abrir el archivo para sobrescribir
    freopen("factura.dat", "wb", file);
    fwrite(facturas, sizeof(struct Factura), numFacturas, file);
    printf("Factura actualizada correctamente.\n");

    free(facturas);
    fclose(file);
}

void deleteFactura() {
    int cedula;
    printf("Ingrese la cedula de la factura a borrar: ");
    scanf("%d", &cedula);

    FILE *file = fopen("factura.dat", "rb");
    FILE *tempFile = fopen("temp.dat", "wb");
    if (file == NULL || tempFile == NULL) {
        printf("Error al abrir el archivo para borrar facturas.\n");
        return;
    }

    struct Factura factura;
    int found = 0;
    while (fread(&factura, sizeof(struct Factura), 1, file)) {
        if (factura.cedula == cedula) {
            found = 1;
            printf("Factura con cedula %d borrada.\n", cedula);
        } else {
            fwrite(&factura, sizeof(struct Factura), 1, tempFile);
        }
    }

    if (!found) {
        printf("No se encontro una factura con la cedula proporcionada.\n");
    }

    fclose(file);
    fclose(tempFile);

    remove("factura.dat");
    rename("temp.dat", "factura.dat");
}
