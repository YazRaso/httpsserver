// create_socket(port_number, ip_address) creates a socket and listens on port_number on ip_address
// requires: 0 <= port_number <= 65535, ip_address must not be null
int create_socket(int port_number, char* ip_address);