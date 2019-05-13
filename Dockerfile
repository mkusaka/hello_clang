FROM ubuntu:xenial-20181005

RUN apt update -y&& apt upgrade -y && apt install gcc make git binutils libc6-dev -y && apt clean

WORKDIR /home/src/
