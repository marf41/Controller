#include "server.hpp"

WebServer server(80);

uint32_t requests;
uint32_t errors[6];

const char indexHead[] PROGMEM = ""
"<!DOCTYPE html><html><head><title>T-18</title>"
"<link rel=\"stylesheet\" type=\"text/css\" href=\"http://ovh.arf.ovh/static/esp.css\" />"
"<meta name=\"viewport\" content=\"width=device-width, initial-scale=1\" />"
"</head><body><h1>T-18</h2><br />\n";
const char indexPage[] PROGMEM = ""
"\n<br /><form action=\"/eval\" method=\"post\">"
"<input id=\"in\" name=\"id\" />"
"<label for=\"in\">Evaluate...</label>"
"<br /><input type=\"submit\" />"
"</form><br />"
"<form class=\"act\" action=\"/eval\" method=\"post\">"
"<input type=\"hidden\" name=\"in\" value=\"clear;\" />"
"<input type=\"submit\" value=\"Clear\" /></form>"
"<form class=\"act\" action=\"/eval\" method=\"post\">"
"<input type=\"hidden\" name=\"in\" value=\";\" />"
"<input type=\"submit\" value=\"NOP\" /></form>"
"<form class=\"act\" action=\"/eval\" method=\"post\">"
"<input type=\"hidden\" name=\"in\" value=\"\" />"
"<input type=\"submit\" value=\"Error\" /></form>"
"<script src=\"http://ovh.arf.ovh/static/esp.js\"></script>"
"</body></html>\n";

String serverInfo() {
    return String("Requests: " + String(requests) + "\nNot found: " + String(errors[4]));
}

String indexCreate(String middle) {
    return String(String(indexHead) + middle + String(indexPage));
}

void handleRoot() {
    requests++;
    server.send(200, "text/html", indexCreate(""));
}

void handleEval() {
    if ((server.method() == HTTP_POST) && (server.args() > 0)) {
        String body = server.arg(0);
        server.send(200, "text/html", indexCreate("<code>> " + tclEval(body+";") + "</code>"));
    } else { server.send(400, "text/html", indexCreate("No POST data.")); }
}

void handleRawEval() {
    if ((server.method() == HTTP_POST) && (server.args() > 0)) {
        String body = server.arg(0);
        server.send(200, "text/plain", tclEval(body + ";"));
    } else { server.send(400, "text/plain", "No POST data."); }
}

void handleNotFound() {
    errors[4]++;
    String message = "File Not Found\n\n";
    message += "URI: ";
    message += server.uri();
    message += "\nMethod: ";
    message += (server.method() == HTTP_GET) ? "GET" : "POST";
    message += "\nArguments: ";
    message += server.args();
    message += "\n";
    for (uint8_t i = 0; i < server.args(); i++) {
        message += " " + server.argName(i) + ": " + server.arg(i) + "\n";
    }
    server.send(404, "text/plain", message);
}

void serverSetup() {
    errors[0] = 0; errors[1] = 0; errors[2] = 0; errors[3] = 0; errors[4] = 0; errors[5] = 0;
    server.on("/", handleRoot);
    server.on("/raw", handleRawEval);
    server.on("/eval", handleEval);
    server.onNotFound(handleNotFound);
    server.begin();
}

void serverLoop() { server.handleClient(); }