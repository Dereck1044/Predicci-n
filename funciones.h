#ifndef FUNCIONES_H
#define FUNCIONES_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>


#define MAX_ZONAS 100
#define DIAS_HISTORICOS 30
#define LIMITE_CO2 400.0
#define LIMITE_SO2 20.0
#define LIMITE_NO2 40.0
#define LIMITE_PM25 25.0


typedef struct {
    char nombre[50];
    float co2;
    float so2;
    float no2;
    float pm25;
    float temperatura;
    float velocidad_viento;
    float humedad;
    int id;
} DatosZona;

typedef struct {
    DatosZona zona;
    char fecha[20];
    int dia;
    int mes;
    int anio;
} RegistroDiario;

typedef struct {
    char nombre_zona[50];
    float co2_pred;
    float so2_pred;
    float no2_pred;
    float pm25_pred;
    int nivel_alerta;
    char fecha_prediccion[20];
} Prediccion;

typedef struct {
    char nombre_zona[50];
    float promedio_co2;
    float promedio_so2;
    float promedio_no2;
    float promedio_pm25;
    int dias_analizados;
    int excede_co2;
    int excede_so2;
    int excede_no2;
    int excede_pm25;
} PromedioHistorico;




void SaltodeLinea(char *cadena, int n);
int esStringVacio(const char* str);
float validarFloat(const char* prompt, float min, float max);
int validarEntero(const char* prompt, int min, int max);
void validarTextoSoloLetras(const char* prompt, char* destino, int max_len);
int verificarZonaExiste(const char* nombre, DatosZona *zonas, int numZonas);


int menu();
void agregarZona(DatosZona *zonas, int *numZonas);
void gestionarZonas(DatosZona *zonas, int *numZonas);
void modificarZona(DatosZona *zonas, int numZonas);
void eliminarZona(DatosZona *zonas, int *numZonas);
void mostrarZonasRegistradas();
void ingresarDatosContaminacion(DatosZona *zonas, int numZonas);
void monitorearContaminacionActual();
void calcularPromediosHistoricos();
void predecirNivelesFuturos();
void generarAlertasPreventivas();
void generarRecomendaciones();
void compararZonas();
void verHistorialCompleto();
void exportarReporte();

#endif