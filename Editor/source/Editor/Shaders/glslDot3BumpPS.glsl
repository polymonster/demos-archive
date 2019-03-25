/*
 www.UltimateGameProgramming.com
 Dot3 Bump 3 Pixel Shader in GLSlang.
 Created by the Programming Ace.


 Varying...
   texCoord = output vertex texture coordinates.
   direction = light direction in texture space.
   camera = camera position.


 Uniform...
   decal = texture image.
   normalMap = bump map texture.


 Output...
   gl_FragColor = output vertex color.
*/


varying vec2 texCoord;
varying vec3 direction;
varying vec3 camera;
varying vec3 viewDir;

uniform sampler2D decal;
uniform sampler2D parallaxMap;
uniform sampler2D normalMap;


void main()
{
   float scale = 0.04f;
   float bias = 0.02;

   vec3 decalCol = texture2D(decal, texCoord).xyz;
   vec3 normalTex = texture2D(normalMap, texCoord).xyz;

   // Normalize the light direction on a per pixel level.
   vec3 lightD = normalize(direction);
   vec3 viewD = normalize(viewDir);

   //vec3 heightTex = texture2D(parallaxMap, texCoord).xyz;
   //float v = scale * heightTex.x - bias;
   //vec2 newTexCoord = (v * viewDir.xy) + texCoord;
   //vec3 decalCol = texture2D(decal, newTexCoord).rgb;
   //vec3 normalTex = texture2D(normalMap, newTexCoord).rgb;
   
   //expand out the normal texture
   normalTex = (normalTex - 0.5f) * 2;

   // Calculate the half angle.
   vec3 cameraPos = normalize(camera);
   vec3 halfAngle = lightD + cameraPos;
   halfAngle = normalize(halfAngle);

   // Calculate the diffuse lighting term and the specular lighting term.
   float diffuse = clamp(dot(normalTex, lightD),0.0,1.0);
   float specular = clamp(dot(normalTex, halfAngle),0.0,1.0);

   float specFactor = specular * specular;
   specular = specFactor * specFactor;
   specFactor = specular * specular;

   // To make things simple I hardcoded material values but we could have passed
   // these as uniform parameters.
   vec3 dMat = vec3(0.7f, 0.7f, 0.7f);
   vec3 sMat = vec3(0.7f, 0.7f, 0.7f);
   float ambient = 0.4f;

   // Combine the decal with the bump value.
   gl_FragColor.r = decalCol.r * (dMat.x * (ambient + diffuse) + sMat.x * specFactor);
   gl_FragColor.g = decalCol.g * (dMat.y * (ambient + diffuse) + sMat.y * specFactor);
   gl_FragColor.b = decalCol.b * (dMat.z * (ambient + diffuse) + sMat.z * specFactor);
   gl_FragColor.a = 1;
}