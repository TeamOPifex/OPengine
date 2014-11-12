# OPifex Model Format Utilities
# OPifex Entertainment LLC

#################################################################
### Imports
#################################################################

import bpy
import mathutils
import shutil
import os
import os.path
import math
import operator
import random
import threading
import time

import logging
logging.basicConfig(level=logging.DEBUG,format='(%(threadName)-10s) %(message)s',)


#################################################################
### Private Methods
#################################################################

######################################
# Defaults / Configuration
######################################

DEFAULTS = {
    "position" : [0, 0, 0],
    "rotation" : [0, 0, 0],
    "scale" : [1, 1, 1]
    }

ROTATE_X_PI2 = mathutils.Quaternion((1.0, 0.0, 0.0), math.radians(-90.0)).to_matrix().to_4x4()

def _veckey3(x,y,z):
    return round(x, 6), round(y, 6), round(z, 6)


#################################################################
### Public Methods
#################################################################

def EnsureExtension(filepath, extension):
    if not filepath.lower().endswith(extension):
        filepath += extension
    return filepath

def GetBit(value, position):
    return value & ( 1 << position )

def GetColorIndices(face_index, colors, mesh):
    c = []
    color_layer = mesh.tessface_vertex_colors.active.data
    face_colors = color_layer[face_index]
    face_colors = face_colors.color1, face_colors.color2, face_colors.color3, face_colors.color4
    for i in face_colors:
        c.append( colors[HexColor(i)] )
    return c

def GetFaces(obj):
    if hasattr(obj, "tessfaces"):
        return obj.tessfaces
    else:
        return obj.faces

def GetNormalIndices(v, normals, mesh):
    n = []
    mv = mesh.vertices
    for i in v:
        normal = mv[i].normal
        key = Veckey3d(normal)
        n.append( normals[key] )
    return n

def GetUVIndices(face_index, uvs, mesh, layer_index):
    uv = []
    uv_layer = mesh.tessface_uv_textures[layer_index].data
    #uv_layer = mesh.tessface_uv_textures.active.data
    for i in uv_layer[face_index].uv:
        uv.append( uvs[Veckey2d(i)] )
    return uv

def HexColor(c):
    return ( int(c[0] * 255) << 16  ) + ( int(c[1] * 255) << 8 ) + int(c[2] * 255)

def Print(obj):    
    logging.debug(obj)

def Rgb2int(rgb):
    color = (int(rgb[0]*255) << 16) + (int(rgb[1]*255) << 8) + int(rgb[2]*255);
    return color
    
def SetBit(value, position, on):
    if on:
        mask = 1 << position
        return (value | mask)
    else:
        mask = ~(1 << position)
        return (value & mask)

def Veckey2d(v):
    return round(v[0], 6), round(v[1], 6)

def Veckey3d(v):
    return _veckey3(v.x, v.y, v.z)