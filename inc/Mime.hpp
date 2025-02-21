#include <iostream>
#include <string>
#include <map>

// Перечисление MIME-типов
enum MimeType {
    TEXT_PLAIN,
    TEXT_HTML,
    TEXT_CSS,
    TEXT_JS,
    APPLICATION_JSON,
    APPLICATION_XML,
    IMAGE_JPEG,
    IMAGE_PNG,
    IMAGE_GIF,
    AUDIO_MP3,
    VIDEO_MP4,
    APPLICATION_PDF,
    APPLICATION_ZIP,
    APPLICATION_DOC,
    APPLICATION_DOCX,
    APPLICATION_XLS,
    APPLICATION_XLSX,
    APPLICATION_UNKNOWN // Для неизвестных форматов
};

std::string getFileExt(MimeType type)
{
    switch (type) 
    {
        case TEXT_PLAIN:      return ".txt";
        case TEXT_HTML:       return ".html";
        case TEXT_CSS:        return ".css";
        case TEXT_JS:         return ".js";
        case APPLICATION_JSON: return ".json";
        case APPLICATION_XML: return ".xml";
        case IMAGE_JPEG:      return ".jpg";
        case IMAGE_PNG:       return ".png";
        case IMAGE_GIF:       return ".gif";
        case AUDIO_MP3:       return ".mp3";
        case VIDEO_MP4:       return ".mp4";
        case APPLICATION_PDF: return ".pdf";
        case APPLICATION_ZIP: return ".zip";
        case APPLICATION_DOC: return ".doc";
        case APPLICATION_DOCX: return ".docx";
        case APPLICATION_XLS: return ".xls";
        case APPLICATION_XLSX: return ".xlsx";
        case APPLICATION_UNKNOWN:
        default:              return ".bin";
    }
}