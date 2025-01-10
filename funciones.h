struct Producto 
{
    char nombre[50];
    int cantidad;
    float precio;
};

struct Factura
{
    char nombre[20];
    int cedula;
    int numProductos; 
    float total; 
    struct Producto productos[5]; 
}; 
int menu ();
void savefactura (struct Factura *factura); 
void createFactura (); 
void leerCadena (char *cadena,int num); 
void readFactura ();
void updateFactura();
void deleteFactura();
