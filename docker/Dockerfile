FROM twinfan/focal-win-mac-lin-compile-env:latest

USER root

# if you get 404 not found errors then use this line to first get the .image file to build.
# RUN sed -i -e 's/http:\/\/archive.ubuntu.com\/ubuntu\//mirror:\/\/mirrors.ubuntu.com\/mirrors.txt/' /etc/apt/sources.list && apt-get clean && apt-get update

RUN sed -i -e 's/http:\/\/archive.ubuntu.com\/ubuntu\//mirror:\/\/mirrors.ubuntu.com\/mirrors.txt/' /etc/apt/sources.list && apt-get update

# Set clang as default compiler for C and C++
RUN update-alternatives --install /usr/bin/cc cc /usr/bin/clang 100
RUN update-alternatives --install /usr/bin/c++ c++ /usr/bin/clang++ 100

# Install required libraries under Linux.
RUN apt-get install -y --no-install-recommends libasound2-dev && apt-get clean
RUN sudo ldconfig /usr/osxcross/lib

USER xpbuild
