import socket
import time

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

    # TIMING
    start_time = time.time()

    # Receive data from client
    request = client_socket.recv(1024).decode()
    print(request)

    # Get the content of the index.html file
    fin = open('web/index.html')
    content = fin.read()
    fin.close()

    # Send HTTP response
    content = 'HTTP/1.0 200 OK\n\n' + content
    client_socket.sendall(content.encode())

    # TIMING
    end_time = time.time()
    elapsed = (end_time - start_time) * 1000
    print(f'Elapsed time: {elapsed} ms')

    client_socket.close()   
    
server_socket.close()
