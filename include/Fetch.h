#pragma once

#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <Certs.h>

class Respond
{
public:
    Respond();
    Respond(int status);
    Respond(int status, const String &payload);
    ~Respond(){};

    bool isOk();
    void setPayload(const String &payload);

    int _status;
    String _payload;
};

class HTTPRequest
{
public:
    HTTPRequest();
    ~HTTPRequest(){};

    Respond GET(const String &url);
    Respond POST(const String &url, const String &body);
    bool begin(const String &url);
    Respond sendRequest(const char *method);
    Respond sendRequest(const char *method, const String &body);
    void end();

    void addHeader(const String &name, const String &value);

private:
    HTTPClient *_http;
    WiFiClient *_tcp;
    X509List _cert;
    WiFiClientSecure *_tls;
};

extern HTTPRequest Fetch;