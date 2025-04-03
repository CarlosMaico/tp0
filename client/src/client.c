#include "client.h"

int main(void)
{
	/*---------------------------------------------------PARTE 2-------------------------------------------------------------*/

	int conexion;
	char* ip;
	char* puerto;
	char* valor;

	t_log* logger;
	t_config* config;

	/* ---------------- LOGGING ---------------- */

	logger = iniciar_logger();

	// Usando el logger creado previamente
	// Escribi: "Hola! Soy un log"
	log_info(logger, "Hola! Soy un log");  //me muestra por consola y me lo guarda en un .log


	/* ---------------- ARCHIVOS DE CONFIGURACION ---------------- */

	config = iniciar_config();

	// Usando el config creado previamente, leemos los valores del config y los 
	// dejamos en las variables 'ip', 'puerto' y 'valor'
	ip = config_get_string_value(config,"IP");   //Todo esto para mostrar los valores en el .log
	puerto = config_get_string_value(config, "PUERTO"); //La config gucniona como un diccionario
	valor = config_get_string_value(config, "CLAVE");

	// Loggeamos el valor de config
	log_info(logger, "VALOR leido de la config: %s", valor);

	/* ---------------- LEER DE CONSOLA ---------------- */

	leer_consola(logger);

	/*---------------------------------------------------PARTE 3-------------------------------------------------------------*/

	// ADVERTENCIA: Antes de continuar, tenemos que asegurarnos que el servidor esté corriendo para poder conectarnos a él

	// Creamos una conexión hacia el servidor
	conexion = crear_conexion(ip, puerto);

	// Enviamos al servidor el valor de CLAVE como mensaje
	enviar_mensaje(valor, conexion);
	// Armamos y enviamos el paquete
	paquete(conexion);

	terminar_programa(conexion, logger, config);

	/*---------------------------------------------------PARTE 5-------------------------------------------------------------*/
	// Proximamente
	printf("\n CLIENTE CERRRADO !!!");
}

t_log* iniciar_logger(void)
{
	t_log* nuevo_logger = 	log_create ("cliente.log", "CLIENTE_LOGGER", 1, LOG_LEVEL_INFO);

	return nuevo_logger;
}

t_config* iniciar_config(void)
{
	t_config* nuevo_config = config_create("/home/utnso/tp0/client/cliente.config");
	if(nuevo_config == NULL){
		perror("Error al intenar cargar el config");
		exit(EXIT_FAILURE);
	}

	return nuevo_config;
}

void leer_consola(t_log* logger)
{
	char* leido;

	// La primera te la dejo de yapa
	leido = readline("> ");
	log_info(logger, ">> %s", leido); //Par poder escribir algo en la temrinal y que finalize

	// El resto, las vamos leyendo y logueando hasta recibir un string vacío
	while (strcmp(leido, "") != 0){  //COmparo si lo que esribo es isinot de vacio
		free(leido);   //free primero siempre , si lo libero al final no posee informacion, por eso debo liberarlo al princiopio, algoq ue no tiene informacion es diferenet a un linea vacia
		leido = readline("> ");
		log_info(logger, ">> %s", leido);
		
	}
	


	// ¡No te olvides de liberar las lineas antes de regresar!
	free(leido);
}

void paquete(int conexion)
{
	// Ahora toca lo divertido!
	char* leido = NULL;
	t_paquete* paquete = crear_paquete();

	// Leemos y esta vez agregamos las lineas al paquete
	
	leido = readline("> ");
	while (strcmp(leido, "") != 0){  //COmparo si lo que esribo es es distinto de vacio
		agregar_a_paquete(paquete, leido, strlen(leido + 1)); // +1 del caracter fin de string
		free(leido);   //free primero siempre , si lo libero al final no posee informacion, por eso debo liberarlo al princiopio, algoq ue no tiene informacion es diferenet a un linea vacia
		leido = readline("> ");		
	}	

	// ¡No te olvides de liberar las líneas y el paquete antes de regresar!
	free(leido);
	//Enviar apquete
	enviar_paquete(paquete, conexion);
	//Eliminar paquete
	eliminar_paquete(paquete);
}

void terminar_programa(int conexion, t_log* logger, t_config* config)
{
	/* Y por ultimo, hay que liberar lo que utilizamos (conexion, log y config) 
	  con las funciones de las commons y del TP mencionadas en el enunciado */
	log_destroy(logger);  // Finalizo el logg lo destruyo
	config_destroy(config);
	liberar_conexion(conexion);
}
