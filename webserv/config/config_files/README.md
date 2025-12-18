# Format of the config file

## 42 assignment
* possible to start several webservs from one file (fork?)
* define the `interface:port` pairs
* setup default error pages
* setup the max body size for client requests
* rules or configurations for URL/route
	* list accepter HTTP methods for said route
	* HTTP redirection
	* Directory location of the requested files
	* enable/disable directory listing
	* default file to serve when a directory is requested (index, index.html, index.php,...)
	* Uploads from the client are allowed and a storage location is provided
	* CGI execution based upon file extension
		* needed environment variables must be available to the CGI
		* chunked requests must be un-chunked
		* CGI expects EOF at the end of a request body
		* CGI response also closed by EOF
		* CGI must run from the right directory for relative path access
		* at least one CGI (python, rust, C, php,...)

## Caveats
* parse the comments after '#' for the remainder of the line
 
