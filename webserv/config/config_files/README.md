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

## NGINX template (server only)

```
server {
    listen        one.example.com;
    server_name   one.example.com  www.one.example.com;

    access_log   /var/log/nginx.access_log  main;

    location / {
        proxy_pass         http://127.0.0.1/;
        proxy_redirect     off;

        proxy_set_header   Host             $host;
        proxy_set_header   X-Real-IP        $remote_addr;
        #proxy_set_header  X-Forwarded-For  $proxy_add_x_forwarded_for;

        client_max_body_size       10m;
        client_body_buffer_size    128k;

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

        charset  koi8-r;
    }

    error_page  404  /404.html;

    location = /404.html {
        root  /spool/www;
    }

    location /old_stuff/ {
        rewrite   ^/old_stuff/(.*)$  /new_stuff/$1  permanent;
    }

    location /download/ {

        valid_referers  none  blocked  server_names  *.example.com;

        if ($invalid_referer) {
            #rewrite   ^/   http://www.example.com/;
            return   403;
        }

        #rewrite_log  on;

        # rewrite /download/*/mp3/*.any_ext to /download/*/mp3/*.mp3
        rewrite ^/(download/.*)/mp3/(.*)\..*$
                /$1/mp3/$2.mp3                   break;

        root         /spool/www;
        #autoindex    on;
        access_log   /var/log/nginx-download.access_log  download;
    }

    location ~* \.(jpg|jpeg|gif)$ {
        root         /spool/www;
        access_log   off;
        expires      30d;
    }
}
```

