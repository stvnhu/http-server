import socket
import sys


def main() -> None:

    host = "127.0.0.1"
    port = 8080

    if len(sys.argv) > 1:
        host = sys.argv[1]

    if len(sys.argv) > 2:
        port = int(sys.argv[2])

    s = socket.socket()
    s.connect((host, port))
    s.sendall("GET / HTTP/1.1\r\n\r\n".encode("ISO-8859-1"))
    data = s.recv(1024).decode("ISO-8859-1")
    print(data)
    s.close()


if __name__ == "__main__":
    main()
