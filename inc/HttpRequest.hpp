#pragma once

#include "Webserv.hpp"

class HttpRequest
{
    private:
        long _max_body_size;
    public:
        void setMaxBodySize(long max_body_size);
};