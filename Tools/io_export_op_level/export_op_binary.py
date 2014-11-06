# OPifex Model Format Binary Tools
# OPifex Entertainment LLC

#################################################################
### Imports
#################################################################
import struct

#################################################################
### Private Methods
#################################################################


#################################################################
### Public Methods
#################################################################

def OpenFile(filepath):
	return open(filepath, "wb")
def Int(fp, val):
	fp.write(struct.pack('i', val))
def UInt(fp, val):
	fp.write(struct.pack('I', val))
def Float(fp, val):
	fp.write(struct.pack('f', val))
def Short(fp, val):
	fp.write(struct.pack('h', val))
def UShort(fp, val):
	fp.write(struct.pack('H', val))
def Short3(fp, vec):
	fp.write(struct.pack('HHH', vec[0], vec[1], vec[2]))
def String(fp, val):
	nameBytes = bytes(val, 'utf-8')
	fp.write(struct.pack('I%ds' % len(nameBytes), len(nameBytes), nameBytes))
def Vec2(fp, vec):
	fp.write(struct.pack('ff', vec[0], vec[1]))
def Vec2Tex(fp, vec):
	fp.write(struct.pack('ff', vec[0], vec[1]))
def Vec3(fp, vec):
	fp.write(struct.pack('fff', vec[0], vec[1], vec[2]))
def Vec4(fp, vec):
	fp.write(struct.pack('ffff', vec[0], vec[1], vec[2], vec[3]))