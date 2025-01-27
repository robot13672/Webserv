#include "../inc/HttpRequest.hpp"

void HttpRequest::setMaxBodySize(long max_body_size)
{
    _max_body_size = max_body_size;
}