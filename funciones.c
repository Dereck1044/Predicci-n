#include "funciones.h"

// --- FUNCIONES DE UTILIDAD ---

void SaltodeLinea(char *cadena, int n) {
    fgets(cadena, n, stdin);
    int len = strlen(cadena) - 1;
    if(cadena[len] == '\n') {
        cadena[len] = '\0';
    }
}

int esStringVacio(const char* str) {
    int i = 0;
    while(str[i] != '\0') {
        if(!isspace(str[i])) {
            return 0;
        }
        i++;
    }
    return 1;
}

float validarFloat(const char* prompt, float min, float max) {
    float valor;
    int valido;
    char linea[50];
    
    do {
        printf("%s (o presione 0 para omitir): ", prompt);
        if(fgets(linea, sizeof(linea), stdin) != NULL) {
            if(sscanf(linea, "%f", &valor) == 1) {
                if(valor < min || valor > max) {
                    printf("Error: El valor debe estar entre %.2f y %.2f.\n", min, max);
                    valido = 0;
                } else {
                    valido = 1;
                }
            } else {
                printf("Error: Debe ingresar un numero valido.\n");
                valido = 0;
            }
        } else {
            valido = 0;
        }
    } while(!valido);
    return valor;
}

int validarEntero(const char* prompt, int min, int max) {
    int valor;
    int valido;
    do {
        printf("%s", prompt);
        valido = (scanf("%d", &valor) == 1);
        fflush(stdin);
        if(!valido) {
            printf("Error: Debe ingresar un numero entero valido.\n");
        } else if(valor < min || valor > max) {
            printf("Error: El valor debe estar entre %d y %d.\n", min, max);
            valido = 0;
        }
    } while(!valido);
    return valor;
}

void validarTextoSoloLetras(const char* prompt, char* destino, int max_len) {
    int valido;
    int j;
    do {
        printf("%s", prompt);
        SaltodeLinea(destino, max_len);
        valido = 1;
        
        if(esStringVacio(destino)) {
            valido = 0;
            printf("Error: El texto no puede estar vacio o contener solo espacios.\n");
        } else {
            for(j = 0; destino[j] != '\0'; j++) {
                if(!((destino[j] >= 'A' && destino[j] <= 'Z') || 
                     (destino[j] >= 'a' && destino[j] <= 'z') ||
                     destino[j] == ' ')) {
                    valido = 0;
                    printf("Error: Solo se permiten letras y espacios.\n");
                    break;
                }
            }
        }
    } while(!valido);
}

int verificarZonaExiste(const char* nombre, DatosZona *zonas, int numZonas) {
    int i;
    for(i = 0; i < numZonas; i++) {
        if(strcmp(zonas[i].nombre, nombre) == 0) {
            return 1;
        }
    }
    return 0;
}

// --- FUNCIONES DEL SISTEMA ---

int menu() {
    int opciones;
    printf("\n========================================================================\n");
    printf("   SISTEMA INTEGRAL DE GESTION Y PREDICCION DE CONTAMINACION DEL AIRE\n");
    printf("========================================================================\n");
    printf("1)  Gestionar zonas (Agregar/Modificar/Eliminar)\n");
    printf("2)  Ingresar datos de contaminacion actual\n");
    printf("3)  Monitorear niveles de contaminacion actual\n");
    printf("4)  Calcular promedios historicos (30 dias)\n");
    printf("5)  Predecir niveles futuros (24 horas)\n");
    printf("6)  Generar alertas preventivas\n");
    printf("7)  Generar recomendaciones de mitigacion\n");
    printf("8)  Comparar niveles entre zonas\n");
    printf("9)  Ver historial completo de registros\n");
    printf("10) Exportar reporte completo\n");
    printf("11) Salir\n");
    printf("========================================================================\n");
    printf("Opcion: ");
    scanf("%d", &opciones);
    fflush(stdin);
    return opciones;
}

void agregarZona(DatosZona *zonas, int *numZonas) {
    DatosZona nuevaZona;
    FILE *archivo;
    int i;
    
    printf("\n--- Agregar Nueva Zona ---\n");
    
    if(*numZonas >= MAX_ZONAS) {
        printf("Error: Se ha alcanzado el limite maximo de zonas.\n");
        return;
    }
    
    validarTextoSoloLetras("Ingrese el nombre de la zona: ", nuevaZona.nombre, 50);
    
    if(verificarZonaExiste(nuevaZona.nombre, zonas, *numZonas)) {
        printf("Error: Ya existe una zona con ese nombre.\n");
        return;
    }
    
    nuevaZona.id = *numZonas + 1;
    nuevaZona.co2 = 0.0;
    nuevaZona.so2 = 0.0;
    nuevaZona.no2 = 0.0;
    nuevaZona.pm25 = 0.0;
    nuevaZona.temperatura = 0.0;
    nuevaZona.velocidad_viento = 0.0;
    nuevaZona.humedad = 0.0;
    
    zonas[*numZonas] = nuevaZona;
    (*numZonas)++;
    
    archivo = fopen("zonas.dat", "wb");
    if(archivo != NULL) {
        fwrite(numZonas, sizeof(int), 1, archivo);
        for(i = 0; i < *numZonas; i++) {
            fwrite(&zonas[i], sizeof(DatosZona), 1, archivo);
        }
        fclose(archivo);
        printf("\n*** Zona agregada exitosamente ***\n");
        printf("ID: %d | Nombre: %s\n", nuevaZona.id, nuevaZona.nombre);
    } else {
        printf("Error al guardar la zona.\n");
    }
}

void mostrarZonasRegistradas() {
    DatosZona zonas[MAX_ZONAS];
    int numZonas = 0;
    FILE *archivo;
    int i;
    
    printf("\n--- Zonas Registradas ---\n");
    
    archivo = fopen("zonas.dat", "rb");
    if(archivo == NULL) {
        printf("No hay zonas registradas en el sistema.\n");
        return;
    }
    
    fread(&numZonas, sizeof(int), 1, archivo);
    if(numZonas == 0) {
        printf("No hay zonas registradas.\n");
        fclose(archivo);
        return;
    }
    
    for(i = 0; i < numZonas; i++) {
        fread(&zonas[i], sizeof(DatosZona), 1, archivo);
    }
    fclose(archivo);
    
    for(i = 0; i < numZonas; i++) {
        printf("\n[Zona #%d]\n", zonas[i].id);
        printf("  Nombre: %s\n", zonas[i].nombre);
        printf("  Ultima actualizacion:\n");
        printf("    CO2: %.2f ppm | SO2: %.2f ug/m3\n", zonas[i].co2, zonas[i].so2);
        printf("    NO2: %.2f ug/m3 | PM2.5: %.2f ug/m3\n", zonas[i].no2, zonas[i].pm25);
        printf("------------------------------------------------------------------------\n");
    }
}

void gestionarZonas(DatosZona *zonas, int *numZonas) {
    int opcion;
    
    do {
        printf("\n--- Gestion de Zonas ---\n");
        printf("1) Agregar zona\n");
        printf("2) Modificar zona\n");
        printf("3) Eliminar zona\n");
        printf("4) Ver zonas registradas\n");
        printf("5) Volver al menu principal\n");
        opcion = validarEntero("Opcion: ", 1, 5);
        
        switch(opcion) {
            case 1:
                agregarZona(zonas, numZonas);
                break;
            case 2:
                modificarZona(zonas, *numZonas);
                break;
            case 3:
                eliminarZona(zonas, numZonas);
                break;
            case 4:
                mostrarZonasRegistradas();
                break;
        }
    } while(opcion != 5);
}

void modificarZona(DatosZona *zonas, int numZonas) {
    FILE *archivo;
    int i, idZona, encontrado = 0;
    char nuevoNombre[50];
    
    if(numZonas == 0) {
        printf("No hay zonas para modificar.\n");
        return;
    }
    
    mostrarZonasRegistradas();
    idZona = validarEntero("\nIngrese el ID de la zona a modificar: ", 1, numZonas);
    
    for(i = 0; i < numZonas; i++) {
        if(zonas[i].id == idZona) {
            encontrado = 1;
            printf("\nZona actual: %s\n", zonas[i].nombre);
            validarTextoSoloLetras("Ingrese el nuevo nombre: ", nuevoNombre, 50);
            strcpy(zonas[i].nombre, nuevoNombre);
            
            archivo = fopen("zonas.dat", "wb");
            if(archivo != NULL) {
                fwrite(&numZonas, sizeof(int), 1, archivo);
                for(i = 0; i < numZonas; i++) {
                    fwrite(&zonas[i], sizeof(DatosZona), 1, archivo);
                }
                fclose(archivo);
                printf("\n*** Zona modificada exitosamente ***\n");
            }
            break;
        }
    }
    
    if(!encontrado) {
        printf("Zona no encontrada.\n");
    }
}

void eliminarZona(DatosZona *zonas, int *numZonas) {
    FILE *archivo;
    int i, j, idZona, encontrado = 0;
    
    if(*numZonas == 0) {
        printf("No hay zonas para eliminar.\n");
        return;
    }
    
    mostrarZonasRegistradas();
    idZona = validarEntero("\nIngrese el ID de la zona a eliminar: ", 1, *numZonas);
    
    for(i = 0; i < *numZonas; i++) {
        if(zonas[i].id == idZona) {
            encontrado = 1;
            printf("\nEliminando zona: %s\n", zonas[i].nombre);
            
            for(j = i; j < *numZonas - 1; j++) {
                zonas[j] = zonas[j + 1];
            }
            (*numZonas)--;
            
            archivo = fopen("zonas.dat", "wb");
            if(archivo != NULL) {
                fwrite(numZonas, sizeof(int), 1, archivo);
                for(j = 0; j < *numZonas; j++) {
                    fwrite(&zonas[j], sizeof(DatosZona), 1, archivo);
                }
                fclose(archivo);
                printf("\n*** Zona eliminada exitosamente ***\n");
            }
            break;
        }
    }
    
    if(!encontrado) {
        printf("Zona no encontrada.\n");
    }
}

void ingresarDatosContaminacion(DatosZona *zonas, int numZonas) {
    FILE *archivo;
    RegistroDiario registro;
    int i, dia, mes, anio;
    
    printf("\n--- Ingresar Datos de Contaminacion Actual ---\n");
    
    if(numZonas == 0) {
        printf("Error: Debe registrar al menos una zona primero.\n");
        return;
    }
    
    printf("\nIngrese la fecha del registro:\n");
    dia = validarEntero("Dia (1-31): ", 1, 31);
    mes = validarEntero("Mes (1-12): ", 1, 12);
    anio = validarEntero("Anio (2020-2026): ", 2020, 2026);
    sprintf(registro.fecha, "%02d/%02d/%d", dia, mes, anio);
    registro.dia = dia;
    registro.mes = mes;
    registro.anio = anio;
    
    mostrarZonasRegistradas();
    
    int idZona = validarEntero("\nSeleccione el ID de la zona: ", 1, numZonas);
    int encontrado = 0;
    
    for(i = 0; i < numZonas; i++) {
        if(zonas[i].id == idZona) {
            encontrado = 1;
            printf("\n--- Datos para: %s ---\n", zonas[i].nombre);
            printf("(Presione 0 para omitir un dato)\n\n");
            
            zonas[i].co2 = validarFloat("CO2 (ppm, 0-2000)", 0.0, 2000.0);
            zonas[i].so2 = validarFloat("SO2 (ug/m3, 0-500)", 0.0, 500.0);
            zonas[i].no2 = validarFloat("NO2 (ug/m3, 0-500)", 0.0, 500.0);
            zonas[i].pm25 = validarFloat("PM2.5 (ug/m3, 0-500)", 0.0, 500.0);
            zonas[i].temperatura = validarFloat("Temperatura (C, -20 a 50)", -20.0, 50.0);
            zonas[i].velocidad_viento = validarFloat("Velocidad del viento (km/h, 0-200)", 0.0, 200.0);
            zonas[i].humedad = validarFloat("Humedad (%, 0-100)", 0.0, 100.0);
            
            registro.zona = zonas[i];
            
            archivo = fopen("historial.dat", "ab");
            if(archivo != NULL) {
                fwrite(&registro, sizeof(RegistroDiario), 1, archivo);
                fclose(archivo);
            }
            
            archivo = fopen("zonas.dat", "wb");
            if(archivo != NULL) {
                fwrite(&numZonas, sizeof(int), 1, archivo);
                for(i = 0; i < numZonas; i++) {
                    fwrite(&zonas[i], sizeof(DatosZona), 1, archivo);
                }
                fclose(archivo);
                printf("\n*** Datos guardados exitosamente ***\n");
            } else {
                printf("Error al guardar los datos.\n");
            }
            break;
        }
    }
    
    if(!encontrado) {
        printf("Zona no encontrada.\n");
    }
}

void monitorearContaminacionActual() {
    DatosZona zonas[MAX_ZONAS];
    int numZonas = 0;
    FILE *archivo;
    int i;
    
    printf("\n--- Monitoreo de Contaminacion Actual ---\n");
    
    archivo = fopen("zonas.dat", "rb");
    if(archivo == NULL) {
        printf("No hay datos disponibles.\n");
        return;
    }
    
    fread(&numZonas, sizeof(int), 1, archivo);
    for(i = 0; i < numZonas; i++) {
        fread(&zonas[i], sizeof(DatosZona), 1, archivo);
    }
    fclose(archivo);
    
    if(numZonas == 0) {
        printf("No hay zonas registradas.\n");
        return;
    }
    
    printf("\nLimites OMS: CO2=%.0f ppm | SO2=%.0f | NO2=%.0f | PM2.5=%.0f ug/m3\n",
           LIMITE_CO2, LIMITE_SO2, LIMITE_NO2, LIMITE_PM25);
    printf("========================================================================\n");
    
    for(i = 0; i < numZonas; i++) {
        printf("\n[%s]\n", zonas[i].nombre);
        printf("------------------------------------------------------------------------\n");
        printf("  CO2:   %.2f ppm", zonas[i].co2);
        if(zonas[i].co2 > LIMITE_CO2) {
            printf(" [EXCEDE: +%.1f%%]", ((zonas[i].co2 - LIMITE_CO2) / LIMITE_CO2) * 100);
        } else {
            printf(" [OK]");
        }
        printf("\n");
        
        printf("  SO2:   %.2f ug/m3", zonas[i].so2);
        if(zonas[i].so2 > LIMITE_SO2) {
            printf(" [EXCEDE: +%.1f%%]", ((zonas[i].so2 - LIMITE_SO2) / LIMITE_SO2) * 100);
        } else {
            printf(" [OK]");
        }
        printf("\n");
        
        printf("  NO2:   %.2f ug/m3", zonas[i].no2);
        if(zonas[i].no2 > LIMITE_NO2) {
            printf(" [EXCEDE: +%.1f%%]", ((zonas[i].no2 - LIMITE_NO2) / LIMITE_NO2) * 100);
        } else {
            printf(" [OK]");
        }
        printf("\n");
        
        printf("  PM2.5: %.2f ug/m3", zonas[i].pm25);
        if(zonas[i].pm25 > LIMITE_PM25) {
            printf(" [EXCEDE: +%.1f%%]", ((zonas[i].pm25 - LIMITE_PM25) / LIMITE_PM25) * 100);
        } else {
            printf(" [OK]");
        }
        printf("\n");
        
        printf("\n  Condiciones climaticas:\n");
        printf("    Temperatura: %.2f C | Viento: %.2f km/h | Humedad: %.2f%%\n",
               zonas[i].temperatura, zonas[i].velocidad_viento, zonas[i].humedad);
    }
    printf("\n========================================================================\n");
}

void calcularPromediosHistoricos() {
    FILE *archivo;
    RegistroDiario registros[1000];
    PromedioHistorico promedios[MAX_ZONAS];
    int totalRegistros = 0;
    int i, j, numPromedios = 0;
    
    printf("\n--- Promedios Historicos (Ultimos 30 Dias) ---\n");
    
    archivo = fopen("historial.dat", "rb");
    if(archivo == NULL) {
        printf("No hay historial disponible.\n");
        return;
    }
    
    while(fread(&registros[totalRegistros], sizeof(RegistroDiario), 1, archivo) == 1 && 
          totalRegistros < 1000) {
        totalRegistros++;
    }
    fclose(archivo);
    
    if(totalRegistros == 0) {
        printf("No hay registros historicos.\n");
        return;
    }
    
    for(i = 0; i < totalRegistros && i < DIAS_HISTORICOS; i++) {
        int encontrado = 0;
        for(j = 0; j < numPromedios; j++) {
            if(strcmp(promedios[j].nombre_zona, registros[i].zona.nombre) == 0) {
                promedios[j].promedio_co2 += registros[i].zona.co2;
                promedios[j].promedio_so2 += registros[i].zona.so2;
                promedios[j].promedio_no2 += registros[i].zona.no2;
                promedios[j].promedio_pm25 += registros[i].zona.pm25;
                promedios[j].dias_analizados++;
                encontrado = 1;
                break;
            }
        }
        
        if(!encontrado) {
            strcpy(promedios[numPromedios].nombre_zona, registros[i].zona.nombre);
            promedios[numPromedios].promedio_co2 = registros[i].zona.co2;
            promedios[numPromedios].promedio_so2 = registros[i].zona.so2;
            promedios[numPromedios].promedio_no2 = registros[i].zona.no2;
            promedios[numPromedios].promedio_pm25 = registros[i].zona.pm25;
            promedios[numPromedios].dias_analizados = 1;
            numPromedios++;
        }
    }
    
    for(i = 0; i < numPromedios; i++) {
        if(promedios[i].dias_analizados > 0) {
            promedios[i].promedio_co2 /= promedios[i].dias_analizados;
            promedios[i].promedio_so2 /= promedios[i].dias_analizados;
            promedios[i].promedio_no2 /= promedios[i].dias_analizados;
            promedios[i].promedio_pm25 /= promedios[i].dias_analizados;
            
            promedios[i].excede_co2 = (promedios[i].promedio_co2 > LIMITE_CO2);
            promedios[i].excede_so2 = (promedios[i].promedio_so2 > LIMITE_SO2);
            promedios[i].excede_no2 = (promedios[i].promedio_no2 > LIMITE_NO2);
            promedios[i].excede_pm25 = (promedios[i].promedio_pm25 > LIMITE_PM25);
        }
    }
    
    archivo = fopen("promedios.dat", "wb");
    if(archivo != NULL) {
        fwrite(&numPromedios, sizeof(int), 1, archivo);
        for(i = 0; i < numPromedios; i++) {
            fwrite(&promedios[i], sizeof(PromedioHistorico), 1, archivo);
        }
        fclose(archivo);
    }
    
    printf("\nDatos analizados: %d registros\n", totalRegistros < DIAS_HISTORICOS ? totalRegistros : DIAS_HISTORICOS);
    printf("========================================================================\n");
    
    for(i = 0; i < numPromedios; i++) {
        printf("\n[%s] - %d dias\n", promedios[i].nombre_zona, promedios[i].dias_analizados);
        printf("------------------------------------------------------------------------\n");
        printf("  CO2:   %.2f ppm   [Limite: %.0f] %s\n", 
               promedios[i].promedio_co2, LIMITE_CO2, 
               promedios[i].excede_co2 ? "[EXCEDE]" : "[OK]");
        printf("  SO2:   %.2f ug/m3 [Limite: %.0f] %s\n", 
               promedios[i].promedio_so2, LIMITE_SO2,
               promedios[i].excede_so2 ? "[EXCEDE]" : "[OK]");
        printf("  NO2:   %.2f ug/m3 [Limite: %.0f] %s\n", 
               promedios[i].promedio_no2, LIMITE_NO2,
               promedios[i].excede_no2 ? "[EXCEDE]" : "[OK]");
        printf("  PM2.5: %.2f ug/m3 [Limite: %.0f] %s\n", 
               promedios[i].promedio_pm25, LIMITE_PM25,
               promedios[i].excede_pm25 ? "[EXCEDE]" : "[OK]");
    }
    printf("\n========================================================================\n");
}

void predecirNivelesFuturos() {
    FILE *archivo;
    RegistroDiario registros[100];
    Prediccion predicciones[MAX_ZONAS];
    DatosZona zonas[MAX_ZONAS];
    int totalRegistros = 0;
    int numZonas = 0;
    int i, j, numPredicciones = 0;
    
    printf("\n--- Prediccion de Niveles Futuros (24 Horas) ---\n");
    
    archivo = fopen("zonas.dat", "rb");
    if(archivo == NULL) {
        printf("No hay zonas registradas.\n");
        return;
    }
    fread(&numZonas, sizeof(int), 1, archivo);
    for(i = 0; i < numZonas; i++) {
        fread(&zonas[i], sizeof(DatosZona), 1, archivo);
    }
    fclose(archivo);
    
    archivo = fopen("historial.dat", "rb");
    if(archivo == NULL) {
        printf("No hay historial para realizar predicciones.\n");
        return;
    }
    
    while(fread(&registros[totalRegistros], sizeof(RegistroDiario), 1, archivo) == 1 && 
          totalRegistros < 100) {
        totalRegistros++;
    }
    fclose(archivo);
    
    if(totalRegistros < 3) {
        printf("Necesita al menos 3 registros para realizar predicciones.\n");
        return;
    }
    
    printf("\nAlgoritmo: Promedio ponderado con factores climaticos\n");
    printf("Registros analizados: %d\n", totalRegistros);
    printf("========================================================================\n");
    
    for(i = 0; i < numZonas; i++) {
        float sum_co2 = 0, sum_so2 = 0, sum_no2 = 0, sum_pm25 = 0;
        float peso_total = 0;
        int registros_zona = 0;
        
        for(j = totalRegistros - 1; j >= 0 && registros_zona < 10; j--) {
            if(strcmp(registros[j].zona.nombre, zonas[i].nombre) == 0) {
                float peso = (float)(10 - registros_zona) / 10.0;
                peso_total += peso;
                
                sum_co2 += registros[j].zona.co2 * peso;
                sum_so2 += registros[j].zona.so2 * peso;
                sum_no2 += registros[j].zona.no2 * peso;
                sum_pm25 += registros[j].zona.pm25 * peso;
                registros_zona++;
            }
        }
        
        if(registros_zona > 0 && peso_total > 0) {
            strcpy(predicciones[numPredicciones].nombre_zona, zonas[i].nombre);
            predicciones[numPredicciones].co2_pred = sum_co2 / peso_total;
            predicciones[numPredicciones].so2_pred = sum_so2 / peso_total;
            predicciones[numPredicciones].no2_pred = sum_no2 / peso_total;
            predicciones[numPredicciones].pm25_pred = sum_pm25 / peso_total;
            
            int excede_count = 0;
            if(predicciones[numPredicciones].co2_pred > LIMITE_CO2) excede_count++;
            if(predicciones[numPredicciones].so2_pred > LIMITE_SO2) excede_count++;
            if(predicciones[numPredicciones].no2_pred > LIMITE_NO2) excede_count++;
            if(predicciones[numPredicciones].pm25_pred > LIMITE_PM25) excede_count++;
            
            predicciones[numPredicciones].nivel_alerta = excede_count;
            strcpy(predicciones[numPredicciones].fecha_prediccion, "24 horas");
            
            numPredicciones++;
        }
    }
    
    printf("\n--- Resultados de Prediccion ---\n\n");
    for(i = 0; i < numPredicciones; i++) {
        printf("[%s]\n", predicciones[i].nombre_zona);
        printf("------------------------------------------------------------------------\n");
        printf("  CO2 predicho:   %.2f ppm   [Limite: %.0f] %s\n",
               predicciones[i].co2_pred, LIMITE_CO2,
               predicciones[i].co2_pred > LIMITE_CO2 ? "[ALERTA]" : "[OK]");
        printf("  SO2 predicho:   %.2f ug/m3 [Limite: %.0f] %s\n",
               predicciones[i].so2_pred, LIMITE_SO2,
               predicciones[i].so2_pred > LIMITE_SO2 ? "[ALERTA]" : "[OK]");
        printf("  NO2 predicho:   %.2f ug/m3 [Limite: %.0f] %s\n",
               predicciones[i].no2_pred, LIMITE_NO2,
               predicciones[i].no2_pred > LIMITE_NO2 ? "[ALERTA]" : "[OK]");
        printf("  PM2.5 predicho: %.2f ug/m3 [Limite: %.0f] %s\n",
               predicciones[i].pm25_pred, LIMITE_PM25,
               predicciones[i].pm25_pred > LIMITE_PM25 ? "[ALERTA]" : "[OK]");
        printf("  Nivel de alerta: %d contaminante(s) excede limite\n",
               predicciones[i].nivel_alerta);
        printf("  Prediccion para: %s\n\n", predicciones[i].fecha_prediccion);
    }
    printf("========================================================================\n");
}

void generarAlertasPreventivas() {
    DatosZona zonas[MAX_ZONAS];
    FILE *archivo;
    int numZonas = 0;
    int i, alertas_count = 0;
    
    printf("\n--- Generar Alertas Preventivas ---\n");
    
    archivo = fopen("zonas.dat", "rb");
    if(archivo == NULL) {
        printf("No hay zonas registradas.\n");
        return;
    }
    
    fread(&numZonas, sizeof(int), 1, archivo);
    for(i = 0; i < numZonas; i++) {
        fread(&zonas[i], sizeof(DatosZona), 1, archivo);
    }
    fclose(archivo);
    
    printf("\n========================================================================\n");
    printf("Evaluando niveles de contaminacion para generar alertas...\n");
    printf("========================================================================\n\n");
    
    for(i = 0; i < numZonas; i++) {
        int alerta = 0;
        printf("[%s]\n", zonas[i].nombre);
        printf("------------------------------------------------------------------------\n");
        
        if(zonas[i].co2 > LIMITE_CO2 * 0.8) {
            printf("  [!] ALERTA CO2: %.2f ppm (Limite: %.0f)\n", zonas[i].co2, LIMITE_CO2);
            alerta = 1;
            alertas_count++;
        }
        if(zonas[i].so2 > LIMITE_SO2 * 0.8) {
            printf("  [!] ALERTA SO2: %.2f ug/m3 (Limite: %.0f)\n", zonas[i].so2, LIMITE_SO2);
            alerta = 1;
            alertas_count++;
        }
        if(zonas[i].no2 > LIMITE_NO2 * 0.8) {
            printf("  [!] ALERTA NO2: %.2f ug/m3 (Limite: %.0f)\n", zonas[i].no2, LIMITE_NO2);
            alerta = 1;
            alertas_count++;
        }
        if(zonas[i].pm25 > LIMITE_PM25 * 0.8) {
            printf("  [!] ALERTA PM2.5: %.2f ug/m3 (Limite: %.0f)\n", zonas[i].pm25, LIMITE_PM25);
            alerta = 1;
            alertas_count++;
        }
        
        if(!alerta) {
            printf("  [OK] Niveles normales\n");
        }
        printf("\n");
    }
    
    printf("========================================================================\n");
    printf("Total de alertas generadas: %d\n", alertas_count);
    printf("========================================================================\n");
}

void generarRecomendaciones() {
    DatosZona zonas[MAX_ZONAS];
    FILE *archivo;
    int numZonas = 0;
    int i;
    
    printf("\n--- Generar Recomendaciones de Mitigacion ---\n");
    
    archivo = fopen("zonas.dat", "rb");
    if(archivo == NULL) {
        printf("No hay zonas registradas.\n");
        return;
    }
    
    fread(&numZonas, sizeof(int), 1, archivo);
    for(i = 0; i < numZonas; i++) {
        fread(&zonas[i], sizeof(DatosZona), 1, archivo);
    }
    fclose(archivo);
    
    printf("\n========================================================================\n");
    
    for(i = 0; i < numZonas; i++) {
        printf("\n[%s]\n", zonas[i].nombre);
        printf("------------------------------------------------------------------------\n");
        
        int hay_recomendaciones = 0;
        
        if(zonas[i].co2 > LIMITE_CO2) {
            printf("  -> Aumentar areas verdes y arboles (capturan CO2)\n");
            printf("  -> Reducir trafico vehicular en horas pico\n");
            printf("  -> Promover uso de transporte publico\n");
            hay_recomendaciones = 1;
        }
        
        if(zonas[i].so2 > LIMITE_SO2) {
            printf("  -> Controlar emisiones de plantas industriales\n");
            printf("  -> Reducir combustibles fosiles\n");
            printf("  -> Implementar filtros en chimeneas\n");
            hay_recomendaciones = 1;
        }
        
        if(zonas[i].no2 > LIMITE_NO2) {
            printf("  -> Regular emision de gases de escape\n");
            printf("  -> Usar combustibles limpios\n");
            printf("  -> Mejorar mantenimiento de vehiculos\n");
            hay_recomendaciones = 1;
        }
        
        if(zonas[i].pm25 > LIMITE_PM25) {
            printf("  -> Riego de areas para controlar polvo\n");
            printf("  -> Limitar actividades de construccion\n");
            printf("  -> Mantener pavimentacion de calles\n");
            hay_recomendaciones = 1;
        }
        
        if(!hay_recomendaciones) {
            printf("  -> Mantener las buenas practicas ambientales\n");
            printf("  -> Continuar monitoreo regular\n");
        }
        printf("\n");
    }
    printf("========================================================================\n");
}

void compararZonas() {
    DatosZona zonas[MAX_ZONAS];
    FILE *archivo;
    int numZonas = 0;
    int i, zona1, zona2;
    
    printf("\n--- Comparar Niveles entre Zonas ---\n");
    
    archivo = fopen("zonas.dat", "rb");
    if(archivo == NULL) {
        printf("No hay zonas registradas.\n");
        return;
    }
    
    fread(&numZonas, sizeof(int), 1, archivo);
    if(numZonas < 2) {
        printf("Se necesita al menos 2 zonas para comparar.\n");
        fclose(archivo);
        return;
    }
    
    for(i = 0; i < numZonas; i++) {
        fread(&zonas[i], sizeof(DatosZona), 1, archivo);
    }
    fclose(archivo);
    
    mostrarZonasRegistradas();
    
    zona1 = validarEntero("\nSeleccione primera zona (ID): ", 1, numZonas);
    zona2 = validarEntero("Seleccione segunda zona (ID): ", 1, numZonas);
    
    if(zona1 == zona2) {
        printf("Error: Debe seleccionar zonas diferentes.\n");
        return;
    }
    
    printf("\n========================================================================\n");
    printf("COMPARACION: %s vs %s\n", zonas[zona1-1].nombre, zonas[zona2-1].nombre);
    printf("========================================================================\n\n");
    
    printf("%-20s | %-15s | %-15s | Diferencia\n", "Parametro", zonas[zona1-1].nombre, zonas[zona2-1].nombre);
    printf("------------------------------------------------------------------------\n");
    
    printf("CO2 (ppm)    | %.2f        | %.2f        | %.2f\n",
           zonas[zona1-1].co2, zonas[zona2-1].co2,
           zonas[zona1-1].co2 - zonas[zona2-1].co2);
    
    printf("SO2 (ug/m3)  | %.2f        | %.2f        | %.2f\n",
           zonas[zona1-1].so2, zonas[zona2-1].so2,
           zonas[zona1-1].so2 - zonas[zona2-1].so2);
    
    printf("NO2 (ug/m3)  | %.2f        | %.2f        | %.2f\n",
           zonas[zona1-1].no2, zonas[zona2-1].no2,
           zonas[zona1-1].no2 - zonas[zona2-1].no2);
    
    printf("PM2.5(ug/m3) | %.2f        | %.2f        | %.2f\n",
           zonas[zona1-1].pm25, zonas[zona2-1].pm25,
           zonas[zona1-1].pm25 - zonas[zona2-1].pm25);
    
    printf("\nZona mas contaminada: %s\n",
           (zonas[zona1-1].co2 + zonas[zona1-1].so2 + zonas[zona1-1].no2 + zonas[zona1-1].pm25) >
           (zonas[zona2-1].co2 + zonas[zona2-1].so2 + zonas[zona2-1].no2 + zonas[zona2-1].pm25) ?
           zonas[zona1-1].nombre : zonas[zona2-1].nombre);
    
    printf("========================================================================\n");
}

void verHistorialCompleto() {
    FILE *archivo;
    RegistroDiario registro;
    int contador = 0;
    
    printf("\n--- Historial Completo de Registros ---\n");
    
    archivo = fopen("historial.dat", "rb");
    if(archivo == NULL) {
        printf("No hay historial disponible.\n");
        return;
    }
    
    printf("\n========================================================================\n");
    
    while(fread(&registro, sizeof(RegistroDiario), 1, archivo) == 1) {
        contador++;
        printf("\n[Registro #%d]\n", contador);
        printf("Fecha: %s | Zona: %s\n", registro.fecha, registro.zona.nombre);
        printf("------------------------------------------------------------------------\n");
        printf("  CO2:   %.2f ppm\n", registro.zona.co2);
        printf("  SO2:   %.2f ug/m3\n", registro.zona.so2);
        printf("  NO2:   %.2f ug/m3\n", registro.zona.no2);
        printf("  PM2.5: %.2f ug/m3\n", registro.zona.pm25);
        printf("  Temperatura: %.2f C | Viento: %.2f km/h | Humedad: %.2f%%\n",
               registro.zona.temperatura, registro.zona.velocidad_viento, registro.zona.humedad);
    }
    
    fclose(archivo);
    printf("\n========================================================================\n");
    printf("Total de registros: %d\n", contador);
    printf("========================================================================\n");
}

void exportarReporte() {
    FILE *archivo_salida, *archivo_entrada;
    DatosZona zonas[MAX_ZONAS];
    RegistroDiario registro;
    int numZonas = 0, i;
    
    printf("\n--- Exportar Reporte Completo ---\n");
    
    archivo_entrada = fopen("zonas.dat", "rb");
    if(archivo_entrada == NULL) {
        printf("No hay datos para exportar.\n");
        return;
    }
    
    fread(&numZonas, sizeof(int), 1, archivo_entrada);
    for(i = 0; i < numZonas; i++) {
        fread(&zonas[i], sizeof(DatosZona), 1, archivo_entrada);
    }
    fclose(archivo_entrada);
    
    archivo_salida = fopen("reporte_completo.txt", "w");
    if(archivo_salida == NULL) {
        printf("Error al crear el archivo de reporte.\n");
        return;
    }
    
    fprintf(archivo_salida, "========================================================================\n");
    fprintf(archivo_salida, "   REPORTE COMPLETO - SISTEMA DE GESTION DE CONTAMINACION DEL AIRE\n");
    fprintf(archivo_salida, "========================================================================\n\n");
    
    fprintf(archivo_salida, "--- ZONAS REGISTRADAS ---\n\n");
    for(i = 0; i < numZonas; i++) {
        fprintf(archivo_salida, "Zona ID %d: %s\n", zonas[i].id, zonas[i].nombre);
        fprintf(archivo_salida, "  CO2: %.2f ppm | SO2: %.2f ug/m3\n", zonas[i].co2, zonas[i].so2);
        fprintf(archivo_salida, "  NO2: %.2f ug/m3 | PM2.5: %.2f ug/m3\n\n", zonas[i].no2, zonas[i].pm25);
    }
    
    fprintf(archivo_salida, "\n--- HISTORIAL DE REGISTROS ---\n\n");
    archivo_entrada = fopen("historial.dat", "rb");
    if(archivo_entrada != NULL) {
        while(fread(&registro, sizeof(RegistroDiario), 1, archivo_entrada) == 1) {
            fprintf(archivo_salida, "Fecha: %s | Zona: %s\n", registro.fecha, registro.zona.nombre);
            fprintf(archivo_salida, "CO2: %.2f | SO2: %.2f | NO2: %.2f | PM2.5: %.2f\n\n",
                   registro.zona.co2, registro.zona.so2, registro.zona.no2, registro.zona.pm25);
        }
        fclose(archivo_entrada);
    }
    
    fprintf(archivo_salida, "\n========================================================================\n");
    fprintf(archivo_salida, "Limites OMS: CO2=%.0f | SO2=%.0f | NO2=%.0f | PM2.5=%.0f\n",
           LIMITE_CO2, LIMITE_SO2, LIMITE_NO2, LIMITE_PM25);
    fprintf(archivo_salida, "========================================================================\n");
    
    fclose(archivo_salida);
    printf("\n*** Reporte exportado exitosamente en 'reporte_completo.txt' ***\n");
}