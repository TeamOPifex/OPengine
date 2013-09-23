#!BPY
"""
me: 'OPMeshBinary (.opm)...'
Blender: 266
Group: 'Export'
Tooltip: 'OPMeshBinary Export'
"""

# import OPmeshBinary2
# from imp import reload
# reload(OPmeshBinary2)

import bpy
import os
import itertools
import math
import struct

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
def WriteData(msh, fp, writeFunc):
	local_dict = {} # local dictionary
	local_faces_list   = [] # lcoal faces index list
	local_vertex_list  = [] # local vertex list
	local_normal_list  = [] # local normal list
	local_tangent_list = [] # local tangent list
	local_uv_list = [] # local uv list
	local_vertex_count = 0 # local vertices count
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

			if (vertex_index is None): # vertex not found
				local_dict[key] = local_vertex_count
				local_vertex_list.append(vec)
				local_normal_list.append(nor)
				local_tangent_list.append(tan)
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
		'Mesh'  : msh
	})

	writeFunc(fp, meshData)
#------------------------------------------------------------------------------
def Export(filename):
	# first thing's first, set the mode so we can get
	# the appropriate data	
	bpy.ops.object.mode_set(mode="OBJECT")

	scene = bpy.context.scene
	
	obj = bpy.context.object
	if not obj or obj.type != 'MESH':
		raise NameError("Cannot export: object %s is not a mesh" % obj)
	mesh = obj.to_mesh(scene, True, "PREVIEW")
	
	filename = filename + ".opm"
	fp = open(filename, "wb")
	
	WriteData(mesh, fp, WriteBinary)
	
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
	
	filename = filename + ".opm.json"
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
#------------------------------------------------------------------------------
def WriteBinary(fp, meshData):
	# OPM Version
	fp.write(struct.pack('H', 1))
	print('Version: ' + str(1))

	local_vertex_list  = meshData.Verts
	local_normal_list  = meshData.Norms
	local_tangent_list = meshData.Tans
	local_uv_list      = meshData.UVs
	local_faces_list   = meshData.Faces
	msh                = meshData.Mesh

	# OPM Features
	features = 31
	fp.write(struct.pack('I', int(features)))
	print('Features: ' + str(features))
				
	# Number of Vertices
	fp.write (struct.pack('I', len(local_vertex_list)))
	print('Vertices: ' + str(len(local_vertex_list)))
	for j in range(0,len(local_vertex_list)):
		fp.write (struct.pack('fff', local_vertex_list[j][0], local_vertex_list[j][1], local_vertex_list[j][2]))
		fp.write (struct.pack('fff', local_normal_list[j][0], local_normal_list[j][1], local_normal_list[j][2]))
		fp.write (struct.pack('fff', local_tangent_list[j][0], local_tangent_list[j][1], local_tangent_list[j][2]))
		fp.write (struct.pack('ff', local_uv_list[j][0], local_uv_list[j][1]))
	
	# Number of Indices
	fp.write (struct.pack('I', len(local_faces_list)))
	print('Indices: ' + str(len(local_faces_list)))
	for j, f in enumerate(local_faces_list):
		fp.write (struct.pack('HHH', f[0], f[1], f[2]))