# Docker Build environment

This directory contains a setup to build all 3 platforms
(Windows, Linux, MacOS) via Docker environments in a
reproducible way. This allows to verify all three environments
compile and link fine. The resulting binaries are supposed
to actually work, though native build environments
(like Visual Studio or XCode) might produce smaller binaries.

## Just Build

If you "just want to build" then

1. Install [Docker Desktop](https://www.docker.com/products/docker-desktop)
   and make sure it is running
2. `cd docker`
3. `make`

During the very first invocation this can take several minutes
as the involved Docker image is downloaded for the first time.
Subsequent builds will run in a matter of seconds.

Find results in

- `build-lin`
- `build-mac`
- `build-win`

## Background Info

### Linux, Windows, MacOS based on Ubuntu 20.04 Focal Fossa

The targets `lin`, `mac`, and `win` are built based on
the Docker image `twinfan/focal-win-mac-lin-compile-env`.

That image contains

- Ubuntu 20.04 as base image
- CMake 3.19
- Mingw64 8.0, a Windows 64 bit cross compiler based on gcc 10.2,
  based on the [`mmozeiko/mingw-w64`](https://hub.docker.com/r/mmozeiko/mingw-w64) image
- including a local build of a static `libcurl`
  to be included in Mingw64 builds
- clang for MacOS cross compilations, based on the
  [`tpoechtrager/osxcross`](https://github.com/tpoechtrager/osxcross) repository,
  including a 11.1 SDK
- standard Ubuntu compile toolchain based on gcc 9.3

The image was originally built using the Dockerfile provided
as `Dockerfile_Focal_Original`, which takes up to an hour
to fully build from scratch as most software listed above
is actually build from sources.

### Shared Traits

The `Makefile` directs the `lin`, `mac`, `win`, and `bash_focal` targets
to the `focal-win-mac-lin-compile-env` docker image.

On first launch, the local docker images are built

1. by downloading the respective `twinfan` image from Docker Hub,
2. adding the local `build.sh` script into the local image, and
3. tagging it `focal-win-mac-lin-compile-env` resp.
   `bionic-lin-compile-env`.

`build.sh` works as the standard entry point, so when the `Makefile`
launches the docker container, this script is executed inside the
container and triggers off the actual build process by calling `cmake`
and `ninja`.

The make targets `bash_focal` launches the docker container with the 
entrypoint `bash` instead of the build script, so that you can interactively 
work with the container.
