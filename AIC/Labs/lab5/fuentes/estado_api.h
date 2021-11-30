 table_state_t *ht_create ( 
	int size
	 );
 int ht_hash ( 
	table_state_t *table_state,
	char *key
	 );
 table_value_t *ht_newpair ( 
	char *key,
	char *text,
	char *class
	 );
 void ht_set ( 
	table_state_t *table_state,
	char *key,
	char *text,
	char *class
	 );
 table_value_t *ht_get ( 
	table_state_t *table_state,
	char *key
	 );
