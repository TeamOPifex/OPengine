# ################################################################
# OPifex Entertainment LLC
#
# This program exports Blender Models into the OPifex Model Format
# 
# ################################################################

# ################################################################
# Init
# ################################################################

bl_info = {
    "name" : "Export OPifex Mesh (.opm)",
    "author" : "OPifex Entertainment LLC",
    "version": (0, 0, 3),
    "blender": (2, 7, 1),
    "location": "File > Export > OPifex (.opm)",
    "description": "Export OPifex models",
    "category": "Import-Export"}

# Try to reload the package but only if there's an available package var

import bpy

if "bpy" in locals():
    import imp
    if "export_opm" in locals():
        imp.reload(export_opm)

from bpy.props import *
from bpy_extras.io_utils import ExportHelper

# ################################################################
# Exporter Settings
# ################################################################

SETTINGS_FILE = "opifex_model_settings.js"

import os
import json

# Ensures read permissions, as well as access
def file_exists(filename):
    try:
        f = open(filename, 'r')
        f.close()
        return True
    except IOError:
        return False

def get_settings_fullpath():
    return os.path.join(bpy.app.tempdir, SETTINGS_FILE)

def save_opifex_settings(properties):
    settings = {
    }

    fname = get_settings_fullpath()
    f = open(fname, "w")
    json.dump(settings, f)

def restore_opifex_settings(properties):
    settings = {}

    fname = get_settings_fullpath()
    if file_exists(fname):
        f = open(fname, "r")
        settings = json.load(f)

    properties.option_vertices = settings.get("option_vertices", True)
    properties.option_vertices_truncate = settings.get("option_vertices_truncate", False)
    properties.option_faces = settings.get("option_faces", True)
    properties.option_normals = settings.get("option_normals", True)

    properties.option_colors = settings.get("option_colors", True)
    properties.option_uv_coords = settings.get("option_uv_coords", True)
    properties.option_materials = settings.get("option_materials", True)

    properties.option_skinning = settings.get("option_skinning", True)
    properties.option_bones = settings.get("option_bones", True)

    properties.align_model = settings.get("align_model", "None")

    properties.option_scale = settings.get("option_scale", 1.0)
    properties.option_flip_yz = settings.get("option_flip_yz", True)

    properties.option_export_scene = settings.get("option_export_scene", False)
    properties.option_embed_meshes = settings.get("option_embed_meshes", True)
    properties.option_copy_textures = settings.get("option_copy_textures", False)

    properties.option_animation_morph = settings.get("option_animation_morph", False)
    properties.option_animation_skeletal = settings.get("option_animation_skeletal", False)
    properties.option_frame_index_as_time = settings.get("option_frame_index_as_time", False)

    properties.option_frame_step = settings.get("option_frame_step", 1)
    properties.option_all_meshes = settings.get("option_all_meshes", True)
    

# ################################################################
# Exporter
# ################################################################

class ExportOPM(bpy.types.Operator, ExportHelper):
    bl_idname = "export.opm"
    bl_label = "Export OPifex Model (.opm)"

    filename_ext = ".opm"

    option_vertices = BoolProperty(name = "Vertices", description = "Export vertices", default = True)
    option_vertices_deltas = BoolProperty(name = "Deltas", description = "Delta vertices", default = False)
    option_vertices_truncate = BoolProperty(name = "Truncate", description = "Truncate vertices", default = False)

    option_faces = BoolProperty(name = "Faces", description = "Export faces", default = True)
    option_faces_deltas = BoolProperty(name = "Deltas", description = "Delta faces", default = False)

    option_normals = BoolProperty(name = "Normals", description = "Export normals", default = True)

    option_colors = BoolProperty(name = "Colors", description = "Export vertex colors", default = True)
    option_uv_coords = BoolProperty(name = "UVs", description = "Export texture coordinates", default = True)
    option_materials = BoolProperty(name = "Materials", description = "Export materials", default = True)

    option_skinning = BoolProperty(name = "Skinning", description = "Export skin data", default = True)
    option_bones = BoolProperty(name = "Bones", description = "Export bones", default = True)

    align_types = [("None","None","None"), ("Center","Center","Center"), ("Bottom","Bottom","Bottom"), ("Top","Top","Top")]
    align_model = EnumProperty(name = "Align model", description = "Align model", items = align_types, default = "None")

    option_scale = FloatProperty(name = "Scale", description = "Scale vertices", min = 0.01, max = 1000.0, soft_min = 0.01, soft_max = 1000.0, default = 1.0)
    option_flip_yz = BoolProperty(name = "Flip YZ", description = "Flip YZ", default = True)

    option_export_scene = BoolProperty(name = "Scene", description = "Export scene", default = False)
    option_embed_meshes = BoolProperty(name = "Embed meshes", description = "Embed meshes", default = True)
    option_copy_textures = BoolProperty(name = "Copy textures", description = "Copy textures", default = False)

    option_animation_morph = BoolProperty(name = "Morph animation", description = "Export animation (morphs)", default = False)
    option_animation_skeletal = BoolProperty(name = "Skeletal animation", description = "Export animation (skeletal)", default = False)
    option_frame_index_as_time = BoolProperty(name = "Frame index as time", description = "Use (original) frame index as frame time", default = False)

    option_frame_step = IntProperty(name = "Frame step", description = "Animation frame step", min = 1, max = 1000, soft_min = 1, soft_max = 1000, default = 1)
    option_all_meshes = BoolProperty(name = "All meshes", description = "All meshes (merged)", default = True)

    def invoke(self, context, event):
        restore_opifex_settings(self.properties)
        return ExportHelper.invoke(self, context, event)

    @classmethod
    def poll(cls, context):
        return context.active_object != None
    
    

    def execute(self, context):
        print("Selected Object: " + context.active_object.name)

        if not self.properties.filepath:
            raise Exception("filename not set")

        save_opifex_settings(self.properties)

        filepath = self.filepath

        import io_export_opm.export_opm
        return io_export_opm.export_opm.save(self, context, **self.properties)

    def draw(self, context):
        layout = self.layout

        row = layout.row()
        row.label(text="Geometry:")

        row = layout.row()
        row.prop(self.properties, "option_vertices")
        # row = layout.row()
        # row.enabled = self.properties.option_vertices
        # row.prop(self.properties, "option_vertices_deltas")
        row.prop(self.properties, "option_vertices_truncate")
        layout.separator()

        row = layout.row()
        row.prop(self.properties, "option_faces")
        row = layout.row()
        row.enabled = self.properties.option_faces
        # row.prop(self.properties, "option_faces_deltas")
        layout.separator()

        row = layout.row()
        row.prop(self.properties, "option_normals")
        layout.separator()

        row = layout.row()
        row.prop(self.properties, "option_bones")
        row.prop(self.properties, "option_skinning")
        layout.separator()

        row = layout.row()
        row.label(text="Materials:")

        row = layout.row()
        row.prop(self.properties, "option_uv_coords")
        row.prop(self.properties, "option_colors")
        row = layout.row()
        row.prop(self.properties, "option_materials")
        layout.separator()

        row = layout.row()
        row.label(text="Settings:")

        row = layout.row()
        row.prop(self.properties, "align_model")
        row = layout.row()
        row.prop(self.properties, "option_flip_yz")
        row.prop(self.properties, "option_scale")
        layout.separator()

        row = layout.row()
        row.label(text="--------- Experimental ---------")
        layout.separator()

        row = layout.row()
        row.label(text="Scene:")

        row = layout.row()
        row.prop(self.properties, "option_export_scene")
        row.prop(self.properties, "option_embed_meshes")

        row = layout.row()
        row.label(text="Animation:")

        row = layout.row()
        row.prop(self.properties, "option_animation_morph")
        row = layout.row()
        row.prop(self.properties, "option_animation_skeletal")
        row = layout.row()
        row.prop(self.properties, "option_frame_index_as_time")
        row = layout.row()
        row.prop(self.properties, "option_frame_step")
        layout.separator()

        row = layout.row()
        row.label(text="Settings:")

        row = layout.row()
        row.prop(self.properties, "option_all_meshes")

        row = layout.row()
        row.prop(self.properties, "option_copy_textures")

        layout.separator()


# ################################################################
# Common
# ################################################################

def menu_func(self, context):
    default_path = bpy.data.filepath.replace(".blend", ".opm")
    self.layout.operator(ExportOPM.bl_idname, text="OPifex Model (.opm)").filepath = default_path

def register():
    bpy.utils.register_module(__name__)
    bpy.types.INFO_MT_file_export.append(menu_func)

def unregister():
    bpy.utils.unregister_module(__name__)
    bpy.types.INFO_MT_file_export.remove(menu_func)

if __name__ == "__main__":
    register()
