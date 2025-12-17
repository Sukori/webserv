# Commented template (nginx)

```yaml
# Defines a "virtual server" block. All directives for a specific website or app go in here.
# Your config file parser will need to handle these blocks, maybe mapping them to a "Server" class in C++.
server {
    # Specifies the address and port the server should listen on for incoming connections.
    # Note: NGINX usually uses an IP:port format here (e.g., `listen 80;` or `listen 127.0.0.1:8080;`).
    # Using a hostname like `one.example.com` is less common and relies on DNS resolution at startup.
    # This directly relates to your requirement: "define the interface:port pairs".
    listen        80; # Changed to a more standard example

    # Defines the domain names that this server block will handle.
    # If a request comes in with a "Host" header matching one of these, this block is used.
    server_name   one.example.com  www.one.example.com;

    # Configures logging. This specifies the file where access logs are written and the format to use.
    access_log   /var/log/nginx.access_log  main;

    # Defines how to handle requests based on the URI. This is the most critical part.
    # This specific block handles every request that doesn't match a more specific location block below.
    location / {
        # This group of directives turns NGINX into a reverse proxy. It forwards the request
        # to another server (e.g., a backend application server). For your project, you won't be
        # proxying; you'll be serving files or running CGIs directly. So you'd have directives like
        # 'root' or 'cgi_pass' here instead.
        proxy_pass         http://127.0.0.1/;
        proxy_redirect     off;

        # These headers are modified before sending the request to the backend server.
        proxy_set_header   Host             $host;
        proxy_set_header   X-Real-IP        $remote_addr;
        #proxy_set_header  X-Forwarded-For  $proxy_add_x_forwarded_for;

        # Sets the maximum allowed size for a client request body (e.g., file uploads).
        # This is a key requirement: "setup the max body size for client requests".
        client_max_body_size       10m;
        # Buffer size for the client request body.
        client_body_buffer_size    128k;

        # The following 'proxy_*' and 'client_body_*' directives are advanced settings for
        # controlling timeouts, buffer sizes, and temporary file paths when proxying.
        # While you don't need the proxying part, understanding that you need to manage
        # buffers and timeouts for network I/O is very important.
        client_body_temp_path      /var/nginx/client_body_temp;
        proxy_connect_timeout      70;
        proxy_send_timeout         90;
        proxy_read_timeout         90;
        proxy_send_lowat           12000;
        proxy_buffer_size          4k;
        proxy_buffers              4 32k;
        proxy_busy_buffers_size    64k;
        proxy_temp_file_write_size 64k;
        proxy_temp_path            /var/nginx/proxy_temp;

        # Sets the character set for responses.
        charset  koi8-r;
    }

    # Defines a custom page to be served for a specific HTTP error code.
    # This fulfills the "setup default error pages" requirement.
    error_page  404  /404.html;

    # This location block handles requests for exactly "/404.html".
    # It then serves the actual file from the specified root directory.
    # This is how NGINX avoids an infinite loop if /404.html itself is not found.
    location = /404.html {
        # Sets the root directory from which to serve files for this location.
        root  /spool/www;
    }

    # A location block that matches any URI starting with "/old_stuff/".
    location /old_stuff/ {
        # Rewrites the request URI using a regular expression and sends a permanent redirect (301) to the client.
        # This implements your "HTTP redirection" requirement.
        rewrite   ^/old_stuff/(.*)$  /new_stuff/$1  permanent;
    }

    # A location block for any URI starting with "/download/".
    location /download/ {

        # A security feature to prevent hotlinking. Only allows requests with a Referer header
        # matching these values. If the referer is invalid, it sets the `$invalid_referer` variable.
        valid_referers  none  blocked  server_names  *.example.com;

        # Checks the variable and returns a 403 Forbidden error if it's set.
        if ($invalid_referer) {
            return   403;
        }

        # Another rewrite example. This one changes the file extension of a request internally
        # without telling the client. The 'break' flag stops processing further rewrite rules.
        rewrite ^/(download/.*)/mp3/(.*)\..*$
                /$1/mp3/$2.mp3                   break;

        # This defines the "Directory location of the requested files" for this route.
        root         /spool/www;

        # 'autoindex on;' would enable directory listing, another of your requirements.
        # It's commented out here, but you'd need to parse this directive.
        #autoindex    on;

        # You can have separate access logs for different locations.
        access_log   /var/log/nginx-download.access_log  download;
    }

    # This location block uses a regular expression to match requests for files ending in
    # .jpg, .jpeg, or .gif (the '~*' makes it case-insensitive).
    # This is a good way to apply rules for specific file types.
    location ~* \.(jpg|jpeg|gif)$ {
        root         /spool/www;
        # You can turn off logging for certain requests, like static assets, to reduce I/O.
        access_log   off;
        # Tells the client's browser it can cache this content for 30 days.
        expires      30d;
    }
}
```

