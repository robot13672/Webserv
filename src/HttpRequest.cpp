#include "../inc/HttpRequest.hpp"
#include <algorithm>
#include <unistd.h>

HttpRequest::HttpRequest() : method(""), uri(""), httpVersion(""), maxBodySize(0), isChunked(false) {}

HttpRequest::HttpRequest(const std::string& rawRequest) {
    parseRequest(rawRequest);
}

HttpRequest::HttpRequest(){
    _method_str[::GET] = "GET";
    _method_str[::POST] = "POST";
    _method_str[::DELETE] = "DELETE";
    _method_str[::PUT] = "PUT";
    _method_str[::HEAD] = "HEAD";
    _path = "";
    _query = "";
    _fragment = "";
    _body_str = "";
    _error_code = 0;
    _chunk_length = 0;
    _method = NONE;
    _method_index = 1;
    _state = Request_Line;
    _fields_done_flag = false;
    _body_flag = false;
    _body_done_flag = false;
    _chunked_flag = false;
    _body_length = 0;
    _storage = "";
    _key_storage = "";
    _multiform_flag = false;
    _boundary = "";
}

HttpRequest::~HttpRequest(){
    
}



HttpRequest::HttpRequest(HttpRequest const &copy){
    this->_method_str = copy._method_str;
    this->_path = copy._path;
    this->_query = copy._query;
    this->_fragment = copy._fragment;
    this->_body_str = copy._body_str;
    this->_error_code = copy._error_code;
    this->_chunk_length = copy._chunked_flag;
    this->_method = copy._method;
    this->_method_index = copy._method_index;
    this->_state = copy._state;
    this->_fields_done_flag = copy._fields_done_flag;
    this->_body_flag = copy._body_flag;
    this->_body_done_flag = copy._body_done_flag;
    this->_chunked_flag = copy._chunked_flag;
    this->_body_length = copy._body_length;
    this->_storage = copy._storage;
    this->_key_storage = copy._key_storage;
    this->_multiform_flag = copy._multiform_flag;
    this->_boundary = copy._boundary;
}

HttpRequest &HttpRequest::operator=(HttpRequest const &copy){
    if (this != &copy)
    {
        _method_str = copy._method_str;
        _path = copy._path;
        _query = copy._query;
        _fragment = copy._fragment;
        _body_str = copy._body_str;
        _error_code = copy._error_code;
        _chunk_length = copy._chunked_flag;
        _method = copy._method;
        _method_index = copy._method_index;
        _state = copy._state;
        _fields_done_flag = copy._fields_done_flag;
        _body_flag = copy._body_flag;
        _body_done_flag = copy._body_done_flag;
        _chunked_flag = copy._chunked_flag;
        _body_length = copy._body_length;
        _storage = copy._storage;
        _key_storage = copy._key_storage;
        _multiform_flag = copy._multiform_flag;
        _boundary = copy._boundary;
    }
    return *this;
}
