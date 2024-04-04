NICKNAME="cguser"

currdir=$(pwd | awk -F/ '{print $NF}')

if [ "$currdir" != "arch-vnc-arm" ]; then
    echo "You are not in correct folder, go to 'arch-vnc-arm'"
    exit 1
fi

docker stop -t 1 arch-vnc-arm
docker start arch-vnc-arm
echo ""
echo "http://localhost:6080"