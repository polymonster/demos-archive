/****************************************
/	Al (c) 2008 - vertsh1.vsh
/****************************************
/	Vertex Shader
/****************************************/

// These lines are just for EffectEdit:
string XFile = "tiger\\tiger.x";   // model
int    BCLR = 0xff202080;          // background

// transformations provided by the app:
float4x4 matWorldViewProj: WORLDVIEWPROJECTION;

// the format of our vertex data
struct VS_OUTPUT
{
    float4 Pos  : POSITION;
};

// Vertex Shader - simply carry out transformation
VS_OUTPUT VS(float4 Pos : POSITION)
{
    VS_OUTPUT Out = (VS_OUTPUT)0;
    Out.Pos = mul(Pos,matWorldViewProj);
    return Out;
}

// Effect technique to be used
technique TVertexAndPixelShader
{
    pass P0
    {
        VertexShader = compile vs_1_1 VS();
    }  
}

