//interpolants
varying vec3 position;
varying vec2 tex_coord;
varying vec4 material_col;
varying vec2 pixel_velocity;
varying vec2 pixel_velocity2;


varying mat3 TBN;

//samplers
uniform sampler2D diffuse_map;
uniform sampler2D normal_map;
uniform sampler2D specular_map;

void main (void)
{
	//declaractions
	vec4 albedo;
	vec3 normal_buffer;
	vec3 position_buffer;
	
	//read textures
	vec4 diffuse_texture = texture2D(diffuse_map, tex_coord);
	vec3 normal = normalize( texture2D(normal_map, tex_coord).xyz * 2.0 - 1.0);
	vec4 specular_texture = texture2D(specular_map, tex_coord);
	
	//invert the normals y instead of above this is dependant on the setup of the normal map
	normal.y *= -1.0;
	normal_buffer = TBN * normal;
	
	normal_buffer = normal_buffer;
	position_buffer = position;
	albedo = diffuse_texture;
	
	gl_FragData[0] = vec4( normal_buffer, 1.0 );
	gl_FragData[1] = vec4( position_buffer, 1.0 );
	//gl_FragData[2] = vec4( albedo.xyz, material_col.a );
	gl_FragData[2] = vec4( pixel_velocity2.xy, 0.0, 1.0 );
	gl_FragData[3] = vec4( pixel_velocity.xy, 0.0, 1.0 );
}
