//interpolants
varying vec4 vertex_position;

//samplers
void main(void)
{
	//get the depth by perspective divide, and covert to 0 - 1 range
	
	float depth = (vertex_position.z / vertex_position.w);
	depth = (depth * 0.5) + 0.5; 

	gl_FragColor = vec4(depth,0.0,0.0,1.0);
}
