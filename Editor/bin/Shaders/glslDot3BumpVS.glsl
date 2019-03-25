/*
 www.UltimateGameProgramming.com
 Dot3 Bump 3 Vertex Shader in GLSlang.
 Created by the Programming Ace.


 Input...
   gl_Vertex = vertex in positon.
   gl_Normal = vertex normal.
   gl_Color = vertex in s tangent.
   gl_ModelViewProjectionMatrix = current model view projection matrix.
   gl_MultiTexCoord0.xy = texture coords for unit 0.


 Varying...
   texCoord = output vertex texture coordinates.
   direction = light direction in texture space.
   camera = camera position.


 Uniform...
   cameraPos = camera position.
   lightPos = light position.


 Output...
   gl_Position = vertex output position.
*/


varying vec2 texCoord;
varying vec3 direction;
varying vec3 camera;
varying vec3 viewDir;


uniform vec4 lightPos;
uniform vec4 cameraPos;


void main()
{
   // Transform to clip space and save texture coord.
   gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;
   vec4 pos = gl_ModelViewMatrixInverse * gl_Vertex;
   texCoord = gl_MultiTexCoord0.xy;

   // Calculate light direction.
   vec3 lightDir;
	lightDir.x = lightPos.x - gl_Vertex.x;
	lightDir.y = lightPos.y - gl_Vertex.y;
	lightDir.z = lightPos.z - gl_Vertex.z;


   // Save a copy of the s tangent which was sent through the color (glColor3f).
   vec3 color = gl_Color.xyz;

   // Calculate the binormal as the cross between tangent and normal.
   //vec3 binormal = cross(gl_Normal,color);
   vec3 binormal = cross(color,gl_Normal);
   
   // Create the matrix used to convert the light direction to texture space.
   mat3 tbnMatrix = mat3(color, binormal, gl_Normal);

   // Convert light direction to texture space.
   direction = tbnMatrix * lightDir;

   // Send cameraPos to pixel shader so it can compute per pixel half angle.
   camera.x = cameraPos.x;
   camera.y = cameraPos.y;
   camera.z = cameraPos.z;

   viewDir.xyz = camera.xyz - pos.xyz;
   viewDir *= tbnMatrix;
}