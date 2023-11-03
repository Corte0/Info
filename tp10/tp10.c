#include "C:\msys64\mingw64\include\curl\curl.h"
#include <stdio.h>
#define CURL_STATICLIB
#include "cJSON.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

typedef struct string_buffer_s {
  char  *ptr;
  size_t len;
} string_buffer_t;

static void   string_buffer_initialize(string_buffer_t *);
static void   string_buffer_finish(string_buffer_t *);
static size_t string_buffer_callback(void *, size_t, size_t, void *);
static size_t header_callback(char *, size_t, size_t, void *);
static size_t write_callback(void *, size_t, size_t, void *);

int main(int argc, char *argv[]) {
     CURL * curl;
  CURLcode res;
  string_buffer_t strbuf;
  char * myurl = "https://api.open-meteo.com/v1/forecast?latitude=52.52&longitude=13.41&current=temperature_2m";
  char * myurl2 = "https://api.telegram.org/bot<token:bot>/sendMessage?chat_id=<chat_id>&text=Buenos_dias";
  string_buffer_initialize( &strbuf );

  /* Inicializar la sesión de curl */

  curl = curl_easy_init();
  if(!curl)
  {
    fprintf(stderr, "Fatal: curl_easy_init() error.\n");
    string_buffer_finish( &strbuf );
    return EXIT_FAILURE;
  }

  /* Especificar la URL a consultar */
  curl_easy_setopt(curl, CURLOPT_URL, myurl2);
  curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L );

  /* Enviar la información de respuesta a la función */
  curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback );

  /* Preparamos el callback para cuando la API responda */
  curl_easy_setopt(curl, CURLOPT_WRITEDATA, &strbuf );
  // curl_easy_setopt(curl, CURLOPT_HEADERDATA, &strbuf );

  /* get it! */
  res = curl_easy_perform(curl);
  /* check for errors */
  if( res != CURLE_OK )
  {
    fprintf( stderr, "Request failed: curl_easy_perform(): %s\n", curl_easy_strerror(res) );
    curl_easy_cleanup( curl );
    string_buffer_finish( &strbuf );
    return EXIT_FAILURE;
  }

  /*--------------------------------------------------------------------------------
   * En strbuf.ptr tenemos la cadena con la respuesta desde la API
   *
   * Tenemos que procesar estos datos. Completar desde acá
   *--------------------------------------------------------------------------------
   */
  printf("La respuesta es:\n%s\n\n", strbuf.ptr );
  printf("%lu bytes retrieved\n", (unsigned long)strbuf.len);


  /*--------------------------------------------------------------------------------
   * Hasta acá
   *--------------------------------------------------------------------------------
   * */


  /* Limpia el buffer */
  curl_easy_cleanup( curl );
  string_buffer_finish( &strbuf );
  return EXIT_SUCCESS;
}

}

void consultApi(char *apiString, string_buffer_t *callback) {
  CURL *curl_api;

  curl_api = curl_easy_init();

  if (!curl_api)
    fprintf(stderr, "Fatal: curl_easy_init() error.\n");

  /* Especificar la URL a consultar */
  curl_easy_setopt(curl_api, CURLOPT_URL, apiString);
  curl_easy_setopt(curl_api, CURLOPT_FOLLOWLOCATION, 1L);

  /* Enviar la información de respuesta del encabezado a la función */
  curl_easy_setopt(curl_api, CURLOPT_WRITEFUNCTION, write_callback);

  /* Preparamos el callback para cuando la API responda */
  curl_easy_setopt(curl_api, CURLOPT_WRITEDATA, callback);
  CURLcode res = curl_easy_perform(curl_api);

  if (res != CURLE_OK) {
    fprintf(stderr, "Request failed: curl_easy_perform(): %s\n", curl_easy_strerror(res));
    curl_easy_cleanup(curl_api);
    string_buffer_finish(callback);
  }
}

static void string_buffer_initialize(string_buffer_t *sb) {
  sb->len = 0;
  sb->ptr = malloc(sb->len + 1); /* will be grown as needed by the realloc belove */
  sb->ptr[0] = '\0';             /* no data at this point */
}

static void string_buffer_finish(string_buffer_t *sb) {
  free(sb->ptr);
  sb->len = 0;
  sb->ptr = NULL;
}

static size_t string_buffer_callback(void *buf, size_t size, size_t nmemb, void *data) {
  string_buffer_t *sb = data;
  size_t           new_len = sb->len + size * nmemb;
  sb->ptr = realloc(sb->ptr, new_len + 1);
  memcpy(sb->ptr + sb->len, buf, size * nmemb);
  sb->ptr[new_len] = '\0';
  sb->len = new_len;
  return size * nmemb;
}

static size_t header_callback(char *buf, size_t size, size_t nmemb, void *data) {
  return string_buffer_callback(buf, size, nmemb, data);
}

static size_t write_callback(void *buf, size_t size, size_t nmemb, void *data) {
  return string_buffer_callback(buf, size, nmemb, data);
}