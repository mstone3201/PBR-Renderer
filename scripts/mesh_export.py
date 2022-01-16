import bpy
import struct

D = bpy.data

vertices = {}
indices = []
index = 0
materials = {}
material_id = 0
materials_packed = ""

for obj in D.objects:
    if obj.type == "MESH":
        mesh = obj.data
        mesh.calc_tangents()
        
        material = obj.material_slots.data.active_material.name
        
        if material not in materials:
            materials[material] = material_id
            material_id += 1
            materials_packed += material + "\0"
        
        startVertexCount = len(vertices)
        startIndexCount = len(indices)

        for face in mesh.polygons:
            for loop_index in reversed(face.loop_indices): # use the right winding order
                loop = mesh.loops[loop_index]
                vertex = mesh.vertices[loop.vertex_index]
                uv = mesh.uv_layers.active.data[loop_index].uv
                packed_data = struct.pack('<fffffffffffffff',
                    vertex.co.x,
                    vertex.co.y,
                    vertex.co.z,
                    loop.tangent.x,
                    loop.tangent.y,
                    loop.tangent.z,
                    loop.bitangent.x,
                    loop.bitangent.y,
                    loop.bitangent.z,
                    loop.normal.x,
                    loop.normal.y,
                    loop.normal.z,
                    uv.x,
                    uv.y,
                    materials[material])
                    
                if packed_data not in vertices:
                    vertices[packed_data] = index
                    index += 1
                indices.append(vertices[packed_data])

        print("\"" + obj.name + "\" has", len(vertices) - startVertexCount, "vertices and", len(indices) - startIndexCount, "indices and uses material \"" + material + "\"")

file = open(D.filepath[:D.filepath.rfind('.') + 1] + "mesh", 'wb')
file.write(struct.pack('<III', len(vertices) * 4 * 15, len(indices) * 4, len(materials_packed)))
for packed_data in vertices:
    file.write(packed_data)
for index in indices:
    file.write(struct.pack('<I', index))
file.write(struct.pack(f'<{len(materials_packed)}s', materials_packed.encode('ascii')))

print("Finished generating \"" + file.name[file.name.rfind('\\') + 1:] + "\"\n")
file.close()
                    
# Input layout (float15)
# POSITION (float3)
# TANGENT (float3)
# BINORMAL (float3)
# NORMAL (float3)
# UV (float3: U, V, MAT_ID)