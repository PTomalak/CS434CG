import bpy

# Path to export OBJ file
export_path = "./exported_scene.obj"

# Select all objects in the scene
bpy.ops.object.select_all(action='SELECT')

# Export selected objects as OBJ
bpy.ops.wm.obj_export(
    filepath=export_path
)
