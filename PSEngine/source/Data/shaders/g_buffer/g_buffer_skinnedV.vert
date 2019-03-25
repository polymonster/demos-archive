//interpolants
varying vec3 position;
varying float positionw;
varying vec2 tex_coord;
varying vec4 current_ss_position;
varying vec4 previous_ss_position;
varying vec4 grandfather_ss_position;
varying mat3 TBN;
varying mat3 rot_mat;
varying vec4 material_col;

//attributes
attribute vec4 tangent;
attribute vec4 bone_weights0;
attribute vec4 bone_weights1;
attribute vec4 bone_weights2;

attribute vec4 bone_indices0;
attribute vec4 bone_indices1;
attribute vec4 bone_indices2;

//uniforms
uniform mat4 model_matrix;
uniform mat4 previous_matrix;
uniform mat4 grandfather_matrix;
uniform mat4 rotation_matrix;
uniform mat4 joint_matrices[78];
uniform mat4 axis_conversion;
uniform mat4 bind_shape_matrix;
			 
void main(void)
{
	//declarations
	mat3 joint_matrix3x3;
	mat3 bind_shape_rot;
	int index;
	
	//do vertex skinning
	bind_shape_rot[0] = bind_shape_matrix[0].xyz;
	bind_shape_rot[1] = bind_shape_matrix[1].xyz;
	bind_shape_rot[2] = bind_shape_matrix[2].xyz;
	
	//transform verts, normals and tangents to bindshape [possibly cache this data]
	vec4 vertex_pos = (gl_Vertex * bind_shape_matrix);
	vec3 nrm = gl_Normal * bind_shape_rot;
	vec3 tan = tangent.xyz * bind_shape_rot;
	
	//use some vectors to accumulate deformation
	vec4 final_pos = vec4(0.0,0.0,0.0,1.0);
	vec3 final_normal = vec3(0.0,0.0,0.0);
	vec3 final_tangent = vec3(0.0,0.0,0.0);
	
	//copy weights into arrays for looping
	vec4 weights[3];
	weights[0] = bone_weights0;
	weights[1] = bone_weights1;
	weights[2] = bone_weights2;
	
	vec4 indices[3];
	indices[0] = bone_indices0;
	indices[1] = bone_indices1;
	indices[2] = bone_indices2;
	
	for(int i = 0; i < 3; i++)
	{
		//weight / index x
		int bone_index = int(indices[i].x);

		joint_matrix3x3[0] = joint_matrices[bone_index][0].xyz;
		joint_matrix3x3[1] = joint_matrices[bone_index][1].xyz;
		joint_matrix3x3[2] = joint_matrices[bone_index][2].xyz;
			
		final_pos += (vertex_pos * joint_matrices[bone_index]) * weights[i].x;
		final_normal += (nrm * joint_matrix3x3) * weights[i].x;
		final_tangent += (tan * joint_matrix3x3) * weights[i].x;
		
		//weight / index y
		bone_index = int(indices[i].y);

		joint_matrix3x3[0] = joint_matrices[bone_index][0].xyz;
		joint_matrix3x3[1] = joint_matrices[bone_index][1].xyz;
		joint_matrix3x3[2] = joint_matrices[bone_index][2].xyz;
			
		final_pos += (vertex_pos * joint_matrices[bone_index]) * weights[i].y;
		final_normal += (nrm * joint_matrix3x3) * weights[i].y;
		final_tangent += (tan * joint_matrix3x3) * weights[i].y;
		
		//weight / index z
		bone_index = int(indices[i].z);

		joint_matrix3x3[0] = joint_matrices[bone_index][0].xyz;
		joint_matrix3x3[1] = joint_matrices[bone_index][1].xyz;
		joint_matrix3x3[2] = joint_matrices[bone_index][2].xyz;
			
		final_pos += (vertex_pos * joint_matrices[bone_index]) * weights[i].z;
		final_normal += (nrm * joint_matrix3x3) * weights[i].z;
		final_tangent += (tan * joint_matrix3x3) * weights[i].z;
		
		//weight / index w
		bone_index = int(indices[i].w);

		joint_matrix3x3[0] = joint_matrices[bone_index][0].xyz;
		joint_matrix3x3[1] = joint_matrices[bone_index][1].xyz;
		joint_matrix3x3[2] = joint_matrices[bone_index][2].xyz;
			
		final_pos += (vertex_pos * joint_matrices[bone_index]) * weights[i].w;
		final_normal += (nrm * joint_matrix3x3) * weights[i].w;
		final_tangent += (tan * joint_matrix3x3) * weights[i].w;
	}
	
	//convert axis system to y up and -z
	mat3 axis_conversion3x3;
	axis_conversion3x3[0] = axis_conversion[0].xyz;
	axis_conversion3x3[1] = axis_conversion[1].xyz;
	axis_conversion3x3[2] = axis_conversion[2].xyz;
	
	final_pos = final_pos * axis_conversion;
	final_pos.w = 1.0;
	
	final_normal = final_normal * axis_conversion3x3;
	final_tangent = final_tangent * axis_conversion3x3;
	
	//create TBN matrix
	mat3 rot3x3;
	rot3x3[0] = rotation_matrix[0].xyz;
	rot3x3[1] = rotation_matrix[1].xyz;
	rot3x3[2] = rotation_matrix[2].xyz;
	
	vec3 n = normalize(final_normal * rot3x3);
	vec3 t = normalize(final_tangent * rot3x3);
	vec3 b = cross( n, t ) * tangent.w;
	
	//TBN interpolant
	TBN[0] = t;
	TBN[1] = b; 
	TBN[2] = n; 	
	
	//set the vertex position and texture coords and material
	tex_coord = gl_MultiTexCoord0.xy;
	material_col = gl_Color;
	
	vec4 pos4 = (final_pos * model_matrix);
	position = pos4.xyz;
	positionw = pos4.w;
	
	//compute positions for perpixel velocities
	current_ss_position =  gl_ModelViewProjectionMatrix * final_pos;
	previous_ss_position = final_pos * previous_matrix;
	grandfather_ss_position = final_pos * grandfather_matrix;
 
	//finally set glposition
	gl_Position = current_ss_position;
}
