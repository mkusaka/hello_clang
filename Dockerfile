FROM ubuntu:xenial-20181005

RUN apt update -y&& apt upgrade -y && apt install gcc make git binutils libc6-dev gdb software-properties-common -y && add-apt-repository ppa:neovim-ppa/stable -y &&  apt update && apt install neovim -y && apt clean

WORKDIR /home/src/
