open Httpaf;

let redirect_handler: (Unix.sockaddr, Reqd.t) => unit =
  (_client_address, request_descriptor) => {
    let response =
      Response.create(
        ~headers=
          Headers.of_list([
            ("Location", "https://localhost:9443"),
            ("Connection", "close"),
          ]),
        `Moved_permanently,
      );
    Reqd.respond_with_string(request_descriptor, response, "");
  };

let redirect_error_handler:
  (
    Unix.sockaddr,
    ~request: Request.t=?,
    _,
    Headers.t => Body.t([ | `write])
  ) =>
  unit =
  (_client_address, ~request as _=?, _error, start_response) => {
    let response_body = start_response(Headers.empty);
    Body.close_writer(response_body);
  };

let request_handler: (Unix.sockaddr, Reqd.t) => unit =
  (_client_address, request_descriptor) => {
    let request = Reqd.request(request_descriptor);
    let response_content_type =
      switch (Headers.get(request.headers, "Content-Type")) {
      | Some(request_content_type) => request_content_type
      | None => "text/html"
      };
    let response_body = "Welcome to ReServer HTTP/1.1 connection";
    let response =
      Response.create(
        ~headers=
          Headers.of_list([
            ("content-type", response_content_type),
            (
              "Content-Length",
              String.length(response_body) |> string_of_int,
            ),
          ]),
        `OK,
      );
    Reqd.respond_with_string(request_descriptor, response, response_body);
  };

let error_handler:
  (
    Unix.sockaddr,
    ~request: Request.t=?,
    _,
    Headers.t => Body.t([ | `write])
  ) =>
  unit =
  (_client_address, ~request as _=?, _error, start_response) => {
    let response_body = start_response(Headers.empty);
    Body.close_writer(response_body);
  };
