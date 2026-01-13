# webserv
42 webserv

## Objectives
* Build a webserver in C++98
	* Implement Client / Server model
	* Implement sockets
	* Implement HTTP requests
		* POST, GET, DELETE
		* Request / Response
	* Implement a CGI
* Debugging and validation
	* Ensure modular tests for each class to verify correctness independently in sandboxed environments
* Learn what is modular design and implement it properly

## Design
Looking for a modular approach to simplify functionality and avoid single points breaking the whole app.

### Directory structure
Each module has its own `main` and `Makefile` in order to run "sandboxed" tests with hypothetical inputs.
Only the Makefile at the root of the project is intended for the full compilation of the final project.

View full work tree [here](directory_tree.md)

### Overview
How modules interact with each other

## Production
Documenting the progression and next steps here in addition from the Issues / Kanban from github
- PBE server start + config parsing

## Usage
```bash
$ make
$ ./webserv "config file here"
```

## Self-evaluation
Post-mortem

## Resources
assets, if need be

## Bibliography
Helpful documentation we used
* [Modular Programming – Most Programmers Think They do it. But Hardly Anyone Does! (by HuwsTube)](https://www.youtube.com/watch?v=MYDAAhB1QIo)
* [C++ Application Architecture - A Mini-Series (by The Cherno)](https://www.youtube.com/watch?v=rUxZ5N77M5E)
* [Beej's Guide to Network Programming](https://beej.us/guide/bgnet/html/)
* [Hypertext Transfer Protocol -- HTTP/1.1](https://www.rfc-editor.org/rfc/rfc2616)
* [cppReference](https://en.cppreference.com/w/)$
* [Building an http server from scratch in c++](https://osasazamegbe.medium.com/showing-building-an-http-server-from-scratch-in-c-2da7c0db6cb7)
