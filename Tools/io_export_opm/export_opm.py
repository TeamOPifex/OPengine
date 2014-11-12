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



#################################################################
### Private Methods
#################################################################

def _export_mesh(objects, scene, filepath, options):

    result = OPMmeshGenerate.Mesh(objects, scene, options)
    meta = OPMhelpers.ExtractMeta(scene, options)
    OPMwriter.Binary(filepath, result, meta, options)


#################################################################
### Public Methods
#################################################################

def save(operator, context, options, filepath = "" ):
    
    filepath = OPMutil.EnsureExtension(filepath, '.opm')

    OPMutil.Print("Creating OPifex Model (.opm)...")

    scene = context.scene
    if scene.objects.active:
        bpy.ops.object.mode_set(mode='OBJECT')

    if options.all_meshes:
        sceneobjects = scene.objects
    else:
        sceneobjects = context.selected_objects

    objects = []
    for obj in sceneobjects:
        objects.append(obj)

    if options.export_scene:
        OPMutil.Print("UNSUPPORTED")
    else:
        _export_mesh(objects, scene, filepath, options)
        
    return {'FINISHED'}