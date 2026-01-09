CC = g++

CFLAGS = -Wall -Wextra -Werror -std=c++98

NAME	= http

SRC = test_http.cpp HttpMessage.cpp HttpRequest.cpp HttpResponse.cpp
OBJ = $(SRC:.cpp=.o)

RM = rm -f

all: $(NAME) target

$(NAME): $(OBJ)
	$(CC) $(OBJ) -o $(NAME)

%.o: %.cpp
	$(CC) $< $(CFLAGS) -c -o $@

clean:
	$(RM) $(OBJ)

fclean: clean
	$(RM) $(NAME)

re: fclean all

target:
	$(info $(NAME))

.PHONY:
	all clean fclean re target
