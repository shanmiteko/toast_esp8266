#include "Fetch.h"

HTTPRequest::HTTPRequest()
{
    _cert.append(cert_DigiCert_Secure_Site_CN_CA_G3);
}

Respond HTTPRequest::GET(const String &url)
{
    begin(url);
    Respond res = sendRequest("GET");
    res.setPayload(_http->getString());
    end();
    return res;
}

Respond HTTPRequest::POST(const String &url, const String &body)
{
    begin(url);
    Respond res = sendRequest("POST", body);
    res.setPayload(_http->getString());
    end();
    return res;
}

bool HTTPRequest::begin(const String &url)
{
    bool status;
    _http = new HTTPClient();
    if (url[4] == 's')
    {
        _tls = new WiFiClientSecure();
        _tls->setTrustAnchors(&_cert);
        status = _http->begin(*_tls, url);
    }
    else
    {
        _tcp = new WiFiClient();
        status = _http->begin(*_tcp, url);
    }
    _http->setReuse(false);
    return status;
}

Respond HTTPRequest::sendRequest(const char *method)
{
    return Respond(_http->sendRequest(method));
}

Respond HTTPRequest::sendRequest(const char *method, const String &body)
{
    return Respond(_http->sendRequest(method, body));
}

void HTTPRequest::end()
{
    _http->end();
    delete _http;
    delete _tcp;
    delete _tls;
}

void HTTPRequest::addHeader(const String &name, const String &value)
{
    _http->addHeader(name, value);
}

Respond::Respond()
{
    _status = 0;
}

Respond::Respond(int status)
{
    _status = status;
}

Respond::Respond(int status, const String &payload)
{
    _payload = payload;
}

bool Respond::isOk()
{
    return _status == HTTP_CODE_OK;
}

void Respond::setPayload(const String &payload)
{
    _payload = payload;
}

HTTPRequest Fetch;