import bpy

# https://docs.blender.org/api/current/bpy.ops.wm.html#bpy.ops.wm.obj_export

# Path to export OBJ file
export_path = "./scene/exported_scene.obj"

# Select all objects in the scene
bpy.ops.object.select_all(action='SELECT')

# Export selected objects as OBJ
bpy.ops.wm.obj_export(
    filepath=export_path,
    global_scale=150,
    forward_axis="NEGATIVE_Y",
    up_axis="Z"
)
