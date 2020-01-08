#pragma once

#include <curl\curl.h>
#include <iostream>
#include <string>

#define WHOIS_PATH "https://www.whois.com/whois/"

std::string getWebpage(std::string url);
std::string extractOutput(std::string content);
std::string cleanOutput(std::string data);
size_t write_callback(char *content, size_t size, size_t nmemb, void *user);