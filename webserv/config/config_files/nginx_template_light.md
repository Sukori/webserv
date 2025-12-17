# Config template (nginx)
```yaml
server {

    # --- Base Block ---
    listen 127.0.0.1:8080; # and 8081, 8082, 8083 for instance

    root /www;

    index index.html index.htm index.php;

    access_log /core/logs/logfiles/info.log;

    client_max_body_size 10m; # 10 megabytes

    error_page 404 /404_custom.html;


    # --- Location Blocks (Routing Rules) ---
    location / {
        limit_except GET POST;

        autoindex on;
    }

    location /old_stuff/ {
        rewrite ^/old_stuff/(.*)$ /new_stuff/$1 permanent;
    }

    location /upload/ {
        limit_except POST;

        upload_path /uploads;
    }

    location ~ /core/cgi/\.py$ {
        cgi_pass /usr/bin/python3; # The interpreter to use
    }
}
```

