*This Project has been created as part of the 42 curriculum by pberset and ylabussi*

# WebServ
This project is about writing your own HTTP server.

You will be able to test it with an actual browser.

HTTP is one of the most widely used protocols on the internet.

Understanding its intricacies will be useful, even if you won’t be working on a website.

## Description

### The project
This project is a single threaded program listening simultaneously on multiple interfaces, if specified in the congiguration file.

As a result, there is only one main process that handles connections and communication on all the sockets and responds to the appropriate client.

The server shall not mix the outputs out. Each client must receive exactly what it requested.

There is only one `poll()` function that orchestrates the connections and ensures that the non-blocking `recv()` and `send()` run till the end of their respective tasks.

`errno` shall not be checked after the read/write operations. It is allowed in other parts of the code.

The web server has been successfully tested with gecko-based browsers (firefox and forks) and chromium-based browsers (chrome, brave, ...).

The server shall never crash, even under pressure from `siege -c255 -b -d1 http://127.0.0.1:8080`. Availability must remain 100% and no packets shall be lost.

### Lerning objectives
* Build a webserver in C++98
	* Implement Client / Server model
	* Implement sockets
	* Implement HTTP requests
		* POST, GET, DELETE
		* Request / Response
	* Implement a CGI

### Design
To build this project, we looked for a modular approach.

The goal was to be abl to build the server functionalities in separate containers with little interdependance. This was indeed possible at th beginning of the project, allowing us to progress without fear to break the whole application with a single change.

Corrections and adjustments proved to be doable with reasonable efforts. We found limitations in this design around the end of the development.

As students, we are satisfied with the result and we already seek to progress in architecturing complex applications with improved modularity.

We recognize that we have much to learn in this area.


### Directory structure
Each module had its own `main` and `Makefile` in order to run "sandboxed" tests with hypothetical inputs.

Only the Makefile at the root of the project is intended for the full compilation of the final project.

View full work tree [here](directory_tree.md)

## Instructions
```bash
$ git clone https://github.com/Sukori/webserv.git
$ cd webserv
$ make
$ ./webserv #this will automatically start the server with a default config file
$ ./webserv config/config_files/config1.conf # this will start the server with a custom config file
```

The minimal valid configuration is as such:
```bash
server { # mandatory server block
	listen 127.0.0.1:8081 # interface:port
	root /www/8081 # root directory for the server (relative to the executable directory)
	client_max_body_size 1m # a maximum of 64M is allowed for compilation reasons.
	# Accepts:
	#	*int
	#	*<int>k 'k' upper- or lower-case stands for "kilo" (x1'000)
	#	*<int>m 'm' upper- or lower-case stands for "mega" (x1'000'000)
	location / { # a location is not necessary to let a server start, but it will only reply with 404 not found
		root / # relative to server root
	}
}
```

A more complete valid configuration can be consulted at [default.conf](webserv/config/config_files/default.conf)

## AI instructions
AI has served as a consultant and mentor to discuss parts of the project. It has not written code on our behalf.

The instruction set to command the interaction can be found in [instruction](instruction).

The AI agent was alway requested to check this directory before any interaction.

The conversation history was regularily flushed to limit the token usage and allow for focus on very specific parts of the project, making the AI less prone to "hallucination" and providing simpler answers that let us reflect.

## Bibliography
Helpful documentation we used
* [Modular Programming – Most Programmers Think They do it. But Hardly Anyone Does! (by HuwsTube)](https://www.youtube.com/watch?v=MYDAAhB1QIo)
* [C++ Application Architecture - A Mini-Series (by The Cherno)](https://www.youtube.com/watch?v=rUxZ5N77M5E)
* [Beej's Guide to Network Programming](https://beej.us/guide/bgnet/html/)
* [Hypertext Transfer Protocol -- HTTP/1.1](https://www.rfc-editor.org/rfc/rfc2616)
* [cppReference](https://en.cppreference.com/w/)
* [CGI - Wikipedia](https://en.wikipedia.org/wiki/Common_Gateway_Interface)
* [nginx documentation - config](https://nginx.org/en/docs/example.html)
* [nginx documentation - HTTP CORE MODULE](https://nginx.org/en/docs/http/ngx_http_core_module.html)
* [ipv4 - ipv6 converter](https://dnschecker.org/ipv4-to-ipv6.php)
* [Building an http server from scratch in c++](https://osasazamegbe.medium.com/showing-building-an-http-server-from-scratch-in-c-2da7c0db6cb7)
* [mozilla http reference (headers list, request methods and response status)](https://developer.mozilla.org/en-US/docs/Web/HTTP/Reference)
* [MDN - focus on GET method](https://developer.mozilla.org/en-US/docs/Web/HTTP/Reference/Methods/GET)
* [http.dev 1.1](https://http.dev/1.1)
