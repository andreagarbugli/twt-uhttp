import socket
import signal
import sys

# Define socket host and port
SERVER_HOST = '0.0.0.0'
SERVER_PORT = 8088

# Create socket
server_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
server_socket.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
server_socket.bind((SERVER_HOST, SERVER_PORT))
server_socket.listen(1)

print(f'Listening on port {SERVER_PORT} ...')

while True:
    # Wait for client connections
    client_socket, client_address = server_socket.accept()

    # Receive data from client
    request = client_socket.recv(1024).decode()

    # Parse HTTP headers
    headers = request.split('\n')
    filename = headers[0].split()[1]

    print(f'The requested file is {filename}')

    # If favicon.ico is requested, skip it
    if filename == '/favicon.ico':
        continue

    # If no file is specified, get index.html
    if filename == '/':
        filename = '/index.html'

    # (optional) Get other HTTP headers
    # in this case, we only print some of them to simplify our output
    for h in headers:
        if 'User-Agent' in h:
            print(h)
        if 'Accept:' in h:
            print(h)

    # Get the content of the index.html file
    fin = open('web' + filename)
    content = fin.read()
    fin.close()

    # Send HTTP response
    content = 'HTTP/1.0 200 OK\n\n' + content
    client_socket.sendall(content.encode())
    client_socket.close()   
    
server_socket.close()
