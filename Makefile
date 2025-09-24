NAME = philo
SRC = philosophers.c\
	utils.c\
	parsing.c\
	
OBJ = $(SRC:.c=.o)
CFLAGS += -g -O1 -pthread #-Wall -Wextra -Werror -fsanitize=thread

all: $(NAME)

$(NAME): $(OBJ)
	cc $(CFLAGS) $(OBJ) -o $(NAME)

%.o: %.c
	cc $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ)

fclean: clean
	rm -f $(NAME)