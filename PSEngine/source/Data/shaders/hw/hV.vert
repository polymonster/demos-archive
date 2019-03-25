//interpolators
varying vec4 position;
varying vec3 normal;
varying vec2 diffuse_texture_coords;

uniform vec3 tangent;

void main(void)
{
	//material setup
	diffuse_texture_coords = gl_MultiTexCoord0.xy;
	
	//lighting setup
	normal = normalize(gl_NormalMatrix * gl_Normal);
	
	//interpolate the position across the polygon
	position = gl_ModelViewMatrix * gl_Vertex;
	
	//set the vertex position
	gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;
}
