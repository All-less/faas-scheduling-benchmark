use warp::Filter;

extern crate handler;


#[tokio::main]
async fn main() {
    // POST /invoke
    let invoke = warp::path!("invoke")
        .and(warp::post())
        .and(warp::body::bytes())
        .map(|body: bytes::Bytes| {
            match String::from_utf8((&body[..]).to_vec()) {
                Err(error) => error.to_string(),
                Ok(s) => handler::handle(s)
            }
        });

    // POST /2016-08-15/proxy/<service-name>/<function-name>/
    let http_invoke = warp::path!("2016-08-15" / "proxy")
        .and(warp::post())
        .and(warp::path::param())
        .and(warp::path::param())
        .and(warp::body::bytes())
        .map(|service: String, function: String, body: bytes::Bytes| {
            match String::from_utf8((&body[..]).to_vec()) {
                Err(error) => error.to_string(),
                Ok(s) => handler::handle(s)
            }
        });

    // POST /*
    let wildcard = warp::post()
        .and(warp::path::tail())
        .and(warp::body::bytes())
        .map(|tail: warp::filters::path::Tail, body: bytes::Bytes| {
            match String::from_utf8((&body[..]).to_vec()) {
                Err(error) => error.to_string(),
                Ok(s) => handler::handle(s)
            }
        });

    let routes = invoke
        .or(http_invoke)
        .or(wildcard);
    println!("listening on 0.0.0.0:9000");
    warp::serve(routes)
        .run(([0, 0, 0, 0], 9000))
        .await;
}
