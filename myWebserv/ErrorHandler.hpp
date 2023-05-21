#ifndef ERRORHANDLER_HPP
# define ERRORHANDLER_HPP

# include "Webserv.hpp"

class ErrorHandler
{
    private:

    public:
        class ErrorException : public std::exception
        {
            private:
                std::string _message;

            public:
                ErrorException(std::string message) throw()
                {
                    _message = "SERVER CONFIG ERROR: " + message;
                }
                virtual const char *what() const throw()
                {
                    return (_message.c_str());
                }
                virtual ~ErrorException() throw() {}
        };
};

#endif