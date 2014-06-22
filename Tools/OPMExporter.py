bl_info = {
	"name": "Export OPifex Mesh (.opm)",
	"author": "Garrett Hoofman",
	"version": (1, 0),
	"blender": (2, 59, 0),
	"location": "File > Export > OPifex (.opm)",
	"description": "Export Model to OPifex Mesh",
	"warning": "",
	"wiki_url": "",
	"tracker_url": "",
	"category": "Import-Export"}

#!BPY
"""
me: 'OPMeshBinary (.opm)...'
Blender: 269
Group: 'Export'
Tooltip: 'OPMeshBinary Export'
"""

# import OPmeshBinary
# from imp import reload
# reload(OPmeshBinary)

# __name__ = "OPM Export" 

import bpy
from bpy.props import *
import os
from os import remove
import itertools
import math
import struct
import mathutils
import operator
import time
import bpy_extras
from bpy_extras.io_utils import ExportHelper 
import shutil

#------------------------------------------------------------------------------
def r3d(v):
    return round(v[0],6), round(v[1],6), round(v[2],6)
#------------------------------------------------------------------------------
def r2d(v):
    return round(v[0],6), round(v[1],6)
#------------------------------------------------------------------------------
def cross(a, b):
	return [
		a[1] * b[2] - a[2] * b[1],
		a[2] * b[0] - a[0] * b[2],
		a[0] * b[1] - a[1] * b[0]
	];
#------------------------------------------------------------------------------
def normalize(v):
	l = dist(0, 0, 0, v[0], v[1], v[2])
	return [v[0] / l, v[1] / l, v[2] / l]
#------------------------------------------------------------------------------
def sub(v1, v2):
	return [v1[0] - v2[0], v1[1] - v2[1], v1[2] - v2[2]]
#------------------------------------------------------------------------------
def dist(x0, y0, z0, x1, y1, z1):
	dx = x0 - x1
	dy = y0 - y1
	dz = z0 - z1
	return math.sqrt(dx * dx + dy * dy + dz * dz)
#------------------------------------------------------------------------------
def WriteData(msh, fp, writeFunc, settings):
	local_dict = {} # local dictionary
	local_faces_list   = [] # lcoal faces index list
	local_vertex_list  = [] # local vertex list
	local_normal_list  = [] # local normal list
	local_tangent_list = [] # local tangent list
	local_uv_list = [] # local uv list
	local_vertex_count = 0 # local vertices count
	local_color_list = []
	isSmooth = False
	hasUV = True    # true by default, it will be verified below
	
	print('Writing Data')

	if (len(msh.tessface_uv_textures)>0):
		if (msh.tessface_uv_textures.active is None):
			hasUV=False
	else:
		hasUV = False

	if (hasUV):
		activeUV = msh.tessface_uv_textures.active.data

	for i,f in enumerate(msh.tessfaces):
		isSmooth = f.use_smooth
		tmpfaces = []
		for j,v in enumerate(f.vertices):  
			vec = msh.vertices[v].co
			vec = r3d(vec)

			if (isSmooth):  # use vertex normal
				nor = msh.vertices[v].normal
			else:           # use face normal
				nor = f.normal

			nor = r3d(nor)

			# calculate the tangent
			tan = normalize(sub(msh.vertices[v].co, msh.vertices[(v + 1) % 3].co))
			tan = normalize(cross(nor, tan))

			if (hasUV):
				co = activeUV[i].uv[j]
				co[1] = 1 - co[1];
				co = r2d(co)
			else:
				print('No UV')
				co = (0.0, 0.0)

			key = vec, nor, co
			vertex_index = local_dict.get(key)
			
			col = msh.materials[f.material_index].diffuse_color

			if (vertex_index is None): # vertex not found
				local_dict[key] = local_vertex_count
				local_vertex_list.append(vec)
				local_normal_list.append(nor)
				local_tangent_list.append(tan)
				local_color_list.append(col)
				local_uv_list.append(co)
				tmpfaces.append(local_vertex_count)
				local_vertex_count+=1
			else:
				inx = local_dict[key]
				tmpfaces.append(inx)

		if (len(tmpfaces)==3): 
			local_faces_list.append(tmpfaces)
		else:
			print('Quad Face')
			local_faces_list.append([tmpfaces[0], tmpfaces[1], tmpfaces[2]])
			local_faces_list.append([tmpfaces[0], tmpfaces[2], tmpfaces[3]])

	meshData = type('obj', (object,), {
		'Verts' : local_vertex_list,
		'Norms' : local_normal_list,
		'Tans'  : local_tangent_list,
		'UVs'   : local_uv_list,
		'Faces' : local_faces_list,
		'Colors' : local_color_list,
		'Mesh'  : msh
	})

	writeFunc(fp, meshData, settings)
#------------------------------------------------------------------------------
def Export(filename, settings):
	# first thing's first, set the mode so we can get
	# the appropriate data	
	bpy.ops.object.mode_set(mode="OBJECT")
	print('Normals' + str(settings.Normals))

	scene = bpy.context.scene
	
	obj = bpy.context.object
	if not obj or obj.type != 'MESH':
		raise NameError("Cannot export: object %s is not a mesh" % obj)
	mesh = obj.to_mesh(scene, True, "PREVIEW")
	
	filename = filename # + ".opm"
	fp = open(filename, "wb")
	
	# mesh = extract_meshes(bpy.data.objects, bpy.data.scenes[0])[0]
	WriteData(mesh, fp, WriteBinary, settings)
	
	fp.close()
#------------------------------------------------------------------------------
def ExportJSON(filename):
	# first thing's first, set the mode so we can get
	# the appropriate data	
	bpy.ops.object.mode_set(mode="OBJECT")

	scene = bpy.context.scene
	
	obj = bpy.context.object
	if not obj or obj.type != 'MESH':
		raise NameError("Cannot export: object %s is not a mesh" % obj)
	mesh = obj.to_mesh(scene, True, "PREVIEW")
	
	filename = filename + ".json"
	fp = open(filename, "w")
	
	WriteData(mesh, fp, WriteJSON)
	
	fp.close()
#------------------------------------------------------------------------------
def WriteJSON(fp, meshData):
	# OPM Version
	#print('Version: ' + str(1))
	json = '{"Version": 1,'

	local_vertex_list  = meshData.Verts
	local_normal_list  = meshData.Norms
	local_tangent_list = meshData.Tans
	local_uv_list      = meshData.UVs
	local_faces_list   = meshData.Faces
	msh                = meshData.Mesh

	# OPM Features
	features = 31
	# open brace of json ----------------------------------------
	json += '"Features": ' + str(features) + ', '
	print('Features: ' + str(features))
				
	# Number of Vertices
	fp.write (struct.pack('I', len(local_vertex_list)))
	print('Vertices: ' + str(len(local_vertex_list)))
	jsonVert = '"Vertices":['
	jsonNorm = '"Normals":['
	jsonTang = '"Tangents":['
	jsonUVs  = '"UVs":['
	
	c = ', '
	first = 1
	for j in range(0,len(local_vertex_list)):
		if(not first):
			jsonVert += c
			jsonNorm += c
			jsonTang += c
			jsonUVs  += c
		
		jsonVert += '%f, %f, %f' % tuple(local_vertex_list[j])
		jsonNorm += '%f, %f, %f' % tuple(local_normal_list[j])
		jsonTang += '%f, %f, %f' % tuple(local_tangent_list[j])
		jsonUVs  += '%f, %f' % tuple(local_uv_list[j])
		first = 0
	jsonVert += "]"
	jsonNorm += "]"
	jsonTang += "]"
	jsonUVs  += "]"

	json += jsonVert + ', ' + jsonNorm + ', ' + jsonTang + ', ' + jsonUVs + '}'

	fp.write(json)

def WriteStringData(fp, val):
	nameBytes = bytes(val, 'utf-8')
	fp.write (struct.pack('I%ds' % len(nameBytes), len(nameBytes), nameBytes))

def ReadBoneData():
	armature, armatureObject = get_armature()
	hierarchy = []
	for bone in armature.bones:
		bonePos = None
		boneIndex = None
		if bone.parent is None:
			bonePos = bone.head_local
			boneIndex = -1
		else:
			bonePos = bone.head_local - bone.parent.head_local
			boneIndex = i = 0
			for parent in armature.bones:
				if parent.name == bone.parent.name:
					boneIndex = i
				i += 1

		bonePosWorld = armatureObject.matrix_world * bonePos

		joint = [boneIndex, bone.name, bonePosWorld.x, bonePosWorld.z, -bonePosWorld.y]
		hierarchy.append(joint)
	return hierarchy, len(armature.bones)


def WriteBoneData(fp):
	hierarchy, count = ReadBoneData()
	fp.write(struct.pack('I', count))
	for joint in hierarchy:
		fp.write(struct.pack('i', joint[0]))
		WriteStringData(fp, joint[1])
		fp.write(struct.pack('fff', joint[2], joint[3], joint[4]))

def ReadSkinData(meshObj):
	indices = []
	weights = []

	mesh = meshObj[0]
	object = meshObj[1]

	armature, armatureObject = get_armature()
	
	i = 0
	mesh_index = -1
	for obj in bpy.data.objects:
		if obj.name == mesh.name or obj == object:
			mesh_index = i
		i += 1

	if mesh_index == -1:
		print('failed to generate indices')
		return

	object = bpy.data.objects[mesh_index]
	for vertex in mesh.vertices:
		bone_array = []
		for group in vertex.groups:
			index = group.group
			weight = group.weight
			bone_array.append( (index, weight) )
		bone_array.sort(key = operator.itemgetter(1), reverse=True)

		for i in range(4):
			if i < len(bone_array):
				bone_proxy = bone_array[i]
				found = 0
				index = bone_proxy[0]
				weight = bone_proxy[1]

				for j, bone in enumerate(armature.bones):
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

def WriteSkinData(fp, meshObj):
	indices, weights = ReadSkinData(meshObj)
	fp.write(struct.pack('I', len(indices)))
	for i in range(len(indices)):
		fp.write(struct.pack('I', indices[i]))
	fp.write(struct.pack('I', len(weights)))
	for i in range(len(weights)):
		fp.write(struct.pack('f', weights[i]))

def ReadAnimationData(actionIndex):
	print('animation')
	action = bpy.data.actions[actionIndex]
	armature, armatureObject = get_armature()
	armatureMat = armatureObject.matrix_world
	l,r,s = armatureMat.decompose()
	armatureRotMat = r.to_matrix()

	parents = []
	parent_index = -1

	fps = bpy.data.scenes[0].render.fps

	end_frame = action.frame_range[1]
	start_frame = action.frame_range[0]

	frame_length = end_frame - start_frame

	for hierarchy in armature.bones:
		parent_index += 1
		print('Bone: ' + str(parent_index))
		keys = []
		for frame in range(int(start_frame), int(end_frame) + 1):
			pos, pchange = position(hierarchy, frame, action, armatureMat)
			rot, rchange = rotation(hierarchy, frame, action, armatureRotMat)

			px, py, pz = pos.x, pos.y, -pos.y
			rx, ry, rz, rw = rot.x, rot.z, -rot.y, rot.w

			# The start frame has to have pos, rot and scale
			if frame == int(start_frame):
				time = (frame - start_frame) / fps
				print('START FRAME ' + str(frame) + ' : ' + str(time))
				keyframe = [time,px,py,pz,rx,ry,rz,rw]
				keys.append(keyframe)

			# The end frame has to have pos, rot, scale and animation length
			elif frame == int(end_frame):
				time = frame_length / fps
				print('END FRAME ' + str(frame) + ' : ' + str(time))
				keyframe = [time,px,py,pz,rx,ry,rz,rw]
				keys.append(keyframe)

			# Only need 1 attribute for a frame in the middle
			elif pchange == True or rchange == True:
				time  = (frame - start_frame) / fps
				print('MIDDLE FRAME ' + str(frame) + ' : ' + str(time))
				if(pchange == True and rchange == True):
					keyframe = [time,px,py,pz,rx,ry,rz,rw]
				elif pchange == True:
					keyframe = [time,px,py,pz]
				elif rchange == True:
					keyframe = [time,rx,ry,rz,rw]
				keys.append(keyframe)
		parents.append([parent_index, keys])
	return parents

def WriteAnimationData(fp, actionIndex):
	animation = ReadAnimationData(actionIndex)
	nameBytes = bytes(bpy.data.actions[actionIndex].name, 'utf-8')
	fp.write (struct.pack('I%ds' % len(nameBytes), len(nameBytes), nameBytes))
	fp.write(struct.pack('I', len(animation)))
	for anim in range(len(animation)):
		parent_index = animation[anim][0]
		keys = animation[anim][1]
		fp.write(struct.pack('i', parent_index))
		fp.write(struct.pack('I', len(keys)))
		for key in range(len(keys)):
			features = 7
			if len(keys[key]) == 4:
				features = 3
				fp.write(struct.pack('I', features))
				fp.write(struct.pack('ffff', keys[key][0], keys[key][1], keys[key][2], keys[key][3]))
			elif len(keys[key]) == 5:
				features = 5
				fp.write(struct.pack('I', features))
				fp.write(struct.pack('fffff', keys[key][0], keys[key][1], keys[key][2], keys[key][3], keys[key][4]))
			else:
				fp.write(struct.pack('I', features))
				fp.write(struct.pack('ffffffff', keys[key][0], keys[key][1], keys[key][2], keys[key][3], keys[key][4], keys[key][5], keys[key][6], keys[key][7]))

#------------------------------------------------------------------------------
def WriteBinary(fp, meshData, settings):
	# OPM Version
	fp.write(struct.pack('H', 1))
	print('Version: ' + str(1))

	local_vertex_list  = meshData.Verts
	local_normal_list  = meshData.Norms
	local_tangent_list = meshData.Tans
	local_uv_list      = meshData.UVs
	local_faces_list   = meshData.Faces
	local_color_list   = meshData.Colors
	msh                = meshData.Mesh

	enum_position = 1
	enum_normal = 2
	enum_uv = 4
	enum_tangent = 8
	enum_index = 16
	enum_bones = 32
	enum_skinning = 64
	enum_animations = 128
	enum_color = 256
	
	# OPM Features
	features = enum_position
	if(settings.Normals):
		features += enum_normal
	if(settings.Tangents):
		features += enum_tangent
	if(settings.UVs):
		features += enum_uv
	if(settings.Colors):
		features += enum_color

	fp.write(struct.pack('I', int(features)))
	print('Features: ' + str(features))
				
	# Number of Vertices
	fp.write (struct.pack('I', len(local_vertex_list)))
	print('Vertices: ' + str(len(local_vertex_list)))
	for j in range(0,len(local_vertex_list)):
		fp.write (struct.pack('fff', local_vertex_list[j][0], local_vertex_list[j][1], local_vertex_list[j][2]))
		if(settings.Normals):
			fp.write (struct.pack('fff', local_normal_list[j][0], local_normal_list[j][1], local_normal_list[j][2]))
		if(settings.Tangents):
			fp.write (struct.pack('fff', local_tangent_list[j][0], local_tangent_list[j][1], local_tangent_list[j][2]))
		if(settings.UVs):
			fp.write (struct.pack('ff', local_uv_list[j][0], local_uv_list[j][1]))
		if(settings.Colors):
			fp.write (struct.pack('fff', local_color_list[j][0], local_color_list[j][1], local_color_list[j][2]))
	

	# Number of Indices
	fp.write (struct.pack('I', len(local_faces_list)))
	print('Indices: ' + str(len(local_faces_list)))
	for j, f in enumerate(local_faces_list):
		fp.write (struct.pack('HHH', f[0], f[1], f[2]))


    # Number of Bones
	# WriteBoneData(fp)


	# WriteSkinData(fp, extract_meshes(bpy.data.objects, bpy.data.scenes[0])[0])
	# WriteAnimationData(fp, 0)
	

# ##############################################################################
# Model exporter - armature
# (only the first armature will exported)
# ##############################################################################
def get_armature():
    if len(bpy.data.armatures) == 0:
        print("Warning: no armatures in the scene")
        return None, None

    armature = bpy.data.armatures[0]

    # Someone please figure out a proper way to get the armature node
    for object in bpy.data.objects:
        if object.type == 'ARMATURE':
            return armature, object

    print("Warning: no node of type 'ARMATURE' in the scene")
    return None, None

def handle_position_channel(channel, frame, position):

    change = False

    if channel.array_index in [0, 1, 2]:
        for keyframe in channel.keyframe_points:
            if keyframe.co[0] == frame:
                change = True

        value = channel.evaluate(frame)

        if channel.array_index == 0:
            position.x = value

        if channel.array_index == 1:
            position.y = value

        if channel.array_index == 2:
            position.z = value

    return change

def handle_rotation_channel(channel, frame, rotation):

    change = False

    if channel.array_index in [0, 1, 2, 3]:

        for keyframe in channel.keyframe_points:
            if keyframe.co[0] == frame:
                change = True

        value = channel.evaluate(frame)

        if channel.array_index == 1:
            rotation.x = value

        elif channel.array_index == 2:
            rotation.y = value

        elif channel.array_index == 3:
            rotation.z = value

        elif channel.array_index == 0:
            rotation.w = value

    return change

def position(bone, frame, action, armatureMatrix):

    position = mathutils.Vector((0,0,0))
    change = False

    bone_label = '"%s"' % bone.name

    for channel in action.fcurves:
        data_path = channel.data_path
        if bone_label in data_path and "location" in data_path:
            hasChanged = handle_position_channel(channel, frame, position)
            change = change or hasChanged

    position = position * bone.matrix_local.inverted()

    if bone.parent is None:

        position.x += bone.head.x
        position.y += bone.head.y
        position.z += bone.head.z

    else:

        parent = bone.parent

        parentInvertedLocalMatrix = parent.matrix_local.inverted()
        parentHeadTailDiff = parent.tail_local - parent.head_local

        position.x += (bone.head * parentInvertedLocalMatrix).x + parentHeadTailDiff.x
        position.y += (bone.head * parentInvertedLocalMatrix).y + parentHeadTailDiff.y
        position.z += (bone.head * parentInvertedLocalMatrix).z + parentHeadTailDiff.z

    return armatureMatrix*position, change

def rotation(bone, frame, action, armatureMatrix):

    # TODO: calculate rotation also from rotation_euler channels

    rotation = mathutils.Vector((0,0,0,1))

    change = False

    bone_label = '"%s"' % bone.name

    for channel in action.fcurves:
        data_path = channel.data_path
        if bone_label in data_path and "quaternion" in data_path:
            hasChanged = handle_rotation_channel(channel, frame, rotation)
            change = change or hasChanged

    rot3 = rotation.to_3d()
    rotation.xyz = rot3 * bone.matrix_local.inverted()
    rotation.xyz = armatureMatrix * rotation.xyz

    return rotation, change

def extract_meshes(objects, scene):

    meshes = []

    for object in objects:

        if object.type == "MESH":

            # collapse modifiers into mesh

            mesh = object.to_mesh(scene, True, 'RENDER')

            if not mesh:
                raise Exception("Error, could not get mesh data from object [%s]" % object.name)

            # preserve original name

            mesh.name = object.name
            X_ROT = mathutils.Matrix.Rotation(-math.pi/2, 4, 'X')
            mesh.transform(X_ROT * object.matrix_world)
                    
                    
            mesh.update(calc_tessface=True)

            mesh.calc_normals()
            mesh.calc_tessface()
            meshes.append([mesh, object])

    return meshes


class Export_opm(bpy.types.Operator, ExportHelper):
	bl_idname = "export_object.opm"
	bl_label = "Export OPifex Mesh (.opm)"
	filename_ext = ".opm"

	json = bpy.props.BoolProperty(name="Export As JSON", default=False)
	normals = bpy.props.BoolProperty(name="Export Normals", default=True)
	uvs = bpy.props.BoolProperty(name="Export UVs", default=True)
	colors = bpy.props.BoolProperty(name="Export Colors", default=False)
	tangents = bpy.props.BoolProperty(name="Export Tangents", default=False)
	bones = bpy.props.BoolProperty(name="Export Bones/Skin", default=False)
	animations = bpy.props.BoolProperty(name="Export Animations", default=False)

	@classmethod
	def poll(cls, context):
		return context.active_object.type in ['MESH', 'CURVE', 'SURFACE', 'FONT']

	def execute(self, context):
		start_time = time.time()
		print('\n_____START_____')
		props = self.properties
		filepath = self.filepath
		filepath = bpy.path.ensure_ext(filepath, self.filename_ext)

		settings = type('obj', (object,), {
			'Normals' : self.normals,
			'UVs' : self.uvs,
			'Colors'  : self.colors,
			'Tangents'   : self.tangents,
			'Bones' : self.bones,
			'Animations' : self.animations,
			"JSON" : self.json
		})

		#context, props, 
		exported = Export(filepath, settings)

		if exported:
			print('finished export in %s seconds' %((time.time() - start_time)))
			print(filepath)

		return {'FINISHED'}


	def invoke( self, context, event):
		wm = context.window_manager

		if True:
			# File selector
			wm.fileselect_add(self) # will run self.execute()
			return {'RUNNING_MODAL'}
		elif True:
			# search the enum
			wm.invoke_search_popup(self)
			return {'RUNNING_MODAL'}
		elif False:
			# Redo popup
			return wm.invoke_props_popup(self, event) #
		elif False:
			return self.execute(context)



### REGISTER ###

def menu_func(self, context):
	self.layout.operator(Export_opm.bl_idname, text="OPifex Model (.opm)")

def register():
	bpy.utils.register_module(__name__)

	bpy.types.INFO_MT_file_export.append(menu_func)

def unregister():
	bpy.utils.unregister_module(__name__)

	bpy.types.INFO_MT_file_export.remove(menu_func)

if __name__ == "__main__":
	register()
