import socket
import signal
import os
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

    file_exists = os.path.isfile('web' + filename)
    filename = "/404.html" if not file_exists else filename

    fin = open('web' + filename)
    content = fin.read()
    fin.close()

    if file_exists:
        content = 'HTTP/1.0 200 OK\n\n' + content
    else:
        content = 'HTTP/1.0 404 NOT FOUND\n\n' + content

    # Send HTTP response
    client_socket.sendall(content.encode())
    client_socket.close()   
    
server_socket.close()

