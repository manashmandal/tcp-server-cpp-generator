import socket
import random


def main():
    server_host = "127.0.0.1"  # Replace with the server IP address if needed
    server_port = 5000

    while True:
        # Create a TCP/IP socket
        with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as client_socket:
            client_socket.connect((server_host, server_port))

            # Send data to the server
            data = "Hello, server! - " + str(random.randint(0, 100))
            client_socket.sendall(data.encode())

            # Receive response from the server
            response = client_socket.recv(1024)
            print(f"Received from server: {response.decode()}")


if __name__ == "__main__":
    main()