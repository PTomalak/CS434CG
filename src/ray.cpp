#include "ray.h"
#include <unordered_set>

struct Ray {
  glm::vec3 origin;
  glm::vec3 direction;
};

struct Sphere {
  glm::vec3 position;
  float radius;
};

// Looking back, I wish I used glm::vec3 from the start
struct Vec3 {
  float x, y, z;
};

struct P {
  glm::vec3 loc;
  glm::vec3 diff;
  glm::vec3 spec;
  float shine;
  glm::vec3 normal;
  float distance;
  float refractive_idx;
};

using namespace std;

extern float antialias;
extern float backgroundx;
extern float backgroundy;
extern float backgroundz;
extern int maxdepth;
extern int resolutionX;
extern int resolutionY;

extern int smooth;
extern float aperature;
extern int sensors;

struct Light {
  Vec3 pos;
  Vec3 diff;
  Vec3 spec;
};

extern vector<Light> lights;
extern vector<tuple<Vec3, float, Vec3, Vec3, float>> spheres;
extern std::vector<glm::vec3> sensor_cell_locs;
extern vector<tuple<vector<Vec3>, Vec3, Vec3, float, float, vector<Vec3>, vector<int>>>
    quads;

const float PI = 3.14159265358979323846f;
const float fov = 150.0f;
const float f_inf = std::numeric_limits<float>::infinity();
const float init_refractive_idx = 1.0f;

// Camera is so far to recreate a similar setup as on example pictures
extern glm::vec3 camera_pos;


float reflection_loss = 1.0f;

// Adjust below depending on the size of your blender scene
glm::vec3 screen_min = glm::vec3(-500.f, -400.f, -800.f);
glm::vec3 screen_max = glm::vec3(500.f, 400.f, 1500.f);
vector<tuple<int, glm::vec3, glm::vec3>> bounding_boxes;
int base = 3; // the number of boxes will be this number^3
extern bool bounding_boxes_enabled;

// Snell's Law, returns refracted vector
glm::vec3 Refraction(float n1, float n2, P p, Ray r) {
  // The following formula is taken from "Reflections and Refractions in Ray
  // Tracing" by Bram de Greve
  // https://graphics.stanford.edu/courses/cs148-10-summer/docs/2006--degreve--reflection_refraction.pdf
  float n = n1 / n2;
  float cosI = glm::abs(glm::dot(p.normal, r.direction));
  float sinT2 = n * n * (1.0 - cosI * cosI);
  if (sinT2 > 1.0) {
    return glm::vec3(-1, -1, -1);
  }
  float cosT = glm::sqrt(1.0 - sinT2);

  glm::vec3 refracted = n * r.direction + (n * cosI - cosT) * p.normal;

  return refracted;

  // float cosi = glm::clamp(-1.f, 1.f, glm::dot(r.direction, p.normal));
  // float etai = n1, etat = n2;
  // glm::vec3 n = p.normal;
  // if (cosi < 0) { cosi = -cosi; } else { std::swap(etai, etat); n = -1.f *
  // p.normal; } float eta = etai / etat; float k = 1 - eta * eta * (1 - cosi *
  // cosi);
  // //return k < 0 ? 0 : eta * I + (eta * cosi - sqrtf(k)) * n;

  // if (k < 0)
  //   return glm::vec3(-1,-1,-1);
  // return eta * r.direction + (eta * cosi - glm::sqrt(k)) * p.normal;
}

// Gramm-Schmidt orthonormalization
void GrammSchmidt(glm::vec3 &l, glm::vec3 &v, glm::vec3 &u) {
  l = glm::normalize(l);
  v = glm::normalize(v - glm::dot(v, l) * l);
  u = glm::cross(v, l);
}

std::vector<Ray> CalculateRays(int x, int y) {
  //printf("x: %d, y: %d\n",x, y);
  std::vector<Ray> vec;

  glm::vec3 lookAt(0.0f, 0.0f, 0.0f); // Look at point
  glm::vec3 up(0.0f, 1.0f, 0.0f);     // Up vector

  float aspectRatio = float(width) / float(height);
  float focalLength = 1.0f / tan((fov * PI / 180.0f) / 2.0f);

  for (int i = 0; i < sensor_cell_locs.size(); i++) {
    glm::vec3 cell_loc = sensor_cell_locs.at(i);
  
  // Calculate l, v, u vectors
  glm::vec3 l = lookAt - camera_pos;
  glm::vec3 v = up;
  glm::vec3 u;
  GrammSchmidt(l, v, u);

  // Calculate ray direction
  float px =
      (2.0f * (x + 0.5f) / float(width) - 1.0f) * aspectRatio * focalLength;
  float py = (1.0f - 2.0f * (y + 0.5f) / float(height)) * focalLength;
  glm::vec3 p = camera_pos + l + px * u + py * v;
  //printf("p: %f %f %f\n", p.x, p.y, p.z);
  glm::vec3 direction = glm::normalize(p - cell_loc);

  //printf("cell loc: %f %f %f, direction: %f %f %f\n", cell_loc.x, cell_loc.y, cell_loc.z, direction.x, direction.y, direction.z);
  vec.push_back(Ray{cell_loc, direction});
  }
  return vec;
}

// Get ray from x,y pixel and setup the scene vectors
Ray CalculateRay(int x, int y) {
  glm::vec3 lookAt(0.0f, 0.0f, 0.0f); // Look at point
  glm::vec3 up(0.0f, 1.0f, 0.0f);     // Up vector

  float aspectRatio = float(width) / float(height);
  float focalLength = 1.0f / tan((fov * PI / 180.0f) / 2.0f);

  // Calculate l, v, u vectors
  glm::vec3 l = lookAt - camera_pos;
  glm::vec3 v = up;
  glm::vec3 u;
  GrammSchmidt(l, v, u);

  // Calculate ray direction
  float px =
      (2.0f * (x + 0.5f) / float(width) - 1.0f) * aspectRatio * focalLength;
  float py = (1.0f - 2.0f * (y + 0.5f) / float(height)) * focalLength;
  glm::vec3 p = camera_pos + l + px * u + py * v;
  glm::vec3 direction = glm::normalize(p - camera_pos);

  return Ray{camera_pos, direction};
}

// Function to check ray-sphere intersection
// Using method from the slides
float ray_intersects_sphere(Ray ray, const Vec3 &pos, float radius) {
  // Calculate the vector from center of the sphere to the ray origin
  glm::vec3 sph_to_ray = glm::vec3(ray.origin.x - pos.x, ray.origin.y - pos.y,
                                   ray.origin.z - pos.z);

  // Setup quadratic equation
  float a = glm::dot(ray.direction, ray.direction);
  float b = 2.0f * glm::dot(sph_to_ray, ray.direction);
  float c = glm::dot(sph_to_ray, sph_to_ray) - radius * radius;

  // Calculate the discriminant
  float discriminant = b * b - 4 * a * c;

  // If discriminant is non-negative, the ray intersects the sphere
  if (discriminant >= 0) {
    float delta = std::sqrt(discriminant);

    // Calculate the distances to intersection points
    float t1 = (-b - delta) / (2.0f * a);
    float t2 = (-b + delta) / (2.0f * a);

    // Return correct distance
    if (t1 > 0 && t2 > 0) {
      return std::min(t1, t2);
    } else if (t1 > 0) {
      return t1;
    } else if (t2 > 0) {
      return t2;
    }
  }

  // -1 indicating no intersection
  return -1.0f;
}

// https://en.wikipedia.org/wiki/M%C3%B6ller%E2%80%93Trumbore_intersection_algorithm
// Function to check ray-triangle intersection
// I took it as-is since it was linked in the class slides (Raytracing slide 47)
float ray_intersects_triangle(glm::vec3 ray_origin, glm::vec3 ray_vector,
                              const std::vector<Vec3> &triangle_vertices,
                              glm::vec3 &out_intersection_point) {
  constexpr float epsilon = std::numeric_limits<float>::epsilon();

  glm::vec3 edge1 = glm::vec3(triangle_vertices[1].x - triangle_vertices[0].x,
                              triangle_vertices[1].y - triangle_vertices[0].y,
                              triangle_vertices[1].z - triangle_vertices[0].z);
  glm::vec3 edge2 = glm::vec3(triangle_vertices[2].x - triangle_vertices[0].x,
                              triangle_vertices[2].y - triangle_vertices[0].y,
                              triangle_vertices[2].z - triangle_vertices[0].z);
  glm::vec3 ray_cross_e2 = glm::cross(ray_vector, edge2);
  float det = glm::dot(edge1, ray_cross_e2);

  if (det > -epsilon && det < epsilon)
    return -1.0f; // This ray is parallel to this triangle.

  float inv_det = 1.0f / det;
  glm::vec3 s =
      ray_origin - glm::vec3(triangle_vertices[0].x, triangle_vertices[0].y,
                             triangle_vertices[0].z);
  float u = inv_det * glm::dot(s, ray_cross_e2);

  if (u < 0 || u > 1)
    return -1.0f;

  glm::vec3 s_cross_e1 = glm::cross(s, edge1);
  float v = inv_det * glm::dot(ray_vector, s_cross_e1);

  if (v < 0 || u + v > 1)
    return -1.0f;

  // At this stage we can compute t to find out where the intersection point is
  // on the line.
  float t = inv_det * glm::dot(edge2, s_cross_e1);

  if (t > epsilon) { // ray intersection
    out_intersection_point = ray_origin + ray_vector * t;
    return t;
  } else { // This means that there is a line intersection but not a ray
           // intersection.
    return -1.0f;
  }
}

glm::vec3 weightedAverageNormal(const glm::vec3& intersectionPoint,
                                const glm::vec3& vertex1,
                                const glm::vec3& vertex2,
                                const glm::vec3& vertex3,
                                const glm::vec3& normal1,
                                const glm::vec3& normal2,
                                const glm::vec3& normal3) {
    float d1 = glm::length(vertex1 - intersectionPoint);
    float d2 = glm::length(vertex2 - intersectionPoint);
    float d3 = glm::length(vertex3 - intersectionPoint);
    
    float totalDistance = d1 + d2 + d3;

    // Calculate weights based on distance
    float w1 = d1 / totalDistance;
    float w2 = d2 / totalDistance;
    float w3 = d3 / totalDistance;

    w1 = 1/w1;
    w2 = 1/w2;
    w3 = 1/w3;

    // Weighted sum of normals
    glm::vec3 weightedNormal = w1 * normal1 + w2 * normal2 + w3 * normal3;
    
    // Normalize
    return glm::normalize(weightedNormal);
}


// The following function is taken from
// https://www.scratchapixel.com/lessons/3d-basic-rendering/minimal-ray-tracer-rendering-simple-shapes/ray-box-intersection.html
bool BoundingBoxIntersection(Ray r, glm::vec3 min, glm::vec3 max) {
  float tmin = (min.x - r.origin.x) / r.direction.x;
  float tmax = (max.x - r.origin.x) / r.direction.x; 

  if (tmin > tmax) swap(tmin, tmax); 

  float tymin = (min.y - r.origin.y) / r.direction.y; 
  float tymax = (max.y - r.origin.y) / r.direction.y; 

  if (tymin > tymax) swap(tymin, tymax); 

  if ((tmin > tymax) || (tymin > tmax)) 
      return false; 

  if (tymin > tmin) tmin = tymin; 
  if (tymax < tmax) tmax = tymax; 

  float tzmin = (min.z - r.origin.z) / r.direction.z; 
  float tzmax = (max.z - r.origin.z) / r.direction.z; 

  if (tzmin > tzmax) swap(tzmin, tzmax); 

  if ((tmin > tzmax) || (tzmin > tmax)) 
      return false; 

  if (tzmin > tmin) tmin = tzmin; 
  if (tzmax < tmax) tmax = tzmax; 

  return true; 
}

void AddBoundingBox(Ray ray, unordered_set<int> &boxes) {
  for (const auto &box : bounding_boxes) {
    if (BoundingBoxIntersection(ray, std::get<1>(box), std::get<2>(box))) {
      boxes.insert(std::get<0>(box));
    }
  }
}

void GenerateBoundingBoxes() {
  bounding_boxes.clear();
  float x_min = screen_min.x;
  float y_min = screen_min.y;
  float z_min = screen_min.z;

  float x_max = screen_max.x;
  float y_max = screen_max.y;
  float z_max = screen_max.z;
  
  float scene_width = x_max - x_min;
  float scene_height = y_max - y_min;
  float scene_depth = z_max - z_min;
  
  float box_width = scene_width / base;
  float box_height = scene_height / base;
  float box_depth = scene_depth / base;

  int label = 1;

  for(int i = 0; i < base; i++) {
    float x_box_min = x_min + i * box_width;
    float x_box_max = x_min + (i + 1) * box_width;

    for (int j = 0; j < base; j++) {
      float y_box_min = y_min + j * box_height;
      float y_box_max = y_min + (j + 1) * box_height;

      for (int k = 0; k < base; k++) {
        float z_box_min = z_min + k * box_depth;
        float z_box_max = z_min + (k + 1) * box_depth;

        glm::vec3 min = glm::vec3(x_box_min, y_box_min, z_box_min);
        glm::vec3 max = glm::vec3(x_box_max, y_box_max, z_box_max);
        auto t = make_tuple(label, min, max);
        bounding_boxes.push_back(t);

        label++;
      }
    }
  }
}

// Function to trace the ray and determine if it intersects any spheres or
// trigs
// mode: 0 - don't ignore refractive / transparent objects
// mode: 1 - ignore refractive / transparent objects
P FirstIntersection(Ray ray, int mode) {
  P result;
  result.diff = glm::vec3(backgroundx, backgroundy, backgroundz);
  result.spec = glm::vec3(backgroundx, backgroundy, backgroundz);
  result.refractive_idx = -1;
  float closest = f_inf;
  float distance = f_inf;
  glm::vec3 intersectionPoint;
  glm::vec3 normal;

  // Check for sphere intersections
  /*for (const auto &sphere : spheres) {
    Vec3 pos = std::get<0>(sphere);
    float radius = std::get<1>(sphere);
    Vec3 diff = std::get<2>(sphere);
    Vec3 spec = get<3>(sphere);
    float shininess = get<4>(sphere);

    // If ray intersects sphere and is closer than before, populate result
    distance = ray_intersects_sphere(ray, pos, radius);
    if (distance > 0.0f && distance < closest) {
      result.diff = glm::vec3(diff.x, diff.y, diff.z);
      result.spec = glm::vec3(spec.x, spec.y, spec.z);
      intersectionPoint = ray.origin + ray.direction * distance;
      result.loc = glm::vec3(intersectionPoint.x, intersectionPoint.y,
                             intersectionPoint.z);
      normal =
          glm::normalize(intersectionPoint - glm::vec3(pos.x, pos.y, pos.z));
      result.normal = glm::vec3(normal.x, normal.y, normal.z);
      closest = distance;
      result.shine = shininess;
      result.refractive_idx = -1;
    }
  }*/
	unordered_set<int> bounding_boxes_hit;

  if (bounding_boxes_enabled)
	  AddBoundingBox(ray, bounding_boxes_hit);

  // Check for triangle intersections
  for (const auto &quad : quads) {
		std::vector<int> boxes = std::get<6>(quad);
    bool valid = false;

    if (bounding_boxes_enabled) {
      for (int i : boxes) {
        if (bounding_boxes_hit.count(i)) {
          valid = true;
          break;
        }
      }

      if (!valid)
        continue;
    }

    std::vector<Vec3> vertices = std::get<0>(quad);
    std::vector<Vec3> vns = std::get<5>(quad);

    // Split the quad into two trigs: (0, 1, 2) and (0, 1, 3)
    std::vector<Vec3> vertices1 = {vertices[0], vertices[1], vertices[2]};
    std::vector<Vec3> vertices2 = {vertices[1], vertices[2], vertices[3]};
    std::vector<Vec3> vns1 = {vns[0], vns[1], vns[2]};
    std::vector<Vec3> vns2 = {vns[1], vns[2], vns[3]};

    distance = ray_intersects_triangle(ray.origin, ray.direction, vertices1,
                                       intersectionPoint);
    // If ray intersects quad and is closer than before, populate result
    if (distance > 0.0f && distance < closest) {
      Vec3 diff = std::get<1>(quad);
      Vec3 spec = get<2>(quad);
      float shininess = get<3>(quad);
      float refractive = get<4>(quad);
      if (refractive > 0.0f && mode == 1)
        continue;
      result.diff = glm::vec3(diff.x, diff.y, diff.z);
      result.spec = glm::vec3(spec.x, spec.y, spec.z);
      result.loc = glm::vec3(intersectionPoint.x, intersectionPoint.y,
                             intersectionPoint.z);
      if (smooth) {
        glm::vec3 normal1 = glm::vec3(vns1[0].x, vns1[0].y, vns1[0].z);
        glm::vec3 normal2 = glm::vec3(vns1[1].x, vns1[1].y, vns1[1].z);
        glm::vec3 normal3 = glm::vec3(vns1[2].x, vns1[2].y, vns1[2].z);
        glm::vec3 vert1 = glm::vec3(vertices1[0].x, vertices1[0].y, vertices1[0].z);
        glm::vec3 vert2 = glm::vec3(vertices1[1].x, vertices1[1].y, vertices1[1].z);
        glm::vec3 vert3 = glm::vec3(vertices1[2].x, vertices1[2].y, vertices1[2].z);
        normal = weightedAverageNormal(intersectionPoint, vert1, vert2, vert3, normal1, normal2, normal3);
      } else {
        normal = glm::cross(glm::vec3(vertices1[1].x - vertices1[0].x,
                                      vertices1[1].y - vertices1[0].y,
                                      vertices1[1].z - vertices1[0].z),
                            glm::vec3(vertices1[2].x - vertices1[0].x,
                                      vertices1[2].y - vertices1[0].y,
                                      vertices1[2].z - vertices1[0].z));
        normal = glm::normalize(-normal); // oops, I needed to flip normals
      }
      result.normal = glm::vec3(normal.x, normal.y, normal.z);
      closest = distance;
      result.shine = shininess;
      result.refractive_idx = refractive;
    }

    distance = ray_intersects_triangle(ray.origin, ray.direction, vertices2,
                                       intersectionPoint);
    // If ray intersects quad and is closer than before, populate result
    if (distance > 0.0f && distance < closest) {
      Vec3 diff = std::get<1>(quad);
      Vec3 spec = get<2>(quad);
      float shininess = get<3>(quad);
      float refractive = get<4>(quad);
      if (refractive > 0.0f && mode == 1)
        continue;
      result.diff = glm::vec3(diff.x, diff.y, diff.z);
      result.spec = glm::vec3(spec.x, spec.y, spec.z);
      result.loc = glm::vec3(intersectionPoint.x, intersectionPoint.y,
                             intersectionPoint.z);
      if (smooth) {
        glm::vec3 normal1 = glm::vec3(vns2[0].x, vns2[0].y, vns2[0].z);
        glm::vec3 normal2 = glm::vec3(vns2[1].x, vns2[1].y, vns2[1].z);
        glm::vec3 normal3 = glm::vec3(vns2[2].x, vns2[2].y, vns2[2].z);
        glm::vec3 vert1 = glm::vec3(vertices2[0].x, vertices2[0].y, vertices2[0].z);
        glm::vec3 vert2 = glm::vec3(vertices2[1].x, vertices2[1].y, vertices2[1].z);
        glm::vec3 vert3 = glm::vec3(vertices2[2].x, vertices2[2].y, vertices2[2].z);
        normal = weightedAverageNormal(intersectionPoint, vert1, vert2, vert3, normal1, normal2, normal3);
      } else {
        normal = glm::cross(glm::vec3(vertices2[1].x - vertices2[0].x,
                                      vertices2[1].y - vertices2[0].y,
                                      vertices2[1].z - vertices2[0].z),
                            glm::vec3(vertices2[2].x - vertices2[0].x,
                                      vertices2[2].y - vertices2[0].y,
                                      vertices2[2].z - vertices2[0].z));
        normal = glm::normalize(normal);
      }

      result.normal = glm::vec3(normal.x, normal.y, normal.z);
      closest = distance;
      result.shine = shininess;
      result.refractive_idx = refractive;
    }
  }
  result.distance = closest;
  return result;
}

// Since none of the slides contain the Phong equation
// I used this as my source
// https://inspirnathan.com/posts/57-shadertoy-tutorial-part-11
// It might look similar, but I can't reinvent the whell since no slide had the
// equation and I had to look it up somewhere on the internet
glm::vec3 Phong(P p, glm::vec3 l, Light light) {
  glm::vec3 diffuse;
  glm::vec3 specular;

  glm::vec3 N = glm::normalize(p.normal);
  glm::vec3 L = glm::normalize(l - p.loc);

  float dotLN = glm::dot(L, N);

  glm::vec3 light_diff = {light.diff.x, light.diff.y, light.diff.z};

  diffuse = p.diff * dotLN * light_diff;

  glm::vec3 R = glm::reflect(L, N);
  glm::vec3 V = glm::normalize(p.loc - camera_pos);

  float dotRV = glm::dot(R, V);
  float alpha = p.shine;

  glm::vec3 light_spec = {light.spec.x, light.spec.y, light.spec.z};

  specular = p.spec * light_spec * pow(dotRV, alpha);

  return diffuse + specular;
}

// Each iteration we should keep track of the current ray
// depth, and the current refractive index
glm::vec3 Trace(Ray ray, int depth, float refractive_idx) {
  glm::vec3 result_color = {0.0f, 0.0f, 0.0f};
  P p = FirstIntersection(ray, 0);

  // this is for black-white depth debugging
  float start = 640.0f;
  float end = 1000.0f;
  float scaledValue = (p.distance - start) / (end - start);
  int remappedValue = static_cast<int>(scaledValue * 255.0f);

  // out of the scene
  if (p.distance == f_inf) {
    // p.diff = glm::vec3(1.0f, 0.0f, 0.0f);
    // printf("hitting background\n");
    return glm::vec3{backgroundx, backgroundy, backgroundz};
  }
  // debugging gray-scale depth map
  // return glm::vec3{remappedValue, remappedValue, remappedValue};

  // get all light contributions
  for (const auto &light : lights) {
    glm::vec3 light_loc(light.pos.x, light.pos.y, light.pos.z);
    Ray light_ray = {light_loc, p.loc - light_loc};
    P l = FirstIntersection(light_ray, 1);

    // need to add some minor offset to account for float precision
    if (glm::distance(l.loc, p.loc) <= 0.1f) {
      glm::vec3 contribution = Phong(p, light_loc, light);
      result_color += contribution;
    }
  }

  // recursion ends or we hit diffuse
  if (depth <= 1 || (p.diff.x > 0.0f || p.diff.y > 0.0f || p.diff.z > 0.0f)) {
    // result_color.x = glm::clamp(result_color.x, 0.0f, 1.0f);
    // result_color.y = glm::clamp(result_color.y, 0.0f, 1.0f);
    // result_color.z = glm::clamp(result_color.z, 0.0f, 1.0f);
    return result_color;
  }

  // setup reflected ray
  Ray reflected;
  reflected.direction = glm::normalize(glm::reflect(ray.direction, p.normal));
  reflected.origin = p.loc + 0.1f * reflected.direction;
  glm::vec3 traced = 0.04f * Trace(reflected, depth - 1, refractive_idx); // add

  // setup refracted ray, if applicable
  if (p.refractive_idx > 0.0f) {
    Ray refracted;
    refracted.direction =
        glm::normalize(Refraction(refractive_idx, p.refractive_idx, p, ray));
    refracted.origin = p.loc + 0.01f * refracted.direction;
    glm::vec3 refracted_trace = Trace(refracted, depth - 1, p.refractive_idx);
    // Add this to the reflected ray result
    traced += 0.94f * refracted_trace;
  }
  // float light_factor = (1.0f / pow((float)(maxdepth-depth+ 1), 2));
  // traced = traced *  light_factor;
  result_color += traced;
  // result_color.x = glm::clamp(result_color.x, 0.0f, 1.0f);
  // result_color.y = glm::clamp(result_color.y, 0.0f, 1.0f);
  // result_color.z = glm::clamp(result_color.z, 0.0f, 1.0f);

  return result_color;
}

void raytrace_blur(int x, int y, int thread_num) {
  //sensor_cell_locs = GenerateSensorCellArray();
  //printf("TEST %d\n", thread_num);
  std::vector<Ray> rays = CalculateRays(x, y);
  glm::vec3 totalColor = glm::vec3(0.0f, 0.0f, 0.0f);
  //printf("LEN: %d\n", sensor_cell_locs.size());
  for (int i = 0; i < sensor_cell_locs.size(); i++) {
    glm::vec3 tracedColor = Trace(rays.at(i), maxdepth, init_refractive_idx);
    totalColor += tracedColor;
  }

  glm::vec3 color = totalColor * (1.0f / sensor_cell_locs.size());
  
  color *= 255;
  // clamp colors
  color.x = std::min(color.x, 255.0f);
  color.y = std::min(color.y, 255.0f);
  color.z = std::min(color.z, 255.0f);
  pixels[x][y] = {static_cast<int>(color.x), static_cast<int>(color.y),
                  static_cast<int>(color.z)};
}

void raytrace(int x, int y, int thread_num) {
  cout << bounding_boxes_enabled << endl;
  Ray ray = CalculateRay(x, y);
  glm::vec3 color = Trace(ray, maxdepth, init_refractive_idx);
  color *= 255;
  // clamp colors
  color.x = std::min(color.x, 255.0f);
  color.y = std::min(color.y, 255.0f);
  color.z = std::min(color.z, 255.0f);
  pixels[x][y] = {static_cast<int>(color.x), static_cast<int>(color.y),
                  static_cast<int>(color.z)};
  return;
}
