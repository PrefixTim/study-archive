// #![feature(test)]

use std::net::SocketAddr;
use axum::http::StatusCode;
use axum::{Json, Router};
use axum::routing::get;
use chrono::{DateTime, Local};

mod mysqrt;

#[tokio::main]
async fn main() -> Result<(), Box<dyn std::error::Error>> {
    let addr = SocketAddr::from(([0, 0, 0, 0], 80));

    let app = Router::new()
        .route("/", get(health))
        .route("/time", get(time_json));

    axum::Server::bind(&addr).serve(app.into_make_service()).await.unwrap();
    Ok(())
}

async fn health() -> StatusCode {
    StatusCode::OK
}

async fn time_json() -> Json<DateTime<Local>>  {
    Json(chrono::offset::Local::now())
}