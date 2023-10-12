FROM rust:latest as build
WORKDIR /usr/src/sqrt
COPY src/dummy.rs ./src/dummy.rs
COPY Cargo.toml .
RUN cargo build --release --bin empty-app
COPY . .
RUN cargo install --bin sqrt-bin --path . --root /usr/

FROM gcr.io/distroless/cc-debian12
COPY --from=build /usr/bin/sqrt-bin /usr/bin/sqrt-bin
CMD ["sqrt-bin"]