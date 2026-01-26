NAME	= webServ_0-1

CPP		= c++ -std=c++98

FLAGS	= -Wall -Wextra -Werror

ODIR	= obj/

WEBDIR	= core/server/
CNFDIR	= config/
CLIDIR	= core/client/

HDRS	= $(WEBDIR)WebServer \
		$(CNFDIR)Configuration \
		$(CNFDIR)helperConfig \
		$(CLIDIR)Client

SRCS	= $(WEBDIR)WebServer \
		$(CNFDIR)Configuration \
		$(CNFDIR)helperConfig \
		$(CLIDIR)Client \
		run

HDR		= $(addsuffix .hpp, $(HDRS))
SRC		= $(addsuffix .cpp, $(SRCS))
OBJ		= $(addsuffix .o, )

all: $(NAME) | $(HDR)

$(NAME): $(SRC)
	$(CPP) $(FLAGS) $^ -o $@

clean:
	rm -rf $(NAME)

fclean: clean
	rm -rf $(ODIR)


.PHONY: all

