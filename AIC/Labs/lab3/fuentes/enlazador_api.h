 void inicializa_programa ( 
	programa_t *prog,
	int num_ficheros
	 );
 void inicializa_objeto ( 
	fichero_objeto_t *obj,
	char *fichero
	 );
 void destruye_objeto ( 
	fichero_objeto_t *obj
	 );
 void destruye_objetos_programa ( 
	programa_t *prog
	 );
 void enlazador ( 
	programa_t *prog
	 );
 void cargador ( 
	programa_t *prog
	 );
 void concatena_objetos ( 
	programa_t *prog
	 );
