float4x4 view_projection_matrix;
float4x4 world_matrix;
float3 shadow_catser_position;
float4x4 shadow_projection_matrix;

Texture x_diffuse_texture;
Texture x_shadow_map;

int lit;

sampler diffuse_texture_unit = 
sampler_state 
{ 
	texture = <x_diffuse_texture>; 
	magfilter = LINEAR; 
	minfilter = LINEAR; 
	mipfilter = LINEAR; 
	AddressU = Wrap;
	AddressV = Wrap;
};

sampler shadow_map_unit = 
sampler_state 
{ 
	texture = <x_shadow_map>; 
	magfilter = LINEAR; 
	minfilter = LINEAR; 
	mipfilter = LINEAR; 
	AddressU = Border;
	AddressV = Border;
};

float3 light_pos = float3(-4,300,0);
float light_power = 1.0f;
float ambient = 0.2f;

struct VertexShaderInput
{
    float4 Position : POSITION0;
    float2 texture_coords : TEXCOORD;
	float3 normal		: NORMAL;
};

struct VertexShaderOutput
{
    float4 Position : POSITION0;
    float2 texture_coords : TEXCOORD;
    float4 shadow_projection : TEXCOORD2;
    float4 world_position : TEXCOORD3;
	float3 normal : COLOR1;
};

VertexShaderOutput VertexShaderFunction(VertexShaderInput input)
{
    VertexShaderOutput output;

    output.Position = mul(input.Position, view_projection_matrix);
    output.texture_coords = input.texture_coords;
    output.shadow_projection = mul(input.Position,shadow_projection_matrix);
    
    output.world_position = mul(input.Position, world_matrix);
    output.world_position /= output.world_position.w;
	
	output.normal = normalize(mul(normalize(input.normal), world_matrix));

    return output;
}

float4 PixelShaderFunction(VertexShaderOutput input) : COLOR0
{
	float4 final = tex2D(diffuse_texture_unit,input.texture_coords);
	
	if(final.a < 0.3f) 
	{
		discard;
	}
		
	float2 shadow_map_coords;
    
    shadow_map_coords.x = 
    (input.shadow_projection.x / input.shadow_projection.w * 0.5f) + 0.5f;
                             
    shadow_map_coords.y =
    (-input.shadow_projection.y / input.shadow_projection.w * 0.5f) + 0.5f;
    
    float map_scale = 1.0f / 1024.0f;
    
    float2 shadow_map_min;
    float2 shadow_map_max;
    
    shadow_map_min.xy = shadow_map_coords + map_scale;
    shadow_map_max.xy = shadow_map_coords - map_scale;
    
    //multi samples
    float4 shadow_color[9];
    shadow_color[0] = tex2D(shadow_map_unit,shadow_map_coords);
    
    shadow_color[1] = tex2D(shadow_map_unit,float2(shadow_map_max.x,shadow_map_coords.y));
    shadow_color[2] = tex2D(shadow_map_unit,float2(shadow_map_min.x,shadow_map_coords.y));
    
    shadow_color[3] = tex2D(shadow_map_unit,float2(shadow_map_coords.x,shadow_map_max.y));
    shadow_color[4] = tex2D(shadow_map_unit,float2(shadow_map_coords.x,shadow_map_min.y));
    
    shadow_color[5] = tex2D(shadow_map_unit,float2(shadow_map_min.x,shadow_map_min.y));
    shadow_color[6] = tex2D(shadow_map_unit,float2(shadow_map_max.x,shadow_map_max.y));
    
    shadow_color[7] = tex2D(shadow_map_unit,float2(shadow_map_max.x,shadow_map_min.y));
    shadow_color[8] = tex2D(shadow_map_unit,float2(shadow_map_min.x,shadow_map_max.y));
    
	float len = length(shadow_catser_position - input.world_position) / 300.0f;
	float main_shad = 9.0f;
	
	for(int i = 0; i < 9; i++)
	{
		if(len - (1.0f / 250.0f) > shadow_color[i].r) main_shad -= 0.6f;
	}
	
	main_shad /= 9.0f;
	
	if(lit == 1)
	{
		//lighting
		float3 light_dir = normalize(input.world_position - float3(50,150,50));
		float diffuse_factor = dot(-light_dir, input.normal); 
		diffuse_factor = saturate(diffuse_factor + ambient);
		diffuse_factor *= 1.2;
		
		//combine
		final *= diffuse_factor;
	}
    
	final *= main_shad;
	
    return float4(final.rgb, 1);
}

technique MainRender
{
    pass rendering
    {
        VertexShader = compile vs_2_0 VertexShaderFunction();
        PixelShader = compile ps_2_0 PixelShaderFunction();
    }
}

struct ShadowShaderInput
{
    float4 Position : POSITION0;
    float2 texture_coords : TEXCOORD;
};

struct ShadowShaderOutput
{
    float4 Position : POSITION0;
    float4 world_position : TEXCOORD1;
    float2 texture_coords : TEXCOORD;
};

ShadowShaderOutput ShadowShaderVertex(ShadowShaderInput input)
{
    ShadowShaderOutput output;

    output.Position = mul(input.Position, view_projection_matrix);
    output.texture_coords = input.texture_coords;
    
    output.world_position = mul(input.Position, world_matrix);
    output.world_position /= output.world_position.w;

    return output;
}

float4 ShadowShaderPixel(ShadowShaderOutput input) : COLOR0
{
	float4 final = tex2D(diffuse_texture_unit,input.texture_coords);
	
	if(final.a < 0.1f) 
	{
		discard;
	}
	
	float depth = length(shadow_catser_position - input.world_position) / 300.0f;

    return float4(depth,depth,depth,1);
}

technique ShadowRender
{
    pass depth_pass
    {
        VertexShader = compile vs_2_0 ShadowShaderVertex();
        PixelShader = compile ps_2_0 ShadowShaderPixel();
    }
}