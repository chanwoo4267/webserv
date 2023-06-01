#ifndef LOCATION_HPP
#define LOCATION_HPP

# include "Webserv.hpp"

/*
    location subject rules

    # config
    1. define accepted method lists (allow_methods)
    2. http redirection (return)
    3. default search path (path)
    4. directory listing (autoindex)
    5. directory default file (index)
    6. CGI extensions
    7. direcotory for uploaded files (root)
*/

class Location
{
    private:
        size_t                              _location_client_max_body_size;
        std::string                         _location_path;
        std::string                         _location_root;
        std::string                         _location_index;
        std::string                         _location_return;
        int                                 _location_return_status;
        bool                                _location_autoindex;
        std::vector<MethodType>             _location_allow_methods;
        std::string                         _location_cgi_path;
        std::string                         _location_cgi_extension;

    public:
        Location();
	    ~Location();
	    Location(const Location& obj);
	    Location& operator=(const Location& obj);

        /* get methods */
        const size_t& getLocationClientMaxBodySize();
        const std::string& getLocationPath();
        const std::string& getLocationRoot();
        const std::string& getLocationIndex();
        const std::string& getLocationReturn();
        const int& getLocationReturnStatus();
        const bool& getLocationAutoindex();
        const std::vector<MethodType>& getLocationAllowMethods();
        const std::string& getLocationCgiPath();
        const std::string& getLocationCgiExtension();

        /* set methods */
        void setLocationClientMaxBodySize(size_t size);
        void setLocationPath(std::string path);
        void setLocationRoot(std::string root);
        void setLocationIndex(std::string index);
        void setLocationReturn(std::string ret);
        void setLocationReturnStatus(int status);
        void setLocationAutoindex(bool autoindex);
        void setLocationAllowMethods(std::vector<MethodType> methods);
        void setLocationCgiPath(std::string cgi_path);
        void setLocationCgiExtension(std::string cgi_extension);

        void printLocation();
};

#endif