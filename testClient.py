#! /usr/bin/env python
#encoding: utf-8

import sys
import socket
import numpy as np
import cv2

MSG_LENGTH = 10

class remoteFrame:
    def __init__(self, address='127.0.0.1', port=8888):
        self.address = address
        self.port = port
        self.cl = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        self.cl.connect((self.address, self.port))
    def getFrame(self):
        msg="IMG       "
        self.cl.sendall(msg)
        print ("Cliente mandó: '"+msg+"'")
        msg = self.cl.recv(16)
        msgb = map(ord, msg)
        rows =(msgb[1]<<8)+msgb[0]+(((msgb[3]<<8)+msgb[2])<<16)
        cols =(msgb[5]<<8)+msgb[4]+(((msgb[7]<<8)+msgb[6])<<16)
        itype =(msgb[9]<<8)+msgb[8]+(((msgb[11]<<8)+msgb[10])<<16)
        size =(msgb[13]<<8)+msgb[12]+(((msgb[15]<<8)+msgb[14])<<16)
        print ("Cliente recibió: ("+str(cols)+", "+str(rows)+", "+str(itype)+", "+str(size)+")")
        recibidos = size
        img=[]
        while (recibidos != 0):
            buff = self.cl.recv(recibidos)
            recibidos -= len(buff)
            img += buff
        I=np.array(bytearray(img)).reshape(rows,cols,3)
        return I
    

port = 8888
address = '127.0.0.1'




argc = len (sys.argv)
if argc > 1:
    address = sys.argv[1]
    if argc > 2:
        port = sys.argv[2]

rF=remoteFrame(address, port)

cv2.namedWindow('MyWindow')

while cv2.waitKey(1) == -1: 
    img=rF.getFrame()
    cv2.imshow('MyWindow', img)
