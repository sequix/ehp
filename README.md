# ehp

## What is ehp?

ehp is a HTTP server, which also is my graduation design.

## How to develop?

```bash
# First, you need make sure you have following package on your system:
gc-devel
gtest-devel

# debug the server with following commands:
$ source switch.sh
$ ./build.sh
$ ehp-test

# run tests with following command
$ ./build.sh test

# Clean _output and so on, with:
$ ./build.sh clean
```

## Where did you get the test site in /root?

Generated with [hugo](https://gohugo.io) and [xianmin/hugo-theme-jane](https://github.com/xianmin/hugo-theme-jane)
