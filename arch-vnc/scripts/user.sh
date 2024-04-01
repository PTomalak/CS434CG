#!/bin/bash

echo $USER
echo $HOME

# Remove lock since stopping containers won't remove it
rm -f /tmp/.X0-lock

expect /scripts/vnc.exp &

# Start noVNC
if [ "$DISABLE_NOVNC" != "true" ]; then
	/noVNC/utils/novnc_proxy --vnc localhost:5900
else
	# prevent process from exiting
	tail -f /dev/null
fi
