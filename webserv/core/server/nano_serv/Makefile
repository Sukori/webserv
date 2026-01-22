NAME	= nanoServ

CPP		= c++ -std=c++98

FLAGS	= -I. -Wall -Wextra -Werror

HDRS	= WebServ \
		Client

SRCS	= WebServ \
		Client \
		run

HDR		= $(addsuffix .hpp , $(HDRS))
SRC		= $(addsuffix .cpp , $(SRCS))

all: $(NAME) | $(HDR)

$(NAME): $(SRC)
	$(CPP) $(FLAGS) $^ -o $@

clean:
	rm -rf $(NAME)

.PHONY: all

