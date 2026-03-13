NAME	= test_conf

CPP		= c++ -std=c++98

FLAGS	= -Wall -Wextra -Werror -g3

HDRS	= helperConfig \
		Parser \
		Configuration

SRCS	= helperConfig \
		Parser \
		Configuration \
		main

HDR		= $(addsuffix .hpp , $(HDRS))
SRC		= $(addsuffix .cpp , $(SRCS))

all: $(NAME) | $(HDR)

$(NAME): $(SRC)
	$(CPP) $(FLAGS) $^ -o $@

clean:
	rm -rf $(NAME)

.PHONY: all

