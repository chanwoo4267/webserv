CLIENT		= client
SERVER		= server

CC          = c++

CLIENT_SRCS	= client.cpp
SERVER_SRCS = echo.cpp

CLIENT_OBJS = $(CLIENT_SRCS:.cpp=.o)
SERVER_OBJS = $(SERVER_SRCS:.cpp=.o)

all : $(CLIENT) $(SERVER)

%.o : %.cpp
	$(CC) -o $@ -c $<

$(CLIENT) : $(CLIENT_OBJS)
	$(CC) -o $(CLIENT) $(CLIENT_OBJS)

$(SERVER) : $(SERVER_OBJS)
	$(CC) -o $(SERVER) $(SERVER_OBJS)

clean :
	rm -rf $(CLIENT_OBJS)
	rm -rf $(SERVER_OBJS)

fclean : clean
	rm -rf $(CLIENT) $(SERVER)

re : fclean all

.PHONY : all clean fclean re