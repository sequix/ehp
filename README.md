# ehp

## What is ehp?

ehp is a HTTP server, which also is my graduation design.

## How to start?

```bash
# To build ehp, make sure you have libgc and libgtest.
# If you are using centos, use the following to install missing libraries:
$ yum install gc-devel gtest-devel

# debug the server with following commands:
$ ./build.sh
$ _output/ehp --debug -c ./config

# run tests
$ ./build.sh test

# clean folder
$ ./build.sh clean

# build release version
$ ./build.sh release
```

## Where did you get the test site in /root?

Generated with [hugo](https://gohugo.io) and [xianmin/hugo-theme-jane](https://github.com/xianmin/hugo-theme-jane)
