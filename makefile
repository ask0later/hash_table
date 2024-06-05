TARGET = hash
CC = g++
CFLAGS = -g -O3 -mavx -mavx2

IFLAGS = -I./inc/ -I./third-party/List/inc/

SRC_FOLDER = ./src/
OBJ_FOLDER = ./obj/

# SRC_LIST_FOLDER = ./third-party/List/src/
# OBJ_LIST_FOLDER = ./third-party/List/obj/

# $(OBJ_LIST_FOLDER)%.o : $(SRC_LIST_FOLDER)%.cpp
# 	@mkdir -p $(@D)
# 	@$(CC) $(IFLAGS) $(CFLAGS) -c $< -o $@

ASM   = ./src/hash_crc32.asm
ASM_O = ./asm/hash_crc32.o


SRC_MAIN    = $(wildcard $(SRC_FOLDER)*.cpp)
OBJ_MAIN    = $(patsubst $(SRC_FOLDER)%.cpp, $(OBJ_FOLDER)%.o, $(SRC_MAIN))
# SRC_LIST    = $(wildcard $(SRC_LIST_FOLDER)*.cpp)
# OBJ_MAIN   += $(patsubst $(SRC_LIST_FOLDER)%.cpp, $(OBJ_LIST_FOLDER)%.o, $(SRC_LIST))


$(TARGET) : $(OBJ_MAIN) $(ASM_O)
	@$(CC) $(IFLAGS) $(CFLAGS) $(OBJ_MAIN) $(ASM_O) -o $(TARGET) -lm

$(OBJ_FOLDER)%.o : $(SRC_FOLDER)%.cpp
	@mkdir -p $(@D)
	@$(CC) $(IFLAGS) $(CFLAGS) -c $< -o $@

$(ASM_O): $(ASM)
	nasm -f elf64 -g $(ASM) -o $(ASM_O)

clean:
	rm $(TARGET) $(OBJ_MAIN) $(ASM_O)