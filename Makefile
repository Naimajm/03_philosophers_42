
DEF_COLOR      	= \033[0;39m
ORANGE         	= \033[38;5;209m
MID_GRAY       	= \033[38;5;245m
DARK_GREEN     	= \033[38;2;75;179;82m

NAME 			:= philo


INCLUDES_DIR 	:= ./includes 

# C FLAGS
CC 				:= cc
CFLAGS 			:= -Wall -Wextra -Werror

#DIRECTORIES TO SEARCH HEADERS FILES .H
INCLUDES		:= -I$(INCLUDES_DIR)

#LIBRARIES UBICATION
EXT_LIBRARYS 	:= -pthread

# SRC --------------------------------------------------------------------------------
SRC_DIR			:= ./src
SRC_FILES 		:= $(addprefix $(SRC_DIR)/, 00_main.c \
				01_init.c 02_philo.c  \
				04_mutex_functions.c  \
				08_time.c \
				10_free_manager.c 11_utils.c 12_utils_debug.c \
				) 
OBJ_FILES 		:= $(SRC_FILES:%.c=%.o)

all: $(NAME)

$(NAME):  $(OBJ_FILES)
	@echo "$(ORANGE)🚀​ Compiling $(NAME)... $(DEF_COLOR)"
	@$(CC) $(CFLAGS) $(OBJ_FILES) $(INCLUDES) $(EXT_LIBRARYS) -o $(NAME)
	@echo "$(DARK_GREEN)📚 $(NAME) has been created			OK$(DEF_COLOR)"

clean:
	@rm -f $(OBJ_FILES)	
	@echo "${MID_GRAY}Cleaning objects $(NAME)			OK$(DEF_COLOR)"

fclean: clean
	@rm -f $(NAME)
	@echo "$(MID_GRAY)Cleaning $(NAME)				OK$(DEF_COLOR)"

debug: CFLAGS 		+= -g -fsanitize=address
debug: EXT_LIBRARYS += -fsanitize=address
debug: re

re: fclean all
	@echo "$(DARK_GREEN)🔁 Cleaning and recompiled -> $(NAME) 	OK$(DEF_COLOR)"	
	@make -s clean

.PHONY: all library clean fclean re debug

# DEBUG -----------------------------------------

# Análisis completo (Memcheck + threads)
# valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes --tool=memcheck ./philo 4 400 200 200

# debugging específico de pthread:
# valgrind --tool=helgrind --history-level=full ./philo 4 400 200 200
 
# Problemas con threads (Helgrind):
# valgrind --tool=helgrind ./philo 4 400 200 200

# Data races y condiciones de carrera:
# valgrind --tool=drd ./philo 4 400 200 200

# Memory Leaks básico  
# valgrind --leak-check=full --show-leak-kinds=all ./philo 4 400 200 200
