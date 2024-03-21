import json
import subprocess
from copy import deepcopy
import os

def modify_scene(json_file, start_counter=0):
  with open(json_file, 'r') as f:
    data = json.load(f)

  os.makedirs('json', exist_ok=True)
  os.makedirs('bmp', exist_ok=True)

  color = [0.0, 0.0, 0.0]
  file_counter = start_counter

  for component in range(3):
    for _ in range(10):
      color[component] += 0.1
      modified_data = deepcopy(data)
      for light in modified_data["lights"]:
        for i in range(3):
          light["DIFF"][i] = color[i]
          light["SPEC"][i] = color[i]
      json_output = f"json/animation_{file_counter}.json"
      with open(json_output, 'w') as outfile:
        json.dump(modified_data, outfile, indent=2)
      subprocess.run(["./ray", json_output, f"bmp/output_{file_counter:04d}.bmp"])
      file_counter += 1

  sphere1_pos = data["spheres"][0]["POS"]
  sphere2_pos = data["spheres"][1]["POS"]

  for t in range(31): 
    if t <= 10:
      x = sphere1_pos[0] + (sphere2_pos[0] - sphere1_pos[0]) * t / 10
      y = sphere1_pos[1] + (sphere2_pos[1] - sphere1_pos[1]) * t / 10
      z = sphere1_pos[2] + (sphere2_pos[2] - sphere1_pos[2]) * t / 10
    else:
      x, y, z = sphere2_pos
    if t > 10:
      x = sphere2_pos[0] + (sphere1_pos[0] - sphere2_pos[0]) * (t - 10) / 10
      y = sphere2_pos[1] + (sphere1_pos[1] - sphere2_pos[1]) * (t - 10) / 10
      z = sphere2_pos[2] + (sphere1_pos[2] - sphere2_pos[2]) * (t - 10) / 10

    modified_data = deepcopy(data)
    modified_data["spheres"][0]["POS"] = [x, y, z]
    modified_data["spheres"][1]["POS"] = [sphere2_pos[0] - x + sphere1_pos[0], 
                                          sphere2_pos[1] - y + sphere1_pos[1], 
                                          sphere2_pos[2] - z + sphere1_pos[2]]

    json_output = f"json/animation_{file_counter}.json"
    with open(json_output, 'w') as outfile:
      json.dump(modified_data, outfile, indent=2)

    subprocess.run(["./ray", json_output, f"bmp/output_{file_counter:04d}.bmp"])
    file_counter += 1

json_file = "animation.json"
modify_scene(json_file)
