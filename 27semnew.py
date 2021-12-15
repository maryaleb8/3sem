#это клиент
#nc -C 81.5.88.117 7500

import socket
import sys
import select

SRV_ADDR = ('81.5.88.117', 7500)

def main():
    #create a TCP socket
    conn = socket.socket(socket.AF_INET, socket.SOCK_STREAM, socket.IPPROTO_TCP)
    #establish connection to the server (TODO: configure timeout handling)
    conn.connect(SRV_ADDR)
    conn_fobj = conn.makefile('rw', encoding='utf-8')
    #create poll object for stdin and socket
    stdin_fd = sys.stdin.fileno()
    poll_array = select.poll()
    poll_array.register(sys.stdin, select.POLLIN)
    poll_array.register(conn, select.POLLIN)
    #handle input and incoming data
    running = True
    try:
        while running:
            for fd, event in poll_array.poll():
                if fd == stdin_fd:
                    line = sys.stdin.readline().strip()
                    conn_fobj.write(line)
                    conn_fobj.flush()
                else:
                    if event == select.POLLIN:
                        line = conn_fobj.readline()
                        sys.stdout.write(line)
                    else:
                        print('Disconnected')
                        running = False
                        break
    except KeyboardInterrupt:
        pass
    #we re done, close connection
    conn.shutdown(socket.SHUT_RDWR)
    conn.close()


if __name__ == '__main__':
    main()
