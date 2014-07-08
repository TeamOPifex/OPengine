# OPifex Model Format Animation
# OPifex Entertainment LLC

#################################################################
### Imports
#################################################################

import bpy
import operator

import io_export_opm.export_opm_utilities       as OPMutil


#################################################################
### Private Methods
#################################################################

def generate_animation(option_animation_skeletal, option_frame_step, flipyz, option_frame_index_as_time, index):

    if not option_animation_skeletal or len(bpy.data.actions) == 0:
        return ""

    # TODO: Add scaling influences

    action = bpy.data.actions[index]
    
    # get current context and then switch to dopesheet temporarily
    
    current_context = bpy.context.area.type
    
    bpy.context.area.type = "DOPESHEET_EDITOR"
    bpy.context.space_data.mode = "ACTION"    
    
    # set active action
    bpy.context.area.spaces.active.action = action
    
    armature, armature_object = get_armature()
    if armature_object is None or armature is None:
        return "", 0
        
    #armature_object = bpy.data.objects['marine_rig']
    
        
    armature_matrix = armature_object.matrix_world

    fps = bpy.data.scenes[0].render.fps

    end_frame = action.frame_range[1]
    start_frame = action.frame_range[0]

    frame_length = end_frame - start_frame

    used_frames = int(frame_length / option_frame_step) + 1

    TEMPLATE_KEYFRAME_FULL  = '{"time":%g,"pos":[%g,%g,%g],"rot":[%g,%g,%g,%g],"scl":[%g,%g,%g]}'
    TEMPLATE_KEYFRAME_BEGIN = '{"time":%g'
    TEMPLATE_KEYFRAME_END   = '}'
    TEMPLATE_KEYFRAME_POS   = ',"pos":[%g,%g,%g]'
    TEMPLATE_KEYFRAME_ROT   = ',"rot":[%g,%g,%g,%g]'
    TEMPLATE_KEYFRAME_SCL   = ',"scl":[%g,%g,%g]'

    keys = []
    channels_location = []
    channels_rotation = []
    channels_scale = []
    
    # Precompute per-bone data
    for pose_bone in armature_object.pose.bones:
        armature_bone = pose_bone.bone
        keys.append([])
        channels_location.append(  find_channels(action, armature_bone, "location"))
        channels_rotation.append(  find_channels(action, armature_bone, "rotation_quaternion"))
        channels_rotation.append(  find_channels(action, armature_bone, "rotation_euler"))
        channels_scale.append(     find_channels(action, armature_bone, "scale"))

    # Process all frames
    for frame_i in range(0, used_frames):

        #print("Processing frame %d/%d" % (frame_i, used_frames))
        # Compute the index of the current frame (snap the last index to the end)
        frame = start_frame + frame_i * option_frame_step
        if frame_i == used_frames-1:
            frame = end_frame

        # Compute the time of the frame
        if option_frame_index_as_time:
            time = frame - start_frame
        else:
            time = (frame - start_frame) / fps

        # Let blender compute the pose bone transformations
        bpy.data.scenes[0].frame_set(frame)

        # Process all bones for the current frame
        bone_index = 0
        for pose_bone in armature_object.pose.bones:

            # Extract the bone transformations
            if pose_bone.parent is None:
                bone_matrix = armature_matrix * pose_bone.matrix
            else:
                parent_matrix = armature_matrix * pose_bone.parent.matrix
                bone_matrix = armature_matrix * pose_bone.matrix
                bone_matrix = parent_matrix.inverted() * bone_matrix
            pos, rot, scl = bone_matrix.decompose()

            pchange = True or has_keyframe_at(channels_location[bone_index], frame)
            rchange = True or has_keyframe_at(channels_rotation[bone_index], frame)
            schange = True or has_keyframe_at(channels_scale[bone_index], frame)

            if flipyz:
                px, py, pz = pos.x, pos.z, -pos.y
                rx, ry, rz, rw = rot.x, rot.z, -rot.y, rot.w
                sx, sy, sz = scl.x, scl.z, scl.y
            else:
                px, py, pz = pos.x, pos.y, pos.z
                rx, ry, rz, rw = rot.x, rot.y, rot.z, rot.w
                sx, sy, sz = scl.x, scl.y, scl.z

            # START-FRAME: needs pos, rot and scl attributes (required frame)

            if frame == start_frame:

                keyframe = TEMPLATE_KEYFRAME_FULL % (time, px, py, pz, rx, ry, rz, rw, sx, sy, sz)
                keys[bone_index].append(keyframe)

            # END-FRAME: needs pos, rot and scl attributes with animation length (required frame)

            elif frame == end_frame:

                keyframe = TEMPLATE_KEYFRAME_FULL % (time, px, py, pz, rx, ry, rz, rw, sx, sy, sz)
                keys[bone_index].append(keyframe)

            # MIDDLE-FRAME: needs only one of the attributes, can be an empty frame (optional frame)

            elif pchange == True or rchange == True:

                keyframe = TEMPLATE_KEYFRAME_BEGIN % time
                if pchange == True:
                    keyframe = keyframe + TEMPLATE_KEYFRAME_POS % (px, py, pz)
                if rchange == True:
                    keyframe = keyframe + TEMPLATE_KEYFRAME_ROT % (rx, ry, rz, rw)
                if schange == True:
                    keyframe = keyframe + TEMPLATE_KEYFRAME_SCL % (sx, sy, sz)
                keyframe = keyframe + TEMPLATE_KEYFRAME_END

                keys[bone_index].append(keyframe)
            bone_index += 1

    # Gather data
    parents = []
    bone_index = 0
    for pose_bone in armature_object.pose.bones:
        keys_string = ",".join(keys[bone_index])
        parent_index = bone_index - 1 # WTF? Also, this property is not used by three.js
        parent = '{"parent":%d,"keys":[%s]}' % (parent_index, keys_string)
        bone_index += 1
        parents.append(parent)
        

    if option_frame_index_as_time:
        length = frame_length
    else:
        length = frame_length / fps

    bpy.data.scenes[0].frame_set(start_frame)

    # reset context
    
    bpy.context.area.type = current_context
    
    return action.name, fps, length, parents

def find_channels(action, bone, channel_type):
    bone_name = bone.name
    ngroups = len(action.groups)
    result = []

    # Variant 1: channels grouped by bone names
    if ngroups > 0:

        # Find the channel group for the given bone
        group_index = -1
        for i in range(ngroups):
            if action.groups[i].name == bone_name:
                group_index = i

        # Get all desired channels in that group
        if group_index > -1:
            for channel in action.groups[group_index].channels:
                if channel_type in channel.data_path:
                    result.append(channel)

    # Variant 2: no channel groups, bone names included in channel names
    else:

        bone_label = '"%s"' % bone_name

        for channel in action.fcurves:
            data_path = channel.data_path
            if bone_label in data_path and channel_type in data_path:
                result.append(channel)

    return result

def find_keyframe_at(channel, frame):
    for keyframe in channel.keyframe_points:
        if keyframe.co[0] == frame:
            return keyframe
    return None

def has_keyframe_at(channels, frame):
    for channel in channels:
        if not find_keyframe_at(channel, frame) is None:
            return True
    return False

def generate_all_animations(option_animation_skeletal, option_frame_step, flipyz, option_frame_index_as_time):
    all_animations_string = ""
    if option_animation_skeletal:
        for index in range(0, len(bpy.data.actions)):
            if index != 0 :
                all_animations_string += ", \n"
            all_animations_string += "{" + generate_animation(option_animation_skeletal, option_frame_step, flipyz, option_frame_index_as_time,index) + "}"
    return all_animations_string

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

def position(bone, frame, action, armatureMatrix):

    position = mathutils.Vector((0,0,0))
    change = False

    ngroups = len(action.groups)

    if ngroups > 0:

        index = 0

        for i in range(ngroups):
            if action.groups[i].name == bone.name:
                index = i

        for channel in action.groups[index].channels:
            if "location" in channel.data_path:
                hasChanged = handle_position_channel(channel, frame, position)
                change = change or hasChanged

    else:

        bone_label = '"%s"' % bone.name

        for channel in action.fcurves:
            data_path = channel.data_path
            if bone_label in data_path and "location" in data_path:
                hasChanged = handle_position_channel(channel, frame, position)
                change = change or hasChanged

    position = position * bone.matrix_local.inverted()

    if bone.parent == None:

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

def rotation(bone, frame, action, armatureMatrix):

    # TODO: calculate rotation also from rotation_euler channels

    rotation = mathutils.Vector((0,0,0,1))

    change = False

    ngroups = len(action.groups)

    # animation grouped by bones

    if ngroups > 0:

        index = -1

        for i in range(ngroups):
            if action.groups[i].name == bone.name:
                index = i

        if index > -1:
            for channel in action.groups[index].channels:
                if "quaternion" in channel.data_path:
                    hasChanged = handle_rotation_channel(channel, frame, rotation)
                    change = change or hasChanged

    # animation in raw fcurves

    else:

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



#################################################################
### Public Methods
#################################################################

def GetArmature():
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