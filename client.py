import socket
import sys
import os
import time
import datetime

var1 = 'cEdDG.png'

# repeat until file created 
while True:
	try:
		# connect to server.....
		client_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
		client_socket.connect(("18.191.105.248", 5001))
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

		#send txt message
		client_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
		client_socket.connect(("18.191.105.248", 5001))
		print("connecting...")  
		
		f = open("output.txt", 'r')
		line = (int)(f.read())
		f.close()
		
		clinet_socket.send(line)
		client_socket_close()
				
		break
	except:
		print("file is not created... )
		time.sleep(3)





time.sleep(3)

#if file changed, send again
while True:

	if os.path.getmtime(var1) > mtime:
		#connect to server			
		client_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)		
		client_socket.connect(("18.191.105.248", 5001))
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
		#send txt message
		client_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
		client_socket.connect(("18.191.105.248", 5001))
		print("connecting...")  
		
		f = open("output.txt", 'r')
		line = (int)(f.read())
		f.close()
		
		clinet_socket.send(line)
		client_socket_close()

	else: 
		print('nothing to do...')
		

	#check every 3 second
	time.sleep(3)
	
		




