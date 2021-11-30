 valor_t operacion_ALU_FP ( 
	unsigned char codop,
	valor_t in1,
	valor_t in2
	 );
 dword comparacion_ALU_FP ( 
	unsigned char codop,
	valor_t in1,
	valor_t in2
	 );
 dword mux_FP_LSsup ( 
	dword ra,
	dword mem,
	dword wb
	 );
 valor_t mux_FP_LSinf ( 
	valor_t reg_val,
	valor_t wb
	 );
 valor_t mux_FP_LS2inf ( 
	valor_t reg_val,
	valor_t wb
	 );
 valor_t mux_FP_ADDinf ( 
	valor_t reg_val,
	valor_t wb
	 );
 valor_t mux_FP_ADDsup ( 
	valor_t reg_val,
	valor_t wb
	 );
 valor_t mux_FP_CMPsup ( 
	valor_t reg_val,
	valor_t wb
	 );
 valor_t mux_FP_CMPinf ( 
	valor_t reg_val,
	valor_t wb
	 );
 valor_t mux_FP_MULsup ( 
	valor_t reg_val,
	valor_t wb
	 );
 valor_t mux_FP_MULinf ( 
	valor_t reg_val,
	valor_t wb
	 );
 FP_WB_t mux_FP_WB ( 
	FP_WB_t fp_ls,
	FP_WB_t fp_add,
	FP_WB_t fp_mul,
	FP_WB_t id_wb
	 );
 void fase_LS ( 
	short i
	 );
 void fase_A ( 
	short i
	 );
 void fase_M ( 
	short i
	 );
 void fase_C ( 
	short i
	 );
 void detectar_riesgos_RAW_FP (void);
 void detectar_riesgos_WAW_FP (void);
 void detectar_riesgos_WB_FP (void);
