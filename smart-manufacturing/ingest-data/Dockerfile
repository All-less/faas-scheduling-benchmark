# build binary
FROM localhost/smart-manufacturing/build-env:latest AS build

WORKDIR /opt

COPY . .

RUN mkdir build && \
    cmake -B build/ -S . -DCMAKE_TOOLCHAIN_FILE=./vcpkg/scripts/buildsystems/vcpkg.cmake && \
    make


# copy binary to a runtime image
FROM localhost/smart-manufacturing/runtime-env:latest

WORKDIR /opt

COPY --from=build /opt/main .

EXPOSE 8080

ENTRYPOINT [ "/opt/main" ]
