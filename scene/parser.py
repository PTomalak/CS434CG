import json

"""

       +Y
       ^      ^ +Z
       |     /
       |    /
       |   /
       |  /
       | /
       |/
       +--------> +X
      /
     /
    /
   /
  /
 v -Z

"""
# https://www.loc.gov/preservation/digital/formats/fdd/fdd000508.shtml

def add_quad(array, vertices, diff, spec, shininess, refractive):
    quad = {
        "vertices": vertices,
        "DIFF": diff,
        "SPEC": spec,
        "SHININESS": shininess,
        "REFRACTIVE": refractive,
    }
    array["quads"].append(quad)


def save_data(data, filename="blended.json"):
    with open(filename, "w") as file:
        json.dump(data, file, indent=1)


def save_object_lines(obj_filename):
    objects = []
    current_object = None
    count = 1

    with open(obj_filename, "r") as obj_file:
        lines = obj_file.readlines()

        for line in lines:
            line = line.strip()

            # Check if line has a new object
            if line.startswith("o "):
                if current_object is not None:
                    objects.append(current_object)  # Append object
                    count += len(current_object["v"])
                current_object = {"v": [], "f": []}  # Prepare for new object

            # Check to skip first couple lines unrelated to objects
            if current_object is not None:
                # Save only f and v
                if line.startswith("v "):
                    vertex_values = [float(value) for value in line[2:].split()]
                    current_object["v"].append(
                        [vertex_values[0] * -1.0, vertex_values[1], vertex_values[2]]
                    )  # Save vertex values as list of floats
                elif line.startswith("f "):
                    face_values = [
                        int(index.split("/")[0])-count for index in line[2:].split()
                    ]
                    current_object["f"].append(
                        face_values
                    )  # Save face values as list of integers

        # append last
        if current_object is not None:
            objects.append(current_object)

    return objects


result = {
    "ANTIALIAS": 1.0,
    "BACKGROUND": [0.0, 0.0, 0.0],
    "MAXDEPTH": 8,
    "RESOLUTION": [240, 240],
    "THREADS": 24,
    "lights": [{"POS": [0, 0, -900], "DIFF": [0.5, 0.5, 0.5], "SPEC": [0.0, 0.0, 0.0]}],
    "spheres": [],
    "quads": [],
}


objects = save_object_lines("exported_scene.obj")




for x in range(len(objects)):
    print("loading object ", x)
    for face in objects[x]["f"]:
        if (len(face) == 4):
            add_quad(
                result,
                vertices=[
                    objects[x]["v"][face[2]],
                    objects[x]["v"][face[3]],
                    objects[x]["v"][face[1]],
                    objects[x]["v"][face[0]],
                ],
                diff=[0.4, 0.7, 1.0],
                spec=[1.0, 1.0, 1.0],
                shininess=100.0,
                refractive=-1,
            )
        if (len(face) == 3):
            add_quad(
                result,
                vertices=[
                    objects[x]["v"][face[2]],
                    objects[x]["v"][face[2]],
                    objects[x]["v"][face[1]],
                    objects[x]["v"][face[0]],
                ],
                diff=[0.0, 0.0, 0.0],
                spec=[0, 0, 0],
                shininess=10000000000000.0,
                refractive=1.2,
            )


# Save the entire array into JSON
save_data(result)
