//interpolants
varying vec3 light_vec; 
varying vec3 eye_vec;
varying vec2 tex_coord;
varying vec3 tangent_space_normal;

varying mat3 TBN;
varying mat3 normal_matrix;

//uniforms
uniform float inv_radius;

//samplers
uniform sampler2D diffuse_map;
uniform sampler2D normal_map;
uniform sampler2D specular_map;

//interpolants
varying vec4 shadow_coord;

//samplers
uniform sampler2D shadow_map;

//uniforms
uniform float g_min_variance = 0.0002;

//locals
vec4 shadow_coord_post_w;

float chebyshev_upper_bound( float distance )
{
	vec2 moments = texture2D(shadow_map,shadow_coord_post_w.xy).rb;
	
	if(distance <= moments.x) return 1.0;

	// The fragment is either in shadow or penumbra. We now use chebyshev's upperBound to check
	float variance = moments.y - (moments.x * moments.x);
	variance = max(variance,g_min_variance);

	float d = distance - moments.x;
	float p_max = variance / (variance + d * d);

	return p_max;
}

void main (void)
{
	//declaractions
	vec4 final = vec4(0.0,0.0,0.0,1.0);

	//light setup
	float distance = length( light_vec );
	vec3 light_direction = normalize( light_vec );
	vec3 view_direction = normalize( eye_vec );
	
	//parallax mapping offset texture coords
	float parallax_scale = 0.04;
	float height = texture2D(specular_map,tex_coord).r;
	vec2 offset_direction = view_direction.xy * (height * parallax_scale);
	
	vec2 warped_texture_coords = tex_coord + offset_direction;
	
	//read textures
	vec4 diffuse_texture = texture2D(diffuse_map, warped_texture_coords);
	vec3 normal = normalize( texture2D(normal_map, warped_texture_coords).xyz * 2.0 - 1.0);
	vec4 specular_texture = texture2D(specular_map, warped_texture_coords);
	
	//invert the normals y instead of above this is dependant on the setup of the normal map
	normal.y *= -1.0;
	
	float n_dot_l = max( dot(normal,light_direction), 0.0 );
	
	if (n_dot_l > 0.0) 
	{
			//calculate attenuation
			float attenuation = 1.0 / 
			(gl_LightSource[0].constantAttenuation +
			gl_LightSource[0].linearAttenuation * distance +
			gl_LightSource[0].quadraticAttenuation * distance * distance);
					
			//diffuse term
			final += attenuation * (gl_LightSource[0].diffuse * diffuse_texture * n_dot_l);
		
			//specular term
			float n_dot_h = pow( clamp(dot(reflect(-light_direction, normal), view_direction), 0.0, 1.0), gl_FrontMaterial.shininess );
		    final += attenuation * gl_LightSource[0].specular * gl_FrontMaterial.specular * n_dot_h * 1.0;
	}
	
	shadow_coord_post_w = shadow_coord / shadow_coord.w;
	float shadowed = chebyshev_upper_bound(shadow_coord_post_w.z) + 0.2;
	
	gl_FragColor = vec4( diffuse_texture.xyz, 1.0 );
}