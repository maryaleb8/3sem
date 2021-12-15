#tcp server
import socket

def maun():
    s = socket.socket(socket.AF_INET, socket.SOCK_STREAM, socket.IPPROTO_TCP)
    s.bind(('', 7500))#ассоциируем сокет с портом
    s.listen(10)#очередь и ее размер
    try:
        while True:
            client_sock, client_addr = s.accept()
            print('Connection from:', client_addr)
            req = b''
            while True:
                buf = client_sock.recv(4096)
                if len(buf) == 0:
                    break
                req += buf
            reply = f'Got request with len=(len(req)) } and addr=(client_addr)\n'
            sys.stdout.write(reply)
            client_sock.sendall(reply.encode('utf-8'))
            client_sock.shutdown(socket.SHUT_RDWR)
            client_sock.close()
    except KeyboardInterrupt:
        s.close()

if __name__ == '__main__':
    main()
