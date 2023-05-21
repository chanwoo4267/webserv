TARGET		= a.out

CC          = c++

SRCS		= main.cpp \
			  Config.cpp \
			  LocationBlock.cpp \
			  ServerBlock.cpp \
			  Utility.cpp 

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