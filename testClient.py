#! /usr/bin/env python
#encoding: utf-8

import sys
import numpy as np
import cv2
from remoteFrame import *

port = 8888
address = '127.0.0.1'
Mask = None

argc = len (sys.argv)
if argc > 1:
    address = sys.argv[1]
    if argc > 2:
        port = int(sys.argv[2])
        if argc > 3:
            Mask = cv2.imread(sys.argv[3])

rF=remoteFrame(address, port, Mask)

cv2.namedWindow('MyWindow')
img =cv2.Mat()
while cv2.waitKey(1) == -1: 
    img=rF.getFrame()
    cv2.imshow('MyWindow', img)
