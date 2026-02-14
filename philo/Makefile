NAME = philo
TSAN_NAME = philo_tsan
CC = cc
CFLAGS += -pthread -Wall -Wextra -Werror -pedantic-errors
TSAN_CFLAGS = $(CFLAGS) -g -O0 -fsanitize=thread
DEBUG_NAME = tester
DEBUG_CFLAGS = $(CFLAGS) -g -O0

SRC = main.c\
	utils.c\
	parsing.c\
	time.c\
	free.c\
	sim.c\
	sim2.c\
	sim3.c\
	create.c\
	wrappers.c\
	monitoring.c

OBJ = $(SRC:.c=.o)

# Para -fsanitze=thread
TSAN_OBJ = $(SRC:.c=.tsan.o)

# Para debug (tester)
DEBUG_OBJ = $(SRC:.c=.debug.o)


# Reglas
all: ob_mssg $(NAME)
	
$(NAME): $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) -o $(NAME)
	@echo -e "\n✅ \e[36mTerminado!\e[0m\n"

$(TSAN_NAME): tsan_mssg $(TSAN_OBJ)
	$(CC) $(TSAN_CFLAGS) $(TSAN_OBJ) -o $(TSAN_NAME)
	@echo -e "\n✅ \e[36mTerminado (TSan)!\e[0m\n"

$(DEBUG_NAME): debug_mssg $(DEBUG_OBJ)
	$(CC) $(DEBUG_CFLAGS) $(DEBUG_OBJ) -o $(DEBUG_NAME)
	@echo -e "\n✅ \e[36mTerminado (Tester)!\e[0m\n"

ob_mssg:
	@echo -e "\n🚧 \e[36mCompilando parte obligatoria...\e[0m\n"

tsan_mssg:
	@echo -e "\n🚧 \e[36mCompilando para thread sanitizer...\e[0m\n"

debug_mssg:
	@echo -e "\n🚧 \e[36mCompilando para debug (tester)...\e[0m\n"

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

%.tsan.o: %.c
	$(CC) $(TSAN_CFLAGS) -c $< -o $@

%.debug.o: %.c
	$(CC) $(DEBUG_CFLAGS) -c $< -o $@

check:
	@echo -e "\n🔍 \e[36mComprobando con cppcheck (análisis estático)...\e[0m\n"
	@cppcheck --inconclusive --enable=all --suppress=missingIncludeSystem --inconclusive --std=c11 --check-level=exhaustive $(SRC) $(BONUS_SRC)
	@echo -e "\n✅ \e[36mTerminado!\e[0m\n"

clean:
	rm -f $(OBJ) $(BONUS_OBJ) $(TSAN_OBJ) $(DEBUG_OBJ)

fclean: clean
	rm -f $(NAME) $(TSAN_NAME) $(DEBUG_NAME) $(TEST_RUNNER_C)

re: fclean all



.PHONY: all  check clean fclean re philo_tsan
