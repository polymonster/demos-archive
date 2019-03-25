//consts
const int MAX_LIGHTS = 25;
const float MATERIAL_PIXEL_STRIDE = 0.03125;
const float MATERIAL_STRIDE = 0.125;
const float PIXEL_TO_TEXEL_OFFSET = 0.015625;
const float SHADOW_BIAS = 0.001;

//interpolants
varying vec2 tex_coord;

//samplers
uniform sampler2D normal_buffer;
uniform sampler2D position_buffer;
uniform sampler2D colour_buffer;
uniform sampler2D shadow_map0;
uniform sampler2D shadow_map1;
uniform sampler2D shadow_map2;
uniform sampler2D materials_lookup;

uniform samplerCube cubemap;

//uniforms
uniform mat4 shadow_matrix0;
uniform mat4 shadow_matrix1;
uniform mat4 shadow_matrix2;

uniform vec3 camera_pos;

uniform float shadow_map_texel_size0;
uniform float shadow_map_texel_size1;
uniform float shadow_map_texel_size2;

uniform vec4 light_pos[MAX_LIGHTS];
uniform vec3 light_diffuse[MAX_LIGHTS];
uniform vec3 spot_direction[MAX_LIGHTS];
uniform float area_of_effect[MAX_LIGHTS];

uniform int  number_of_lights;

#define SHADOWED
#define WORLD_SPACE

void main(void)
{
	//jump out at alpha pixels
	vec4 diffuse_colour = texture2D(colour_buffer,tex_coord);
	
	//material lookup
	float material_id = diffuse_colour.a;
	vec2 material_lookup_pos = vec2(PIXEL_TO_TEXEL_OFFSET + (MATERIAL_STRIDE * material_id),1.0 - PIXEL_TO_TEXEL_OFFSET);
	vec4 mat_ambient = texture2DLod(materials_lookup,material_lookup_pos, 0.0);
	vec4 mat_diffuse = texture2DLod(materials_lookup,material_lookup_pos += vec2(MATERIAL_PIXEL_STRIDE,0.0),  0.0);
	vec4 mat_specular = texture2DLod(materials_lookup,material_lookup_pos += vec2(MATERIAL_PIXEL_STRIDE,0.0),  0.0);
	float mat_env = texture2DLod(materials_lookup,material_lookup_pos += vec2(MATERIAL_PIXEL_STRIDE,0.0),  0.0).r;
	
	//setup lighting components
	vec4 pos_spec_pack = texture2DLod(position_buffer,tex_coord, 0.0);
	vec3 position = pos_spec_pack.xyz;
	vec3 normal  = texture2DLod(normal_buffer,tex_coord,  0.0).xyz;
	vec3 view_direction = normalize(camera_pos - position);
	
	//shadows
	float shadowed = 1.0;
	vec4 pre_w_pos = vec4(position,1.0);
	float pixel_depth;
	float shadow_map_depth;
	vec4 homo_pos;
	

#ifdef SHADOWED
#ifdef WORLD_SPACE

	//cascade 1
	homo_pos = (pre_w_pos * shadow_matrix0);
	homo_pos /= homo_pos.w;
	pixel_depth = homo_pos.z;
	
	vec3 sd = vec3(0.0,1.0,1.0);
	
	
	float found = 0.0;
	if( homo_pos.x >= 0.0 + shadow_map_texel_size0 && homo_pos.x <= 1.0 - shadow_map_texel_size0 &&
	    homo_pos.y >= 0.0 + shadow_map_texel_size0 && homo_pos.y <= 1.0 - shadow_map_texel_size0 &&
	    homo_pos.z >= 0.0 + shadow_map_texel_size0 && homo_pos.z <= 1.0 - shadow_map_texel_size0 && found == 0.0)
	{
		found = 1.0;
		shadowed = 0.0;
		
		shadow_map_depth = texture2DLod(shadow_map0,homo_pos.xy, 0.0).r;
		if(pixel_depth > (shadow_map_depth + SHADOW_BIAS)) shadowed += 1.0;
		
		shadow_map_depth = texture2DLod(shadow_map0,homo_pos.xy + vec2(-1.0 * shadow_map_texel_size0,-1.0 * shadow_map_texel_size0), 0.0).r;
		if(pixel_depth > (shadow_map_depth + SHADOW_BIAS)) shadowed += 1.0;
		
		shadow_map_depth = texture2D(shadow_map0,homo_pos.xy + vec2(-1.0 * shadow_map_texel_size0, 1.0 * shadow_map_texel_size0), 0.0).r;
		if(pixel_depth > (shadow_map_depth + SHADOW_BIAS)) shadowed += 1.0;
		
		shadow_map_depth = texture2DLod(shadow_map0,homo_pos.xy + vec2( 1.0 * shadow_map_texel_size0, 1.0 * shadow_map_texel_size0), 0.0).r;
		if(pixel_depth > (shadow_map_depth + SHADOW_BIAS)) shadowed += 1.0;
		
		shadow_map_depth = texture2DLod(shadow_map0,homo_pos.xy + vec2( 1.0 * shadow_map_texel_size0,-1.0 * shadow_map_texel_size0), 0.0).r;
		if(pixel_depth > (shadow_map_depth + SHADOW_BIAS)) shadowed += 1.0;
		
		shadow_map_depth = texture2DLod(shadow_map0,homo_pos.xy + vec2(-1.0 * shadow_map_texel_size0, 0.0 * shadow_map_texel_size0), 0.0).r;
		if(pixel_depth > (shadow_map_depth + SHADOW_BIAS)) shadowed += 1.0;
		
		shadow_map_depth = texture2DLod(shadow_map0,homo_pos.xy + vec2( 1.0 * shadow_map_texel_size0, 0.0 * shadow_map_texel_size0), 0.0).r;
		if(pixel_depth > (shadow_map_depth + SHADOW_BIAS)) shadowed += 1.0;
		
		shadow_map_depth = texture2DLod(shadow_map0,homo_pos.xy + vec2( 0.0 * shadow_map_texel_size0,-1.0 * shadow_map_texel_size0), 0.0).r;
		if(pixel_depth > (shadow_map_depth + SHADOW_BIAS)) shadowed += 1.0;
		
		shadow_map_depth = texture2DLod(shadow_map0,homo_pos.xy + vec2( 0.0 * shadow_map_texel_size0, 1.0 * shadow_map_texel_size0), 0.0).r;
		if(pixel_depth > (shadow_map_depth + SHADOW_BIAS)) shadowed += 1.0;
		
		shadowed = 1.0 - (shadowed / 9.0);
	}
	

	//cascade 2
	homo_pos = (pre_w_pos * shadow_matrix1);
	homo_pos /= homo_pos.w;
	pixel_depth = homo_pos.z;

	if( homo_pos.x >= 0.0 + shadow_map_texel_size1 && homo_pos.x <= 1.0 - shadow_map_texel_size1 &&
	    homo_pos.y >= 0.0 + shadow_map_texel_size1 && homo_pos.y <= 1.0 - shadow_map_texel_size1 &&
	    homo_pos.z >= 0.0 + shadow_map_texel_size1 && homo_pos.z <= 1.0 - shadow_map_texel_size1 && found == 0.0)
	{
		found = 1.0;
		shadowed = 0.0;
		
		shadow_map_depth = texture2DLod(shadow_map1,homo_pos.xy,0.0).r;
		if(pixel_depth > (shadow_map_depth + SHADOW_BIAS)) shadowed += 1.0;

		shadow_map_depth = texture2DLod(shadow_map1,homo_pos.xy + vec2(-1.0 * shadow_map_texel_size1,-1.0 * shadow_map_texel_size1), 0.0).r;
		if(pixel_depth > (shadow_map_depth + SHADOW_BIAS)) shadowed += 1.0;
		
		shadow_map_depth = texture2DLod(shadow_map1,homo_pos.xy + vec2(-1.0 * shadow_map_texel_size1, 1.0 * shadow_map_texel_size1), 0.0).r;
		if(pixel_depth > (shadow_map_depth + SHADOW_BIAS)) shadowed += 1.0;
		
		shadow_map_depth = texture2DLod(shadow_map1,homo_pos.xy + vec2( 1.0 * shadow_map_texel_size1, 1.0 * shadow_map_texel_size1), 0.0).r;
		if(pixel_depth > (shadow_map_depth + SHADOW_BIAS)) shadowed += 1.0;
		
		shadow_map_depth = texture2DLod(shadow_map1,homo_pos.xy + vec2( 1.0 * shadow_map_texel_size1,-1.0 * shadow_map_texel_size1), 0.0).r;
		if(pixel_depth > (shadow_map_depth + SHADOW_BIAS)) shadowed += 1.0;
		
		shadow_map_depth = texture2DLod(shadow_map1,homo_pos.xy + vec2(-1.0 * shadow_map_texel_size1, 0.0 * shadow_map_texel_size1), 0.0).r;
		if(pixel_depth > (shadow_map_depth + SHADOW_BIAS)) shadowed += 1.0;
		
		shadow_map_depth = texture2DLod(shadow_map1,homo_pos.xy + vec2( 1.0 * shadow_map_texel_size1, 0.0 * shadow_map_texel_size1), 0.0).r;
		if(pixel_depth > (shadow_map_depth + SHADOW_BIAS)) shadowed += 1.0;
		
		shadow_map_depth = texture2DLod(shadow_map1,homo_pos.xy + vec2( 0.0 * shadow_map_texel_size1,-1.0 * shadow_map_texel_size1), 0.0).r;
		if(pixel_depth > (shadow_map_depth + SHADOW_BIAS)) shadowed += 1.0;
		
		shadow_map_depth = texture2DLod(shadow_map1,homo_pos.xy + vec2( 0.0 * shadow_map_texel_size1, 1.0 * shadow_map_texel_size1), 0.0).r;
		if(pixel_depth > (shadow_map_depth + SHADOW_BIAS)) shadowed += 1.0;
		
		shadowed = 1.0 - (shadowed / 9.0);
	}
	
	//cascade 3
	homo_pos = (pre_w_pos * shadow_matrix2);
	homo_pos /= homo_pos.w;
	pixel_depth = homo_pos.z;
	
	if( homo_pos.x >= 0.0 + shadow_map_texel_size0 && homo_pos.x <= 1.0 - shadow_map_texel_size0 &&
	    homo_pos.y >= 0.0 + shadow_map_texel_size0 && homo_pos.y <= 1.0 - shadow_map_texel_size0 &&
	    homo_pos.z >= 0.0 + shadow_map_texel_size0 && homo_pos.z <= 1.0 - shadow_map_texel_size0 && found == 0.0)
	{
		found = 1.0;
		shadowed = 0.0;
		
		shadow_map_depth = texture2DLod(shadow_map2,homo_pos.xy, 0.0).r;
		if(pixel_depth > (shadow_map_depth + SHADOW_BIAS)) shadowed += 1.0;

		shadow_map_depth = texture2DLod(shadow_map2,homo_pos.xy + vec2(-1.0 * shadow_map_texel_size2,-1.0 * shadow_map_texel_size2), 0.0).r;
		if(pixel_depth > (shadow_map_depth + SHADOW_BIAS)) shadowed += 1.0;
		
		shadow_map_depth = texture2DLod(shadow_map2,homo_pos.xy + vec2(-1.0 * shadow_map_texel_size2, 1.0 * shadow_map_texel_size2), 0.0).r;
		if(pixel_depth > (shadow_map_depth + SHADOW_BIAS)) shadowed += 1.0;
		
		shadow_map_depth = texture2DLod(shadow_map2,homo_pos.xy + vec2( 1.0 * shadow_map_texel_size2, 1.0 * shadow_map_texel_size2), 0.0).r;
		if(pixel_depth > (shadow_map_depth + SHADOW_BIAS)) shadowed += 1.0;
		
		shadow_map_depth = texture2DLod(shadow_map2,homo_pos.xy + vec2( 1.0 * shadow_map_texel_size2,-1.0 * shadow_map_texel_size2), 0.0).r;
		if(pixel_depth > (shadow_map_depth + SHADOW_BIAS)) shadowed += 1.0;
		
		shadow_map_depth = texture2DLod(shadow_map2,homo_pos.xy + vec2(-1.0 * shadow_map_texel_size2, 0.0 * shadow_map_texel_size2), 0.0).r;
		if(pixel_depth > (shadow_map_depth + SHADOW_BIAS)) shadowed += 1.0;
		
		shadow_map_depth = texture2DLod(shadow_map2,homo_pos.xy + vec2( 1.0 * shadow_map_texel_size2, 0.0 * shadow_map_texel_size2), 0.0).r;
		if(pixel_depth > (shadow_map_depth + SHADOW_BIAS)) shadowed += 1.0;
		
		shadow_map_depth = texture2DLod(shadow_map2,homo_pos.xy + vec2( 0.0 * shadow_map_texel_size2,-1.0 * shadow_map_texel_size2), 0.0).r;
		if(pixel_depth > (shadow_map_depth + SHADOW_BIAS)) shadowed += 1.0;
		
		shadow_map_depth = texture2DLod(shadow_map2,homo_pos.xy + vec2( 0.0 * shadow_map_texel_size2, 1.0 * shadow_map_texel_size2), 0.0).r;
		if(pixel_depth > (shadow_map_depth + SHADOW_BIAS)) shadowed += 1.0;
		
		shadowed = 1.0 - (shadowed / 9.0);
	}

#endif	
#endif
	
	//Lighting
	vec4 final = vec4(0.0,0.0,0.0,1.0);
	
	//global ambient
	final += diffuse_colour * 0.005;
	
	//light sources
	for(int i = 0; i < number_of_lights; i++)
	{
		vec3 light_direction = normalize(light_pos[i].xyz - position) * (1.0 - light_pos[i].w);
		//light_direction += normalize(light_pos[i].xyz * light_pos[i].w);
		//vec3 light_direction = normalize(light_pos[i].xyz - position);

		float light_world_distance = length(light_pos[i].xyz - position);
		//float cos_cutoff = cos(0.34 * spot_direction[i].x);

		float n_dot_l = max( dot(normal, normalize(light_direction)), 0.0);

		//attenuation
		float attenuation = max((area_of_effect[i] - light_world_distance) / area_of_effect[i],0.0);
		attenuation = min(attenuation + light_pos[i].w,1.0);

		//diffuse
		final.xyz += attenuation * ( light_diffuse[i] * diffuse_colour.xyz * mat_diffuse.xyz * n_dot_l);

		//specular
		vec3 light_specular = clamp(vec3(1.0,1.0,1.0),0.6,0.9);
		float n_dot_h = pow( max( dot( reflect(-light_direction.xyz, normal), view_direction), 0.0), 5.0 );
		final.xyz += attenuation * light_specular * mat_specular.xyz * n_dot_h * pos_spec_pack.w * shadowed;
	}

	final *= clamp((shadowed + 0.3),0.0,1.0);
	
	//output
	gl_FragColor = vec4(final.xyz,1.0);
}
