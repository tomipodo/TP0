#include "client.h"


int main(void) {
   /*---------------------------------------------------PARTE 2-------------------------------------------------------------*/


   int conexion;
   char *ip;
   char *puerto;
   char *valor;


   t_log *logger;
   t_config *config;


   /* ---------------- LOGGING ---------------- */


   logger = iniciar_logger();


   /* ---------------- ARCHIVOS DE CONFIGURACION ---------------- */


   config = iniciar_config();


   valor = config_get_string_value(config, "CLAVE");
   ip = config_get_string_value(config, "IP");
   puerto = config_get_string_value(config, "PUERTO");


   //log_info(logger, "%s", valor);


   // ADVERTENCIA: Antes de continuar, tenemos que asegurarnos que el servidor esté corriendo para poder conectarnos a él
   // Creamos una conexión hacia el servidor
   conexion = crear_conexion(ip, puerto);


   // Enviamos al servidor el valor de CLAVE como mensaje
   enviar_mensaje(valor, conexion);


   // Armamos y enviamos el paquete
   paquete(conexion, logger);


   terminar_programa(conexion, logger, config);


   /*---------------------------------------------------PARTE 5-------------------------------------------------------------*/
   // Proximamente
}


t_log* iniciar_logger(void) {
   t_log *nuevo_logger;
   if ((nuevo_logger = log_create("tp0.log", "Log: ", 1, LOG_LEVEL_INFO))
           == NULL) {
       printf("No pude crear el logger\n");
       exit(1);
   }


   return nuevo_logger;
}


t_config* iniciar_config(void) {
   t_config *nuevo_config;
   char* ruta = getcwd(NULL, 0);
   string_append(&ruta, "/cliente.config");
   printf("Ruta: %s", ruta);
   if ((nuevo_config = config_create(ruta)) == NULL) {
       printf("No pude crear el config");
       exit(2);
   }


   return nuevo_config;
}


void leer_consola(t_log *logger) {
   char *leido;


   while (1) {
       leido = readline("> ");
       if (!strcmp(leido, "")) {
           break;
       }
       log_info(logger, "%s", leido);
       free(leido);
   }
   free(leido);
}


void paquete(int conexion, t_log *logger) {
   t_paquete *paquete = crear_paquete();
   char *leido;


   while (1) {
       leido = readline("> ");
       if (!strcmp(leido, "")) {
           break;
       }
       agregar_a_paquete(paquete, leido, strlen(leido)+1);
       log_info(logger, "%s", leido);
       free(leido);
   }
   enviar_paquete(paquete, conexion);
   free(leido);
   eliminar_paquete(paquete);
}


void terminar_programa(int conexion, t_log *logger, t_config *config) {
   log_destroy(logger);
   config_destroy(config);
   liberar_conexion(conexion);
}
