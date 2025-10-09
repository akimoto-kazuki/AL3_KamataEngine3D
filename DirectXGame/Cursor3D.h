#pragma once
#include <d3d11.h>
#include <dxcapi.h>
#include <DirectXMath.h>
#include "KamataEngine.h"

#pragma comment(lib, "dxcompiler.lib")


class Cursor3D 
{
public:

	void Initialize();

	Cursor3D(ID3D12Device* device);
	~Cursor3D();

	void Draw(ID3D11DeviceContext* context, int screenWidth, int screenHeight, int mouseX, int mouseY);

private:
	void InitShader(ID3D12Device* device);

	ID3D11VertexShader* m_vertexShader = nullptr;
	ID3D11PixelShader* m_pixelShader = nullptr;
	ID3D11InputLayout* m_inputLayout = nullptr;

	D3D12_ROOT_SIGNATURE_DESC descriptionRootSignature{};
	ID3DBlob* signatureBlob = nullptr;
	ID3DBlob* errorBlob = nullptr;
	ID3D12RootSignature* rootSignature = nullptr;

	ID3D12Device* device;

};
