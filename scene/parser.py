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


def save_object_lines():
    objects = []
    current_object = None
    count = 1

    with open("exported_scene.obj", "r") as obj_file:
        lines = obj_file.readlines()

        for line in lines:
            line = line.strip()

            # Check if line has a new object
            if line.startswith("o "):
                if current_object is not None:
                    objects.append(current_object)  # Append object
                    count += len(current_object["v"])
                current_object = {"Name": [], "v": [], "f": [], "usemtl": []}  # Prepare for new object
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
                        int(index.split("/")[0])-count for index in line[2:].split()
                    ]
                    current_object["f"].append(
                        face_values
                    )  # Save face values as list of integers
                elif line.startswith("usemtl"):
                    mat_name = line[7:]
                    current_object["usemtl"].append(mat_name)

        # append last
        if current_object is not None:
            objects.append(current_object)

    return objects

def get_materials():
    materials = []
    current_material = None
    
    with open("exported_scene.mtl", "r") as mtl_file:
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
    "MAXDEPTH": 8,
    "RESOLUTION": [1200, 1200],
    "THREADS": 60,
    "lights": [{"POS": [0, 0, -900], "DIFF": [0.5, 0.5, 0.5], "SPEC": [0.3, 0.3, 0.3]}],
    "spheres": [],
    "quads": [],
}


objects = save_object_lines()
materials = get_materials()

print_materials(materials)



for x in range(len(objects)):
    print("loading object", objects[x]["Name"][0], "with material", objects[x]["usemtl"][0])
    mat = None
    index = 0
    while (mat == None):
        if (objects[x]["usemtl"][0] == materials[index]["Name"][0]) :
            mat = materials[index]
        else:
            index += 1

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
                diff=mat["Kd"],
                spec=mat["Ks"],
                shininess=mat["Ns"][0],
                refractive=-mat["d"],
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
                diff=mat["Kd"],
                spec=mat["Ks"],
                shininess=mat["Ns"][0],
                refractive=-mat["d"],
            )


# Save the entire array into JSON
save_data(result)
