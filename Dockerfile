FROM ubuntu:16.04
RUN apt-get update && apt-get -y install rustc cargo make gcc-aarch64-linux-gnu qemu-system-arm
RUN curl https://sh.rustup.rs -sSf | sh -s -- --default-toolchain nightly --default-host aarch64-unknown-linux-gnu -y
RUN useradd -ms /bin/bash dev
ADD * /home/dev/
ADD src /home/dev/
ADD fvp /home/dev/fvp/
WORKDIR /home/dev/
RUN chown -R dev /home/dev/fvp/*
ENTRYPOINT ["/bin/bash"]
