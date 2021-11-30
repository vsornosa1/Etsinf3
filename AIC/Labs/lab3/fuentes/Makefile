PRAC=3

CFLAGS		= -I. -O2 -fno-strength-reduce -fPIC -Wall -Wno-unused -Wno-unused-result -Wno-format -Wno-switch
LFLAGS		= -lm
CC		= gcc

TARGET=mips-m
#OBJECTS=control.o enlazador.o simbolos.o log.o memoria.o mips_fp.o mips_syscall.o presentacion-txt.o datapath.o instrucciones.o main.o mips.o mips_int.o presentacion-html.o registros.o input.lex.o input.yacc.o mips_int_alum.o mips_alum.o
OBJECTS=control.o enlazador.o simbolos.o log.o memoria.o mips_fp.o mips_syscall.o presentacion-txt.o datapath.o instrucciones.o main.o mips.o mips_int.o presentacion-html.o registros.o input.lex.o input.yacc.o


all: $(TARGET)

$(TARGET): $(OBJECTS)
	@echo " [LINK] Building exec file '${TARGET}'"
	@$(CC) $(OBJECTS) -o $(TARGET) $(LFLAGS)

clean:
	@rm -vf $(OBJECTS) *.output

distclean: clean
	@rm -vf $(TARGET)

%.o: %.c
	@echo " [GCC] Building object file '$@' using $<"
	@$(CC) -c $(CFLAGS) -o $@ -D PRAC=$(PRAC) $<

%.s: %.c
	@echo " [GCC] Building asm file '$@' using $<"
	@$(CC) -S $(CFLAGS) -mtune=intel -o $@ -D PRAC=$(PRAC) $<

%.o: %.s
	@echo " [GCC] Building object file '$@' using $<"
	@$(CC) -c $(CFLAGS) -o $@ -D PRAC=$(PRAC) $<

%.lex.c: %.lex.l
	@echo " [FLEX] Building scanner file '$@' using $<"
	@lex -t $*.lex.l > $*.lex.c

input.lex.o: input.yacc.h

%.yacc.h: %.yacc.y
	@echo " [BISON] Building parser file '$@' using $<"
	@bison -o $*.yacc.c -d $*.yacc.y

%.yacc.c: %.yacc.y
	@echo " [BISON] Building parser file '$@' using $<"
	@bison -v -o $*.yacc.c $*.yacc.y 

