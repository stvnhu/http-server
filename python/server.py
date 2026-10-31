import socket
import sys


def main() -> None:
    port = 8080

    if len(sys.argv) > 1:
        port = sys.argv[1]

    with socket.socket() as s:
        s.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
        s.bind(("0", port))
        s.listen()

        while True:
            s2, addr = s.accept()
            with s2:
                buffer = ""
                print(f"Connection with {addr}\n")
                while True:
                    data = s2.recv(1024).decode("ISO-8859-1")
                    buffer = buffer + data
                    if "\r\n" in buffer:
                        break
                print(buffer)
                send_msg = ""
                "HTTP/1.1 200 OK\r\n"
                "Content-Type: text/plain\r\n"
                "Content-Length: 6\r\n"
                "Connection: close\r\n\r\n"
                "Hello!"
                s2.sendall(send_msg.encode("ISO-8859-1"))


if __name__ == "__main__":
    main()
