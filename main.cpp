#include "main.h"

int main(int argc, char** argv)
{
	SetConsoleOutputCP(CP_UTF8);

	if (argc != 2)
	{
		std::cout << "Usage: whois <target address>" << std::endl;
		return 1;
	}

	std::string target = WHOIS_PATH + std::string(argv[1]);

	std::string webpage = getWebpage(target);
	if (!webpage.empty())
	{
		std::string output = extractOutput(webpage);
		std::cout << "Output from whois:\n" << std::endl;
		std::cout << output << std::endl;
	}

	return 0;
}

// get website raw content
std::string getWebpage(std::string url)
{
	std::string content_str;
	std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> str_wstr_converter;
	std::wstring url_w = str_wstr_converter.from_bytes(url);

	if (SUCCEEDED(OleInitialize(NULL)))
	{
		IWebBrowser2* browser = nullptr;
		HRESULT hr = CoCreateInstance(CLSID_InternetExplorer, NULL, CLSCTX_LOCAL_SERVER, IID_IWebBrowser2, (void**)&browser);

		VARIANT empty;
		VariantInit(&empty);

		if (hr == S_OK)
		{
			BSTR URL = SysAllocString(url_w.c_str());
			hr = browser->Navigate(URL, &empty, &empty, &empty, &empty);
			

			READYSTATE browserState;
			do 
			{
				Sleep(10);
				browser->get_ReadyState(&browserState);

			} while (browserState != READYSTATE_COMPLETE);

#ifdef _DEBUG
			browser->put_Visible(VARIANT_TRUE);
#endif

			IDispatch* iPatch = nullptr;
			hr = browser->get_Document(&iPatch);
#ifdef _DEBUG
			assert(iPatch != nullptr);
#endif

			IHTMLDocument2* iDoc = nullptr;
			hr = iPatch->QueryInterface(IID_IHTMLDocument2, (void**)&iDoc);
#ifdef _DEBUG
			assert(iDoc != nullptr);
#endif

			IHTMLElement* iElem = nullptr;
			hr = iDoc->get_body(&iElem);
#ifdef _DEBUG
			assert(iElem != nullptr);
#endif

			BSTR content = nullptr;
			hr = iElem->get_innerHTML(&content);
#ifdef _DEBUG
			assert(content != nullptr);
#endif

			std::wstring content_wstr = std::wstring(content, SysStringLen(content));
			content_str = str_wstr_converter.to_bytes(content_wstr);

			iElem->Release();
			iDoc->Release();	
			iPatch->Release();
			
			SysFreeString(URL);
			browser->Quit();
			browser->Release();
		}
		else
		{
			std::cout << "Failed to create instance of internet explorer" << std::endl;
#ifdef _DEBUG
			printError();
#endif
		}
		OleUninitialize();
	}
	else
	{
		std::cout << "Failed to initialize OLE" << std::endl;
#ifdef _DEBUG
		printError();
#endif
	}

	return content_str;

	//CURL* curl = curl_easy_init();
	//std::string htmlBuffer;
	//struct curl_slist* header = NULL;
	//curl_slist_append(header, "Content-Type: text/html; charset=UTF-8");
	////char* realurl = curl_easy_escape(curl, url.c_str(), url.length);
	//curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback);
	//curl_easy_setopt(curl, CURLOPT_WRITEDATA, &htmlBuffer);
	//curl_easy_setopt(curl, CURLOPT_HTTPHEADER, header);
	//curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
	//curl_easy_setopt(curl, CURLOPT_MAXREDIRS, 3L);
	//curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
	//CURLcode result = curl_easy_perform(curl);
	//curl_slist_free_all(header);
	////curl_free(realurl);
	//curl_easy_cleanup(curl);

	//if (result != CURLE_OK)
	//{
	//	std::cout << "Failed to connnect to whois.com, please check your network" << std::endl;
	//}

	//return htmlBuffer;
}

// extract desired data
std::string extractOutput(std::string content)
{
	std::string result = "Sorry, no data found";
	size_t pos = content.find("registryData");
	// always prioritize data fetched from ID = registryData
	if (pos != std::string::npos)
	{
		size_t pos_start = content.find(">", pos) + 1;
		size_t pos_stop = content.find("</pre>", pos_start);
		result = content.substr(pos_start, pos_stop - pos_start);
		result = cleanOutput(result);
	}
	else
	{
		pos = content.find("registrarData");
		if (pos != std::string::npos)
		{
			size_t pos_start = content.find(">", pos) + 1;
			size_t pos_stop = content.find("</pre>", pos_start);
			result = content.substr(pos_start, pos_stop - pos_start);
			result = cleanOutput(result);
		}
	}
	return result;
}

// clean the output data
std::string cleanOutput(std::string data)
{
	// first clean the img tags
	size_t pos = data.find("<img");
	while (pos != std::string::npos)
	{
		size_t end = data.find(">", pos);
		data.replace(pos, end - pos + 1, "<image>");
		pos = data.find("<img");
	}
	// replace non-breaking space
	pos = data.find("&nbsp;");
	while (pos != std::string::npos)
	{
		data.replace(pos, 6, " ");
		pos = data.find("&nbsp;");
	}
	// replace less than
	pos = data.find("&lt;");
	while (pos != std::string::npos)
	{
		data.replace(pos, 4, "<");
		pos = data.find("&lt;");
	}
	// replace greater than
	pos = data.find("&gt;");
	while (pos != std::string::npos)
	{
		data.replace(pos, 4, ">");
		pos = data.find("&gt;");
	}
	// replace ampersand
	pos = data.find("&amp;");
	while (pos != std::string::npos)
	{
		data.replace(pos, 5, "&");
		pos = data.find("&amp;");
	}
	// replace double quotation mark
	pos = data.find("&quot;");
	while (pos != std::string::npos)
	{
		data.replace(pos, 6, "\"");
		pos = data.find("&quot;");
	}
	// replace single quotation mark
	pos = data.find("&apos;");
	while (pos != std::string::npos)
	{
		data.replace(pos, 6, "\'");
		pos = data.find("&apos;");
	}
	return data;
}

//size_t write_callback(char* content, size_t size, size_t nmemb, void* user)
//{
//	size_t realsize = size * nmemb;
//	((std::string*)user)->append(content, realsize);
//	return realsize;
//}

#ifdef _DEBUG
void printError()
{
	DWORD errorID = GetLastError();
	std::string lastError = "None";
	if (errorID)
	{
		LPSTR messageBuf = nullptr;
		size_t size = FormatMessageA(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
			NULL, errorID, 0x0800, (LPSTR)&messageBuf, 0, NULL);
		lastError = std::string(messageBuf, size);
		LocalFree(messageBuf);
	}
	std::cout << "Last Error: " << lastError << std::endl;
}
#endif