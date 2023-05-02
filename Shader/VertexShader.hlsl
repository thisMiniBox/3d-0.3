
// ��������ṹ��
struct VS_INPUT
{
    float3 position : POSITION; // ����λ�ã��� float3 ���ͱ�ʾ
    float4 color : COLOR; // ������ɫ���� float4 ���ͱ�ʾ
};

// ��������ṹ��
struct VS_OUTPUT
{
    float4 position : SV_POSITION; // ����ü��ռ�����
    float4 color : COLOR0; // ���������ɫ
};

// ������
VS_OUTPUT main(VS_INPUT input)
{
    VS_OUTPUT output;

    // �����붥��λ��ת��Ϊ������꣬��������ü��ռ�
    output.position = float4(input.position, 1.0f);

    // �����붥����ɫֱ�����
    output.color = input.color;

    return output;
}
