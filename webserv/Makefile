TARGET		= a.out

CC          = c++

SRCS		= main.cpp \
			  Cluster.cpp \
			  Location.cpp \
			  Server.cpp \
			  Utility.cpp \
			  ConfigParser.cpp

OBJS 	=	 $(SRCS:.cpp=.o)


all : $(TARGET)

%.o : %.cpp
	$(CC) -o $@ -c $<

$(TARGET) : $(OBJS)
	$(CC) -o $(TARGET) $(OBJS)

clean :
	rm -rf $(OBJS)

fclean : clean
	rm -rf $(TARGET)

re : fclean all

.PHONY : all clean fclean re