 void jwOpen ( 
	FILE *stream,
	enum jwNodeType rootType,
	int isPretty
	 );
 int jwClose (void);
 int jwEnd (void);
 int jwErrorPos (void);
 void jwObj_raw ( 
	char *key,
	char *rawtext
	 );
 void jwObj_string ( 
	char *key,
	char *value
	 );
 void jwObj_int ( 
	char *key,
	int value
	 );
 void jwObj_double ( 
	char *key,
	double value
	 );
 void jwObj_bool ( 
	char *key,
	int oneOrZero
	 );
 void jwObj_null ( 
	char *key
	 );
 void jwObj_object ( 
	char *key
	 );
 void jwObj_array ( 
	char *key
	 );
 void jwArr_raw ( 
	char *rawtext
	 );
 void jwArr_string ( 
	char *value
	 );
 void jwArr_int ( 
	int value
	 );
 void jwArr_double ( 
	double value
	 );
 void jwArr_bool ( 
	int oneOrZero
	 );
 void jwArr_null (void);
 void jwArr_object (void);
 void jwArr_array (void);
 char *jwErrorToString ( 
	int err
	 );
 void jwPretty (void);
 void jwPush ( 
	enum jwNodeType nodeType
	 );
 enum jwNodeType jwPop (void);
 void jwPutch ( 
	char c
	 );
 void jwPutstr ( 
	char *str
	 );
 void jwPutraw ( 
	char *str
	 );
 int _jwObj ( 
	char *key
	 );
 int _jwArr (void);
 void modp_itoa10 ( 
	int32_t value,
	char *str
	 );
 void modp_dtoa2 ( 
	double value,
	char *str,
	int prec
	 );
