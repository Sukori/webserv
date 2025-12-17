# Config template (nginx)
```yaml
# This is the main block for a virtual server.
# In the C++ parser, this maps to a 'Configuration' class.
server {
	# server name is relevant in case of the hosts file of the machine we run on has the DNS entry
	# Only for real life scenario
	# server_name example.com www.example.com; # we don't need it for this project

    # Listen on a specific IP and port. For a local server, 127.0.0.1 (localhost)
    # is perfect. 8080 is a common port for development.
    # Requirement: "define the interface:port pairs"
    listen 127.0.0.1:8080; # and 8081, 8082, 8083 for instance

    # The root directory for files. All file paths in location blocks
    # will be relative to this, unless they have their own root.
    # Requirement: "Directory location of the requested files"
    root /www;

    # Sets the default file to serve if a directory is requested.
    # Your server should try these in order.
    # Requirement: "default file to serve when a directory is requested"
    index index.html index.htm index.php;

    # Path for your server's log files.
    access_log /core/logs/logfiles/access.log;
	error_log /core/logs/logfiles/error.log

    # Sets the maximum allowed size of a client request body (e.g., for file uploads).
    # Requirement: "setup the max body size for client requests"
    client_max_body_size 10m; # 10 megabytes

    # Specifies a custom page for a given error code.
    # Requirement: "setup default error pages"
    error_page 404 /404_custom.html;

    # --- Location Blocks (Routing Rules) ---
    # In C++, each 'location' could be a 'RouteConfig' or 'LocationConfig' object
    # within your 'ServerConfig'.

    # A default location for any request not matched by other blocks.
    location / {
        # List of HTTP methods this route will accept.
        # Requirement: "list accepter HTTP methods for said route"
        limit_except GET POST;

        # Enable or disable directory listing for this route.
        # Requirement: "enable/disable directory listing"
        autoindex on;
    }

    # Redirects any request from /old_stuff/ to /new_stuff/
    # This is a permanent (301) redirect.
    # Requirement: "HTTP redirection"
    location /old_stuff/ {
		
		# rewrite is too complex
        rewrite ^/old_stuff/(.*)$ /new_stuff/$1 permanent;

		# return is simpler, straightforward. Choose one for each route if wanted
		return 301 /new_stuff.html; # permanent redirect
		return 403; # stop and return forbidden if applicable

    }

    # A location for handling file uploads.
    # Requirement: "Uploads from the client are allowed and a storage location is provided"
    location /upload/ {
        # Only allow POST requests for uploading.
        limit_except POST;

        # You would need a custom directive to specify the upload path. NGINX uses
        # more complex logic here, but for your server, a simple custom directive is perfect.
        upload_path /uploads;
		
		# root of the route
		# appends the requested uri to the root path
		root /uploads/;

		# alias of the route
		# replaces the location part of the requested uri by the alias path
		alias /uploads/;

		# checks for a file sequence and returns the first one found
		# first file of uri request.
		# if not found first directory.
		# last if nothing found internal redirection
		# for instance on request /image, first looks for a file "image", then a directory "image/(index.html)" and last goes to /www/index.html in our case
		try_files $uri $uri/ /www/index.html
    }

    # A location for handling CGI scripts.
    # Matches any request ending in .py
    # Requirement: "CGI execution based upon file extension"
    location ~ /core/cgi/\.py$ {

		# Set environment variables to pass to the server
		cgi_param SCRIPT_FILENAME /path/to/script.py;

        # This is where your server's logic would kick in to execute the Python script.
        # You'd need custom directives to configure it, for example:
        cgi_pass /usr/bin/python3; # The interpreter to use
    }
}
```

