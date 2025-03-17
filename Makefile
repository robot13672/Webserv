
NAME	= webserv
CC		= c++
CFLAGS 	= -std=c++98 -g
CFLAGS	+= -Wall -Wextra -Werror 
# CFLAGS += -Wpedantic
# garantees no memory leaks:
# CFLAGS += -fsanitize=address
# LDFLAGS	=
# LDFLAGS += -Wpedantic
# garantees no memory leaks:
# LDFLAGS += -fsanitize=address

SRCS 	= $(shell find . -not -path './training/*' -not -name 'main1.cpp' -name '*.cpp')
OBJS 	= $(SRCS:.cpp=.o)

# Colors
RESET	= \033[0;39m
GRAY	= \033[0;90m
RED		= \033[0;91m
GREEN	= \033[0;92m
YELLOW	= \033[0;93m
BLUE	= \033[0;94m
MAGENTA = \033[0;95m
CYAN	= \033[0;96m
WHITE	= \033[0;97m

all: $(NAME)

$(NAME): $(OBJS)
	echo "$(GREEN)"
	$(CC) $(LDFLAGS) $(OBJS) -o $(NAME)
	echo "$(RESET)"

%.o: %.cpp inc/*.hpp
	@echo "$(YELLOW)"
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) *.o

fclean: clean
	rm -f $(NAME)

re: fclean all

prn:
	@echo $(CFLAGS)

val:
	valgrind --leak-check=full ./$(NAME)

.PHONY: all clean fclean re prn val