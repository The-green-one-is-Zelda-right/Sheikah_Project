cbuffer ConstantBuffer : register(b0)
{
    float width;
    float height;
    float x;
    float y;
};

//struct VS_OUTPUT
//{
//    float4 position : SV_POSITION;
//    float2 uv : TEXCOORD;
//};

//VS_OUTPUT main(uint id : SV_VertexID)
//{
//    VS_OUTPUT output;
    
//    output.uv = float2((id << 1) & 2, id & 2);
    
//    output.position = float4(output.uv * float2(2, -2) + float2(-1, 1), 0, 1);
    
//    return output;
//}

// Test

struct VS_INPUT
{
    float3 Position : POSITION;
    float2 UV : TEXCOORD;
};

struct VS_OUTPUT
{
    float4 position : SV_POSITION;
    float2 uv : TEXCOORD;
};

VS_OUTPUT main(VS_INPUT input)
{
    VS_OUTPUT output;
    output.uv = input.UV;
    output.position = float4((input.Position.x * width) + x - 0.5f, (input.Position.y * height) + (1 - y) - 0.5, input.Position.z, 1.0f);
    return output;
}