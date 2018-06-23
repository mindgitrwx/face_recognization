
from socket import *
import socket
import os
import time
import sys





def fileNameCreater(flag):
    dte = time.localtime()
    Year = dte.tm_year
    Mon = dte.tm_mon
    Day = dte.tm_mday
    WDay = dte.tm_wday
    Hour = dte.tm_hour
    Min = dte.tm_min
    Sec = dte.tm_sec
    if flag==0:
        imgFileName = str(Year) + '_' + str(Mon) + '_' + str(Day) + '_' + str(Hour) + '_' + str(Min) + '_' + str(Sec) + '.jpg'
    elif flag==1:
        imgFileName = str(Year) + '_' + str(Mon) + '_' + str(Day) + '_' + str(Hour) + '_' + str(Min) + '_' + str(Sec) + '.txt'

    return imgFileName



server_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
server_socket.bind(("", 5001))
server_socket.listen(5)

print("TCPServer Waiting for client on port 5000")

flag=0
while True:

    
    client_socket, address = server_socket.accept()

    print("I got a connection from ", address)

    fileName = fileNameCreater(flag)
    data = None

   # extension = client_socket.recv(1024)
   # extension = int(extension)
    if flag == 0 :        #it's image
        print("I got some image for you!")
        while True:
            img_data = client_socket.recv(1024)
            data = img_data
            if img_data :
                while img_data:
                    print("recving Img...")
                    img_data = client_socket.recv(1024)
                    data += img_data
                else:
                    break

        img_file = open(fileName, "wb")
        print(sys.getsizeof(data))
        img_file.write(data)
        img_file.close()
        print("Finish ")
        flag=1

    elif flag == 1  :  #it's text
        print("I got some text for you!")
        innerText = client_socket.recv(1024)
        print (innerText)
        innerText = str(innerText)
        print (innerText)
        txt_file = open(fileName, 'w')
        txt_file.write(innerText)
        txt_file.close()
        print("Finish ")
        flag=0
    
    
    fileName=""
   


client_socket.close()
print("SOCKET closed... END")
