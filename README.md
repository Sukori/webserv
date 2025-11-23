# webserv
42 webserv

## Objectives
* Build a webserver in C++98
* Implement sockets
* Implement HTTP requests
* Learn what is modular design and implement it properly

## Design
Looking for a modular approach to simplify functionality and avoid single points breaking the whole app.

### Directory structure
Each module has its own `main` and `Makefile` in order to run "sandboxed" tests with hypothetical inputs.
Only the Makefile at the root of the project is intended for the full compilation of the final project.

.
├── README.md
└── webserv
    ├── core
    │   ├── cgi
    │   │   ├── Cgi.cpp
    │   │   ├── Cgi.hpp
    │   │   ├── Makefile
    │   │   └── test_cgi.cpp
    │   ├── client
    │   │   ├── Client.cpp
    │   │   ├── Client.hpp
    │   │   ├── Makefile
    │   │   └── test_client.cpp
    │   ├── cookies
    │   │   ├── Cookie.hpp
    │   │   ├── Cookies.cpp
    │   │   ├── Makefile
    │   │   └── test_cookies.cpp
    │   ├── http_requests
    │   │   ├── Http.cpp
    │   │   ├── Http.hpp
    │   │   ├── Makefile
    │   │   └── test_http.cpp
    │   ├── server
    │   │   ├── Makefile
    │   │   ├── Server.cpp
    │   │   ├── Server.hpp
    │   │   └── test_server.cpp
    │   └── www
    │       ├── html
    │       │   └── index.html
    │       └── php
    │           └── index.php
    ├── main
    │   ├── Webserv.cpp
    │   └── Webserv.hpp
    └── Makefile

## Production
Explain design choices after finishing the project

## Self-evaluation
Post-mortem

## Resources
assets, if need be

## Bibliography
Helpful documentation we used
* [ Modular Programming – Most Programmers Think They do it. But Hardly Anyone Does! (by HuwsTube)](https://www.youtube.com/watch?v=MYDAAhB1QIo)
* [ C++ Application Architecture - A Mini-Series (by The Cherno)](https://www.youtube.com/watch?v=rUxZ5N77M5E)

