FROM ubuntu:16.04
RUN apt-get update && apt-get -y install make gcc-aarch64-linux-gnu qemu-system-arm
RUN useradd -ms /bin/bash dev
ADD * /home/dev/
ADD fvp /home/dev/fvp/
WORKDIR /home/dev/
RUN chown -R dev /home/dev/fvp/*
USER dev
