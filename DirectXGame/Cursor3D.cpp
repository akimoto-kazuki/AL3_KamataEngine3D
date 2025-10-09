#include "Cursor3D.h"
#include <d3dcompiler.h>
using namespace DirectX;

struct Vertex {
	XMFLOAT3 pos;
	XMFLOAT4 color;
};

void Log(const std::string& message) {
	// os << message << std::endl;
	OutputDebugStringA(message.c_str());
}

std::wstring ConvertString(const std::string& str) {
	if (str.empty()) {
		return std::wstring();
	}
	auto sizeNeeded = MultiByteToWideChar(CP_UTF8, 0, reinterpret_cast<const char*>(&str[0]), static_cast<int>(str.size()), NULL, 0);
	if (sizeNeeded == 0) {
		return std::wstring();
	}
	std::wstring result(sizeNeeded, 0);
	MultiByteToWideChar(CP_UTF8, 0, reinterpret_cast<const char*>(&str[0]), static_cast<int>(str.size()), &result[0], sizeNeeded);
	return result;
}
std::string ConvertString(const std::wstring& str) {
	if (str.empty()) {
		return std::string();
	}
	auto sizeNeeded = WideCharToMultiByte(CP_UTF8, 0, str.data(), static_cast<int>(str.size()), NULL, 0, NULL, NULL);
	if (sizeNeeded == 0) {
		return std::string();
	}
	std::string result(sizeNeeded, 0);
	WideCharToMultiByte(CP_UTF8, 0, str.data(), static_cast<int>(str.size()), result.data(), sizeNeeded, NULL, NULL);
	return result;
}

IDxcBlob* CompieShadaer(
    // CompilerするShaderファイルへのパス
    const std::wstring& filePath,
    // Compilerに使用するProfile
    const wchar_t* profile,
    // 初期化で生成したものを３つ
    IDxcUtils* dxcUtils, IDxcCompiler3* dxcCompiler, IDxcIncludeHandler* includeHandler) {
	// 1. hlslファイルを読む
	// これからシェーダーをコンパイルする旨をログに出す
	Log(ConvertString(std::format(L"BeGin CompileShader,path:{},proile:{}\n", filePath, profile)));
	// hlslファイルを読む
	IDxcBlobEncoding* shaderSource = nullptr;
	HRESULT hr = dxcUtils->LoadFile(filePath.c_str(), nullptr, &shaderSource);
	// 読めなかったら止める
	assert(SUCCEEDED(hr));
	// 読み込んだファイルの内容を説明する
	DxcBuffer shaderSourceBuffer;
	shaderSourceBuffer.Ptr = shaderSource->GetBufferPointer();
	shaderSourceBuffer.Size = shaderSource->GetBufferSize();
	shaderSourceBuffer.Encoding = DXC_CP_UTF8; // UTF8の文字コードであることを通知
	// 2. Cimpileする
	LPCWSTR arguments[] = {
	    filePath.c_str(),
	    L"-E",
	    L"main", // 　コンパイル対象のhlslファイル名
	    L"-T",
	    profile, // 　エントリーポイントの指定。基本的にmain以外にはしない
	    L"-Zi",
	    L"-Qembed_debug", // 　ShaderProfilenに設定
	    L"-Od",           // 　最適化を外しておく
	    L"-Zpr",          // 　メモリレイアウトは行優先
	};
	// 実際にShaderをコンパイルする
	IDxcResult* shaderResult = nullptr;
	hr = dxcCompiler->Compile(
	    &shaderSourceBuffer,        // 読み込んだファイル
	    arguments,                  // コンパイルオプション
	    _countof(arguments),        // コンパイルオプションの数
	    includeHandler,             // inccludeが含まれた
	    IID_PPV_ARGS(&shaderResult) // コンパイル結果
	);
	// コンパイルエラーではなくdxcが起動出来ないなと致命的な状況
	assert(SUCCEEDED(hr));
	// 3. 警告，エラーが出ていないか確認する
	// 警告，エラーが出てきたらログに出して止める
	IDxcBlobUtf8* shaderError = nullptr;
	shaderResult->GetOutput(DXC_OUT_ERRORS, IID_PPV_ARGS(&shaderError), nullptr);
	if (shaderError != nullptr && shaderError->GetStringLength() != 0) {
		Log(shaderError->GetStringPointer());
		// 警告、エラーだめ絶対
		assert(false);
	}
	// 4. Compile結果を受け取って返す
	// コンパイル結果から実行用のバイナリ部分を取得
	IDxcBlob* shaderBlob = nullptr;
	hr = shaderResult->GetOutput(DXC_OUT_OBJECT, IID_PPV_ARGS(&shaderBlob), nullptr);
	assert(SUCCEEDED(hr));
	// 成功したログを出す
	Log(ConvertString(std::format(L"Compile Succeeded, path:{}, profile:{}\n", filePath, profile)));
	// もう使わないリソースを解放
	shaderSource->Release();
	shaderResult->Release();
	// 実行用のバイナリを返却
	return shaderBlob;
	// struct VertexShaderInput
	//{
	//	float32_t4 position : POSITION0;
	// };
}

void Cursor3D::Initialize()
{
	D3D12_ROOT_SIGNATURE_DESC descriptionRootSignature{};
	descriptionRootSignature.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;
	// シリアライズしてバイナリにする
	ID3DBlob* signatureBlob = nullptr;
	ID3DBlob* errorBlob = nullptr;
	hr = D3D12SerializeRootSignature(&descriptionRootSignature, D3D_ROOT_SIGNATURE_VERSION_1, &signatureBlob, &errorBlob);
	if (FAILED(hr)) {
		Log(reinterpret_cast<char*>(errorBlob->GetBufferPointer()));
		assert(false);
	}
	// バイナリを元に生成
	ID3D12RootSignature* rootSignature = nullptr;
	hr = device->CreateRootSignature(0, signatureBlob->GetBufferPointer(), signatureBlob->GetBufferSize(), IID_PPV_ARGS(&rootSignature));
	assert(SUCCEEDED(hr));
}

Cursor3D::Cursor3D(ID3D12Device* device) {
	InitShader(device); 
}

Cursor3D::~Cursor3D() 
{
	if (m_vertexShader)
		m_vertexShader->Release();
	if (m_pixelShader)
		m_pixelShader->Release();
	if (m_inputLayout)
		m_inputLayout->Release();
}

void Cursor3D::InitShader(ID3D12Device* device)
{
	const char* vsCode = "struct VS_IN { float3 pos : POSITION; float4 color : COLOR; };"
	                     "struct PS_IN { float4 pos : SV_POSITION; float4 color : COLOR; };"
	                     "PS_IN main(VS_IN i){ PS_IN o; o.pos=float4(i.pos,1); o.color=i.color; return o; }";

	const char* psCode = "float4 main(float4 pos : SV_POSITION, float4 color : COLOR) : SV_TARGET { return color; }";

	ID3DBlob* vsBlob = nullptr;
	ID3DBlob* psBlob = nullptr;

	D3DCompile(vsCode, strlen(vsCode), nullptr, nullptr, nullptr, "main", "vs_5_0", 0, 0, &vsBlob, nullptr);
	D3DCompile(psCode, strlen(psCode), nullptr, nullptr, nullptr, "main", "ps_5_0", 0, 0, &psBlob, nullptr);

	device.(vsBlob->GetBufferPointer(), vsBlob->GetBufferSize(), nullptr, &m_vertexShader);
	device->CreatePixelShader(psBlob->GetBufferPointer(), psBlob->GetBufferSize(), nullptr, &m_pixelShader);

	D3D11_INPUT_ELEMENT_DESC layout[] = {
	    {"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT,    0, 0,  D3D11_INPUT_PER_VERTEX_DATA, 0},
	    {"COLOR",    0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0},
	};
	device->InputLayout(layout, 2, vsBlob->GetBufferPointer(), vsBlob->GetBufferSize(), &m_inputLayout);

	vsBlob->Release();
	psBlob->Release();
}

void Cursor3D::Draw(ID3D11DeviceContext* context, int screenWidth, int screenHeight, int mouseX, int mouseY)
{
	float x = (mouseX / (float)screenWidth * 2.0f - 1.0f);
	float y = -((mouseY / (float)screenHeight) * 2.0f - 1.0f);

	Vertex vertices[] = {
	    {XMFLOAT3(x - 0.02f, y,         0.0f), XMFLOAT4(1, 1, 0, 1)},
	    {XMFLOAT3(x + 0.02f, y,         0.0f), XMFLOAT4(1, 1, 0, 1)},
	    {XMFLOAT3(x,         y - 0.03f, 0.0f), XMFLOAT4(1, 1, 0, 1)},
	    {XMFLOAT3(x,         y + 0.03f, 0.0f), XMFLOAT4(1, 1, 0, 1)},
	};

	D3D11_BUFFER_DESC bd = {};
	bd.Usage = D3D11_USAGE_DYNAMIC;
	bd.ByteWidth = sizeof(vertices);
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	D3D11_SUBRESOURCE_DATA initData = {vertices, 0, 0};
	ID3D11Buffer* vb = nullptr;

	ID3D11Device* device = nullptr;
	context->GetDevice(&device);
	device->CreateBuffer(&bd, &initData, &vb);
	device->Release();

	UINT stride = sizeof(Vertex);
	UINT offset = 0;

	context->IASetInputLayout(m_inputLayout);
	context->IASetVertexBuffers(0, 1, &vb, &stride, &offset);
	context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINELIST);	

	context->VSSetShader(m_vertexShader, nullptr, 0);
	context->PSSetShader(m_pixelShader, nullptr, 0);

	context->Draw(4, 0);

	vb->Release();
}