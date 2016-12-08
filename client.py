#! /usr/bin/env python
#encoding: utf-8

import sys
import socket
import cv2

MSG_LENGTH = 10

def getFrame(cl):
	msg = cl.recv(MSG_LENGTH)
	print ("Cliente recibió: '"+msg[:3]+"'")
	if msg[:3] == "SND":
		msg="IMG       "
		cl.sendall(msg)
	print ("Cliente mandó: '"+msg+"'")
	msg = cl.recv(16)
	msgb = map(ord, msg)
	rows =(msgb[1]<<8)+msgb[0]+(((msgb[3]<<8)+msgb[2])<<16)
	cols =(msgb[5]<<8)+msgb[4]+(((msgb[7]<<8)+msgb[6])<<16)
	itype =(msgb[9]<<8)+msgb[8]+(((msgb[11]<<8)+msgb[10])<<16)
	size =(msgb[13]<<8)+msgb[12]+(((msgb[15]<<8)+msgb[14])<<16)
	print ("Cliente recibió: ("+str(cols)+", "+str(rows)+", "+str(itype)+", "+str(size)+")")
	img = cl.recv(size)
	imgb=map(ord,img)
	print ("Cliente recibió: '"+str(imgb)+"'")
	

port = 8888
address = '127.0.0.1'




argc = len (sys.argv)
if argc > 1:
	address = sys.argv[1]
	if argc > 2:
		port = sys.argv[2]

client = socket.socket(socket.AF_INET, socket.SOCK_STREAM)

client.connect((address, port))


cont=0
while True:
	print ("Inside main loop: "+str(cont))
	getFrame(client)
	cont+=1
