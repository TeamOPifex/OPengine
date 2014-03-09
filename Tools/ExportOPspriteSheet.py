## A GIMP plug-in to export an OPengine spritesheet image and meta data
#  @author Kirk Roerig
#  @license GPL v3

from gimpfu import *
from struct import *
import gtk
import os
import re
import gettext

gettext.install("gimp20-python", gimp.locale_directory, unicode=True)
#------------------------------------------------------------------------------
def show_save_dialog(img):
	chooser = gtk.FileChooserDialog(
		title=None,action=gtk.FILE_CHOOSER_ACTION_SAVE,
		buttons=(gtk.STOCK_CANCEL,gtk.RESPONSE_CANCEL,gtk.STOCK_SAVE,gtk.RESPONSE_OK)
		)

	# save folder will be desktop
	save_dir = os.path.join(os.path.expanduser('~'), 'Desktop')
			
	chooser.set_current_folder(save_dir)
	chooser.set_current_name(re.sub(r'\..*', '', img.name))
	chooser.set_do_overwrite_confirmation(True)
		
	filter = gtk.FileFilter()
	filter.set_name("Save as opss")
	filter.add_pattern("*.opss")
	chooser.add_filter(filter) 
		
	response = chooser.run()
	if response != gtk.RESPONSE_OK:
		return
	filename = chooser.get_filename()
	img.filename = filename
	chooser.destroy()
	
	return filename
#------------------------------------------------------------------------------
def get_frame_count(meta):
	frames = 0
	for key in meta:
		sprite = meta[key]
		if(type(sprite) is int):
			continue
		if(type(sprite) is list):
			for frame in sprite:
				frames += 1
		else:
			frames += 1
	return frames
#------------------------------------------------------------------------------
def pck_str(s):
	return pack("I%ds" % (len(s),), len(s), s)
#------------------------------------------------------------------------------
def write_sprite_frame(file, sprite_def):
        def_ints = [];
        for i in range(0, 4):
                def_ints.append(sprite_def[i])
        file.write(pack('%si' % len(def_ints), *def_ints))
#------------------------------------------------------------------------------
def binify_and_write_meta(meta, file):
        file.write(pack('ii', meta['__width'], meta['__height']))

	# don't try saving width and height as sprites
	del meta['__width']
	del meta['__height']

	# save the number of sprites, and the number of total frames
	file.write(pack('i', len(meta)))
	file.write(pack('i', get_frame_count(meta)))

        for sprite_name in meta:
                sprite = meta[sprite_name]
                flags = 0x00 # indicates anim and stuff

                file.write(pck_str(sprite_name)) # write the identifier of the sprite

		# is this an animation?
                if type(sprite) is list:
                        flags |= 0x01 # yes, it is 

		file.write(pack('i', flags)) # write the sprite flags

		# if it's an animation, write all the frames
		# otherwise write a single frame
		if type(sprite) is list:
                        
			file.write(pack('i', len(sprite))) # write the number of frames
                        for frame in sprite:
                                write_sprite_frame(file, frame)
                else:
                        write_sprite_frame(file, sprite)		
#------------------------------------------------------------------------------
def build_meta(img):
	sprite_dic = {
		'__width':  img.width,
		'__height': img.height
	}

	for layer in img.layers:
		base_name = re.sub(r'_[0-9]+$', '', layer.name)
		frame_index = re.sub(r'^.+_', '', layer.name)
		sprite_def = (
			layer.offsets[0], # X offset
			layer.offsets[1], # Y offset
			layer.width,      # width
			layer.height      # height
			)

		# add an entry for the sprite definition, if it
		# does not already exist
		if not base_name in sprite_dic:
			sprite_dic[base_name] = []
			
		try:
			frame_index = int(frame_index)
			sprite_def += (frame_index,)
			sprite_dic[base_name].append(sprite_def)
		except ValueError:
			sprite_dic[base_name] = sprite_def

	# for all the sprite entries sort the ones that
	# are lists (i.e. contain anim frames)
	for desc in sprite_dic:
		sprite = sprite_dic[desc]
		
		if type(sprite) is list:
			sprite_dic[desc] = sorted(sprite, key = lambda spt: spt[4])

	return sprite_dic

## Export a list of layers from a GIMP image as an image.
#
#  @param img The image to export layers from.
#  @param drw A GIMP drawable.
#  @param path A prefix for the export path.
#  @param combfilename A file containing combinations a 
#  Python tuple literals.
def export_spritesheet(img, drw):
	filename = show_save_dialog(img)

	# if no filename is retrieved, just stop the export
	if not filename:
		return

	meta = build_meta(img)

	file = open(filename + '.opss', 'wb')
	binify_and_write_meta(meta, file)

	# save the png file
	dup = img.duplicate()
	pdb.gimp_image_merge_visible_layers(dup, 0)
	dup.layers[0].resize_to_image_size()
	pdb.file_png_save2(
		dup,
		dup.layers[0],
		filename + '.temp',
		filename,
		1,
		9,
		1,
		1,
		1,
		1,
		1,
		0,
		1
		)

	# open the temp file (image) and
	# append it to the opss file
	src = open(filename + '.temp', 'rb')
	file.write(src.read())
	
	# close all files
	file.close()
	src.close()

	# remove the temporary file
	os.remove(filename + '.temp')

	print filename
	return

# register the plug-in in the GIMP plug-in registry 		
register(
	proc_name=("python-fu-export-op-engine"),
	blurb=("Export Layers as an OPengine sprite sheet."),
	help=(""),
	author=("Kirk Roerig <mrpossoms@gmail.com>"),
	copyright=("OPifex Entertainment"),
	date=("2014"),
	label=("As spritesheet"),
	imagetypes=("*"),
	params=[
		(PF_IMAGE, "img", "Image", None),
		(PF_DRAWABLE, "drw", "Drawable", None)
		],
	results=[],
	function=(export_spritesheet),
	menu=("<Image>/File/O_Pengine Exports"),
	domain=("gimp20-python", gimp.locale_directory)
	)

main()
