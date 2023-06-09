#pragma target 3.0

// 定义输入结构体
struct PS_INPUT
{
    float4 position : SV_POSITION; // 坐标
    float2 texcoord : TEXCOORD0; // 纹理坐标
};

// 纹理采样器
Texture2D tex;
SamplerState samp;

// 主函数
float4 main(PS_INPUT input) : SV_Target
{
    float4 color = tex.Sample(samp, input.texcoord); // 从纹理中采样颜色

    return color; // 直接输出纹理颜色
}
