FROM ubuntu:bionic

RUN apt-get update -y && \
    apt-get install -yq libmysqlcppconn7v5 libboost-all-dev
