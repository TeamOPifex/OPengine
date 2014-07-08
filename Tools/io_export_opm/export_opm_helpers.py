# OPifex Model Format Helpers
# OPifex Entertainment LLC

#################################################################
### Imports
#################################################################

import bpy
import operator

import io_export_opm.export_opm_utilities       as OPMutil
import io_export_opm.export_opm_animation       as OPManim

# For Skinning
MAX_INFLUENCES = 4

#################################################################
### Private Methods
#################################################################

def _generate_face(f, faceIndex, normals, uv_layers, colors, mesh, option_normals, option_colors, option_uv_coords, option_materials, vertex_offset, material_offset):
    isTriangle = ( len(f.vertices) == 3 )

    if isTriangle:
        nVertices = 3
    else:
        nVertices = 4

    hasMaterial = option_materials

    hasFaceUvs = False # not supported in Blender
    hasFaceVertexUvs = option_uv_coords

    hasFaceNormals = False # don't export any face normals (as they are computed in engine)
    hasFaceVertexNormals = option_normals

    hasFaceColors = False       # not supported in Blender
    hasFaceVertexColors = option_colors

    faceType = 0
    faceType = OPMutil.SetBit(faceType, 0, not isTriangle)
    faceType = OPMutil.SetBit(faceType, 1, hasMaterial)
    faceType = OPMutil.SetBit(faceType, 2, hasFaceUvs)
    faceType = OPMutil.SetBit(faceType, 3, hasFaceVertexUvs)
    faceType = OPMutil.SetBit(faceType, 4, hasFaceNormals)
    faceType = OPMutil.SetBit(faceType, 5, hasFaceVertexNormals)
    faceType = OPMutil.SetBit(faceType, 6, hasFaceColors)
    faceType = OPMutil.SetBit(faceType, 7, hasFaceVertexColors)

    faceData = []

    # order is important, must match order in JSONLoader

    # face type
    # vertex indices
    # material index
    # face uvs index
    # face vertex uvs indices
    # face color index
    # face vertex colors indices

    faceData.append(faceType)

    # must clamp in case on polygons bigger than quads

    for i in range(nVertices):
        index = f.vertices[i] + vertex_offset
        faceData.append(index)

    if hasMaterial:
        index = f.material_index + material_offset
        faceData.append( index )

    if hasFaceVertexUvs:
        for layer_index, uvs in enumerate(uv_layers):
            uv = OPMutil.GetUVIndices(faceIndex, uvs, mesh, layer_index)
            for i in range(nVertices):
                index = uv[i]
                faceData.append(index)

    if hasFaceVertexNormals:
        n = OPMutil.GetNormalIndices(f.vertices, normals, mesh)
        for i in range(nVertices):
            index = n[i]
            faceData.append(index)

    if hasFaceVertexColors:
        c = OPMutil.GetColorIndices(faceIndex, colors, mesh)
        for i in range(nVertices):
            index = c[i]
            faceData.append(index)

    return faceData





#################################################################
### Public Methods
#################################################################

def GenerateNormals(normals, option_normals):
    if not option_normals:
        return ""

    chunks = []
    for key, index in sorted(normals.items(), key = operator.itemgetter(1)):
        chunks.append(key)

    return chunks

def GenerateVertexColors(colors, option_colors):
    if not option_colors:
        return ""

    chunks = []
    for key, index in sorted(colors.items(), key=operator.itemgetter(1)):
        chunks.append(key)

    return chunks

def GenerateUVs(uv_layers, option_uv_coords):
    if not option_uv_coords:
        return "[]"

    layers = []
    for uvs in uv_layers:
        chunks = []
        for key, index in sorted(uvs.items(), key=operator.itemgetter(1)):
            chunks.append(key)
        layer = ",".join(generate_uv(n) for n in chunks)
        layers.append(layer)

    return layers

def GenerateBones(meshes, option_bones, flipyz):

    if not option_bones:
        return "", 0

    armature, armature_object = OPManim.GetArmature()
    if armature_object is None:
        return "", 0

    hierarchy = []
    armature_matrix = armature_object.matrix_world
    pose_bones = armature_object.pose.bones
    #pose_bones = armature.bones

    TEMPLATE_BONE = '{"parent":%d,"name":"%s","pos":[%g,%g,%g],"rotq":[%g,%g,%g,%g],"scl":[%g,%g,%g]}'

    for pose_bone in pose_bones:
        armature_bone = pose_bone.bone
        #armature_bone = pose_bone
        bonePos = armature_matrix * armature_bone.head_local
        boneIndex = None

        if armature_bone.parent is None:
            bone_matrix = armature_matrix * armature_bone.matrix_local
            bone_index = -1
        else:
            parent_matrix = armature_matrix * armature_bone.parent.matrix_local
            bone_matrix = armature_matrix * armature_bone.matrix_local
            bone_matrix = parent_matrix.inverted() * bone_matrix

            bone_index = i = 0
            for pose_parent in pose_bones:
                armature_parent = pose_parent.bone
                #armature_parent = pose_parent
                if armature_parent.name == armature_bone.parent.name:
                    bone_index = i
                i += 1

        pos, rot, scl = bone_matrix.decompose()

        if flipyz:
            joint = (bone_index, armature_bone.name, pos.x, pos.z, -pos.y, rot.x, rot.z, -rot.y, rot.w, scl.x, scl.z, scl.y)
            hierarchy.append(joint)
        else:
            joint = (bone_index, armature_bone.name, pos.x, pos.y,  pos.z, rot.x, rot.y,  rot.z, rot.w, scl.x, scl.y, scl.z)
            hierarchy.append(joint)

    return hierarchy, len(pose_bones)

def GenerateIndicesAndWeights(meshes, option_skinning):

    if not option_skinning or len(bpy.data.armatures) == 0:
        return "", ""

    indices = []
    weights = []

    armature, armature_object = OPManim.GetArmature()

    for mesh, object in meshes:

        i = 0
        mesh_index = -1

        # find the original object

        for obj in bpy.data.objects:
            if obj.name == mesh.name or obj == object:
                mesh_index = i
            i += 1

        if mesh_index == -1:
            print("generate_indices: couldn't find object for mesh", mesh.name)
            continue

        object = bpy.data.objects[mesh_index]

        for vertex in mesh.vertices:

            # sort bones by influence

            bone_array = []

            for group in vertex.groups:
                index = group.group
                weight = group.weight

                bone_array.append( (index, weight) )
                
            bone_array.sort(key = operator.itemgetter(1), reverse=True)
            
            # select first N bones

            for i in range(MAX_INFLUENCES):

                if i < len(bone_array):
                    bone_proxy = bone_array[i]
                    
                    found = 0
                    index = bone_proxy[0]
                    weight = bone_proxy[1]

                    for j, bone in enumerate(armature_object.pose.bones):
                        if object.vertex_groups[index].name == bone.name:
                            indices.append(j)
                            weights.append(weight)
                            found = 1
                            break

                    if found != 1:
                        indices.append(0)
                        weights.append(0)

                else:
                    indices.append(0)
                    weights.append(0)
    
    

    return indices, weights

def GenerateFaces(normals, uv_layers, colors, meshes, option_normals, option_colors, option_uv_coords, option_materials, option_faces):

    if not option_faces:
        return "", 0

    vertex_offset = 0
    material_offset = 0

    chunks = []
    for mesh, object in meshes:

        vertexUV = len(mesh.uv_textures) > 0
        vertexColors = len(mesh.vertex_colors) > 0

        mesh_colors = option_colors and vertexColors
        mesh_uvs = option_uv_coords and vertexUV

        if vertexUV:
            active_uv_layer = mesh.uv_textures.active
            if not active_uv_layer:
                mesh_extract_uvs = False

        if vertexColors:
            active_col_layer = mesh.vertex_colors.active
            if not active_col_layer:
                mesh_extract_colors = False

        for i, f in enumerate(OPMutil.GetFaces(mesh)):
            face = _generate_face(f, i, normals, uv_layers, colors, mesh, option_normals, mesh_colors, mesh_uvs, option_materials, vertex_offset, material_offset)
            chunks.append(face)

        vertex_offset += len(mesh.vertices)

        material_count = len(mesh.materials)
        if material_count == 0:
            material_count = 1

        material_offset += material_count

    return chunks, len(chunks)

def VertexNormals(mesh, normals, count):
    for f in OPMutil.GetFaces(mesh):
        for v in f.vertices:

            normal = mesh.vertices[v].normal
            key = OPMutil.Veckey3d(normal)

            if key not in normals:
                normals[key] = count
                count += 1

    return count


def VertexColors(mesh, colors, count):
    color_layer = mesh.tessface_vertex_colors.active.data

    for face_index, face in enumerate(OPMutil.GetFaces(mesh)):

        face_colors = color_layer[face_index]
        face_colors = face_colors.color1, face_colors.color2, face_colors.color3, face_colors.color4

        for c in face_colors:
            key = hexcolor(c)
            if key not in colors:
                colors[key] = count
                count += 1

    return count


def UVs(mesh, uv_layers, counts):
    for index, layer in enumerate(mesh.tessface_uv_textures):

        if len(uv_layers) <= index:
            uvs = {}
            count = 0
            uv_layers.append(uvs)
            counts.append(count)
        else:
            uvs = uv_layers[index]
            count = counts[index]

        uv_layer = layer.data

        for face_index, face in enumerate(OPMutil.GetFaces(mesh)):

            for uv_index, uv in enumerate(uv_layer[face_index].uv):

                key = OPMutil.Veckey2d(uv)
                if key not in uvs:
                    uvs[key] = count
                    count += 1

        counts[index] = count

    return counts