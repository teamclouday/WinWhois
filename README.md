# Whois for Windows  

The logic of this program is quite simple: It connect to the whois website and query requested information and output it in console  

In order to scrap dynamically generated web page content, the program drives a webbrowser control (`IWebBrowser2`) to launch an Internet Explorer at background, navigate to the desired target url and fetch the html content  
Therefore, there's no longer requirements for libcurl static libs. Everything is provided by Windows environment  

------

The main purpose of building this program is that on Windows system, there's no default programs like whois on linux  
More importantly, the [whois](https://docs.microsoft.com/en-us/sysinternals/downloads/whois) program provided by Microsoft is not functioning the same as the one on linux (For example, it cannot query information of an ip address)  

------

Because the program is one execution at a time, so I didn't check for any potential memory leaks. If you find one, please open an Issue  