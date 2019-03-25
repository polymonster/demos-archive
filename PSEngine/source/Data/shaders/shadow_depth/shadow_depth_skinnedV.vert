//varyings
varying vec4 vertex_position;

//attributes
attribute vec4 bone_weights0;
attribute vec4 bone_weights1;
attribute vec4 bone_weights2;

attribute vec4 bone_indices0;
attribute vec4 bone_indices1;
attribute vec4 bone_indices2;

//uniforms
uniform mat4 joint_matrices[78];
uniform mat4 axis_conversion;
uniform mat4 bind_shape_matrix;
			 
void main(void)
{
	//declarations
	mat3 joint_matrix3x3;
	mat3 bind_shape_rot;
	int index;
	
	//transform verts, normals and tangents to bindshape [possibly cache this data]
	vec4 vertex_pos = (gl_Vertex * bind_shape_matrix);

	//use some vectors to accumulate deformation
	vec4 final_pos = vec4(0.0,0.0,0.0,1.0);
	
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
		final_pos += (vertex_pos * joint_matrices[bone_index]) * weights[i].x;

		//weight / index y
		bone_index = int(indices[i].y);
		final_pos += (vertex_pos * joint_matrices[bone_index]) * weights[i].y;

		//weight / index z
		bone_index = int(indices[i].z);
		final_pos += (vertex_pos * joint_matrices[bone_index]) * weights[i].z;
		
		//weight / index w
		bone_index = int(indices[i].w);
		final_pos += (vertex_pos * joint_matrices[bone_index]) * weights[i].w;
	}
	
	//convert axis system to y up and -z
	final_pos = final_pos * axis_conversion;
	final_pos.w = 1.0;
 
	//finally set glposition
	gl_Position = gl_ModelViewProjectionMatrix * final_pos;
	vertex_position = gl_Position;
}
