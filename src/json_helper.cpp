#include "json_helper.h"

using namespace rapidjson;
using namespace std;

struct Vec3 {
  float x, y, z;
};
extern float antialias;
extern float backgroundx;
extern float backgroundy;
extern float backgroundz;
extern int maxdepth;
extern int resolutionX;
extern int resolutionY;
extern int THREADS;

struct Light {
  Vec3 pos;
  Vec3 diff;
  Vec3 spec;
};

extern vector<Light> lights;
extern vector<tuple<Vec3, float, Vec3, Vec3, float>> spheres;
extern vector<tuple<vector<Vec3>, Vec3, Vec3, float, float, vector<Vec3>>> quads;

int readJSON(std::string filename) {
  // Read the JSON file
  ifstream ifs(filename);
  if (!ifs.is_open()) {
    std::cerr << "Error: Unable to open file " << filename << std::endl;
    return -1;
  }
  string content((istreambuf_iterator<char>(ifs)),
                 (istreambuf_iterator<char>()));
  Document document;
  document.Parse(content.c_str());

  // Extract content
  antialias = document["ANTIALIAS"].GetFloat();
  backgroundx = document["BACKGROUND"][0].GetFloat();
  backgroundy = document["BACKGROUND"][1].GetFloat();
  backgroundz = document["BACKGROUND"][2].GetFloat();

  maxdepth = document["MAXDEPTH"].GetInt();
  resolutionX = document["RESOLUTION"][0].GetInt();
  resolutionY = document["RESOLUTION"][1].GetInt();
  THREADS = document["THREADS"].GetInt();

  // Read lights
  for (const auto &light : document["lights"].GetArray()) {
    Vec3 pos = {light["POS"][0].GetFloat(), light["POS"][1].GetFloat(),
                light["POS"][2].GetFloat()};
    Vec3 diff = {light["DIFF"][0].GetFloat(), light["DIFF"][1].GetFloat(),
                 light["DIFF"][2].GetFloat()};
    Vec3 spec = {light["SPEC"][0].GetFloat(), light["SPEC"][1].GetFloat(),
                 light["SPEC"][2].GetFloat()};
    lights.push_back({pos, diff, spec});
  }

  // Read spheres
  for (const auto &sphere : document["spheres"].GetArray()) {
    Vec3 pos = {sphere["POS"][0].GetFloat(), sphere["POS"][1].GetFloat(),
                sphere["POS"][2].GetFloat()};
    float radius = sphere["RADIUS"].GetFloat();
    Vec3 diff = {sphere["DIFF"][0].GetFloat(), sphere["DIFF"][1].GetFloat(),
                 sphere["DIFF"][2].GetFloat()};
    Vec3 spec = {sphere["SPEC"][0].GetFloat(), sphere["SPEC"][1].GetFloat(),
                 sphere["SPEC"][2].GetFloat()};
    float shininess = sphere["SHININESS"].GetFloat();
    spheres.push_back(make_tuple(pos, radius, diff, spec, shininess));
  }

  // Read quads
  for (const auto &quad : document["quads"].GetArray()) {
    vector<Vec3> vertices;
    vector<Vec3> normals;
    for (const auto &vertex : quad["vertices"].GetArray()) {
      Vec3 v = {vertex[0].GetFloat(), vertex[1].GetFloat(),
                vertex[2].GetFloat()};
      vertices.push_back(v);
    }
    for (const auto &vertex : quad["normals"].GetArray()) {
      Vec3 vn = {vertex[0].GetFloat(), vertex[1].GetFloat(),
                vertex[2].GetFloat()};
      normals.push_back(vn);
    }
    Vec3 diff = {quad["DIFF"][0].GetFloat(), quad["DIFF"][1].GetFloat(),
                 quad["DIFF"][2].GetFloat()};
    Vec3 spec = {quad["SPEC"][0].GetFloat(), quad["SPEC"][1].GetFloat(),
                 quad["SPEC"][2].GetFloat()};
    float shininess = quad["SHININESS"].GetFloat();
    float refractive = quad["REFRACTIVE"].GetFloat();
    quads.push_back(make_tuple(vertices, diff, spec, shininess, refractive, normals));
  }
  return 0;
}

void printJSON() {
  // Output misc
  cout << "ANTIALIAS: " << antialias << endl;
  cout << "BACKGROUND: (" << backgroundx << ", " << backgroundy << ", "
       << backgroundz << ")" << endl;
  cout << "MAXDEPTH: " << maxdepth << endl;
  cout << "RESOLUTION: (" << resolutionX << ", " << resolutionY << ")" << endl;
  cout << "THREADS: " << THREADS << endl;

  // Output lights
  cout << "Lights:" << endl;
  for (const auto &light : lights) {
    cout << "POS: (" << light.pos.x << ", " << light.pos.y << ", "
         << light.pos.z << "), ";
    cout << "DIFF: (" << light.diff.x << ", " << light.diff.y << ", "
         << light.diff.z << "), ";
    cout << "SPEC: (" << light.spec.x << ", " << light.spec.y << ", "
         << light.spec.z << ")" << endl;
  }

  // Output spheres
  cout << "Spheres:" << endl;
  for (const auto &sphere : spheres) {
    Vec3 pos = get<0>(sphere);
    float radius = get<1>(sphere);
    Vec3 diff = get<2>(sphere);
    Vec3 spec = get<3>(sphere);
    float shininess = get<4>(sphere);
    cout << "POS: (" << pos.x << ", " << pos.y << ", " << pos.z << "), ";
    cout << "RADIUS: " << radius << ", ";
    cout << "DIFF: (" << diff.x << ", " << diff.y << ", " << diff.z << "), ";
    cout << "SPEC: (" << spec.x << ", " << spec.y << ", " << spec.z << "), ";
    cout << "SHININESS: " << shininess << endl;
  }

  // Output quads
  cout << "Quads:" << endl;
  for (const auto &quad : quads) {
    vector<Vec3> vertices = get<0>(quad);
    Vec3 diff = get<1>(quad);
    Vec3 spec = get<2>(quad);
    float shininess = get<3>(quad);
    cout << "Vertices:" << endl;
    for (const auto &vertex : vertices) {
      cout << "(" << vertex.x << ", " << vertex.y << ", " << vertex.z << ")"
           << endl;
    }
    cout << "DIFF: (" << diff.x << ", " << diff.y << ", " << diff.z << "), ";
    cout << "SPEC: (" << spec.x << ", " << spec.y << ", " << spec.z << "), ";
    cout << "SHININESS: " << shininess << endl;
  }
}
