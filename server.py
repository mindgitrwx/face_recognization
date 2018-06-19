
from socket import *
import socket
import os
import time
import sys





def fileName():
    dte = time.localtime()
    Year = dte.tm_year
    Mon = dte.tm_mon
    Day = dte.tm_mday
    WDay = dte.tm_wday
    Hour = dte.tm_hour
    Min = dte.tm_min
    Sec = dte.tm_sec
    imgFileName = str(Year) + '_' + str(Mon) + '_' + str(Day) + '_' + str(Hour) + '_' + str(Min) + '_' + str(
        Sec) + '.jpg'
    return imgFileName



server_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
server_socket.bind(("", 5001))
server_socket.listen(5)

print("TCPServer Waiting for client on port 5000")

while True:


    client_socket, address = server_socket.accept()

    print("I got a connection from ", address)

    data = None


    while True:
        img_data = client_socket.recv(1024)
        data = img_data
        if img_data:
            while img_data:
                print("recving Img...")
                img_data = client_socket.recv(1024)
                data += img_data
            else:
                break



    img_fileName = fileName()
    img_file = open(img_fileName, "wb")
    print("finish img recv")
    print(sys.getsizeof(data))
    img_file.write(data)
    img_file.close()
    print("Finish ")




client_socket.close()
print("SOCKET closed... END")
