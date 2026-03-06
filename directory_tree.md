```
.
├── directory_tree.md
├── README.md
└── webserv
    ├── config
    │   ├── config_files
    │   │   ├── config1.conf
    │   │   ├── default.conf
    │   │   ├── nginx_template_light_commented.md
    │   │   ├── nginx_template_light.md
    │   │   ├── nginx_template_overkill.md
    │   │   └── README.md
    │   ├── Configuration.cpp
    │   ├── Configuration.hpp
    │   ├── helperConfig.cpp
    │   ├── helperConfig.hpp
    │   ├── main.cpp
    │   ├── Makefile
    │   └── Pseudocode
    ├── core
    │   ├── cgi
    │   │   ├── Cgi.cpp
    │   │   ├── Cgi.hpp
    │   │   ├── Cgi.o
    │   │   ├── Makefile
    │   │   ├── req
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
    │   ├── errors
    │   │   ├── ErrorHandler.cpp
    │   │   ├── ErrorHandler.hpp
    │   │   ├── Makefile
    │   │   └── test_errors.cpp
    │   ├── event_loop
    │   │   ├── EventLoop.cpp
    │   │   ├── EventLoop.hpp
    │   │   ├── Makefile
    │   │   ├── test_loop.cpp
    │   │   └── utils
    │   │       └── LoopUtils.hpp
    │   ├── http
    │   │   ├── http
    │   │   ├── Http.cpp
    │   │   ├── Http.hpp
    │   │   ├── Http.o
    │   │   ├── Makefile
    │   │   ├── req
    │   │   ├── res
    │   │   ├── test_http.cpp
    │   │   └── test_http.o
    │   ├── logs
    │   │   ├── logfiles
    │   │   │   ├── access.log
    │   │   │   └── error.log
    │   │   ├── Logger.cpp
    │   │   ├── Logger.hpp
    │   │   ├── Makefile
    │   │   └── test_logger.cpp
    │   ├── server
    │   │   ├── Makefile
    │   │   ├── Server.cpp
    │   │   ├── Server.hpp
    │   │   └── test_server.cpp
    │   └── utils
    │       ├── core
    │       │   ├── NetworkUtils.hpp
    │       │   └── SocketUtils.hpp
    │       ├── http
    │       │   └── PostUtils.hpp
    │       ├── Makefile
    │       ├── shared
    │       │   └── StrUtils.hpp
    │       └── test_StrUtils.cpp
    ├── main
    │   ├── Webserv.cpp
    │   └── Webserv.hpp
    ├── Makefile
    ├── tests
    │   └── integration
    │       └── test_postToCGI.cpp.cpp
    ├── uploads
    │   └── upload0.txt
    └── www
        ├── html
        │   └── index.html
        ├── php
        │   └── index.php
        └── python
            └── index.py
```
26 directories, 71 files
