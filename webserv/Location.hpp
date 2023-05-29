#ifndef LOCATION_HPP
#define LOCATION_HPP

# include "Webserv.hpp"

class Location
{
    private:
        size_t                              _location_client_max_body_size;
        std::string                         _location_path;
        std::string                         _location_root;
        std::string                         _location_index;
        std::string                         _location_return;
        bool                                _location_autoindex;
        std::vector<MethodType>             _location_allow_methods;
        std::map<std::string, std::string>  _location_cgi_info;

    public:
        Location();
	    ~Location();
	    Location(const Location& obj);
	    Location &operator=(const Location& obj);

        /* get methods */
        const size_t& getLocationClientMaxBodySize();
        const std::string& getLocationPath();
        const std::string& getLocationRoot();
        const std::string& getLocationIndex();
        const std::string& getLocationReturn();
        const bool& getLocationAutoindex();
        const std::vector<MethodType>& getLocationAllowMethods();
        const std::map<std::string, std::string>& getLocationCgiInfo();

        /* set methods */
        void setLocationClientMaxBodySize(size_t size);
        void setLocationPath(std::string path);
        void setLocationRoot(std::string root);
        void setLocationIndex(std::string index);
        void setLocationReturn(std::string ret);
        void setLocationAutoindex(bool autoindex);
};

#endif