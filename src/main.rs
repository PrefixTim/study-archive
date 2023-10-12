// #![feature(test)]

use std::net::SocketAddr;
use axum::http::StatusCode;
use axum::Router;
use axum::routing::get;

mod mysqrt;

#[tokio::main]
async fn main() -> Result<(), Box<dyn std::error::Error>> {
    let addr = SocketAddr::from(([127, 0, 0, 1], 3000));

    let app = Router::new().route("/", get(health));

    axum::Server::bind(&addr).serve(app.into_make_service()).await.unwrap();
    Ok(())
}

async fn health() -> StatusCode {
    StatusCode::OK
}