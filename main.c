#include <stdio.h>
#include "funciones.h"

int main (int argc, char *argv[]) {
    int opc; 
    do
    {
        opc = menu(); 
        switch (opc)
        {
        case 1:
            createFactura();  
            break;
        case 2:
            readFactura();   
            break;
        case 3:
            updateFactura(); 
            break;
        case 4:
            deleteFactura(); 
            break;
        case 5:
            printf("Saliendo...\n"); 
            break;
        default:
            printf("Opcion no valida\n"); 
            break;
        }
    } while (opc != 5);

    return 0;
}
