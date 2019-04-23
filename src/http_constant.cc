#include "http.h"
#include <stdlib.h>

// HTTP Status Code
static const char *http_status_msg_1xx[] = {
    "Continue",            // 100
    "Switching Protocols", // 101
    "Processing",          // 102 (WebDAV; RFC 2518)
    "Early Hints"          // 103 (RFC 8297)
};

static const char *http_status_msg_2xx[] = {
    "OK",                            // 200
    "Created",                       // 201
    "Accepted",                      // 202
    "Non-Authoritative Information", // 203 (since HTTP/1.1)
    "No Content",                    // 204
    "Reset Content",                 // 205
    "Partial Content",               // 206 (RFC 7233)
    "Multi-Status",                  // 207 (WebDAV; RFC 4918)
    "Already Reported",              // 208 (WebDAV; RFC 5842)
    // TODO: padding here
    "IM Used",                       // 226 (RFC 3229)
};

static const char *http_status_msg_3xx[] = {
    "Multiple Choices"    // 300
    "Moved Permanently",  // 301
    "Found",              // 302 (Previously "Moved temporarily")
    "See Other",          // 303 (since HTTP/1.1)
    "Not Modified",       // 304 (RFC 7232)
    "Use Proxy",          // 305 (since HTTP/1.1)
    "Switch Proxy",       // 306
    "Temporary Redirect", // 307 (since HTTP/1.1)
    "Permanent Redirect"  // 308 (RFC 7538)
};

static const char *http_status_msg_4xx[] = {
    "Bad Request",                     // 400
    "Unauthorized",                    // 401 (RFC 7235)
    "Payment Required",                // 402
    "Forbidden",                       // 403
    "Not Found",                       // 404
    "Method Not Allowed",              // 405
    "Not Acceptable",                  // 406
    "Proxy Authentication Required",   // 407 (RFC 7235)
    "Request Timeout",                 // 408
    "Conflict",                        // 409
    "Gone",                            // 410
    "Length Required",                 // 411
    "Precondition Failed",             // 412 (RFC 7232)
    "Payload Too Large",               // 413 (RFC 7231)
    "URI Too Long",                    // 414 (RFC 7231)
    "Unsupported Media Type",          // 415
    "Range Not Satisfiable",           // 416 (RFC 7233)
    "Expectation Failed",              // 417
    "I'm a teapot",                    // 418 (RFC 2324
    // TODO: padding here
    "Misdirected Request",             // 421 (RFC 7540)
    "Unprocessable Entity",            // 422 (WebDAV; RFC 4918)
    "Locked",                          // 423 (WebDAV; RFC 4918)
    "Failed Dependency",               // 424 (WebDAV; RFC 4918)
    "Too Early",                       // 425 (RFC 8470)
    "Upgrade Required",                // 426
    "Precondition Required",           // 428 (RFC 6585)
    "Too Many Requests",               // 429 (RFC 6585)
    "Request Header Fields Too Large", // 431 (RFC 6585)
    "Unavailable For Legal Reasons"    // 451 (RFC 7725)
};

static const char *http_status_msg_5xx[] = {
    "Internal Server Error",           // 500
    "Not Implemented",                 // 501
    "Bad Gateway",                     // 502
    "Service Unavailable",             // 503
    "Gateway Timeout",                 // 504
    "HTTP Version Not Supported",      // 505
    "Variant Also Negotiates",         // 506 (RFC 2295)
    "Insufficient Storage",            // 507 (WebDAV; RFC 4918)
    "Loop Detected",                   // 508 (WebDAV; RFC 5842)
    // TODO: padding here
    "Not Extended",                    // 510 (RFC 2774)
    "Network Authentication Required"  // 511 (RFC 6585)
};

const char *http_get_status_msg(http_status_code_t code)
{
    if (code < 100) {
        return NULL;
    }
    if (code < 200) {
        return http_status_msg_1xx[code - 100];
    }
    if (code < 300) {
        return http_status_msg_2xx[code - 200];
    }
    if (code < 400) {
        return http_status_msg_3xx[code - 300];
    };
    if (code < 500) {
        return http_status_msg_4xx[code - 400];
    }
    if (code < 600) {
        return http_status_msg_5xx[code - 500];
    }
    return NULL;
}

// HTTP Methods
http_method_t HTTP_METHOD_GET     = "GET";
http_method_t HTTP_METHOD_POST    = "POST";
http_method_t HTTP_METHOD_PUT     = "PUT";
http_method_t HTTP_METHOD_DELETE  = "DELETE";
http_method_t HTTP_METHOD_PATCH   = "PATCH";
http_method_t HTTP_METHOD_HEAD    = "HEAD";
http_method_t HTTP_METHOD_CONNECT = "CONNECT";
http_method_t HTTP_METHOD_TRACE   = "TRACE";
http_method_t HTTP_METHOD_OPTIONS = "OPTIONS";

// HTTP Version
http_version_t HTTP_VERSION_1_0 = "HTTP/1.0";
http_version_t HTTP_VERSION_1_1 = "HTTP/1.1";
http_version_t HTTP_VERSION_2   = "HTTP/2";
