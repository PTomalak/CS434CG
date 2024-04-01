#!/bin/bash
NICKNAME="cguser"

docker stop -t 1 arch-vnc && docker rm arch-vnc
docker build -t arch-vnc .
docker run -d --name arch-vnc -p 5900:5900 -p 6080:6080 -e VNC_PASSWD=password -v ./../:/CS434CG -v ./persist:/home/$NICKNAME -e USERNAME=$NICKNAME arch-vnc
echo "http://localhost:6080"