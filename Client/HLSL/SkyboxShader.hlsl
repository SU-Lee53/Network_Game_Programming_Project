#include "Common.hlsl"

struct VS_SKYBOX_OUTPUT
{
    float4 position : SV_POSITION;
};

struct GS_SKYBOX_OUTPUT
{
    float4 position : SV_POSITION;
    float3 uvw : TEXCOORD0;
};

VS_SKYBOX_OUTPUT VSSkybox(uint nVertexID : SV_VertexID)
{
    VS_SKYBOX_OUTPUT output;
    
    output.position = float4(gCameraData.gvCameraPosition, 1.0f);
    
    return output;
}

[maxvertexcount(24)]
void GSSkybox(point VS_SKYBOX_OUTPUT input[1], inout TriangleStream<GS_SKYBOX_OUTPUT> outStream)
{
    // nVertexID 에 따라서...
    // nVertexID == 0 : 앞 (+z)
    // nVertexID == 1 : 뒤 (-z)
    // nVertexID == 2 : 왼쪽 (-x)
    // nVertexID == 3 : 오른쪽 (+x)
    // nVertexID == 4 : 위 (+y)
    // nVertexID == 5 : 아래 (-y)
    
    // 기본적으로 winding order 은 동일하게 가고, Rasterizer State 에서 뒷면이 보이도록 뒤집음
    // 다만 uv는 안에서 바라봤을때 기준으로 가야 맞을듯?
    
    float fSkyboxExtent = 10.f;
    float3 v3SkyboxTopPosition = input[0].position.xyz + (float3(0, 1, 0) * fSkyboxExtent);
    float3 v3SkyboxBottomPosition = input[0].position.xyz - (float3(0, 1, 0) * fSkyboxExtent);
    
    float3 rx = float3(1, 0, 0) * fSkyboxExtent;
    float3 rz = float3(0, 0, 1) * fSkyboxExtent;
    
    // top
    float3 t00 = v3SkyboxTopPosition - rx + rz;
    float3 t10 = v3SkyboxTopPosition + rx + rz;
    float3 t01 = v3SkyboxTopPosition - rx - rz;
    float3 t11 = v3SkyboxTopPosition + rx - rz;
    
    // bottom
    float3 b00 = v3SkyboxBottomPosition - rx + rz;
    float3 b10 = v3SkyboxBottomPosition + rx + rz;
    float3 b01 = v3SkyboxBottomPosition - rx - rz;
    float3 b11 = v3SkyboxBottomPosition + rx - rz;
    
    float4x4 VP = mul(gCameraData.gmtxView, gCameraData.gmtxProjection);
    GS_SKYBOX_OUTPUT output;
    
    // 앞 (+z)
    output.position = mul(float4(t10, 1.f), VP).xyww; output.uvw = float3(1, 0, 0);
    outStream.Append(output);
    output.position = mul(float4(t00, 1.f), VP).xyww; output.uvw = float3(0, 0, 0);
    outStream.Append(output);
    output.position = mul(float4(b10, 1.f), VP).xyww; output.uvw = float3(1, 1, 0);
    outStream.Append(output);
    output.position = mul(float4(b00, 1.f), VP).xyww; output.uvw = float3(0, 1, 0);
    outStream.Append(output);
    outStream.RestartStrip();
    
    // 뒤 (-z)
    output.position = mul(float4(t01, 1.f), VP).xyww; output.uvw = float3(1, 0, 1);
    outStream.Append(output);
    output.position = mul(float4(t11, 1.f), VP).xyww; output.uvw = float3(0, 0, 1);
    outStream.Append(output);
    output.position = mul(float4(b01, 1.f), VP).xyww; output.uvw = float3(1, 1, 1);
    outStream.Append(output);
    output.position = mul(float4(b11, 1.f), VP).xyww; output.uvw = float3(0, 1, 1);
    outStream.Append(output);
    outStream.RestartStrip();
    
    // 왼쪽 (-x)
    output.position = mul(float4(t00, 1.f), VP).xyww; output.uvw = float3(1, 0, 2);
    outStream.Append(output);
    output.position = mul(float4(t01, 1.f), VP).xyww; output.uvw = float3(0, 0, 2);
    outStream.Append(output);
    output.position = mul(float4(b00, 1.f), VP).xyww; output.uvw = float3(1, 1, 2);
    outStream.Append(output);
    output.position = mul(float4(b01, 1.f), VP).xyww; output.uvw = float3(0, 1, 2);
    outStream.Append(output);
    outStream.RestartStrip();
    
    // 오른쪽 (+x)
    output.position = mul(float4(t11, 1.f), VP).xyww; output.uvw = float3(1, 0, 3);
    outStream.Append(output);
    output.position = mul(float4(t10, 1.f), VP).xyww; output.uvw = float3(0, 0, 3);
    outStream.Append(output);
    output.position = mul(float4(b11, 1.f), VP).xyww; output.uvw = float3(1, 1, 3);
    outStream.Append(output);
    output.position = mul(float4(b10, 1.f), VP).xyww; output.uvw = float3(0, 1, 3);
    outStream.Append(output);
    outStream.RestartStrip();
    
    // 위 (+y)
    output.position = mul(float4(t00, 1.f), VP).xyww; output.uvw = float3(1, 0, 4);
    outStream.Append(output);
    output.position = mul(float4(t10, 1.f), VP).xyww; output.uvw = float3(1, 1, 4);
    outStream.Append(output);
    output.position = mul(float4(t01, 1.f), VP).xyww; output.uvw = float3(0, 0, 4);
    outStream.Append(output);
    output.position = mul(float4(t11, 1.f), VP).xyww; output.uvw = float3(0, 1, 4);
    outStream.Append(output);
    outStream.RestartStrip();
    
    // 아래 (-y)
    output.position = mul(float4(b00, 1.f), VP).xyww; output.uvw = float3(0, 0, 5);
    outStream.Append(output);
    output.position = mul(float4(b10, 1.f), VP).xyww; output.uvw = float3(1, 0, 5);
    outStream.Append(output);
    output.position = mul(float4(b01, 1.f), VP).xyww; output.uvw = float3(0, 1, 5);
    outStream.Append(output);
    output.position = mul(float4(b11, 1.f), VP).xyww; output.uvw = float3(1, 1, 5);
    outStream.Append(output);
    outStream.RestartStrip();
}

float4 PSSkybox(GS_SKYBOX_OUTPUT input) : SV_Target
{
    return gtxtSkyboxTextures.Sample(gSkyboxSamplerState, input.uvw);
}
