 simbolo_t *nuevo_simbolo ( 
	char *nombre,
	dword direccion,
	boolean global
	 );
 void ts_inserta_etiqueta ( 
	tabla_simbolos_t *ts,
	char *nombre,
	dword offset,
	region_t region
	 );
 etiqueta_t *ts_busca_etiqueta ( 
	tabla_simbolos_t *ts,
	char *nombre
	 );
 void ts_inserta_referencia ( 
	tabla_simbolos_t *ts,
	char *nombre,
	region_t region,
	dword offset,
	parte_direccion_t parte_dir,
	tipo_region_t tipo_region
	 );
 void ts_resuelve_referencias_locales ( 
	mapa_memoria_t *mm_global,
	tabla_simbolos_t *ts,
	mapa_memoria_t *mm
	 );
 simbolo_t *ts_inserta_simbolo ( 
	tabla_simbolos_t *ts,
	char *nombre
	 );
 void ts_resuelve_simbolos ( 
	tabla_simbolos_t *ts,
	mapa_memoria_t *mm
	 );
 void ts_mueve_simbolos ( 
	char *fichero_objeto,
	simbolo_t **p_sims_global,
	simbolo_t **sims_local
	 );
 void ts_ordena_simbolos ( 
	simbolo_t **sims
	 );
 simbolo_t *ts_busca_simbolo ( 
	simbolo_t *sims,
	char *nombre
	 );
 void ts_inserta_simbolos_locales ( 
	tabla_simbolos_t *ts,
	mapa_memoria_t *mm
	 );
 simbolo_t *ts_busca_direccion ( 
	simbolo_t *sims,
	dword direccion,
	int *p_index
	 );
 void ts_simbolo_direccion ( 
	simbolo_t *sims,
	dword direccion,
	char *etiqueta,
	boolean f_etiqueta
	 );
 void ts_resuelve_referencias_globales ( 
	simbolo_t *sims_globales,
	tabla_simbolos_t *ts,
	mapa_memoria_t *mm
	 );
 void ts_libera_etiquetas ( 
	etiqueta_t **l_etiq
	 );
 void ts_libera_simbolos ( 
	simbolo_t **l_sim
	 );
