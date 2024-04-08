
NUMSTEPS=40

for i in $(seq 0 $NUMSTEPS);
do

    blender -b -P ./scene/move.py -- $i
    A="./scene/anim_blends/anim.blend"

    j=$i

    B="./anim_bmps/retinaproj/${j}.bmp"
    bash ./c.sh $A $B
done