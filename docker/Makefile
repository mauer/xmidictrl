platforms=win mac lin
container_focal=xmidictrl-compile-env

all: $(platforms)

.image_focal: Dockerfile
#	docker image pull $(container_focal)
	docker build . -t $(container_focal) -f Dockerfile
	docker inspect -f "{{ .ID}}" $(container_focal) > .image_focal

bash_focal: .image_focal
	docker run -it --rm -v "$(realpath ..):/build" --entrypoint bash $(container_focal)

lin win mac: .image_focal
	docker run -i --rm -v "$(realpath ..):/build" --entrypoint docker/build.sh $(container_focal) $@
	
.PHONY: $(platforms) bash_focal
