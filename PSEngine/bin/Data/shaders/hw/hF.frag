//inputs
varying vec4 position;
varying vec3 normal;
varying vec2 diffuse_texture_coords;

//samplers
uniform sampler2D diffuse_map;
uniform sampler2D specular_map;

void main(void)
{
	//data 1 = lit
	vec4 final = vec4(0.0,0.0,0.0,1.0);
	
	vec3 n, view_direction;
	vec4 light_direction;
	vec4 total_diffuse;
	float n_dot_l, n_dot_h, attenuation, distance;
	
	//setup light position and direction
	distance = length(gl_LightSource[0].position - position );
	light_direction = normalize ( gl_LightSource[0].position - position);
	view_direction = normalize(-position.xyz);
	
	distance = length( light_direction );
	n = normalize(normal);
	
	//diffuse texture compoent
	vec4 diffuse_colour = texture2D(diffuse_map, diffuse_texture_coords);
	vec4 specular_texture = texture2D(specular_map, diffuse_texture_coords);
	
	//lighting components
	n_dot_l = max( dot(n, normalize(light_direction.xyz) ), 0.0);
	
	//ambient
	//final += diffuse_colour * gl_FrontMaterial.ambient;
	
	if (n_dot_l > 0.0) 
	{
			//calculate attenuation
			attenuation = 1.0 / 
			(gl_LightSource[0].constantAttenuation +
			gl_LightSource[0].linearAttenuation * distance +
			gl_LightSource[0].quadraticAttenuation * distance * distance);
					
			//diffuse
			final += attenuation * (gl_LightSource[0].diffuse * diffuse_colour * gl_FrontMaterial.diffuse * n_dot_l);
			
			//specular
			float n_dot_h = pow( clamp(dot(reflect(-light_direction.xyz, n), view_direction), 0.0, 1.0), gl_FrontMaterial.shininess );
		    final += attenuation * gl_LightSource[0].specular * gl_FrontMaterial.specular * n_dot_h * 1.0;
	}
	
	gl_FragData[0] = vec4(final.xyz,1);
	gl_FragData[1] = vec4(n,1);
	gl_FragData[2] = diffuse_colour;
}
