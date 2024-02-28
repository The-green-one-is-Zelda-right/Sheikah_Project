struct VS_INPUT
{
    float4 mPosition : POSITION;
};

struct VS_OUTPUT
{
    float4 mPosition : POSITION;
};

float4x4 gWorldMatrix;
float4x4 gViewMatrix;
float4x4 gProjectionMatrix;


VS_OUTPUT main(VS_INPUT Input)
{
    VS_OUTPUT Output;

    Output.mPosition = mul(Input.mPosition, gWorldMatrix);
    Output.mPosition = mul(Output.mPosition, gViewMatrix);
    Output.mPosition = mul(Output.mPosition, gProjectionMatrix);
   
    return Output;
}