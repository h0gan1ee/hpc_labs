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