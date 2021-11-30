 void inicializa_prediccion (void);
 boolean obtener_prediccion ( 
	dword PC,
	ciclo_t orden,
	boolean *prediccion,
	dword *destino,
	prediccion_t *pred_aux
	 );
 void actualizar_prediccion ( 
	int rob_entry,
	dword PC,
	ciclo_t orden,
	boolean condicion,
	dword destino
	 );
