# OPifex Model Format Main
# OPifex Entertainment LLC

# TODO - JSON FORMAT

#################################################################
### Imports
#################################################################

import bpy
import io_export_op_level.export_op_utilities       as OPutil
import io_export_op_level.export_op_binary          as OPbinary


#################################################################
### Private Methods
#################################################################

def _export_level(objects, scene, filepath, options):

    #result = OPMmeshGenerate.Mesh(objects, scene, options)

    objs = []

    for object in objects:
        OPutil.Print(object.name)
        if object.type == "MESH":
            mesh = object.to_mesh(scene, True, "RENDER")
            OPutil.Print("It's a mesh")

        OPutil.Print(str(object))
        OPutil.Print(str(object.type))
        OPutil.Print(str(object.location))
        OPutil.Print(str(object.rotation_euler))
        OPutil.Print(str(object.scale))
        OPutil.Print(str(object.data))
        OPutil.Print(str(object.OPifexModel))

        objType = 0 # Unkown

        if object.type == "MESH":
            objType = 1
        if object.type == "CAMERA":
            objType = 2
        if object.type == "LIGHT":
            objType = 3

        hasMesh = 1
        if object.OPifexModel == "":
            hasMesh = 0

        OPutil.Print("Has Mesh? " + str(hasMesh))

        result = {
            "name" : object.name,
            "hasMesh" : hasMesh,
            "opm" : object.OPifexModel,
            "type" : objType,
            "position" : [object.location.x, object.location.y, object.location.z],
            "rotation" : [object.rotation_euler.x, object.rotation_euler.y, object.rotation_euler.z],
            "scale" : [object.scale.x, object.scale.y, object.scale.z]
        }
        objs.append(result)



    fp = OPbinary.OpenFile(filepath)
    OPutil.Print("Obj Count: " + str(len(objs)))

    OPbinary.UShort(fp, len(objs)) # Number of Objs
    for obj in objs:
        OPbinary.String(fp, obj["name"])
        OPbinary.UShort(fp, obj["hasMesh"])
        OPbinary.String(fp, obj["opm"])
        OPbinary.UShort(fp, obj["type"])
        OPbinary.Vec3(fp, obj["position"])
        OPbinary.Vec3(fp, obj["rotation"])
        OPbinary.Vec3(fp, obj["scale"])


    # OPLVLwriter.Binary(filepath, result, options)


#################################################################
### Public Methods
#################################################################

def save(operator, context, options, filepath = "" ):
    
    filepath = OPutil.EnsureExtension(filepath, '.oplvl')

    OPutil.Print("Creating OPifex Level (.oplvl)...")

    scene = context.scene
    if scene.objects.active:
        bpy.ops.object.mode_set(mode='OBJECT')

    sceneobjects = scene.objects

    objects = []
    for obj in sceneobjects:
        objects.append(obj)

    _export_level(objects, scene, filepath, options)
        
    return {'FINISHED'}