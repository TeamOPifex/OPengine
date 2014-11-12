# ################################################################
# OPifex Entertainment LLC
#
# This program exports Blender Scenes into the OPifex Level Format
# 
# ################################################################

# ################################################################
# Init
# ################################################################

bl_info = {
    "name" : "Export OPifex Level (.oplvl)",
    "author" : "OPifex Entertainment LLC",
    "version": (0, 0, 1),
    "blender": (2, 7, 1),
    "location": "File > Export > OPifex Level (.oplvl)",
    "description": "Export OPifex levels",
    "category": "Import-Export"}

# Try to reload the package but only if there's an available package var

import bpy

if "bpy" in locals():
    import imp
    if "export_oplvl" in locals():
        imp.reload(export_oplvl)

from bpy.props import *
from bpy_extras.io_utils import ExportHelper

# ################################################################
# Exporter Settings
# ################################################################

class Struct:
    def __init__(self, **entries): 
        self.__dict__.update(entries)

SETTINGS_FILE = "opifex_level_settings.js"

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
    settings = properties

    fname = get_settings_fullpath()
    f = open(fname, "w")
    json.dump(settings, f)

def restore_opifex_settings(properties):
    settings = {}

    fname = get_settings_fullpath()
    if file_exists(fname):
        f = open(fname, "r")
        settings = json.load(f)

    properties.option_vertices = settings.get("vertices", True)
    

# ################################################################
# Exporter
# ################################################################

class ExportOPM(bpy.types.Operator, ExportHelper):
    bl_idname = "export.oplvl"
    bl_label = "Export OPifex Level (.oplvl)"

    filename_ext = ".oplvl"

    option_vertices = BoolProperty(name = "Vertices", description = "Export vertices", default = True)

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


        optionsDict = {
            "vertices": self.option_vertices
        }
        options = Struct(**optionsDict)
        save_opifex_settings(optionsDict)

        filepath = self.filepath

        import io_export_op_level.export_op_level
        return io_export_op_level.export_op_level.save(self, context, options, filepath)

    def draw(self, context):
        layout = self.layout

        row = layout.row()
        row.label(text="Geometry:")

        row = layout.row()
        row.prop(self.properties, "option_vertices")
        # row = layout.row()
        # row.enabled = self.properties.option_vertices
        # row.prop(self.properties, "option_vertices_deltas")
        #row.prop(self.properties, "option_vertices_truncate")
        #layout.separator()

# ################################################################
# Common
# ################################################################


def menu_func(self, context):
    default_path = bpy.data.filepath.replace(".blend", ".oplvl")
    self.layout.operator(ExportOPM.bl_idname, text="OPifex Level (.oplvl)").filepath = default_path

def register():

    bpy.utils.register_module(__name__)
    bpy.types.INFO_MT_file_export.append(menu_func)

def unregister():
    bpy.utils.unregister_module(__name__)
    bpy.types.INFO_MT_file_export.remove(menu_func)

if __name__ == "__main__":
    register()

