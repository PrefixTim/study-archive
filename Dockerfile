FROM rust:latest as build
LABEL authors="devel"
WORKDIR /usr/src/sqrt
COPY . .
RUN cargo build --release
CMD ["./target/release/sqrt"]