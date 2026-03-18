NAME	= nanoServ

CPP		= c++ -std=c++98

FLAGS	= -I. -Wall -Wextra -Werror

HDRS	= WebServer \
		../../config/Configuration \
		../../config/helperConfig \
		helperWebServer \
		../client/Client

SRCS	= WebServer \
		../client/Client \
		../../config/Configuration \
		../../config/helperConfig \
		helperWebServer \
		memberFunctionsWebServer \
		mainLoopWebServer \
		run

HDR		= $(addsuffix .hpp , $(HDRS))
SRC		= $(addsuffix .cpp , $(SRCS))

all: $(NAME) | $(HDR)

$(NAME): $(SRC)
	$(CPP) $(FLAGS) $^ -o $@

clean:
	rm -rf $(NAME)

.PHONY: all

