# Config template (nginx)
```yaml
# This is the main block for a virtual server.
# In the C++ parser, this maps to a 'Configuration' class.
server {
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

    # Path for your server's access log file.
    access_log /core/logs/logfiles/info.log;

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
        rewrite ^/old_stuff/(.*)$ /new_stuff/$1 permanent;
    }

    # A location for handling file uploads.
    # Requirement: "Uploads from the client are allowed and a storage location is provided"
    location /upload/ {
        # Only allow POST requests for uploading.
        limit_except POST;

        # You would need a custom directive to specify the upload path. NGINX uses
        # more complex logic here, but for your server, a simple custom directive is perfect.
        upload_path /uploads;
    }

    # A location for handling CGI scripts.
    # Matches any request ending in .py
    # Requirement: "CGI execution based upon file extension"
    location ~ /core/cgi/\.py$ {
        # This is where your server's logic would kick in to execute the Python script.
        # You'd need custom directives to configure it, for example:
        cgi_pass /usr/bin/python3; # The interpreter to use
    }
}
```

