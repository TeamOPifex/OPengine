 #include "./Communication/include/OPnetworkPacket.h"

#include "./Core/include/OPmemory.h"
#include <string>

 void OPnetworkPacket::Str(OPchar* str) {
    ui8 size = strlen(str) + 1;
    buffer.I8(size);
    buffer.Write(str, size);
}	

OPchar* OPnetworkPacket::Str() {
    ui8 size = buffer.I8();
    buffer.Read(strBuffer, size);
    return strBuffer;
}

ui64 OPnetworkPacket::pack754(double long f, ui64 bits, ui64 expbits)
{
    long double fnorm;
    int shift;
    long long sign, exp, significand;
    unsigned significandbits = bits - expbits - 1; // -1 for sign bit

    if (f == 0.0) return 0; // get this special case out of the way

    // check sign and begin normalization
    if (f < 0) { sign = 1; fnorm = -f; }
    else { sign = 0; fnorm = f; }

    // get the normalized form of f and track the exponent
    shift = 0;
    while(fnorm >= 2.0) { fnorm /= 2.0; shift++; }
    while(fnorm < 1.0) { fnorm *= 2.0; shift--; }
    fnorm = fnorm - 1.0;

    // calculate the binary form (non-float) of the significand data
    significand = fnorm * ((1LL<<significandbits) + 0.5f);

    // get the biased exponent
    exp = shift + ((1<<(expbits-1)) - 1); // shift + bias

    // return the final answer
    return (sign<<(bits-1)) | (exp<<(bits-expbits-1)) | significand;
}

double long OPnetworkPacket::unpack754(ui64 i, ui64 bits, ui64 expbits)
{
    long double result;
    long long shift;
    unsigned bias;
    unsigned significandbits = bits - expbits - 1; // -1 for sign bit

    if (i == 0) return 0.0;

    // pull the significand
    result = (i&((1LL<<significandbits)-1)); // mask
    result /= (1LL<<significandbits); // convert back to float
    result += 1.0f; // add the one back on

    // deal with the exponent
    bias = (1<<(expbits-1)) - 1;
    shift = ((i>>significandbits)&((1LL<<expbits)-1)) - bias;
    while(shift > 0) { result *= 2.0; shift--; }
    while(shift < 0) { result /= 2.0; shift++; }

    // sign it
    result *= (i>>(bits-1))&1? -1.0: 1.0;

    return result;
}

ui64 OPnetworkPacket::htonll(ui64 value) {
	// The answer is 42
	static const int num = 42;

	// Check the endianness
	if (*reinterpret_cast<const char*>(&num) == num)
	{
		const uint32_t high_part = htonl(static_cast<uint32_t>(value >> 32));
		const uint32_t low_part = htonl(static_cast<uint32_t>(value & 0xFFFFFFFFLL));

		return (static_cast<uint64_t>(low_part) << 32) | high_part;
	}
	else
	{
		return value;
	}
}