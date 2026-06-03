NAME = codexion

SRC = coder.c dongle.c dq.c init.c log.c main.c monitor.c sim.c utils_coder.c utils_coder2.c utils_dongle.c utils_dq.c utils.c

OBJ = $(SRC:.c=.o)

CC = cc

CFLAGS = -Wall -Wextra -Werror -pthread

all: $(NAME)

$(NAME): $(OBJ)
		$(CC) $(CFLAGS) $(OBJ) -o $(NAME)

clean:
	rm -f $(OBJ)

fclean: clean
	rm -f $(NAME)

re: fclean all