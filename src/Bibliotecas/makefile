# Nome do projeto
PROJETO = teste

# Arquivos fonte
FONTES = teste.c learquivo.c

# Pasta de saída
OUTPUT = output/
LOGS = $(OUTPUT)logs/

# Cria a pasta de saída se ela não existir
$(shell mkdir -p $(OUTPUT))
$(shell mkdir -p $(LOGS))

# Arquivos objeto na pasta de saída
OBJETOS = $(addprefix $(OUTPUT), $(FONTES:.c=.o))

# Compilador e opções de compilação
CC = gcc
CFLAGS = -Wall -Wextra -pedantic-errors -g 

# Regra padrão
all: $(OUTPUT)$(PROJETO)

# Regra para compilar o projeto
$(OUTPUT)$(PROJETO): $(OBJETOS)
	$(CC) $(OBJETOS) -o $@ 

# Regra para criar os arquivos objeto
$(OUTPUT)%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@ -MMD -MP

# Inclui as dependências dos arquivos objeto
-include $(OBJETOS:.o=.d)

# Regra para limpar os arquivos objeto e o executável
clean:
	rm -rf $(OUTPUT) $(PROJETO)
	rm -rf $(LOGS)

# Regra para executar o programa com o Valgrind
run:
	cd $(OUTPUT) && valgrind --leak-check=full ./$(PROJETO)

.PHONY: all clean