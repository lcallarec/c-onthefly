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

## Run
```bash
spawn-fcgi -a127.0.0.1 -p9000 -n onthefly
```