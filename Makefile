#################################
#  Sources                      #
#################################

SRCS	=	src/main.cpp \
			src/glad.c \
			src/engine/Shader.cpp \
			src/utils/FileSystem.cpp \
			src/maths/Mat4.cpp \
			src/maths/Vec3.cpp \
			src/maths/Vec2.cpp \
			src/maths/Utils.cpp \

OBJS	=	$(filter %.o, $(SRCS:.cpp=.o) $(SRCS:.c=.o))

#################################
#  Constants                    #
#################################

NAME		=	scop

CXX			=	c++
CXXFLAGS	=	-Wall -Wextra -Werror

INCLUDES	=	-Iinclude
LIBS		=	-lglfw -lGL

#################################
#  Targets                      #
#################################

%.o: %.cpp
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

%.o: %.c
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

all: $(NAME)

test:
	echo $(OBJS)

## Explanation to why Objects before Libraries
#####
# Linker Order Explanation:
# In the linking process, order matters. The linker processes files from left to right.
# Listing object files before libraries ensures that any unresolved references in the
# object files can be resolved by definitions in the libraries that follow.
$(NAME): $(OBJS)
	$(CXX) $(CXXFLAGS) $(INCLUDES) $^ $(LIBS) -o $(NAME)

run: $(NAME)
	./$(NAME)

debug:
	make -sC ./ CXXFLAGS="$(CXXFLAGS) -g -fsanitize=address -DDEBUG" re

clean:
	rm -f $(OBJS)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re debug run