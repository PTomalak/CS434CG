NICKNAME="cguser"

currdir=$(pwd | awk -F/ '{print $NF}')

if [ "$currdir" != "arch-vnc" ]; then
    echo "You are not in correct folder, go to 'arch-vnc'"
    exit 1
fi

docker stop -t 1 arch-vnc
docker start arch-vnc
echo ""
echo "http://localhost:6080"