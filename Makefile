SRCS	=	main.c \
			utils.c \
			threads.c \
			routine.c \

OBJS	= $(SRCS:.c=.o)

NAME	= philo

GCC		= gcc

FLAGS	= -Wall -Wextra -Werror -pthread
# -fsanitize=thread -g
HEADER_FILE = philo.h

all:	$(NAME)

$(NAME):	$(OBJS)
	$(GCC) $(FLAGS) -o $(NAME) $(OBJS)

%.o: %.c $(HEADER_FILE)
	$(GCC) -c $< -o  $(<:.c=.o)

clean:
	rm -f $(OBJS)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY:	clean fclean re
