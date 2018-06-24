import socket
import sys
import os
import time
import datetime

ipadr = "18.222.182.247"
var1 = 'test.png'

def strSlasher(string):
    str = string.encode()
    str.rstrip('\n')
    print(string)
    time.sleep(120)




# connect to server.....
client_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
client_socket.connect((ipadr, 5001))
print("connecting...")

# file open
print("open  " + var1)
file = open(var1, "rb")
img_size = os.path.getsize(var1)
img = file.read(img_size)
file.close()

# save file-change time
mtime = os.path.getmtime(var1)

# save the image in the socket
client_socket.sendall(img)

client_socket.close()
print("Finish Image Send")

# send txt message
client_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
client_socket.connect((ipadr, 5001))
print("connecting...")

print("target file open")
f = open("output.txt", 'rt')

print("reading file..")
innerText = f.read().splitlines()
size = len(innerText)
message = innerText[size-1]
message = message.encode()

print(message)
#strSlasher(innerText)

#print(newtxt)

f.close()

print("text sending")
client_socket.sendall(message)
client_socket.close()

time.sleep(3)

# if file changed, send again
while True:

    if os.path.getmtime(var1) > mtime:
        # connect to server
        client_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        client_socket.connect((ipadr, 5001))
        print("connecting...")

        # file open
        print("open  " + var1)
        file = open(var1, "rb")
        img_size = os.path.getsize(var1)
        img = file.read(img_size)
        file.close()

        # save file-change time
        mtime = os.path.getmtime(var1)

        # save the image in the socket
        client_socket.sendall(img)

        client_socket.close()
        print("Finish Image Send ")

        # send txt message
        client_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        client_socket.connect((ipadr, 5001))
        print("connecting...")

        print("target file open")
        f = open("output.txt", 'rt')

        print("reading file..")
        innerText = f.read().splitlines()
        size = len(innerText)
        message = innerText[size - 1]
        message = message.encode()

        f.close()

        print("text sending")
        client_socket.send(message)
        client_socket.close()

        time.sleep(3)

    else:
        print('nothing to do...')

    # check every 3 second
    time.sleep(3)
