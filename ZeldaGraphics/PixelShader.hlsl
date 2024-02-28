Texture2D shaderTexture : register(t0);
SamplerState SampleType;

cbuffer LightBuffer : register(b0)
{
    float4 ambient;
    float4 diffuse;
    float4 specular;
    float3 lightDirection;
    float padding;
};

cbuffer UseBufferType : register(b1)
{
    unsigned int useNormal;
    unsigned int useTexture;
    unsigned int useColor;
    unsigned int useSRGB;

    unsigned int useTemp0;
    unsigned int useTemp1;
    unsigned int useTemp2;
    unsigned int useTemp3;
};
    
cbuffer ColorBufferType : register(b2)
{
    float4 baseColor;
};

struct PixelInputType
{
    float4 position : SV_POSITION;
    float3 normal : NORMAL;
    float2 tex : TEXCOORD0;
};

float4 main(PixelInputType input) : SV_TARGET
{
    float4 textureColor;
    float3 lightDir;
    float diffuseIntensity;
    float specularIntensity = 0.0f;
    float4 color;
    
	// Sample the pixel color from the texture using the sampler at this texture coordinate location.
    if (useTexture == true)
    {
        textureColor = shaderTexture.Sample(SampleType, input.tex);
        
        if (useSRGB)
        {
            // SRGB 색 공간에서 선형 색 공간으로.
            textureColor = pow(textureColor, 1.0f / 2.2f);
        }
    }
    else if (useColor == true)
    {
        textureColor = baseColor;
    }
    else
    {
        textureColor = float4(1.0f, 1.0f, 1.0f, 1.0f);
    }
    
    // Invert the light direction for calculations.
    lightDir = -lightDirection;
	// Calculate the amount of light on this pixel.
    diffuseIntensity = saturate(dot(input.normal, lightDir));
    
    if (diffuseIntensity > 0.0f)
    {
        float3 viewDir = float3(0.0f, 0.0f, -1.0f);
        float specFactor = pow(saturate(dot(reflect(-viewDir, input.normal), lightDir)), 32.0f);
        specularIntensity = specFactor;
    }

    // Calculate final color using the Phong lighting model.
    color = (ambient + diffuseIntensity * diffuse) * textureColor + specularIntensity * specular;
    
    return color;
}