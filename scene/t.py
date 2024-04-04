import bpy
import math

verts = []
faces = []
edges = []

sphere_radius = 24
sphere_radius2 = 12
lens_radius = 8


divisions_theta = 12

divisions_z = 4
    
#generate circle section

#print("Hello, world!")
#print("Z length: %f", z_length)

def generate_circle_section(sphere_radius, lens_radius, divisions_theta, divisions_z, direction):
    section_verts = []
    section_faces = []
    section_edges = []
        
    initial_z = math.sqrt(math.pow(sphere_radius, 2) - math.pow(lens_radius, 2))
    z_length = sphere_radius - initial_z
    
    DELTA_theta = 2 * math.pi / divisions_theta
    DELTA_z = z_length / divisions_z
    
    theta = 0
    z = 0
    for i in range (divisions_z):
        z = i * DELTA_z 
        local_radius = math.sqrt(math.pow(sphere_radius, 2) - (math.pow((z + initial_z), 2)))
        print("RADIUS: {}", local_radius)
        for j in range(divisions_theta):
            #print("i: %d\n", i)
            theta += DELTA_theta
            vert = (local_radius * math.cos(theta), local_radius * math.sin(theta), z * direction)
            section_verts.append(vert)
            
    section_verts.append((0, 0, z_length * direction))
            
    for i in range (divisions_z - 1):
        for j in range(divisions_theta):
            A = i * divisions_theta + j
            B = i * divisions_theta + (j + 1) % divisions_theta
            C = (i + 1) * divisions_theta + j
            D = (i + 1) * divisions_theta + (j + 1) % divisions_theta
            
            print(A, B, D, C)
            
            face = (A,B, D, C)
            section_faces.append(face)
        
    last_ring = (divisions_z - 1) * divisions_theta
    for i in range(divisions_theta):
        A = last_ring + i
        B = last_ring + (i + 1) % divisions_theta
        C = divisions_theta * divisions_z
        face = (A, B, C)
        section_faces.append(face)
    return [section_verts, section_faces]    
    

#create mesh and object
verts, faces = generate_circle_section(sphere_radius, lens_radius, divisions_theta, divisions_z, 1)
mymesh = bpy.data.meshes.new("lens1")
myobject = bpy.data.objects.new("lens1",mymesh)
 
#set mesh location
myobject.location = (0, 0, 0)
bpy.context.collection.objects.link(myobject)
 
#create mesh from python data
mymesh.from_pydata(verts,edges,faces)
mymesh.update(calc_edges=True)


#create mesh and object
verts2, faces2 = generate_circle_section(sphere_radius2, lens_radius, divisions_theta, divisions_z, 1)
mymesh2 = bpy.data.meshes.new("lens2")
myobject2 = bpy.data.objects.new("lens2",mymesh2)
    
#set mesh location
myobject2.location = (0, 0, 0)
bpy.context.collection.objects.link(myobject2)
 
#create mesh from python data
mymesh2.from_pydata(verts2,edges,faces2)
mymesh2.update(calc_edges=True)
    
bpy.data.objects["lens2"].select_set(True)
bpy.data.objects["lens1"].select_set(True)
bpy.context.view_layer.objects.active = myobject
bpy.ops.object.join()

myobject.name = "lens"

bpy.ops.object.mode_set(mode='EDIT')


# show mesh as smooth
mypolys = mymesh.polygons
for p in mypolys:
    p.use_smooth = True

# remove duplicate vertices
bpy.ops.mesh.remove_doubles() 
 
# recalculate normals
bpy.ops.mesh.normals_make_consistent(inside=False)
bpy.ops.object.mode_set(mode='OBJECT')

bpy.ops.object.select_all(action='DESELECT')

