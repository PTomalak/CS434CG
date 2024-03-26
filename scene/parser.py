import json

raw = {
  "ANTIALIAS": 2,
  "BACKGROUND": [0.0, 0.0, 0.0],
  "MAXDEPTH": 20,
  "RESOLUTION": [650, 650],
  "THREADS": 32,
  "lights": [
    {
      "POS": [0, 0, -20],
      "DIFF": [1.0, 1.0, 1.0],
      "SPEC": [1.0, 1.0, 1.0]
    }
  ],
  "spheres": [],
  "quads": []
}

with open('blended.json', 'w') as output:
    json.dump(raw, output, indent=1)
