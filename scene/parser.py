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
                        vertex_values
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
    "ANTIALIAS": 2,
    "BACKGROUND": [0.0, 0.0, 0.0],
    "MAXDEPTH": 2,
    "RESOLUTION": [400, 400],
    "THREADS": 32,
    "lights": [{"POS": [-100, 0, -200], "DIFF": [1.0, 1.0, 1.0], "SPEC": [1.0, 1.0, 1.0]}, {"POS": [100, 0, -200], "DIFF": [1.0, 1.0, 1.0], "SPEC": [1.0, 1.0, 1.0]}],
    "spheres": [],
    "quads": [],
}


objects = save_object_lines("exported_scene.obj")

num = 3

for face in objects[num]["f"]:
    if (len(face) == 4):
        add_quad(
            result,
            vertices=[
                objects[num]["v"][face[3]],
                objects[num]["v"][face[2]],
                objects[num]["v"][face[0]],
                objects[num]["v"][face[1]],
            ],
            diff=[1.0, 1.0, 1.0],
            spec=[1.0, 1.0, 1.0],
            shininess=100.0,
            refractive=-1,
        )
    if (len(face) == 3):
        add_quad(
            result,
            vertices=[
                objects[num]["v"][face[2]],
                objects[num]["v"][face[2]],
                objects[num]["v"][face[0]],
                objects[num]["v"][face[1]],
            ],
            diff=[1.0, 1.0, 1.0],
            spec=[1.0, 1.0, 1.0],
            shininess=100.0,
            refractive=-1,
        )


# Save the entire array into JSON
save_data(result)
