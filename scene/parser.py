import json


def add_quad(vertices, diff, spec, shininess, refractive):
    return {
        "vertices": vertices,
        "DIFF": diff,
        "SPEC": spec,
        "SHININESS": shininess,
        "REFRACTIVE": refractive,
    }


def save_quads(quads_data, filename="blended.json"):
    with open(filename, "r") as file:
        data = json.load(file)
        data["quads"].extend(quads_data)
        with open(filename, "w") as output:
            json.dump(data, output, indent=1)


raw = {
    "ANTIALIAS": 2,
    "BACKGROUND": [0.0, 0.0, 0.0],
    "MAXDEPTH": 20,
    "RESOLUTION": [650, 650],
    "THREADS": 32,
    "lights": [{"POS": [0, 0, -20], "DIFF": [1.0, 1.0, 1.0], "SPEC": [1.0, 1.0, 1.0]}],
    "spheres": [],
    "quads": [],
}

# Add each quad to the quads_data list
quads_data = []

# First Quad
quad1 = add_quad(
    vertices=[
        [-150, -150, 150],
        [150, -150, 150],
        [-150, 150, 150],
        [150, 150, 150],
    ],
    diff=[0.25, 0.25, 0.25],
    spec=[1.0, 1.0, 0.0],
    shininess=2000.0,
    refractive=-1,
)
quads_data.append(quad1)

# Second Quad
quad2 = add_quad(
    vertices=[
        [150, -150, 150],
        [150, -150, -150],
        [150, 150, 150],
        [150, 150, -150],
    ],
    diff=[1.0, 1.0, 1.0],
    spec=[1.0, 0, 0],
    shininess=10.0,
    refractive=-1,
)
quads_data.append(quad2)

# Third Quad
quad3 = add_quad(
    vertices=[
        [-150, -150, -150],
        [-150, -150, 150],
        [-150, 150, -150],
        [-150, 150, 150],
    ],
    diff=[0.7, 0.7, 0.1255],
    spec=[1.0, 1.0, 1.0],
    shininess=10.0,
    refractive=-1,
)
quads_data.append(quad3)

# Fourth Quad
quad4 = add_quad(
    vertices=[
        [150, -150, 150],
        [-150, -150, 150],
        [150, -150, -150],
        [-150, -150, -150],
    ],
    diff=[0, 1.0, 0],
    spec=[0, 0, 0],
    shininess=10.0,
    refractive=-1,
)
quads_data.append(quad4)

# Fifth Quad
quad5 = add_quad(
    vertices=[
        [-150, 150, 150],
        [150, 150, 150],
        [-150, 150, -150],
        [150, 150, -150],
    ],
    diff=[1.0, 0.2, 0.2],
    spec=[1, 1, 1],
    shininess=100.0,
    refractive=-1,
)
quads_data.append(quad5)

save_quads(quads_data)
