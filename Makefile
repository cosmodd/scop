################################################################################
#  SOURCES                                                                     #
################################################################################

SRCS	:=	src/main.cpp \
			src/glad.cpp \

OBJS	:=	$(SRCS:.cpp=.o)

################################################################################
#  CONSTANTS                                                                   #
################################################################################

OS			:=	$(shell uname -s)

CXX			:=	c++
CXXFLAGS	:=	-Wall -Wextra -std=c++11

NAME		:=	scop

INCLUDES	:=	-Iheaders
LIBS		:=	-lglfw

ifeq ($(OS), Darwin)
	LIBS += -framework OpenGL
else ifeq ($(OS), Linux)
	LIBS += -lGL
endif

################################################################################
#  MAKEFILE VISUALS                                                            #
################################################################################

SHELL	:=	zsh
REDO	:=	\r\e[2K

# COLORS
BG_RD	:=	\033[48;2;237;66;69m
BG_GR	:=	\033[48;2;0;194;60m
BG_BL	:=	\033[48;2;88;101;242m
FG_WH	:=	\033[38;2;255;255;255m
FG_BK	:=	\033[38;2;0;0;0m
BOLD	:=	\033[1m
NOCOL	:=	\033[0m

# CHARS
TICK	:=	✓
PEN		:=	✐
CROSS	:=	𐄂

# MESSAGES
VALID	:=	$(BOLD)$(FG_WH)$(BG_GR) $(TICK)
INFO	:=	$(BOLD)$(FG_WH)$(BG_BL) $(PEN)
DEL		:=	$(BOLD)$(FG_WH)$(BG_RD) $(CROSS)

################################################################################
#  TARGETS / RULES                                                             #
################################################################################

%.o: %.cpp
	@$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@
	@echo -n '$(REDO)$(INFO) $(notdir $@) $(NOCOL)'

all: $(NAME)

$(NAME): $(OBJS)
	@$(CXX) $(CXXFLAGS) $(LIBS) $(INCLUDES) $^ -o $(NAME)
	@echo '$(REDO)$(VALID) $@ $(NOCOL)'

run: $(NAME)
	@./$(NAME)

debug:
	@echo '$(INFO) Debugging project ! $(NOCOL)'
	@make -sC ./ CXXFLAGS="${CXXFLAGS} -g -fsanitize=address -DDEBUG" re

clean:
	@rm -f $(OBJS)
	@echo '$(DEL) Removed $(words $(OBJS)) object files $(NOCOL)'

fclean: clean
	@rm -f $(NAME)
	@echo '$(DEL) $(NAME) binary $(NOCOL)'

re: fclean all

.PHONY: all clean fclean re run