#include <exception>
#include <string>
#include <sstream>

#define ThrowExceptionWithInfo() throw ExceptionWithInfo(__func__, __LINE__, __FILE__)

class ExceptionWithInfo : public std::runtime_error
{
  public:
    ExceptionWithInfo(const char *func, int line, const char *file)
        : std::runtime_error(createErrorMsg(func, line, file))
    {
    }

  private:
    static std::string createErrorMsg(const char *func, int line, const char *file)
    {
        std::ostringstream oss;
        oss << "Error in function " << func << " at line " << line << " in " << file;
        return oss.str();
    }
};
