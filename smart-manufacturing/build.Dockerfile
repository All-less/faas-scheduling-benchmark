FROM ubuntu:bionic

WORKDIR /opt

RUN apt-get update -y && \
    apt-get install -yq git build-essential tar curl zip unzip cmake autoconf libtool pkg-config libmysqlcppconn-dev libboost-all-dev && \
    git clone https://github.com/microsoft/vcpkg && \
    ./vcpkg/bootstrap-vcpkg.sh && \
    ./vcpkg/vcpkg install http-parser fmt restinio

