//Texture uniuts
Texture x_texture_1;
Texture x_texture_2;
Texture x_texture_3;
Texture x_texture_4;
Texture x_shadow_map;

//Samplers

sampler texture_unit_1 = 
sampler_state 
{ 
	texture = <x_texture_1>; 
	magfilter = LINEAR; 
	minfilter = LINEAR; 
	mipfilter = LINEAR;
	AddressU = Wrap;
	AddressV = Wrap;
};

sampler texture_unit_2 = 
sampler_state 
{ 
	texture = <x_texture_2>; 
	magfilter = LINEAR; 
	minfilter = LINEAR; 
	mipfilter = LINEAR; 
	AddressU = Wrap;
	AddressV = Wrap;
};

sampler texture_unit_3 = 
sampler_state 
{ 
	texture = <x_texture_3>; 
	magfilter = LINEAR; 
	minfilter = LINEAR; 
	mipfilter = LINEAR;
	AddressU = Wrap;
	AddressV = Wrap;
};

sampler texture_unit_4 = 
sampler_state 
{ 
	texture = <x_texture_4>; 
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


//UNIFROMS
float4x4 view_projection_matrix;
float4x4 shadow_projection_matrix;
float4x4 world_matrix;
float3 shadow_catser_position;

float3 light_pos = float3(0,400,0);
float light_power = 5.0f;
float ambient = 0.1f;

struct VertexShaderInput
{
    float4 Position : POSITION0;
    float4 weight : COLOR;
    float2 tex_coords_1 : TEXCOORD;
    float3 normal : NORMAL;
    
};

struct VertexShaderOutput
{
    float4 Position : POSITION0;
    float4 weight : TEXCOORD4;
    float2 tex_coords_1 : TEXCOORD0;
    float3 normal : COLOR1;
    float4 shadow_projection : TEXCOORD2;
    float2 tex_coords_2 : TEXCOORD1;
    float4 world_position : TEXCOORD3;
};

VertexShaderOutput VertexShaderFunction(VertexShaderInput input)
{
    VertexShaderOutput output = (VertexShaderOutput)0;

    output.Position = mul(input.Position, view_projection_matrix);
    output.tex_coords_1 = input.tex_coords_1;
    output.tex_coords_2 = input.tex_coords_1;
    output.normal = normalize(mul(normalize(input.normal), world_matrix));
    output.weight = input.weight;
    output.shadow_projection = mul(input.Position,shadow_projection_matrix);
    
    output.world_position = mul(input.Position, world_matrix);
    output.world_position /= output.world_position.w;
  

    return output;
}

float4 PixelShaderFunction(VertexShaderOutput input) : COLOR0
{	
	//shadow mapping
    float2 shadow_map_coords;
    
    shadow_map_coords.x = 
    (input.shadow_projection.x / input.shadow_projection.w * 0.5f) + 0.5f;
                             
    shadow_map_coords.y =
    (-input.shadow_projection.y / input.shadow_projection.w * 0.5f) + 0.5f;
    
    float map_scale = 1.0f / 2048.0f;
    
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
		if(len > shadow_color[i].r) main_shad -= 0.6f;
	}
	
	main_shad /= 9.0f;
	
	//splatting
	float4 col_1 = tex2D(texture_unit_1,input.tex_coords_1);
    float4 col_2 = tex2D(texture_unit_2,input.tex_coords_1);
    float4 col_3 = tex2D(texture_unit_3,input.tex_coords_1);
    float4 col_4 = tex2D(texture_unit_4,input.tex_coords_1);
	
    float4 temp = col_1 * input.weight.r;
    temp = lerp(temp,col_2,input.weight.g);
    temp = lerp(temp,col_3,input.weight.b);
    float4 final = lerp(temp,col_4,input.weight.a);
    
	//lighting
	float3 light_dir = normalize(input.world_position - float3(50,150,50));
    float diffuse_factor = dot(-light_dir, input.normal); 
	diffuse_factor = saturate(diffuse_factor + ambient);
	
	//combine
	final *= diffuse_factor * main_shad;
	
    return float4(final.rgb, 1);
   
}

technique TerrainShader
{
    pass splatting
    {
        VertexShader = compile vs_2_0 VertexShaderFunction();
        PixelShader = compile ps_2_0 PixelShaderFunction();
    }
}
