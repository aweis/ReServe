open! Lwt.Infix;

let startHTTPServer = () => {
  Console.log("Starting ReServer on port 3000");
  let listenAddress = Unix.(ADDR_INET(inet_addr_loopback, 3000));
  Lwt.async(() =>
    Lwt_io.establish_server_with_client_socket(
      listenAddress,
      Httpaf_lwt_unix.Server.create_connection_handler(
        ~config=?None,
        ~request_handler=Http1_handler.request_handler,
        ~error_handler=Http1_handler.error_handler,
      ),
    )
    >>= (_server => Lwt.return_unit)
  );
  let (forever, _) = Lwt.wait();
  forever;
};

let () = {
  Sys.(set_signal(sigpipe, Signal_ignore));
  Lwt_main.run(Lwt.join([startHTTPServer()]));
};
