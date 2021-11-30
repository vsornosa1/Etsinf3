 void init_instruc_json ( 
	dword PC,
	ciclo_t orden
	 );
 void muestra_fase_json ( 
	char *fase,
	ciclo_t orden,
	boolean exception
	 );
 void muestra_datos_json ( 
	region_t r,
	dword from,
	dword to,
	mem_status_t status
	 );
 void imprime_inicio_json (void);
 void inicio_ciclo_json ( 
	ciclo_t ciclo
	 );
 void fin_ciclo_json ( 
	ciclo_t ciclo
	 );
 void imprime_final_json (void);
 void imprime_estado_json (void);
