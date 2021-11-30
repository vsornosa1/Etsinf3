 size_t tam_dato ( 
	tipo_t tipo
	 );
 dword mem_direccion_alineada ( 
	dword direccion,
	int size
	 );
 void mem_rango ( 
	dword offset,
	tipo_t tipo,
	dword *m_from,
	dword *m_to
	 );
 void mem_etiqueta ( 
	mapa_memoria_t *mm,
	region_t r,
	dword offset,
	tipo_t tipo,
	boolean activar_dirty
	 );
 void inicializa_region_datos ( 
	mapa_memoria_t *mm,
	region_t r,
	size_t tam
	 );
 void destruye_region_datos ( 
	mapa_memoria_t *mm,
	region_t r
	 );
 void inicializa_memoria ( 
	mapa_memoria_t *mm
	 );
 void destruye_memoria ( 
	mapa_memoria_t *mm
	 );
 region_t dir_a_region ( 
	mapa_memoria_t *mm,
	dword dir
	 );
 valor_t mem_lee_region_datos ( 
	mapa_memoria_t *mm,
	region_t r,
	dword offset,
	tipo_t tipo,
	boolean *dirty
	 );
 valor_t _lee_mem_datos ( 
	mapa_memoria_t *mm,
	dword direccion,
	tipo_t tipo_dato,
	boolean *dirty,
	char *file,
	int line
	 );
 void mem_escribe_region_datos ( 
	mapa_memoria_t *mm,
	region_t r,
	dword offset,
	valor_t valor,
	tipo_t tipo_dato,
	boolean activar_dirty
	 );
 void escribe_mem_datos ( 
	mapa_memoria_t *mm,
	dword direccion,
	valor_t valor,
	tipo_t tipo_dato,
	boolean activar_dirty
	 );
 instruccion_t mem_lee_region_instruc ( 
	mapa_memoria_t *mm,
	region_t r,
	dword offset
	 );
 instruccion_t _lee_mem_instruc ( 
	mapa_memoria_t *mm,
	dword direccion,
	char *file,
	int line
	 );
 void mem_escribe_region_instruc ( 
	mapa_memoria_t *mm,
	region_t r,
	dword offset,
	instruccion_t instruccion
	 );
 void escribe_mem_instruc ( 
	mapa_memoria_t *mm,
	dword direccion,
	instruccion_t instruccion
	 );
