PROJ_NAME=ted

ALUNO= muriloa
LIBS=-lm
OBJETOS= main.o radialtree.o geo.o qry.o svg.o dot.o Bibliotecas/path.o Bibliotecas/utilities.o Bibliotecas/listadupla.o Bibliotecas/arqsvg.o Bibliotecas/learquivo.o Bibliotecas/geradores.o

# compilador
CC=gcc

# Flags
CFLAGS= -ggdb -O0 -std=c99 -fstack-protector-all -Werror=implicit-function-declaration -g
LDFLAGS= -O0

$(PROJ_NAME): $(OBJETOS)
	$(CC) -o $(PROJ_NAME) $(LDFLAGS) $(OBJETOS) $(LIBS)

%.o : %.c
	$(CC) -c $(CFLAGS) $< -o $@

Bibliotecas/%.o : Bibliotecas/%.c
	$(CC) -c $(CFLAGS) $< -o $@

clean:
	rm -f *.o $(PROJ_NAME)
	rm -f Bibliotecas/*.o $(PROJ_NAME)

pack: $(PROJ_NAME)
	rm -f ../$(ALUNO).zip
	echo $(ALUNO)
	date >> .entrega
	cd ..; zip $(ALUNO).zip -r src/*.c src/*.h src/Bibliotecas/*.c src/Bibliotecas/*.h src/Makefile LEIA-ME.txt .entrega