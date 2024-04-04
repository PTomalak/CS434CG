#!/bin/bash
NICKNAME="cguser"

currdir=$(pwd | awk -F/ '{print $NF}')

if [ "$currdir" != "arch-vnc-arm" ]; then
    echo "You are not in correct folder, go to 'arch-vnc-arm'"
    exit 1
fi

chmod +x ./scripts/*
docker stop -t 1 arch-vnc-arm && docker rm arch-vnc-arm
docker build -t arch-vnc-arm .
docker run -d --name arch-vnc-arm -p 5900:5900 -p 6080:6080 -e VNC_PASSWD=password -v ./../:/CS434CG -v ./persist:/home/$NICKNAME -e USERNAME=$NICKNAME arch-vnc-arm
echo ""
echo "http://localhost:6080"