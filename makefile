# Nome do projeto
PROJETO = ted

# Arquivos fonte
FONTES = main.c radialtree.c geo-qry.c svg.c Bibliotecas/utilities.c Bibliotecas/listadupla.c Bibliotecas/arqsvg.c Bibliotecas/learquivo.c

# Pasta de saída
OUTPUT = output/
LOGS = logs/

# Cria a pasta de saída se ela não existir
$(shell mkdir -p $(OUTPUT) $(OUTPUT)Bibliotecas)
$(shell mkdir -p $(LOGS))

# Arquivos objeto na pasta de saída
OBJETOS = $(addprefix $(OUTPUT), $(FONTES:.c=.o))

# Compilador e opções de compilação
CC = gcc
CFLAGS = -Wall -Wextra -pedantic-errors -fstack-protector-all -Werror=implicit-function-declaration -g

# Regra padrão
all: $(OUTPUT)$(PROJETO)

# Regra para compilar o projeto
$(OUTPUT)$(PROJETO): $(OBJETOS)
	$(CC) $(OBJETOS) -o $@ -lm

# Regra para criar os arquivos objeto
$(OUTPUT)%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@ -MMD -MP

# Inclui as dependências dos arquivos objeto
-include $(OBJETOS:.o=.d)

# Regra para limpar os arquivos objeto e o executável
clean:
	rm -rf $(OUTPUT) $(PROJETO)

# Regra para executar o programa com o Valgrind
run:
	cd $(OUTPUT) && valgrind --leak-check=full --show-leak-kinds=all ./$(PROJETO)

.PHONY: all run clean