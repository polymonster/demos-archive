//interpolants
varying vec2 tex_coord;

//samplers
uniform sampler2D image;
uniform sampler2D blurred_image;
uniform sampler2D depth_texture;

//uniforms
uniform float focal_plane;
uniform float focus_range;

void main(void)
{
	vec3 final = texture2D(image,tex_coord).xyz;
	vec3 blurred = texture2D(blurred_image,tex_coord).xyz;
	float depth = texture2D(depth_texture,tex_coord).z;
	
	float weight = clamp( abs(depth - focal_plane) / focus_range, 0.0, 1.0);

	final = mix(final,blurred,weight);
	
	//final = (final + weight) * (blurred - final);
	
	gl_FragColor = vec4(final.xyz,1.0);
}