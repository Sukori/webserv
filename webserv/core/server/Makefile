NAME	= nanoServ

CPP		= c++ -std=c++98

FLAGS	= -I. -Wall -Wextra -Werror

HDRS	= WebServer \
		Configuration \
		helperConfig \
		Client

SRCS	= WebServer \
		Client \
		Configuration \
		helperConfig \
		run

HDR		= $(addsuffix .hpp , $(HDRS))
SRC		= $(addsuffix .cpp , $(SRCS))

all: $(NAME) | $(HDR)

$(NAME): $(SRC)
	$(CPP) $(FLAGS) $^ -o $@

clean:
	rm -rf $(NAME)

.PHONY: all

