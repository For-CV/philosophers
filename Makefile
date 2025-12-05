NAME = philo
BONUS_NAME = philo_bonus
CC = cc
CFLAGS += -g -pthread -Wall -Wextra -Werror -pedantic-errors

# Parte obligatoria
SRC = src/main.c\
	src/utils.c\
	src/parsing.c\
	src/time.c\
	src/free.c\
	src/sim.c\
	src/sim2.c\
	src/sim3.c\
	src/create.c
OBJ = $(SRC:.c=.o)

# Bonus
BONUS_SRC = bonus_src/philo_bonus.c\
	bonus_src/utils_bonus.c\
	bonus_src/parsing_bonus.c\
	bonus_src/sim_bonus.c\
	bonus_src/sim2_bonus.c\
	bonus_src/sim3_bonus.c\
	bonus_src/free_bonus.c\
	bonus_src/time_bonus.c\
	bonus_src/wrappers_bonus.c
BONUS_OBJ = $(BONUS_SRC:.c=.o)

# Tests
TEST_C_FILES = tests/test_units.c\
	tests/test_atoi.c\
	tests/test_parse.c\
	src/utils.c\
	src/parsing.c\
	src/time.c\
	src/free.c\
	src/sim.c\
	src/sim2.c\
	src/sim3.c\
	src/create.c

TEST_SH_FILES = tests/test.sh
TEST_RUNNER_C = test_runner_c

all: $(NAME)

$(NAME): $(OBJ)
	@echo -e "\n🚧 \e[36mCompilando parte obligatoria...\e[0m\n"
	$(CC) $(CFLAGS) $(OBJ) -o $(NAME)
	@echo -e "\n✅ Terminado!\e[0m\n"

bonus: $(BONUS_NAME)

$(BONUS_NAME): $(BONUS_OBJ)
	@echo -e "\n🚧 \e[36mCompilando bonus...\e[0m\n"
	$(CC) $(CFLAGS) $(BONUS_OBJ) -o $(BONUS_NAME)
	@echo
	@echo -e "✅ \e[36mTerminado!\e[0m"
	@echo

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

check:
	@echo -e "\n🔍 \e[36mComprobando con cppcheck (análisis estático)...\e[0m\n"
	@cppcheck --inconclusive --enable=all --suppress=missingIncludeSystem --inconclusive --std=c11 --check-level=exhaustive $(SRC) $(BONUS_SRC)
	@echo -e "\n✅ \e[36mTerminado!\n"

tests: $(TEST_RUNNER_C)
	mv $(TEST_RUNNER_C) tests
	@echo -e "\n🔍 \e[36m--- Running tests ---\e[0m\n"
	bash $(TEST_SH_FILES)
	@echo -e "\n✅ \e[36mTerminado!\e[0m\n"

$(TEST_RUNNER_C): $(TEST_C_FILES)
	@echo -e "\n🚧 \e[36mCompilando tests con fsanitize=address,undefined...\e[0m\n"
	$(CC) $(CFLAGS) -fsanitize=address,undefined $(TEST_C_FILES) -o $(TEST_RUNNER_C)
	@echo -e "\n✅ \e[36mTerminado!\e[0m\n"

clean:
	rm -f $(OBJ) $(BONUS_OBJ)

clean_bonus: clean
	rm -f /dev/shm/sem.die /dev/shm/sem.forks /dev/shm/sem.seats /dev/shm/sem.printer
fclean: clean
	rm -f $(NAME) $(BONUS_NAME) $(TEST_RUNNER_C)

re: fclean all

docker-run:
	@echo "\n\e[36m🐳 Entrando al contenedor...\e[0m\n"
	docker run --rm -it \
		--cap-add=SYS_PTRACE \
		--security-opt seccomp=unconfined \
		-v $$(pwd):/app \
		entorno-c-pro

.PHONY: all bonus check tests clean fclean clean_bonus re

# In a Arch type Linux, run 