FROM alpine:3.15.4
LABEL maintainer="Theo Jacquelin <thjacque.42@gmail.com>"
RUN apk --no-cache add make gcc g++ musl-dev binutils autoconf automake libtool pkgconfig check-dev file patch sudo