#ifndef HTTP_REQUEST_HPP
#define HTTP_REQUEST_HPP

#include <string>
#include <map>
#include <sstream>

<<<<<<< HEAD
enum HttpMethod //методы реквестов для обработки
{
    GET,
    POST,
    DELETE,
    PUT,
    HEAD,
    NONE
};

enum ParsingState
{
    Request_Line,
    Request_Line_Post_Put,
    Request_Line_Method,
    Request_Line_First_Space,
    Request_Line_URI_Path_Slash,
    Request_Line_URI_Path,
    Request_Line_URI_Query,
    Request_Line_URI_Fragment,
    Request_Line_Ver,
    Request_Line_HT,
    Request_Line_HTT,
    Request_Line_HTTP,
    Request_Line_HTTP_Slash,
    Request_Line_Major,
    Request_Line_Dot,
    Request_Line_Minor,
    Request_Line_CR,
    Request_Line_LF,
    Field_Name_Start,
    Fields_End,
    Field_Name,
    Field_Value,
    Field_Value_End,
    Chunked_Length_Begin,
    Chunked_Length,
    Chunked_Ignore,
    Chunked_Length_CR,
    Chunked_Length_LF,
    Chunked_Data,
    Chunked_Data_CR,
    Chunked_Data_LF,
    Chunked_End_CR,
    Chunked_End_LF,
    Message_Body,
    Parsing_Done
};

/**
 *
 * - HttpRequest Class will be used to parase and store the request.
   It gets feeded with the request and will triiger a flag when parasing is finished.
   - If any error was found in the request, _code will be set to the correct error code.
**/
class HttpRequest
{
    public:
        HttpRequest();
        ~HttpRequest();
        HttpRequest(HttpRequest const &copy);
        HttpRequest &operator=(HttpRequest const &copy);

        HttpMethod                                  &getMethod();
        std::string                                 &getPath();
        std::string                                 &getQuery();
        std::string                                 &getFragment();
        std::string                                 getHeader(std::string const &);
		const std::map<std::string, std::string>    &getHeaders() const;
		std::string                                 getMethodStr();
        std::string                                 &getBody();
        std::string                                 getServerName();
        std::string                                 &getBoundary();
        bool                                        getMultiformFlag();
        
        void        setMethod(HttpMethod &);
        void        setHeader(std::string &, std::string &);
        void        setMaxBodySize(long max_body_size);
        void        setBody(std::string name);

        void        feed(char *data, size_t size);
        bool        parsingCompleted();
        void        printMessage();
        void        clear();
        short       errorCode();
        bool        keepAlive();
        void        cutReqBody(int bytes);

    private:
        std::string                         _path;
        std::string                         _query;
        std::string                         _fragment;
        std::map<std::string, std::string>  _request_headers;
        std::vector<u_int8_t>               _body;
        std::string                         _boundary;
        HttpMethod                          _method;
        std::map<u_int8_t, std::string>     _method_str;
        ParsingState                        _state;
        long                              _max_body_size;
        size_t                              _body_length;
        short                               _error_code;
        size_t                              _chunk_length;
        std::string                         _storage;
        std::string                         _key_storage;
        short                               _method_index;
        u_int8_t                            _ver_major;
        u_int8_t                            _ver_minor;
        std::string                         _server_name;
        std::string                         _body_str;
        /* flags */
        bool                                _fields_done_flag;
        bool                                _body_flag;
        bool                                _body_done_flag;
        bool                                _complete_flag;
        bool                                _chunked_flag;
        bool                                _multiform_flag;

        void            _handle_headers();

};

#endif
=======
class HttpRequest 
{
private:
    std::string method;
    std::string uri;
    std::string httpVersion;
    std::map<std::string, std::string> headers;
    std::string body;
    size_t maxBodySize;  // Новый член класса
    bool isChunked;  // Флаг для chunked передачи
    std::string path;  // Часть URI до знака ?
    std::map<std::string, std::string> queryParams;  // Параметры после ?
    
    // Private helper method for chunked parsing
    bool parseChunkedBody(std::istringstream& requestStream);
    size_t parseChunkSize(const std::string& line);
    
    // Новый приватный метод для парсинга URI
    void parseUri(const std::string& fullUri);
    void parseQueryParam(const std::string& param);  // Добавляем объявление метода
    
public:
    // Constructors and destructor
    HttpRequest();
    explicit HttpRequest(const std::string& rawRequest);
    ~HttpRequest();
    
    // Parse methods
    bool parseRequest(const std::string& rawRequest);
    bool parseRequest(int fd, size_t contentLength);
    bool parseRequestLine(const std::string& line);
    bool parseHeaders(std::istringstream& requestStream);
    bool parseBody(std::istringstream& requestStream);
    
    // Getters
    const std::string& getMethod() const;
    const std::string& getUri() const;
    const std::string& getHttpVersion() const;
    const std::map<std::string, std::string>& getHeaders() const;
    const std::string& getBody() const;
    std::string getHeader(const std::string& key) const;
    
    // Новые геттеры для query параметров
    const std::string& getPath() const;
    const std::map<std::string, std::string>& getQueryParams() const;
    std::string getQueryParam(const std::string& key) const;
    bool hasQueryParam(const std::string& key) const;
    
    // Validation methods
    bool isValidMethod() const;
    bool isValidUri() const;
    bool isValidHttpVersion() const;

    // New method
    void setMaxBodySize(size_t size);
    
    // New method
    bool isChunkedTransfer() const;
};

>>>>>>> origin
