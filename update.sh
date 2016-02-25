#!/bin/sh
# it's just another test everything is okey with updates
if [ -d /boot/UPDATE ]
then
	mv /boot/UPDATE/* /home/pi/pixel_camera/
	chmod +x update.sh
	rm -r /boot/UPDATE
fi