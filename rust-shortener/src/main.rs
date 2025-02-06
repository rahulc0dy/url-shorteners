use actix_web::{
    get,
    web::{self, post},
    App, HttpResponse, HttpServer, Responder,
};
use nanoid::nanoid;
use serde::{Deserialize, Serialize};
use sqlx::sqlite::SqlitePool;
use sqlx::Row;

#[derive(Serialize, Deserialize)]
struct ShortenRequest {
    url: String,
}

#[derive(Serialize)]
struct ShortenResponse {
    original_url: String,
    shortened_url: String,
}

async fn shorten_url(
    pool: web::Data<SqlitePool>,
    data: web::Json<ShortenRequest>,
) -> impl Responder {
    let short_key = nanoid!(6);
    let shortened_url = format!("http://localhost:8080/{}", short_key);

    let result = sqlx::query("INSERT INTO urls (short_key, original_url) VALUES (?, ?)")
        .bind(&short_key)
        .bind(&data.url)
        .execute(pool.get_ref())
        .await;

    match result {
        Ok(_) => HttpResponse::Ok().json(ShortenResponse {
            original_url: data.url.clone(),
            shortened_url,
        }),
        Err(_) => HttpResponse::InternalServerError().body("Error saving URL"),
    }
}

#[get("/{short_key}")]
async fn redirect_url(pool: web::Data<SqlitePool>, path: web::Path<String>) -> impl Responder {
    let short_key = path.into_inner();

    let row = sqlx::query("SELECT original_url FROM urls WHERE short_key = ?")
        .bind(&short_key)
        .fetch_optional(pool.get_ref())
        .await;

    match row {
        Ok(Some(record)) => {
            let original_url: String = record.get(0);
            HttpResponse::TemporaryRedirect()
                .append_header(("Location", original_url))
                .finish()
        }
        _ => HttpResponse::NotFound().body("URL not found"),
    }
}

#[actix_web::main]
async fn main() -> std::io::Result<()> {
    let pool = SqlitePool::connect("sqlite:urls.db")
        .await
        .expect("Failed to connect to database");

    HttpServer::new(move || {
        App::new()
            .app_data(web::Data::new(pool.clone()))
            .service(web::resource("/shorten").route(post().to(shorten_url)))
            .service(redirect_url)
    })
    .bind("127.0.0.1:8080")?
    .run()
    .await
}
