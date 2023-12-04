import os
import socket

# Define socket host and port
SERVER_HOST = '0.0.0.0'
SERVER_PORT = 8088

# Create socket
server_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
server_socket.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
server_socket.bind((SERVER_HOST, SERVER_PORT))
server_socket.listen(1)

print('Welcome to uHTTP server: a simple HTTP server in Python')
print(f'Current directory: {os.getcwd()}')
print(f'Listening on port {SERVER_HOST}:{SERVER_PORT} ...')

while True:
    # Wait for client connections
    client_socket, client_address = server_socket.accept()

    # Receive data from client
    request = client_socket.recv(1024).decode()
    print('A new REQUEST is received:')
    print(request)

    # Send HTTP response
    response = 'HTTP/1.1 200 OK\n\nHello from server!'
    client_socket.sendall(response.encode())
    client_socket.close()   
    
server_socket.close()
