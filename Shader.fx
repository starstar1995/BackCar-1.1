//坐标变换矩阵的常量缓存
cbuffer MatrixBuffer
{
	matrix World;             //世界变换矩阵
	matrix View;              //观察变换矩阵
	matrix Projection;        //投影变换矩阵
};

Texture2D Texture;  //纹理变量

SamplerState Sampler  //定义采样器
{
	Filter = MIN_MAG_MIP_LINEAR;  //采用线性过滤
	AddressU = WRAP;  //寻址模式为Wrap
	AddressV = WRAP;  //寻址模式为Wrap
};

//顶点着色器的输入结构
struct VS_INPUT
{
	float4 Pos:POSITION;  //顶点坐标
	float2 Tex:TEXCOORD;  //纹理坐标
};

//顶点着色器的输出结构
struct VS_OUTPUT
{
	float4 Pos:SV_POSITION;  //位置
	float2 Tex:TEXCOORD;  //纹理坐标
};

//顶点着色器
VS_OUTPUT VS(VS_INPUT input)
{
	VS_OUTPUT output = (VS_OUTPUT)0;
	output.Pos = mul(input.Pos, World);           //世界坐标变换，mul为矩阵乘法的函数
	output.Pos = mul(output.Pos, View);       //观察坐标变换，mul第一个参数为向量
	output.Pos = mul(output.Pos, Projection);   //投影坐标变换

	output.Tex = input.Tex;  //纹理设置
	return output;

}
//像素着色器的输入结构
float4 PS(VS_OUTPUT input) :SV_Target
{
	return Texture.Sample(Sampler, input.Tex);  //返回纹理
}

//定义Technique
//////////////////////////////////////////////////////////////////////////
// Technique将不同着色器整合起来实现某种功能
// 定义Technique由technique11关键字进行
//////////////////////////////////////////////////////////////////////////
technique11 TexTech
{
	//Technique通过Pass来应用不同的效果，每个Technique可以有多个Pass
	//本例中只有一个Pass
	pass P0
	{
		//设置顶点着色器
		//CompileShader包含两个版本，一个是目标着色器，这里使用的是SM5.0
		//另一个参数为我们自己顶一个着色器函数
		SetVertexShader(CompileShader(vs_5_0, VS()));
		SetGeometryShader(NULL);
		SetPixelShader(CompileShader(ps_5_0, PS())); //设置像素着色器
	}
}
