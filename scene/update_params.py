import bpy
import json

scene = bpy.context.scene
render = scene.render
world = bpy.data.worlds["World"]
eevee = scene.eevee

lights_info = []

for obj in scene.objects:
    if obj.type == 'LIGHT':
        light_data = obj.data
        light_info = {
            "POS": list(obj.location * 150.0),
            "DIFF": list(light_data.color * light_data.diffuse_factor),
            "SPEC": list(light_data.color * light_data.specular_factor),
        }
        light_info["POS"][1] = -light_info["POS"][1]
        lights_info.append(light_info)

result = {
    "ANTIALIAS": render.resolution_percentage / 100,
    "BACKGROUND": list(world.node_tree.nodes["Background"].inputs[0].default_value),
    "MAXDEPTH": eevee.taa_samples,
    "RESOLUTION": [render.resolution_x, render.resolution_y],
    "SHADE": int(eevee.use_taa_reprojection),
    "SENSOR_SPACING": eevee.volumetric_sample_distribution,
    "SENSORS": eevee.volumetric_samples,
    "THREADS": eevee.taa_render_samples,
    "lights": lights_info,
    "spheres": [],
    "quads": [],
}

with open("./scene/settings.json", "w") as json_file:
    json.dump(result, json_file, indent=4)
