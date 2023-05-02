
// 定义输入结构体
struct VS_INPUT
{
    float3 position : POSITION; // 顶点位置，用 float3 类型表示
    float4 color : COLOR; // 顶点颜色，用 float4 类型表示
};

// 定义输出结构体
struct VS_OUTPUT
{
    float4 position : SV_POSITION; // 输出裁剪空间坐标
    float4 color : COLOR0; // 输出顶点颜色
};

// 主函数
VS_OUTPUT main(VS_INPUT input)
{
    VS_OUTPUT output;

    // 将输入顶点位置转换为齐次坐标，并输出到裁剪空间
    output.position = float4(input.position, 1.0f);

    // 将输入顶点颜色直接输出
    output.color = input.color;

    return output;
}
