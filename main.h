#pragma once

#include <Windows.h>
#include <MsHTML.h>
#include <ExDisp.h>
#include <ExDispid.h>

#include <iostream>
#include <string>
#include <codecvt>
#ifdef _DEBUG
#include <cassert>
#endif

#define WHOIS_PATH "https://www.whois.com/whois/"

std::string getWebpage(std::string url);
std::string extractOutput(std::string content);
std::string cleanOutput(std::string data);
//size_t write_callback(char *content, size_t size, size_t nmemb, void *user);

#ifdef _DEBUG
void printError();
#endif