```
.
├── directory_tree.md
├── README.md
└── webserv
    ├── config
    │   └── default.conf
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
    │   ├── http_requests
    │   │   ├── Http.cpp
    │   │   ├── Http.hpp
    │   │   ├── Makefile
    │   │   └── test_http.cpp
    │   ├── logs
    │   │   ├── error.log
    │   │   ├── logfiles
    │   │   │   ├── debug.log
    │   │   │   └── info.log
    │   │   ├── Logger.cpp
    │   │   ├── Logger.hpp
    │   │   ├── Makefile
    │   │   └── test_logger.cpp
    │   ├── server
    │   │   ├── Makefile
    │   │   ├── Server.cpp
    │   │   ├── Server.hpp
    │   │   └── test_server.cpp
    │   ├── utils
    │   │   ├── Makefile
    │   │   ├── NetworkUtils.hpp
    │   │   ├── PostUtils.hpp
    │   │   ├── SocketUtils.hpp
    │   │   ├── StrUtils.hpp
    │   │   └── test_StrUtils.cpp
    │   └── www
    │       ├── html
    │       │   └── index.html
    │       └── php
    │           └── index.php
    ├── main
    │   ├── Webserv.cpp
    │   └── Webserv.hpp
    ├── Makefile
    └── tests
        └── integration
            └── test_postToCGI.cpp.cpp
```