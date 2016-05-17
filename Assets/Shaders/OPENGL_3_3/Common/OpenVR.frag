#version 330
uniform sampler2D mytexture;

in vec2 v2UVred;
in vec2 v2UVgreen;
in vec2 v2UVblue;

out vec4 outputColor;

void main()
{
	float fBoundsCheck = ( (dot( vec2( lessThan( v2UVgreen.xy, vec2(0.05, 0.05)) ), vec2(1.0, 1.0))+dot( vec2( greaterThan( v2UVgreen.xy, vec2( 0.95, 0.95)) ), vec2(1.0, 1.0))) );
	if( fBoundsCheck > 1.0 )
	{ outputColor = vec4( 0, 0, 0, 1.0 ); }
	else
	{
		float red = texture(mytexture, v2UVred).x;
		float green = texture(mytexture, v2UVgreen).y;
		float blue = texture(mytexture, v2UVblue).z;
		outputColor = vec4( red, green, blue, 1.0  ); }
}
