#include<stdio.h>

typedef struct 
{
    int id;
    float precio;
    char nombre[50];
}producto_t;


typedef struct 
{
    producto_t barato;
    producto_t caro;
}estadistica_t;


void inicializar_producto(producto_t*p);


int main(){

    FILE* datos_parseados;
    FILE* datos_estadisticos;

    datos_parseados = fopen("datos_parseados.csv", "r");
    datos_estadisticos = fopen("datos_estadisticos.txt", "w");

    if(datos_estadisticos == NULL || datos_parseados == NULL){
        printf("Error al abrir el archivo de datos estadisticos\n");
    } else {
        printf("Archivo de datos estadisticos abierto correctamente\n");

        estadistica_t estadistica;
        producto_t aux;

        inicializar_producto(&estadistica.barato);
        inicializar_producto(&estadistica.caro);
        inicializar_producto(&aux);

        char string [100];
        fgets(string,sizeof(string),datos_parseados);
        sscanf(string,"%d,%f,%[^/]s",&aux.id,&aux.precio,&aux.nombre);

        printf("%d, %f, %s",aux.id, aux.precio, aux.nombre );


        fclose(datos_parseados);
        fclose(datos_estadisticos);
    }

}

void inicializar_producto(producto_t*p){
    p->id=0;
    p->precio=0;
}