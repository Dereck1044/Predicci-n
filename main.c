#include <stdio.h>
#include "funciones.h"

int main() {
    DatosZona zonas[MAX_ZONAS];
    int numZonas = 0;
    FILE *archivo;
    int i;
    int opcion;
    
    
    
    archivo = fopen("zonas.dat", "rb");
    if(archivo != NULL) {
        if(fread(&numZonas, sizeof(int), 1, archivo) == 1) {
            for(i = 0; i < numZonas && i < MAX_ZONAS; i++) {
                fread(&zonas[i], sizeof(DatosZona), 1, archivo);
            }
        }
        fclose(archivo);
    }
    

    do {
        opcion = menu();
        
        switch(opcion) {
            case 1:
                gestionarZonas(zonas, &numZonas);
                break;
            case 2:
                ingresarDatosContaminacion(zonas, numZonas);
                break;
            case 3:
                monitorearContaminacionActual();
                break;
            case 4:
                calcularPromediosHistoricos();
                break;
            case 5:
                predecirNivelesFuturos();
                break;
            case 6:
                generarAlertasPreventivas();
                break;
            case 7:
                generarRecomendaciones();
                break;
            case 8:
                compararZonas();
                break;
            case 9:
                verHistorialCompleto();
                break;
            case 10:
                exportarReporte();
                break;
            case 11:
                printf("\n*** Programa finalizado ***\n");
                break;
            default:
                printf("Opcion invalida. Intente de nuevo.\n");
        }
    } while(opcion != 11);
    
    return 0;
}