# On The Fly

## Pre-requisites

* libfcgi-dev
* libmagic-dev
* spawn-fcgi

## Build
```bash
git clone https://github.com/lcallarec/onthefly.git
cd onthefly
./configure
make
```

## Run the fast cgi process
```bash
spawn-fcgi -a127.0.0.1 -p9000 -n onthefly
```

## Run a reverse proxy

Exemple of minimal nginx server configuration :

```nginx
server {

    listen 8000;
    server_name localhost;

    location / {
        fastcgi_pass 127.0.0.1:9000;
        fastcgi_param  GATEWAY_INTERFACE  CGI/1.1;
        fastcgi_param  QUERY_STRING       $query_string;
        fastcgi_param  REQUEST_METHOD     $request_method;
        fastcgi_param  REQUEST_URI        $request_uri;
    }
}
```

## Watch

Open your browser at `http://localhost:8000/<urlencoded_image_url>`, where is an url encoded url of an image, as : https%3A%2F%2Fwww.gnome.org%2Fwp-content%2Fuploads%2F2013%2F06%2Fgnome-logos.png.
