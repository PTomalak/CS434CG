
NUMSTEPS=40

for i in $(seq 0 $NUMSTEPS);
do
    blender -b -P ./scene/move.py -- $i
    A="./scene/anim_blends/anim.blend"

    B="./anim_bmps/${i}.bmp"
    ./c.sh $A $B
done