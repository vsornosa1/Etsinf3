 void reverse ( 
	char s[]
	 );
 void itoa ( 
	int n,
	char s[],
	int radix
	 );
 void crea_firma ( 
	char *fichero
	 );
 void abre_firma ( 
	char *fichero
	 );
 void ignora_firma (void);
 void nueva_firma (void);
 char *procesa_firma ( 
	char *campo,
	char *valor
	 );
 void cierra_firma (void);
 void color_etiqueta_memdatos ( 
	char *color,
	dword direccion
	 );
 void __imprime_instruccion ( 
	char *instruc,
	dword PC,
	char *file,
	int line
	 );
 void imprime_codigo ( 
	marca_t s,
	char *nom
	 );
 void imprime_operacion ( 
	codop_t c,
	char *nom
	 );
 void imprime_tipo_predictor ( 
	char *aux
	 );
 void valor2str ( 
	char *dst,
	valor_t dato,
	tipo_t tipo_dato
	 );
 void FIRMA_valor ( 
	char *cadena,
	int i,
	valor_t dato,
	tipo_t tipo_dato
	 );
 tipo_t obtener_tipo ( 
	codop_t codop
	 );
 void imprime_config (void);
 void imprime_mapa_memoria (void);
 void imprime_estadisticas (void);
 void imprime_inicio_null (void);
 void imprime_final_null (void);
 void inicio_ciclo_null ( 
	ciclo_t ciclo
	 );
 void fin_ciclo_null ( 
	ciclo_t ciclo
	 );
 void imprime_estado_null (void);
 void imprime_crono_null (void);
 void imprime_predictor_null (void);
 void init_instruc_null ( 
	dword PC,
	ciclo_t orden
	 );
 void muestra_fase_null ( 
	char *fase,
	ciclo_t orden,
	boolean exception
	 );
 void muestra_datos_null ( 
	region_t r,
	dword from,
	dword to,
	mem_status_t status
	 );
