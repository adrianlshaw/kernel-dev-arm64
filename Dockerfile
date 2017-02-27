FROM ubuntu:16.04
RUN apt-get update && apt-get -y install curl cargo make gcc-aarch64-linux-gnu qemu-system-arm
RUN curl https://sh.rustup.rs -sSf | sh -s -- --default-toolchain nightly -y
RUN bash -c "source ~/.cargo/env; rustup target add aarch64-unknown-linux-gnu"
RUN useradd -ms /bin/bash dev
ENV PATH="/root/.cargo/bin:$PATH"
ADD ./fvp /home/dev/fvp/
WORKDIR /home/dev/
RUN chown -R dev /home/dev/fvp/*
RUN echo "source ~/.cargo/env" >> ~/.bashrc
ADD * /home/dev/
ENTRYPOINT ["/bin/bash"]
