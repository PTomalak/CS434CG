import bpy
import sys

numsteps = 40

startloc = 0
endloc = 1/64

delta = (endloc - startloc) / numsteps

argv = sys.argv
argv = argv[argv.index("--") + 1:]  # get all args after "--"

print(argv)  # --> ['example', 'args', '123']

#zloc = sys.argv[0]

#print(zloc)

#delta = (startlocation - endlocation) / numbersteps

bpy.ops.wm.open_mainfile(filepath="./scene/anim_blends/anim.blend") 

#lens = bpy.data.objects['lens']
#grid = bpy.data.objects['Plane']

whichstep = int(argv[0])
zloc = startloc + delta * whichstep
print("moving to {}".format(zloc))
#lens.location = (0, 0, float(zloc))
bpy.context.scene.eevee.volumetric_sample_distribution = zloc
#bpy.ops.wm.save_as_mainfile(filepath="./scene/anim_blends/{}.blend".format(i))
bpy.ops.wm.save_mainfile()