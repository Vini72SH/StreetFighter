# Nome do executável
TARGET = StreetFighter

# Compilador
CC = gcc

# Flags do compilador
CFLAGS = -Wall -g
 
# Flags para a biblioteca Allegro
ALLEGRO_FLAGS = $(shell pkg-config --cflags --libs allegro-5 allegro_image-5 allegro_font-5 allegro_ttf-5 allegro_primitives-5)

# Diretório de objetos
OBJDIR = obj

# Arquivos fonte
SRCS = StreetFighter.c render.c game.c character.c joystick.c

# Arquivos objeto (no diretório de objetos)
OBJS = $(SRCS:%.c=$(OBJDIR)/%.o)

# Regra padrão
all: $(OBJDIR) $(TARGET)

# Regra para criar o diretório de objetos
$(OBJDIR):
	mkdir -p $(OBJDIR)

# Regra para criar o executável
$(TARGET): $(OBJS)
	@$(CC) -o $@ $^ $(ALLEGRO_FLAGS)

# Regra para criar os arquivos objeto
$(OBJDIR)/%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Limpeza
clean:
	rm -f $(OBJS)
	rm -rf $(OBJDIR)
purge:
	rm -f $(TARGET) $(OBJS)
	rm -rf $(OBJDIR)

.PHONY: all clean purge
