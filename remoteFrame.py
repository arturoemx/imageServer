#! /usr/bin/env python
#encoding: utf-8

import socket
import numpy as np
import cv2

class remoteFrame:
    def __init__(self, address='127.0.0.1', port=8888, Mask=None):
        self.address = address
        self.port = port
        self.cl = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        self.cl.connect((self.address, self.port))
        self.msgLength = 10
        if Mask != None:
            b, g, r = cv2.split(Mask)
            self.where = b & 255
            self.where &= g & 255
            self.where &= r & 255
            self.where = 255-self.where
            self.where = 255 - self.where
            self.where=cv2.merge((self.where,self.where,self.where))
            self.Mask = Mask & ~self.where
        else:
            self.Mask = None
    def __del__ (self):
        msg="QUIT      "
        self.cl.sendall(msg)
        msg = self.cl.recv(10)
        if msg[:3] != "BYE":
            print 'No se recibió "BYE" :-(, se recibió |'+msg+'| '
        else:
            print 'remoteFrame: Cerrando conexión'
    def getFrame(self):
        msg="IMG       "
        self.cl.sendall(msg)
        msg = self.cl.recv(16)
        msgb = map(ord, msg)
        rows =(msgb[1]<<8)+msgb[0]+(((msgb[3]<<8)+msgb[2])<<16)
        cols =(msgb[5]<<8)+msgb[4]+(((msgb[7]<<8)+msgb[6])<<16)
        itype =(msgb[9]<<8)+msgb[8]+(((msgb[11]<<8)+msgb[10])<<16)
        size =(msgb[13]<<8)+msgb[12]+(((msgb[15]<<8)+msgb[14])<<16)
        recibidos = size
        img=[]
        while (recibidos != 0):
            buff = self.cl.recv(recibidos)
            recibidos -= len(buff)
            img += buff
        I=np.array(bytearray(img)).reshape(rows,cols,3)
        if self.Mask != None:
            I = (I & self.where) + self.Mask

        return I
