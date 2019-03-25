/*

% Basic Shader to control particles

*/

struct VS_INPUT
{
    float3 Position : POSITION;
    float4 Colour   : COLOR;
};

struct VS_OUTPUT
{
    float4 Position :POSITION;
    float4 Colour : COLOR;

};

float4x4 WorldViewProj : WorldViewProjection;
float4 Colourx;

VS_OUTPUT mainVS(VS_INPUT input) 
{
  VS_OUTPUT output = (VS_OUTPUT)0;
  
  output.Position = mul(float4(input.Position.xyz, 1.0), WorldViewProj);
  output.Colour = input.Colour;
  return output;
}
float4 PixelShader( VS_OUTPUT vsout ) : COLOR
{
    return vsout.Colour;
}


technique technique0
{
	pass p0 
  {
		VertexShader = compile vs_2_0 mainVS();
		PixelShader = compile ps_1_1 PixelShader();
  }
}
