// create_socket(port_number, ip_address) binds a socket and listens on port_number on ip_address
// accepting up to and including max_connection_requests
// requires: 0 <= port_number <= 65535, ip_address must not be null
int create_server_socket(int port_number, const char* ip_address, int max_connection_requests);