# Lab 3

## Build

To build single-threaded version of all source files, use command:

```bash
make
```

Similarly, as for the multi-threaded version:

```
make FLAG=-DMULTI
```

Build artifacts are in `./target` directory.

## Expected behaviours

The program `socket_server_demo` will listen on port 11451 for HTTP request, and will respond with text `Hello World!` after waiting for ~=3s. Obviously, if a new request income during wait, the single-threaded version will handle it only after current wait, while the multi-threaded version is non-blocking.

Unfortunately, as for the multi-threaded version, if you terminate the process using `^C` (the `INT` signal), the parent process will still be alive. So you might need to kill the process by hand using `killall -INT socket_server_demo`

## Test

Currently, both single-threaded and multi-threaded versions function normally.

To test the server, run the program and test with curl using following command:

```bash
curl -vvv --output - localhost:11451
```

If the server works normally, the output should be similar to:

```
* Uses proxy env variable NO_PROXY == 'localhost,127.0.0.1'
*   Trying 127.0.0.1:11451...
* Connected to localhost (127.0.0.1) port 11451 (#0)
> GET / HTTP/1.1
> Host: localhost:11451
> User-Agent: curl/7.79.1
> Accept: */*
> 
* Mark bundle as not supporting multiuse
< HTTP/1.1 200 OK
< Content-Length: 13
< Content-Type: text/html; charset=utf-8
< 
* Connection #0 to host localhost left intact
Hello World!%
```