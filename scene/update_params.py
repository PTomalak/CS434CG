import bpy
import json

scene = bpy.context.scene
render = bpy.context.scene.render
background_color_rgb = bpy.data.worlds["World"].node_tree.nodes["Background"].inputs[0].default_value
eevee = bpy.data.scenes["Scene"].eevee

result = {
    "ANTIALIAS": render.resolution_percentage / 100,
    "BACKGROUND": [background_color_rgb[0], background_color_rgb[1], background_color_rgb[2]],
    "MAXDEPTH": eevee.taa_samples,
    "RESOLUTION": [render.resolution_x, render.resolution_y],
    "SHADE": int(eevee.use_taa_reprojection),
    "SENSOR_SPACING": eevee.volumetric_sample_distribution,
    "SENSORS": eevee.volumetric_samples,
    "THREADS": eevee.taa_render_samples,
    "lights": [{"POS": [0, 0, -900], "DIFF": [0.5, 0.5, 0.5], "SPEC": [0.3, 0.3, 0.3]}],
    "spheres": [],
    "quads": [],
}

with open("./scene/settings.json", "w") as json_file:
    json.dump(result, json_file, indent=4)
