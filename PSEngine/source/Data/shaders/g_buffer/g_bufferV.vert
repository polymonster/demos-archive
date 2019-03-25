//consts
const float near = 1.0;
const float far = 10000.0;

//interpolants
varying vec3 position;
varying float positionw;
varying vec2 tex_coord;
varying vec4 current_ss_position;
varying vec4 previous_ss_position;
varying vec4 grandfather_ss_position;

varying mat3 TBN;
varying vec4 material_col;

attribute vec4 tangent;

uniform mat4 model_matrix;
uniform mat4 previous_matrix;
uniform mat4 grandfather_matrix;
uniform mat4 rotation_matrix;
uniform mat4 model_view_matrix;
			 
void main(void)
{
	//create TBN matrix
	mat3 rot3x3;
	rot3x3[0] = rotation_matrix[0].xyz;
	rot3x3[1] = rotation_matrix[1].xyz;
	rot3x3[2] = rotation_matrix[2].xyz;

	vec3 n = normalize(gl_Normal * rot3x3);
	vec3 t = normalize(tangent.xyz * rot3x3);
	vec3 b = cross( n, t ) * tangent.w;
	
	TBN[0] = t;
	TBN[1] = b; 
	TBN[2] = n; 	
	
	//set the vertex position and texture coords
	tex_coord = gl_MultiTexCoord0.xy;
	position = (gl_Vertex * model_matrix).xyz;
	positionw = 1.0;
	
	material_col = gl_Color;
	
	current_ss_position =  gl_ModelViewProjectionMatrix * gl_Vertex;
	previous_ss_position = gl_Vertex * previous_matrix;
	grandfather_ss_position = gl_Vertex * grandfather_matrix;

	gl_Position = current_ss_position;
}
