import json
import re

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

norm_arr = []

def bounding_box(vertices):
    bounding_boxes = []

    for v in vertices:
        if (v[0] <= 0):
            if (v[1] <= 0):
                if (v[2] <= 0):
                    bounding_boxes.append(3)
                else:
                    bounding_boxes.append(7)
            else:
                if (v[2] <= 0):
                    bounding_boxes.append(1)
                else:
                    bounding_boxes.append(5)
        else:
            if (v[1] <= 0):
                if (v[2] <= 0):
                    bounding_boxes.append(4)
                else:
                    bounding_boxes.append(8)
            else:
                if (v[2] <= 0):
                    bounding_boxes.append(2)
                else:
                    bounding_boxes.append(6)
    return bounding_boxes

def add_quad(array, vertices, normals, diff, spec, shininess, refractive):
    quad = {
        "vertices": vertices,
        "normals": normals,
        "DIFF": diff,
        "SPEC": spec,
        "SHININESS": shininess,
        "REFRACTIVE": refractive,
        "BOUNDING_BOX": bounding_box(vertices)
    }
    array["quads"].append(quad)


def save_data(data, filename="scene/blended.json"):
    with open(filename, "w") as file:
        json.dump(data, file, indent=1)


def save_object_lines():
    objects = []
    current_object = None
    count = 1

    with open("scene/exported_scene.obj", "r") as obj_file:
        lines = obj_file.readlines()
        

        for line in lines:
            line = line.strip()

            # Check if line has a new object
            if line.startswith("o "):
                if current_object is not None:
                    objects.append(current_object)  # Append object
                    count += len(current_object["v"])
                current_object = {"Name": [], "v": [], "f": [], "usemtl": [], "vn": []}  # Prepare for new object
                name = line[2:]
                current_object["Name"].append(name)

            # Check to skip first couple lines unrelated to objects
            if current_object is not None:
                # Save only f and v and material
                if line.startswith("v "):
                    vertex_values = [float(value) for value in line[2:].split()]
                    current_object["v"].append(
                        [vertex_values[0] * -1.0, vertex_values[1], vertex_values[2]]
                    )  # Save vertex values as list of floats
                elif line.startswith("f "):
                    face_values = [
                        int(index.split("//")[0]) - count if "//" in index else int(index.split("/")[0]) - count
                        for index in line[2:].split()
                        ]

                    norm_values = [
                        int(index.split("//")[1]) -1 if "//" in index else int(index.split("/")[2]) -1
                        for index in line[2:].split()
                    ]
                    current_object["f"].append(
                        face_values
                    )  # Save face values as list of integers
                    current_object["vn"].append(
                        norm_values
                    )  # Save normal values as list of integers
                    #print("got")
                    #print(face_values)
                    #print(norm_values)
                elif line.startswith("usemtl"):
                    mat_name = line[7:]
                    current_object["usemtl"].append(mat_name)
                elif line.startswith("vn "):
                    normal_values = [float(value) for value in line[2:].split()]
                    norm_arr.append(normal_values)

        # append last
        if current_object is not None:
            objects.append(current_object)

    return objects

def get_materials():
    materials = []
    current_material = None
    
    with open("scene/exported_scene.mtl", "r") as mtl_file:
        lines = mtl_file.readlines()

        for line in lines:
            line = line.strip()

            # Check if line has a new material
            if line.startswith("newmtl "):
                if current_material is not None:
                    materials.append(current_material)  # Append object
                current_material = {"Name": [], "Ns": [], "Ka": [], "Kd": [], "Ks": [], "Ke": [], "Ni": [], "d": [], "illum": []}  # Prepare for new material
                name = line[7:]
                current_material["Name"].append(name)

            # Check to skip first couple lines unrelated to materials
            if current_material is not None:
                if line.startswith("Ns "):
                    Ns_value = float(line[3:])
                    current_material["Ns"].append(Ns_value)
                elif line.startswith("Ka "):
                    Ka_values = [float(x) for x in line[3:].split()]
                    current_material["Ka"] = Ka_values
                elif line.startswith("Kd "):
                    Kd_values = [float(x) for x in line[3:].split()]
                    current_material["Kd"] = Kd_values
                elif line.startswith("Ks "):
                    Ks_values = [float(x) for x in line[3:].split()]
                    current_material["Ks"] = Ks_values
                elif line.startswith("Ke "):
                    Ke_values = [float(x) for x in line[3:].split()]
                    current_material["Ke"] = Ke_values
                elif line.startswith("Ni "):
                    Ni_value = float(line[3:])
                    current_material["Ni"] = Ni_value
                elif line.startswith("d "):
                    d_value = float(line[2:])
                    current_material["d"] = d_value
                elif line.startswith("illum "):
                    illum_value = int(line[6:])
                    current_material["illum"] = illum_value


        # append last
        if current_material is not None:
            materials.append(current_material)

    return materials

def print_materials(materials):
    for material in materials:
        print("Material:")
        for key, value in material.items():
            print(f"{key}: {value}")
        print()

result = {
    "ANTIALIAS": 1.0,
    "BACKGROUND": [0.0, 0.0, 0.0],
    "MAXDEPTH": 4,
    "RESOLUTION": [200, 200],
    "SHADE": 0,
    "APERATURE": 1.0,
    "THREADS": 60,
    "lights": [{"POS": [0, 0, -900], "DIFF": [0.5, 0.5, 0.5], "SPEC": [0.3, 0.3, 0.3]}],
    "spheres": [],
    "quads": [],
}


objects = save_object_lines()
materials = get_materials()

print_materials(materials)
#print(len(norm_arr))


if (len(materials) < 1):
    print("Assign materials! Exiting...")
else:


    for x in range(len(objects)):
        print("loading object", objects[x]["Name"][0], "with material", objects[x]["usemtl"][0])
        mat = None
        indexmat = 0
        while (mat == None):
            if (objects[x]["usemtl"][0] == materials[indexmat]["Name"][0]) :
                mat = materials[indexmat]
            else:
                indexmat += 1
        
        index = 0
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
                    normals=[
                        norm_arr[objects[x]["vn"][index][2]],
                        norm_arr[objects[x]["vn"][index][3]],
                        norm_arr[objects[x]["vn"][index][1]],
                        norm_arr[objects[x]["vn"][index][0]]
                    ],
                    diff=mat["Kd"],
                    spec=mat["Ks"],
                    shininess=mat["Ns"][0],
                    refractive=-mat["d"],
                )
            elif (len(face) == 3):
                add_quad(
                    result,
                    vertices=[
                        objects[x]["v"][face[2]],
                        objects[x]["v"][face[2]],
                        objects[x]["v"][face[1]],
                        objects[x]["v"][face[0]],
                    ],
                    normals=[
                        norm_arr[objects[x]["vn"][index][2]],
                        norm_arr[objects[x]["vn"][index][2]],
                        norm_arr[objects[x]["vn"][index][1]],
                        norm_arr[objects[x]["vn"][index][0]]
                    ],
                    diff=mat["Kd"],
                    spec=mat["Ks"],
                    shininess=mat["Ns"][0],
                    refractive=-mat["d"],
                )
            else:
                print("Note: we render only trigs and quads")
            index += 1


    # Save the entire array into JSON
    save_data(result)
