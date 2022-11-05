#include <iostream>

#include "mysql_connection.h"
#include <cppconn/driver.h>
#include <cppconn/exception.h>
#include <cppconn/resultset.h>
#include <cppconn/statement.h>

#include <restinio/all.hpp>

#include "function.hpp"


template <typename RESP>
RESP
init_resp(RESP resp)
{
	resp.append_header(restinio::http_field::server, "RESTinio sample server /v.0.2");
	resp.append_header_date_field();

	return resp;
}

using router_t = restinio::router::express_router_t<>;

auto create_request_handler()
{
	auto router = std::make_unique< router_t >();

	router->http_get(
		"/",
		[](auto req, auto){
			init_resp(req->create_response())
				.append_header(restinio::http_field::content_type, "text/plain; charset=utf-8")
				.set_body("not implemented")
				.done();

			return restinio::request_accepted();
		});

	 router->non_matched_request_handler(
            [](auto req){
                string s;
                try {
                    s = handle(req->body());
                } catch(exception &e) {
                    s = string(e.what());
                }
                return
                    init_resp(req->create_response())
                        .set_body(s)
                        .done();

            });

	return router;
}

int main()
{
	using namespace std::chrono;

	try {
		using traits_t =
			restinio::traits_t<
				restinio::asio_timer_manager_t,
				restinio::single_threaded_ostream_logger_t,
				router_t >;

		restinio::run(
			restinio::on_this_thread<traits_t>()
				.port(8080)
				.address("0.0.0.0")
				.request_handler(create_request_handler()));
	} catch(const std::exception & ex) {
		std::cerr << "Error: " << ex.what() << std::endl;
		return 1;
	}

	return 0;
}
