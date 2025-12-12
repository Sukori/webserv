NAME	= test_conf

CPP		= c++ -std=c++98

FLAGS	= -I. -Wall -Wextra -Werror

HDRS	= Configuration

SRCS	= Configuration \
		main

HDR		= $(addsuffix .hpp , $(HDRS))
SRC		= $(addsuffix .cpp , $(SRCS))

all: $(NAME) | $(HDR)

$(NAME): $(SRC)
	$(CPP) $(FLAGS) $^ -o $@

.PHONY: all

