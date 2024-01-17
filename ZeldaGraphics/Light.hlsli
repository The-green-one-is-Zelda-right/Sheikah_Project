#ifndef _LIGHT_HLSLI_
#define _LIGHT_HLSLI_

#include "Buffers.hlsli"

void CalculateLight(int lightIndex, float3 normal, float3 viewPos, out float4 diffuse, out float4 ambient, out float4 specular)
{
    diffuse = float4(0.f, 0.f, 0.f, 0.f);
    ambient = float4(0.f, 0.f, 0.f, 0.f);
    specular = float4(0.f, 0.f, 0.f, 0.f);
    
    float3 viewDirection = normalize(viewPos);
    
    if (lights[lightIndex].type == LIGHT_TYPE_DIRECTIONAL)
    {
        float3 lightVec = normalize(mul(float4(lights[lightIndex].direction.xyz, 0.f), viewMatrix).xyz);
        ambient = lights[lightIndex].color.ambient;
        
        float diffuseFactor = dot(-lightVec, normal);
        
        if (diffuseFactor > 0.f)
        {
            float3 reflectDir = normalize(lightVec + 2 * (saturate(dot(-lightVec, normal)) * normal));
            float specFactor = pow(max(dot(-viewDirection, reflectDir), 0.f), 16.f);
            
            diffuse = diffuseFactor * lights[lightIndex].color.diffuse;
            specular = specFactor * lights[lightIndex].color.specular;
        }
    }
    else if (lights[lightIndex].type == LIGHT_TYPE_POINT)
    {
        float diffuseRatio = 0.0f;
        float distanceRatio = 1.0f;
        
        float3 lightPos = mul(float4(lights[lightIndex].position.xyz, 1.0f), viewMatrix).xyz;
        float3 lightVec = normalize(viewPos - lightPos);
        
        diffuseRatio = saturate(dot(-lightVec, normal));

        float d = distance(viewPos, lightPos);
        if (lights[lightIndex].range == 0.0f)
        {
            distanceRatio = 0.0f;
        }
        else
        {
            distanceRatio = saturate(1.0f - pow(d / lights[lightIndex].range, 2.0f));
        }

        float3 reflectDir = normalize(lightVec + 2 * (saturate(dot(-lightVec, normal)) * normal));
        float specFactor = pow(max(dot(-viewDirection, reflectDir), 0.f), 16.f);
        
        ambient = lights[lightIndex].color.ambient * distanceRatio;
        diffuse = lights[lightIndex].color.diffuse * diffuseRatio * distanceRatio;
        specular = specFactor * lights[lightIndex].color.specular * distanceRatio;
    }
    else if (lights[lightIndex].type == LIGHT_TYPE_SPOT)
    {
        
    }
    else
    {
        
    }
}

#endif