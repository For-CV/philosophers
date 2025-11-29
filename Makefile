NAME = philo
BONUS_NAME = philo_bonus
CC = cc
CFLAGS += -g -pthread -Wall -Wextra -Werror -pedantic

SRC = src/philo.c\
	src/utils.c\
	src/parsing.c\
	src/time.c\
	src/free.c\
	src/sim.c\
	src/sim2.c\
	src/sim3.c\
	src/create.c
OBJ = $(SRC:.c=.o)

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
			src/utils.c\
			src/parsing.c\
			src/time.c\
			src/free.c\
			src/sim.c\
			src/sim2.c\
			src/sim3.c\
			src/create.c

TEST_SH_FILES = tests/test_units.sh
TEST_RUNNER_C = test_runner_c

all: $(NAME)

$(NAME): $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) -o $(NAME)

bonus: $(BONUS_NAME)

$(BONUS_NAME): $(BONUS_OBJ)
	$(CC) $(CFLAGS) $(BONUS_OBJ) -o $(BONUS_NAME)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

check:
	@cppcheck --inconclusive --enable=all --suppress=missingIncludeSystem --inconclusive --std=c11 --check-level=exhaustive $(SRC) $(BONUS_SRC)

tests: $(TEST_RUNNER_C)
	mv $(TEST_RUNNER_C) tests
# 	@echo "--- Running C tests ---"
# 	./$(TEST_RUNNER_C)
	@echo "--- Running shell script tests ---"
	bash $(TEST_SH_FILES)

$(TEST_RUNNER_C): $(TEST_C_FILES)
	$(CC) $(CFLAGS) -fsanitize=address $(TEST_C_FILES) -o $(TEST_RUNNER_C)
clean:
	rm -f $(OBJ) $(BONUS_OBJ)

clean_bonus: clean
	rm -f /dev/shm/sem.die /dev/shm/sem.forks /dev/shm/sem.seats /dev/shm/sem.printer	
fclean: clean
	rm -f $(NAME) $(BONUS_NAME) $(TEST_RUNNER_C)

re: fclean all

.PHONY: all bonus check tests clean fclean re
