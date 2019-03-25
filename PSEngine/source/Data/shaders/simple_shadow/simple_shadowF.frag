//#define DEPTH_SHADOW
#define VARIANCE_SHADOW

//interpolants
varying vec4 shadow_coord;

#ifdef VARIANCE_SHADOW
//samplers
//uniform sampler2D shadow_map;
uniform sampler2D shadow_map;

//uniforms
uniform float g_min_variance = 0.00002;

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

void main(void)
{
	shadow_coord_post_w = shadow_coord / shadow_coord.w;
	
	float shadowed = 1.0;
	float depth = texture2D(shadow_map,shadow_coord_post_w.xy).r;
	if(shadow_coord_post_w.z >= depth) shadowed = 0.0;
	
	//float shadowed = chebyshev_upper_bound(shadow_coord_post_w.z);
	
	gl_FragColor = vec4(vec3(shadowed) ,1.0);
}

#endif

#ifdef DEPTH_SHADOW

uniform sampler2DShadow shadow_map;

vec4 depth_shadow_mapping()
{
	vec3 shadow_uv = shadow_coord.xyz / shadow_coord.w;
	float map_scale = 1.0 / 1024.0;

	vec4 shadow_colour = shadow2D(shadow_map, shadow_uv.xyz);

	shadow_colour += shadow2D(shadow_map, shadow_uv.xyz + vec3( map_scale,  map_scale, 0));
	shadow_colour += shadow2D(shadow_map, shadow_uv.xyz + vec3( map_scale, -map_scale, 0));
	shadow_colour += shadow2D(shadow_map, shadow_uv.xyz + vec3( map_scale,  	  0, 0));
	shadow_colour += shadow2D(shadow_map, shadow_uv.xyz + vec3(-map_scale,  map_scale, 0));
	shadow_colour += shadow2D(shadow_map, shadow_uv.xyz + vec3(-map_scale, -map_scale, 0));
	shadow_colour += shadow2D(shadow_map, shadow_uv.xyz + vec3(-map_scale,  	  0, 0));
	shadow_colour += shadow2D(shadow_map, shadow_uv.xyz + vec3(        0,  map_scale, 0));
	shadow_colour += shadow2D(shadow_map, shadow_uv.xyz + vec3(        0, -map_scale, 0));

	shadow_colour = shadow_colour / 9.0;

	shadow_colour = clamp(shadow_colour, 0.0, 1.0);

	return shadow_colour;
	
	return vec4(1,1,1,1);
}

void main(void)
{
	vec4 colour = vec4(1,1,1,1);
	
	colour *= depth_shadow_mapping();

	gl_FragColor = vec4(vec3(colour) ,1.0);
}

#endif
