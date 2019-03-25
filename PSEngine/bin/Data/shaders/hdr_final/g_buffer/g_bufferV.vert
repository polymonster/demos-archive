//interpolants
varying vec3 position;
varying vec2 tex_coord;

varying mat3 TBN;
varying mat3 rot_mat;
varying vec2 pixel_velocity;
varying vec2 pixel_velocity2;
varying vec4 material_col;

attribute vec4 tangent;

uniform mat4 model_matrix;
uniform mat4 previous_matrix;
uniform mat4 rotation_matrix;
					 
void main(void)
{
	rot_mat[0] = rotation_matrix[0].xyz;
	rot_mat[1] = rotation_matrix[1].xyz;
	rot_mat[2] = rotation_matrix[2].xyz;
	
	//create TBN matrix
	vec3 n = normalize(gl_Normal * rot_mat);
	vec3 t = normalize(tangent.xyz * rot_mat);
	vec3 b = cross( n, t ) * tangent.w;
	
	TBN[0] = t;
	TBN[1] = b; 
	TBN[2] = n; 	
	
	//set the vertex position and texture coords
	tex_coord = gl_MultiTexCoord0.xy;
	position = (gl_Vertex * model_matrix).xyz;
	
	material_col = gl_Color;
	
	vec4 current_ss_position =  gl_ModelViewProjectionMatrix * gl_Vertex;
	vec4 previous_ss_position =  gl_ProjectionMatrix * (gl_Vertex * previous_matrix);
	
	//pixel_velocity =  ((current_ss_position / current_ss_position.w ).xy - (previous_ss_position / previous_ss_position.w ).xy) * 0.5;
	
	pixel_velocity = (previous_ss_position / previous_ss_position.w ).xy;
	pixel_velocity2 =  (current_ss_position / current_ss_position.w ).xy;

	
	gl_Position = current_ss_position;
}
