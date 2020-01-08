# Whois for Windows  

This program is quite simple: It connect to the whois website and query requested information and output it in console  

The main purpose of building this program is that on Windows system, there's no default programs like whois on linux  
More importantly, the [whois](https://docs.microsoft.com/en-us/sysinternals/downloads/whois) provided by Microsoft is not functioning the same as the one on linux (For example, it cannot query information of an ip address)  

Program Dependencies:  
1. Ws2_32.lib  
2. Wldap32.lib  
3. crypt32.lib  
4. libcurl.lib  

They're all statically built into the program, so no external dlls are needed  
For actual executables, see Release page  