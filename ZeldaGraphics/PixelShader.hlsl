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
    unsigned int useTemp0;
};
    
cbuffer ColorBufferType : register(b2)
{
    float4 baseColor;
};

struct PixelInputType
{
    float4 position : SV_POSITION;
    float2 tex : TEXCOORD0;
    float3 normal : NORMAL;
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
	/* �Ʒ� �ڵ忡�� ��ֺ��Ϳ� �������� ������ ���ϰ� [0, 1] �������� clamp�ϴ°ǵ�
	���� ��ֺ��Ϳ� ������ ������ ���� 90������ ũ�ٸ� �������� ������ �ǹǷ�
	clamp�� ����� 0�� ��(90���̾ 0). ���� lightIntensity = 0�̰�, ���� �ƿ� �ȹ޴°� �ȴ�.
	���� 90������ �۴ٸ� clamp�� ����� 0���� ũ�� �ȴ�. 
	���� ������ ���� ���� clamp�� ����� 1�� ���������, ���� ����� ��������. */
    
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