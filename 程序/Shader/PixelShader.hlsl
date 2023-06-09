#pragma target 3.0

// ��������ṹ��
struct PS_INPUT
{
    float4 position : SV_POSITION; // ����
    float2 texcoord : TEXCOORD0; // ��������
};

// ���������
Texture2D tex;
SamplerState samp;

// ������
float4 main(PS_INPUT input) : SV_Target
{
    float4 color = tex.Sample(samp, input.texcoord); // �������в�����ɫ

    return color; // ֱ�����������ɫ
}
