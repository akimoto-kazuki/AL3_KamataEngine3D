#pragma once
#include <d3d11.h>
#include <dxcapi.h>
#include <DirectXMath.h>
#include "KamataEngine.h"

#include "externals/DirectXTex/DirectXTex.h"

// libのリンク
#pragma comment(lib, "d3d12.lib")
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "dxguid.lib")
#pragma comment(lib, "dxcompiler.lib")

class Cursor3D 
{
public:

	void Initialize();

	~Cursor3D();

	void Update();

	void Draw();

private:

	HRESULT hr;

	ID3D12Resource* vertexResource;
	IDxcBlob* vertexShaderBlob;
	IDxcBlob* pixelShaderBlob;
	D3D12_ROOT_SIGNATURE_DESC descriptionRootSignature{};
	ID3D12PipelineState* graphicsPipelineState;
	D3D12_VERTEX_BUFFER_VIEW vertexBufferView{};
	ID3DBlob* signatureBlob = nullptr;
	ID3DBlob* errorBlob = nullptr;
	ID3D12RootSignature* rootSignature = nullptr;

	ID3D12GraphicsCommandList* commandList = nullptr;

	ID3D12Device* device_;
	D3D12_VIEWPORT viewport{};
	D3D12_RECT scissorRect{};

	ID3D12Resource* CreatBufferResource(ID3D12Device* device, size_t sizeInBytes);

	ID3D12Resource* CreateDepthStencilTextureResource(ID3D12Device* device, int32_t width, int32_t height);

	void UploadTextureData(ID3D12Resource* texture, const DirectX::ScratchImage& mipImages);

};
