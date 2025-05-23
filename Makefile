CXX = g++
# Flags para debug: -g (símbolos) e -O0 (sem otimização)
CXXFLAGS = -std=c++14 -Wall -g -O0
LDFLAGS = -g  # Também inclui símbolos no linker

# Diretórios
SRC_DIR = src
OBJ_DIR = obj
BIN_DIR = bin

# Arquivos
SOURCES = $(SRC_DIR)/date.cpp $(SRC_DIR)/main.cpp $(SRC_DIR)/Conta.cpp $(SRC_DIR)/CartaoCredito.cpp $(SRC_DIR)/Util.cpp $(SRC_DIR)/Despesa.cpp
OBJECTS = $(SOURCES:$(SRC_DIR)/%.cpp=$(OBJ_DIR)/%.o)
EXEC = $(BIN_DIR)/financas

# Regras
$(EXEC): $(OBJECTS)
	@mkdir -p $(BIN_DIR)  # Garante que o diretório existe
	$(CXX) $(LDFLAGS) -o $@ $(OBJECTS)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(OBJ_DIR)  # Garante que o diretório existe
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Debug com GDB
debug: $(EXEC)
	gdb -q $(EXEC)

# Limpeza
clean:
	rm -rf $(OBJ_DIR) $(BIN_DIR)

# Execução normal
run: $(EXEC)
	./$(EXEC)

# Rebuild completo
rebuild: $(EXEC)

.PHONY: clean run debug rebuild