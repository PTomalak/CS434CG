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


result = {
    "ANTIALIAS": 2,
    "BACKGROUND": [0.0, 0.0, 0.0],
    "MAXDEPTH": 20,
    "RESOLUTION": [650, 650],
    "THREADS": 32,
    "lights": [{"POS": [0, 0, 0], "DIFF": [1.0, 1.0, 1.0], "SPEC": [1.0, 1.0, 1.0]}],
    "spheres": [],
    "quads": [],
}

add_quad(
    result,
    vertices=[
        [-150, -150, 150],
        [150, -150, 150],
        [-150, 150, 150],
        [150, 150, 150],
    ],
    diff=[1.0, 1.0, 1.0],
    spec=[1.0, 1.0, 1.0],
    shininess=100.0,
    refractive=-1,
)

add_quad(
    result,
    vertices=[
        [150, -150, 150],
        [150, -150, -150],
        [150, 150, 150],
        [150, 150, -150],
    ],
    diff=[1.0, 1.0, 1.0],
    spec=[1.0, 1.0, 1.0],
    shininess=100.0,
    refractive=-1,
)

add_quad(
    result,
    vertices=[
        [-150, -150, -150],
        [-150, -150, 150],
        [-150, 150, -150],
        [-150, 150, 150],
    ],
    diff=[1.0, 1.0, 1.0],
    spec=[1.0, 1.0, 1.0],
    shininess=100.0,
    refractive=-1,
)

add_quad(
    result,
    vertices=[
        [150, -150, 150],
        [-150, -150, 150],
        [150, -150, -150],
        [-150, -150, -150],
    ],
    diff=[1.0, 1.0, 1.0],
    spec=[1.0, 1.0, 1.0],
    shininess=100.0,
    refractive=-1,
)

add_quad(
    result,
    vertices=[
        [-150, 150, 150],
        [150, 150, 150],
        [-150, 150, -150],
        [150, 150, -150],
    ],
    diff=[1.0, 1.0, 1.0],
    spec=[1.0, 1.0, 1.0],
    shininess=100.0,
    refractive=-1,
)

add_quad(
    result,
    vertices=[
        [-100, -100, 100],
        [-80, -100, 100],
        [-100, -50, 100],
        [-80, -50, 100],
    ],
    diff=[1.0, 0, 0],
    spec=[1.0, 1.0, 1.0],
    shininess=100.0,
    refractive=-1,
)


# Save the entire array into JSON
save_data(result)
