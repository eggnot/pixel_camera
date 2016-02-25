#!/usr/bin/env python

from __future__ import print_function

import os, sys, random, time

#gpio settings
import RPi.GPIO as GPIO
GPIO.setmode(GPIO.BCM)
GPIO.setup(26, GPIO.IN, pull_up_down = GPIO.PUD_UP)
GPIO.setup(19, GPIO.IN, pull_up_down = GPIO.PUD_UP)
GPIO.setup(13, GPIO.IN, pull_up_down = GPIO.PUD_UP)
GPIO.setup(06, GPIO.IN, pull_up_down = GPIO.PUD_UP)

import picamera
import picamera.array

#import sys, os

data_dir = "/boot/DCIM"


#some screen ready-like
screen_ready = ''.join(chr(16) for _ in range(32*32*3))

def matrixScreen(s):
	sys.stdout.write(s)
	sys.stdout.flush()
	time.sleep(.5)

class matrixOut(object):
    def __init__(self):
        pass

    def write(self, s):
        sys.stdout.write(s)
        sys.stdout.flush()

    def flush(self):
        pass
    
state = 0 # 0 - ready, 1 - preview, 2 - recording
matrixScreen(screen_ready)

with picamera.PiCamera() as camera:
	camera.rotation = 90
	camera.resolution = (512, 512)
	while True:
		
		if(GPIO.input(19) == 0): # 1(up)
			if(state == 0):
				camera.start_recording(matrixOut(), splitter_port=2, resize=(32, 32), format='rgb')
				state = 1
			if(state == 1):
				camera.wait_recording(1, splitter_port=2)
		
		elif(GPIO.input(26) == 0): # 2
			if(state == 1):
				camera.start_recording(data_dir.join(filename))
				camera.wait_recording(1)
				state = 2
			elif(state == 2):
				camera.wait_recording(1)
				
		elif(GPIO.input(13) == 0): # 3
			camera.stop_recording(splitter_port=2)
			matrixScreen(screen_ready)
			state = 0
		
		elif(GPIO.input(06) == 0): # 4 (bottom)
			if(state == 0):
				break
		
		else:
			if(state == 2):
				camera.stop_recording(splitter_port=2)
				state == 1
			

os.system("sudo shutdown -h now")
