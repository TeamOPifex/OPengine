# OPifex Model Format Main
# OPifex Entertainment LLC

# TODO - JSON FORMAT

#################################################################
### Imports
#################################################################

import bpy

import io_export_opm.export_opm_utilities       as OPMutil
import io_export_opm.export_opm_alignment       as OPMalign
import io_export_opm.export_opm_helpers         as OPMhelpers
import io_export_opm.export_opm_mesh_generation as OPMmeshGenerate
import io_export_opm.export_opm_writer          as OPMwriter

class Struct:
    def __init__(self, **entries): 
        self.__dict__.update(entries)


#################################################################
### Private Methods
#################################################################

def _export_mesh(objects, scene, filepath, options):

    result = OPMmeshGenerate.Mesh(objects, scene, options)

    OPMwriter.Binary(filepath, result, options)


#################################################################
### Public Methods
#################################################################

def save(operator, context, filepath = "",
         option_flip_yz = True,
         option_vertices = True,
         option_vertices_truncate = False,
         option_faces = True,
         option_normals = True,
         option_uv_coords = True,
         option_materials = False,
         option_colors = True,
         option_bones = True,
         option_skinning = True,
         align_model = 0,
         option_export_scene = False,
         option_scale = 1.0,
         option_embed_meshes = True,
         option_copy_textures = False,
         option_animation_morph = False,
         option_animation_skeletal = False,
         option_frame_step = 1,
         option_all_meshes = True,
         option_frame_index_as_time = False):
    
    filepath = OPMutil.EnsureExtension(filepath, '.opm')

    OPMutil.Print("Creating OPifex Model (.opm)...")

    scene = context.scene
    if scene.objects.active:
        bpy.ops.object.mode_set(mode='OBJECT')

    if option_all_meshes:
        sceneobjects = scene.objects
    else:
        sceneobjects = context.selected_objects

    objects = []
    for obj in sceneobjects:
        objects.append(obj)

    optionsDict = {
        "vertices": option_vertices,
        "vertices_truncate": option_vertices_truncate,
        "faces": option_faces,
        "normals": option_normals,
        "uv_coords": option_uv_coords,
        "materials": option_materials,
        "colors": option_colors,
        "bones": option_bones,
        "skinning": option_skinning,
        "align_model": align_model,
        "flip_yz": option_flip_yz,
        "scale": option_scale,
        "single_model": True,            # export_single_model
        "copy_textures": option_copy_textures,
        "animation_morph": option_animation_morph,
        "animation_skeletal": option_animation_skeletal,
        "frame_step": option_frame_step,
        "frame_index_as_time": option_frame_index_as_time
    }
    options = Struct(**optionsDict)
    
    OPMutil.Print(optionsDict)

    if option_export_scene:
        OPMutil.Print("UNSUPPORTED")
    else:
        _export_mesh(objects, scene, filepath, options)
        
    return {'FINISHED'}