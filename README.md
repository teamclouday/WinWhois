# Whois for Windows  

The logic of this program is quite simple: It connects to the [whois](https://www.whois.com/) website, queries requested information and outputs the content in console  

In order to crawl dynamically generated web page content, the program drives a webbrowser control (`IWebBrowser2`) to launch an Internet Explorer at background, navigatse to the desired target url and fetches the html content  
Therefore, there's no longer requirements for libcurl static libs. Everything is already provided by Windows environment  

------

The main purpose of building this program is that on Windows system, there's no default programs like whois on linux  
More importantly, the [whois](https://docs.microsoft.com/en-us/sysinternals/downloads/whois) program provided by Microsoft is not functioning the same as the one on linux (For example, it cannot query information of an ip address)  

------

Because the program is one execution at a time, I didn't check for any potential memory leaks. If you find one, please post an Issue  

Another potential problem is the url  
This program by default uses UTF-8, so if the input url contains unicode characters, it may not be successfully parsed to be a valid target url. Currently, the program can only handle basic english characters or ip address. Anyone is welcome to contribute an url parser  

------

### To try the program, see the Release page  
