# Mesh Generation
# OPifex Entertainment LLC


#################################################################
### Imports
#################################################################

import bpy
import math
import mathutils

import io_export_opm.export_opm_utilities       as OPMutil
import io_export_opm.export_opm_helpers         as OPMhelpers
import io_export_opm.export_opm_alignment       as OPMalign


#################################################################
### Private Methods
#################################################################

def _extract_meshes(objects, scene, export_single_model, option_scale, flipyz):
    meshes = []
    for object in objects:
        if object.type == "MESH":
            mesh = object.to_mesh(scene, True, "RENDER")
            if not mesh:
                raise Exception("Error, could not get the mesh from object [%s]" % object.name)

            mesh.name = object.name
            if(export_single_model):
               if flipyz:
                   X_ROT = mathutils.Matrix.Rotation(-math.pi/2, 4, 'X')
                   mesh.transform(X_ROT * object.matrix_world)
               else:
                   mesh.transform(object.matrix_world)
            mesh.update(calc_tessface=True)
            mesh.calc_normals()
            mesh.calc_tessface()
            mesh.transform(mathutils.Matrix.Scale(option_scale, 4))
            meshes.append([mesh, object])
    return meshes
            

def _generate_model(meshes, morphs, scene, options):

    vertices = []

    #vertex_offset = 0
    #vertex_offsets = []

    nnormal = 0
    normals = {}

    ncolor = 0
    colors = {}

    nuvs = []
    uv_layers = []

    for mesh, object in meshes:

        vertexUV = len(mesh.uv_textures) > 0
        vertexColors = len(mesh.vertex_colors) > 0

        mesh_extract_colors = options.colors and vertexColors
        mesh_extract_uvs = options.uv_coords and vertexUV

        if vertexUV:
            active_uv_layer = mesh.uv_textures.active
            if not active_uv_layer:
                mesh_extract_uvs = False

        if vertexColors:
            active_col_layer = mesh.vertex_colors.active
            if not active_col_layer:
                mesh_extract_colors = False

        #vertex_offsets.append(vertex_offset)
        #vertex_offset += len(vertices)

        for i in range(len(mesh.vertices)):
            vertices.append([mesh.vertices[i].co[0], mesh.vertices[i].co[1], mesh.vertices[i].co[2]])

        if options.normals:
            nnormal = OPMhelpers.VertexNormals(mesh, normals, nnormal)

        if mesh_extract_colors:
            ncolor = OPMhelpers.VertexColors(mesh, colors, ncolor)

        if mesh_extract_uvs:
            nuvs = OPMhelpers.UVs(mesh, uv_layers, nuvs)

    if options.align_model == 1:
        OPMalign.Center(vertices)
    elif options.align_model == 2:
        OPMalign.Bottom(vertices)
    elif options.align_model == 3:
        OPMalign.Top(vertices)

    faces, nfaces = OPMhelpers.GenerateFaces(normals, uv_layers, colors, meshes, options.normals, options.colors, options.uv_coords, options.materials, options.faces)
    bones, nbone = OPMhelpers.GenerateBones(meshes, options.bones, options.flip_yz)
    indices, weights = OPMhelpers.GenerateIndicesAndWeights(meshes, options.skinning)

    model_result = {
        "scale" : options.scale,

        "uvs"       : OPMhelpers.GenerateUVs(uv_layers, options.uv_coords),
        "normals"   : OPMhelpers.GenerateNormals(normals, options.normals),
        "colors"    : OPMhelpers.GenerateVertexColors(colors, options.colors),
        "vertices" : vertices,
        "faces"    : faces,
        "bones"     : bones,
        "indices"   : indices,
        "weights"   : weights,
        #"animations" : generate_all_animations(option_animation_skeletal, option_frame_step, flipyz, option_frame_index_as_time)
        }

    result = {
        "nvertex"   : len(vertices),
        "nuvs"      : len(nuvs),
        "nnormal"   : nnormal,
        "ncolor"    : ncolor,
        "nface"     : nfaces,
        "nbone"     : nbone,

        "model"     : model_result
        }


    return result


#################################################################
### Public Methods
#################################################################

def Mesh(objects, scene, options):
    
    meshes = _extract_meshes(objects, scene, options.single_model, options.scale, options.flip_yz)

    OPMutil.Print("Meshes extracted from scene")

    morphs = []
    
    result = _generate_model(meshes, morphs, scene, options)

    # remove temp meshes

    for mesh, object in meshes:
        bpy.data.meshes.remove(mesh)

    return result