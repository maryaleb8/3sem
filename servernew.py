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
            client_fobj = client_sock.makefile('rw', encoding='utf-8')
            while True:
                line = client_fobj.readline()
                if len(line) == 0:
                    break
                client_fobj.write('Reply: "%s"\n' % line.strip())
                client_fobj.flush()
            client_fobj.close()
            client_sock.shutdown(socket.SHUT_RDWR)
            client_sock.close()
    except KeyboardInterrupt:
        s.close()

if __name__ == '__main__':
    main()
