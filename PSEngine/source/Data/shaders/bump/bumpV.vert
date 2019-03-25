//interpolants
varying vec3 light_vec; 
varying vec3 eye_vec;
varying vec2 tex_coord;
varying vec3 tangent_space_normal;
varying vec4 shadow_coord;

//attributes
attribute vec4 tangent; 
					 
void main(void)
{
	//create TBN matrix
	vec3 n = normalize(gl_NormalMatrix * gl_Normal);
	vec3 t = normalize(gl_NormalMatrix * tangent.xyz);
	vec3 b = cross(n, t) * tangent.w;
	
	//convert light vector to tangent space
	vec3 vertex_pos = vec3(gl_ModelViewMatrix * gl_Vertex);
	vec3 aux = gl_LightSource[0].position.xyz - vertex_pos;
	light_vec.x = dot(aux, t);
	light_vec.y = dot(aux, b);
	light_vec.z = dot(aux, n);

	//convert eye vector to tangent space
	aux = -vertex_pos;
	eye_vec.x = dot(aux, t);
	eye_vec.y = dot(aux, b);
	eye_vec.z = dot(aux, n);
	
	//convert the normal into tanget spac
	aux = n;
	tangent_space_normal.x = dot(aux, t);
	tangent_space_normal.y = dot(aux, b);
	tangent_space_normal.z = dot(aux, n);	
	
	//set the vertex position and texture coords
	tex_coord = gl_MultiTexCoord0.xy;
	gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;
	shadow_coord = gl_TextureMatrix[7] * gl_Vertex;
}
