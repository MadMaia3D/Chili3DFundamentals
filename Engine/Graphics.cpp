﻿/******************************************************************************************
*	Chili DirectX Framework Version 16.10.01											  *
*	Graphics.cpp																		  *
*	Copyright 2016 PlanetChili.net <http://www.planetchili.net>							  *
*																						  *
*	This file is part of The Chili DirectX Framework.									  *
*																						  *
*	The Chili DirectX Framework is free software: you can redistribute it and/or modify	  *
*	it under the terms of the GNU General Public License as published by				  *
*	the Free Software Foundation, either version 3 of the License, or					  *
*	(at your option) any later version.													  *
*																						  *
*	The Chili DirectX Framework is distributed in the hope that it will be useful,		  *
*	but WITHOUT ANY WARRANTY; without even the implied warranty of						  *
*	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the						  *
*	GNU General Public License for more details.										  *
*																						  *
*	You should have received a copy of the GNU General Public License					  *
*	along with The Chili DirectX Framework.  If not, see <http://www.gnu.org/licenses/>.  *
******************************************************************************************/
#include "MainWindow.h"
#include "Graphics.h"
#include "DXErr.h"
#include "ChiliException.h"
#include <assert.h>
#include <string>
#include <array>
#include <functional>
#include <Windows.h>

// Ignore the intellisense error "cannot open source file" for .shh files.
// They will be created during the build sequence before the preprocessor runs.
namespace FramebufferShaders {
#include "FramebufferPS.shh"
#include "FramebufferVS.shh"
}

#pragma comment( lib,"d3d11.lib" )

using Microsoft::WRL::ComPtr;

Graphics::Graphics(HWNDKey& key)
	:
	sysBuffer(ScreenWidth, ScreenHeight) {
	assert(key.hWnd != nullptr);

	//////////////////////////////////////////////////////
	// create device and swap chain/get render target view
	DXGI_SWAP_CHAIN_DESC sd = {};
	sd.BufferCount = 1;
	sd.BufferDesc.Width = Graphics::ScreenWidth;
	sd.BufferDesc.Height = Graphics::ScreenHeight;
	sd.BufferDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
	sd.BufferDesc.RefreshRate.Numerator = 1;
	sd.BufferDesc.RefreshRate.Denominator = 60;
	sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	sd.OutputWindow = key.hWnd;
	sd.SampleDesc.Count = 1;
	sd.SampleDesc.Quality = 0;
	sd.Windowed = TRUE;

	D3D_FEATURE_LEVEL	featureLevelsRequested = D3D_FEATURE_LEVEL_9_1;
	UINT				numLevelsRequested = 1;
	D3D_FEATURE_LEVEL	featureLevelsSupported;
	HRESULT				hr;
	UINT				createFlags = 0u;
#ifdef _DEBUG
#ifdef USE_DIRECT3D_DEBUG_RUNTIME
	createFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif
#endif

	// create device and front/back buffers
	if (FAILED(hr = D3D11CreateDeviceAndSwapChain(
		nullptr,
		D3D_DRIVER_TYPE_HARDWARE,
		nullptr,
		createFlags,
		&featureLevelsRequested,
		numLevelsRequested,
		D3D11_SDK_VERSION,
		&sd,
		&pSwapChain,
		&pDevice,
		&featureLevelsSupported,
		&pImmediateContext))) {
		throw CHILI_GFX_EXCEPTION(hr, L"Creating device and swap chain");
	}

	// get handle to backbuffer
	ComPtr<ID3D11Resource> pBackBuffer;
	if (FAILED(hr = pSwapChain->GetBuffer(
		0,
		__uuidof(ID3D11Texture2D),
		(LPVOID*)&pBackBuffer))) {
		throw CHILI_GFX_EXCEPTION(hr, L"Getting back buffer");
	}

	// create a view on backbuffer that we can render to
	if (FAILED(hr = pDevice->CreateRenderTargetView(
		pBackBuffer.Get(),
		nullptr,
		&pRenderTargetView))) {
		throw CHILI_GFX_EXCEPTION(hr, L"Creating render target view on backbuffer");
	}


	// set backbuffer as the render target using created view
	pImmediateContext->OMSetRenderTargets(1, pRenderTargetView.GetAddressOf(), nullptr);


	// set viewport dimensions
	D3D11_VIEWPORT vp;
	vp.Width = float(Graphics::ScreenWidth);
	vp.Height = float(Graphics::ScreenHeight);
	vp.MinDepth = 0.0f;
	vp.MaxDepth = 1.0f;
	vp.TopLeftX = 0.0f;
	vp.TopLeftY = 0.0f;
	pImmediateContext->RSSetViewports(1, &vp);


	///////////////////////////////////////
	// create texture for cpu render target
	D3D11_TEXTURE2D_DESC sysTexDesc;
	sysTexDesc.Width = Graphics::ScreenWidth;
	sysTexDesc.Height = Graphics::ScreenHeight;
	sysTexDesc.MipLevels = 1;
	sysTexDesc.ArraySize = 1;
	sysTexDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
	sysTexDesc.SampleDesc.Count = 1;
	sysTexDesc.SampleDesc.Quality = 0;
	sysTexDesc.Usage = D3D11_USAGE_DYNAMIC;
	sysTexDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	sysTexDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	sysTexDesc.MiscFlags = 0;
	// create the texture
	if (FAILED(hr = pDevice->CreateTexture2D(&sysTexDesc, nullptr, &pSysBufferTexture))) {
		throw CHILI_GFX_EXCEPTION(hr, L"Creating sysbuffer texture");
	}

	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
	srvDesc.Format = sysTexDesc.Format;
	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MipLevels = 1;
	// create the resource view on the texture
	if (FAILED(hr = pDevice->CreateShaderResourceView(pSysBufferTexture.Get(),
													  &srvDesc, &pSysBufferTextureView))) {
		throw CHILI_GFX_EXCEPTION(hr, L"Creating view on sysBuffer texture");
	}


	////////////////////////////////////////////////
	// create pixel shader for framebuffer
	// Ignore the intellisense error "namespace has no member"
	if (FAILED(hr = pDevice->CreatePixelShader(
		FramebufferShaders::FramebufferPSBytecode,
		sizeof(FramebufferShaders::FramebufferPSBytecode),
		nullptr,
		&pPixelShader))) {
		throw CHILI_GFX_EXCEPTION(hr, L"Creating pixel shader");
	}


	/////////////////////////////////////////////////
	// create vertex shader for framebuffer
	// Ignore the intellisense error "namespace has no member"
	if (FAILED(hr = pDevice->CreateVertexShader(
		FramebufferShaders::FramebufferVSBytecode,
		sizeof(FramebufferShaders::FramebufferVSBytecode),
		nullptr,
		&pVertexShader))) {
		throw CHILI_GFX_EXCEPTION(hr, L"Creating vertex shader");
	}


	//////////////////////////////////////////////////////////////
	// create and fill vertex buffer with quad for rendering frame
	const FSQVertex vertices[] =
	{
		{ -1.0f,1.0f,0.5f,0.0f,0.0f },
		{ 1.0f,1.0f,0.5f,1.0f,0.0f },
		{ 1.0f,-1.0f,0.5f,1.0f,1.0f },
		{ -1.0f,1.0f,0.5f,0.0f,0.0f },
		{ 1.0f,-1.0f,0.5f,1.0f,1.0f },
		{ -1.0f,-1.0f,0.5f,0.0f,1.0f },
	};
	D3D11_BUFFER_DESC bd = {};
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(FSQVertex) * 6;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = 0u;
	D3D11_SUBRESOURCE_DATA initData = {};
	initData.pSysMem = vertices;
	if (FAILED(hr = pDevice->CreateBuffer(&bd, &initData, &pVertexBuffer))) {
		throw CHILI_GFX_EXCEPTION(hr, L"Creating vertex buffer");
	}


	//////////////////////////////////////////
	// create input layout for fullscreen quad
	const D3D11_INPUT_ELEMENT_DESC ied[] =
	{
		{ "POSITION",0,DXGI_FORMAT_R32G32B32_FLOAT,0,0,D3D11_INPUT_PER_VERTEX_DATA,0 },
		{ "TEXCOORD",0,DXGI_FORMAT_R32G32_FLOAT,0,12,D3D11_INPUT_PER_VERTEX_DATA,0 }
	};

	// Ignore the intellisense error "namespace has no member"
	if (FAILED(hr = pDevice->CreateInputLayout(ied, 2,
											   FramebufferShaders::FramebufferVSBytecode,
											   sizeof(FramebufferShaders::FramebufferVSBytecode),
											   &pInputLayout))) {
		throw CHILI_GFX_EXCEPTION(hr, L"Creating input layout");
	}


	////////////////////////////////////////////////////
	// Create sampler state for fullscreen textured quad
	D3D11_SAMPLER_DESC sampDesc = {};
	sampDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_POINT;
	sampDesc.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
	sampDesc.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
	sampDesc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
	sampDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
	sampDesc.MinLOD = 0;
	sampDesc.MaxLOD = D3D11_FLOAT32_MAX;
	if (FAILED(hr = pDevice->CreateSamplerState(&sampDesc, &pSamplerState))) {
		throw CHILI_GFX_EXCEPTION(hr, L"Creating sampler state");
	}
}

Graphics::~Graphics() {
	// clear the state of the device context before destruction
	if (pImmediateContext) pImmediateContext->ClearState();
}

void Graphics::EndFrame() {
	HRESULT hr;

	// lock and map the adapter memory for copying over the sysbuffer
	if (FAILED(hr = pImmediateContext->Map(pSysBufferTexture.Get(), 0u,
										   D3D11_MAP_WRITE_DISCARD, 0u, &mappedSysBufferTexture))) {
		throw CHILI_GFX_EXCEPTION(hr, L"Mapping sysbuffer");
	}
	// perform the copy line-by-line
	sysBuffer.Present(mappedSysBufferTexture.RowPitch,
					  reinterpret_cast<BYTE*>(mappedSysBufferTexture.pData));
	// release the adapter memory
	pImmediateContext->Unmap(pSysBufferTexture.Get(), 0u);

	// render offscreen scene texture to back buffer
	pImmediateContext->IASetInputLayout(pInputLayout.Get());
	pImmediateContext->VSSetShader(pVertexShader.Get(), nullptr, 0u);
	pImmediateContext->PSSetShader(pPixelShader.Get(), nullptr, 0u);
	pImmediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	const UINT stride = sizeof(FSQVertex);
	const UINT offset = 0u;
	pImmediateContext->IASetVertexBuffers(0u, 1u, pVertexBuffer.GetAddressOf(), &stride, &offset);
	pImmediateContext->PSSetShaderResources(0u, 1u, pSysBufferTextureView.GetAddressOf());
	pImmediateContext->PSSetSamplers(0u, 1u, pSamplerState.GetAddressOf());
	pImmediateContext->Draw(6u, 0u);

	// flip back/front buffers
	if (FAILED(hr = pSwapChain->Present(1u, 0u))) {
		throw CHILI_GFX_EXCEPTION(hr, L"Presenting back buffer");
	}
}

void Graphics::BeginFrame() {
	sysBuffer.Clear(Colors::Red);
}

void Graphics::DrawLine(const Vec2 & p1, const Vec2 & p2, Color c) {
	DrawLine(p1.x, p1.y, p2.x, p2.y, c);
}

void Graphics::DrawLine(const Vec3 & p1, const Vec3 & p2, Color c) {
	DrawLine(p1.x, p1.y, p2.x, p2.y, c);
}


//////////////////////////////////////////////////
//           Graphics Exception
Graphics::Exception::Exception(HRESULT hr, const std::wstring& note, const wchar_t* file, unsigned int line)
	:
	ChiliException(file, line, note),
	hr(hr) {
}

std::wstring Graphics::Exception::GetFullMessage() const {
	const std::wstring empty = L"";
	const std::wstring errorName = GetErrorName();
	const std::wstring errorDesc = GetErrorDescription();
	const std::wstring& note = GetNote();
	const std::wstring location = GetLocation();
	return    (!errorName.empty() ? std::wstring(L"Error: ") + errorName + L"\n"
			   : empty)
		+ (!errorDesc.empty() ? std::wstring(L"Description: ") + errorDesc + L"\n"
		   : empty)
		+ (!note.empty() ? std::wstring(L"Note: ") + note + L"\n"
		   : empty)
		+ (!location.empty() ? std::wstring(L"Location: ") + location
		   : empty);
}

std::wstring Graphics::Exception::GetErrorName() const {
	return DXGetErrorString(hr);
}

std::wstring Graphics::Exception::GetErrorDescription() const {
	std::array<wchar_t, 512> wideDescription;
	DXGetErrorDescription(hr, wideDescription.data(), wideDescription.size());
	return wideDescription.data();
}

std::wstring Graphics::Exception::GetExceptionType() const {
	return L"Chili Graphics Exception";
}

void Graphics::DrawLine(float x1, float y1, float x2, float y2, Color c) {
	const float dx = x2 - x1;
	const float dy = y2 - y1;

	if (dy == 0.0f && dx == 0.0f) {
		PutPixel(int(x1), int(y1), c);
	} else if (abs(dy) > abs(dx)) {
		if (dy < 0.0f) {
			std::swap(x1, x2);
			std::swap(y1, y2);
		}

		const float m = dx / dy;
		float y = y1;
		int lastIntY;
		for (float x = x1; y < y2; y += 1.0f, x += m) {
			lastIntY = int(y);
			PutPixel(int(x), lastIntY, c);
		}
		if (int(y2) > lastIntY) {
			PutPixel(int(x2), int(y2), c);
		}
	} else {
		if (dx < 0.0f) {
			std::swap(x1, x2);
			std::swap(y1, y2);
		}

		const float m = dy / dx;
		float x = x1;
		int lastIntX;
		for (float y = y1; x < x2; x += 1.0f, y += m) {
			lastIntX = int(x);
			PutPixel(lastIntX, int(y), c);
		}
		if (int(x2) > lastIntX) {
			PutPixel(int(x2), int(y2), c);
		}
	}
}


void Graphics::DrawTriangle(const Vec2 & v0, const Vec2 & v1, const Vec2 & v2, Color c) {
	const Vec2* pv0 = &v0;
	const Vec2* pv1 = &v1;
	const Vec2* pv2 = &v2;

	if (pv1->y < pv0->y) { std::swap(pv0, pv1); }
	if (pv2->y < pv1->y) { std::swap(pv1, pv2); }
	if (pv1->y < pv0->y) { std::swap(pv0, pv1); }

	// for each case, points should be ordered in clockwise direction, starting from the most top-left point
	if (pv0->y == pv1->y) {									// render flat top triangle
		if (pv1->x < pv0->x) { std::swap(pv0, pv1); }
		DrawFlatTopTriangle(*pv0, *pv1, *pv2, c);
	} else if (pv1->y == pv2->y) {							// render flat bottom triangle
		if (pv1->x < pv2->x) { std::swap(pv1, pv2); }
		DrawFlatBottomTriangle(*pv0, *pv1, *pv2, c);
	} else {												// render separated triangles
		// find alpha for linear interpolation (find the percentage of middle y between bottom y and top y)
		const float alpha = (pv1->y - pv0->y) / (pv2->y - pv0->y);
		// split vertex is the linear interpolation between v0 and v1
		const Vec2 sv = *pv0 + (*pv2 - *pv0) * alpha;
		if (sv.x < pv1->x) {
			DrawFlatBottomTriangle(*pv0, *pv1, sv, c);
			DrawFlatTopTriangle(sv, *pv1, *pv2, c);
		} else {
			DrawFlatBottomTriangle(*pv0, sv, *pv1, c);
			DrawFlatTopTriangle(*pv1, sv, *pv2, c);
		}
	}
}

// mind the order of vertices: clockwise from the top
void Graphics::DrawFlatBottomTriangle(const Vec2 & v0, const Vec2 & v1, const Vec2 & v2, Color c) {
	// use linear equation (y = mx +b) but the version of x in function of y (x = wy + p),
	// where w is the run over the rise (inverse of m) and p is the x-intercept (the original b is the y-intercept)
	// "y in function of x" would cause problems with vertical lines since 'm' in that case would result in division by zero
	// so we use "x in function of x" version cause the lines that we won't need to calculate the bottom horizontal edge of the triangle

	// run over rise for left and right sides
	const float lw = (v2.x - v0.x) / (v2.y - v0.y);
	const float rw = (v1.x - v0.x) / (v1.y - v0.y);

	// following Microsoft DirectX10 rasterization "top-edge" rule
	const int startY = (int)std::ceil(v0.y - 0.5f);
	const int endY = (int)std::ceil(v2.y - 0.5f);

	for (int y = startY; y < endY; y++) {
		// add 0.5 to y value because we are making calculations based on the pixel center
		const float leftX = lw * (y + 0.5f - v0.y) + v0.x;
		const float rightX = rw * (y + 0.5f - v0.y) + v0.x;
		// following Microsoft DirectX10 rasterization "left-edge" rule
		const int startX = (int)std::ceil(leftX + 0.5f);
		const int endX = (int)std::ceil(rightX + 0.5f);
		for (int x = startX; x < endX; x++) {
			PutPixel(x, y, c);
		}
	}
}
// mind the order of vertices: clockwise from the top left
void Graphics::DrawFlatTopTriangle(const Vec2 & v0, const Vec2 & v1, const Vec2 & v2, Color c) {
	// use linear equation (y = mx +b) but the version of x in function of y (x = wy + p),
	// where w is the run over the rise (inverse of m) and p is the x-intercept (the original b is the y-intercept)
	// "y in function of x" would cause problems with vertical lines since 'm' in that case would result in division by zero
	// so we use "x in function of x" version cause the lines that we won't need to calculate the top horizontal edge of the triangle

	const float lw = (v2.x - v0.x) / (v2.y - v0.y);
	const float rw = (v2.x - v1.x) / (v2.y - v1.y);

	// following Microsoft DirectX10 rasterization "top-edge" rule
	const int startY = (int)std::ceil(v0.y - 0.5f);
	const int endY = (int)std::ceil(v2.y - 0.5f);

	for (int y = startY; y < endY; y++) {
		// add 0.5 to y value because we are making calculations based on the pixel center
		const float leftX = lw * (y + 0.5f - v0.y) + v0.x;
		const float rightX = rw * (y + 0.5f - v1.y) + v1.x;
		// following Microsoft DirectX10 rasterization "left-edge" rule
		const int startX = (int)std::ceil(leftX + 0.5f);
		const int endX = (int)std::ceil(rightX + 0.5f);
		for (int x = startX; x < endX; x++) {
			PutPixel(x, y, c);
		}
	}
}

void Graphics::DrawTriangle(const TexVertex & v0, const TexVertex & v1, const TexVertex & v2, const Surface& surf) {
	const TexVertex* pv0 = &v0;
	const TexVertex* pv1 = &v1;
	const TexVertex* pv2 = &v2;

	if (pv1->pos.y < pv0->pos.y) { std::swap(pv0, pv1); }
	if (pv2->pos.y < pv1->pos.y) { std::swap(pv1, pv2); }
	if (pv1->pos.y < pv0->pos.y) { std::swap(pv0, pv1); }

	// for each case, points should be ordered in clockwise direction, starting from the most top-left point
	if (pv0->pos.y == pv1->pos.y) {									// render flat top triangle
		if (pv1->pos.x < pv0->pos.x) { std::swap(pv0, pv1); }
		DrawFlatTopTriangle(*pv0, *pv1, *pv2, surf);
	} else if (pv1->pos.y == pv2->pos.y) {							// render flat bottom triangle
		if (pv1->pos.x < pv2->pos.x) { std::swap(pv1, pv2); }
		DrawFlatBottomTriangle(*pv0, *pv1, *pv2, surf);
	} else {												// render separated triangles
		// find alpha for linear interpolation (find the percentage of middle y between bottom y and top y)
		const float alpha = (pv1->pos.y - pv0->pos.y) / (pv2->pos.y - pv0->pos.y);
		// split vertex is the linear interpolation between v0 and v1
		const TexVertex sv = pv0->InterpolateTo(*pv2, alpha);

		if (sv.pos.x < pv1->pos.x) {
			DrawFlatBottomTriangle(*pv0, *pv1, sv, surf);
			DrawFlatTopTriangle(sv, *pv1, *pv2, surf);
		} else {
			DrawFlatBottomTriangle(*pv0, sv, *pv1, surf);
			DrawFlatTopTriangle(*pv1, sv, *pv2, surf);
		}
	}
}

void Graphics::DrawFlatBottomTriangle(const TexVertex & v0, const TexVertex & v1, const TexVertex & v2, const Surface& surf) {
	// use linear equation (y = mx +b) but the version of x in function of y (x = wy + p),
	// where w is the run over the rise (inverse of m) and p is the x-intercept (the original b is the y-intercept)
	// "y in function of x" would cause problems with vertical lines since 'm' in that case would result in division by zero
	// so we use "x in function of x" version cause the lines that we won't need to calculate the bottom horizontal edge of the triangle

	// run over rise for left and right sides
	const float lw = (v2.pos.x - v0.pos.x) / (v2.pos.y - v0.pos.y);
	const float rw = (v1.pos.x - v0.pos.x) / (v1.pos.y - v0.pos.y);

	// following Microsoft DirectX10 rasterization "top-edge" rule
	const int yStart = (int)std::ceil(v0.pos.y - 0.5f);
	const int yEnd = (int)std::ceil(v2.pos.y - 0.5f);

	// init text coord edges
	Vec2 tcEdgeL = v0.tc;
	Vec2 tcEdgeR = v0.tc;

	// calculate tex coord edge unit steps
	const Vec2 tcEdgeStepL = (v2.tc - v0.tc) / (v2.pos.y - v0.pos.y);
	const Vec2 tcEdgeStepR = (v1.tc - v0.tc) / (v1.pos.y - v0.pos.y);

	// do tex coord pre step
	tcEdgeL += tcEdgeStepL * ((float)yStart - v0.pos.y + 0.5f);
	tcEdgeR += tcEdgeStepR * ((float)yStart - v0.pos.y + 0.5f);

	// get texture info
	const int surfaceWidth = surf.GetWidth();
	const int surfaceHeight = surf.GetHeight();
	const int sufaceMaxWidth = surfaceWidth - 1;
	const int surfaceMaxHeight = surfaceHeight - 1;

	for (int y = yStart; y < yEnd; y++,
		 tcEdgeL += tcEdgeStepL, tcEdgeR += tcEdgeStepR) {
		// add 0.5 to y value because we are making calculations based on the pixel center
		const float px0 = lw * (y + 0.5f - v0.pos.y) + v0.pos.x;
		const float px1 = rw * (y + 0.5f - v0.pos.y) + v0.pos.x;
		// following Microsoft DirectX10 rasterization "left-edge" rule
		const int xStart = (int)std::ceil(px0 - 0.5f);
		const int xEnd = (int)std::ceil(px1 - 0.5f);

		// calculate text coord scanline unit step
		const Vec2 tcScanStep = (tcEdgeR - tcEdgeL) / (px1 - px0);
		Vec2 tcScanPos = tcEdgeL + tcScanStep * ((float)xStart - px0 + 0.5f);

		for (int x = xStart; x < xEnd; x++, tcScanPos += tcScanStep) {
			const int surfaceX = std::min(int(tcScanPos.x * surfaceWidth), sufaceMaxWidth);
			const int surfaceY = std::min(int(tcScanPos.y * surfaceHeight), surfaceMaxHeight);
			PutPixel(x, y, surf.GetPixel(surfaceX, surfaceY));
		}
	}
}

void Graphics::DrawFlatTopTriangle(const TexVertex & v0, const TexVertex & v1, const TexVertex & v2, const Surface& surf) {
	// use linear equation (y = mx +b) but the version of x in function of y (x = wy + p),
	// where w is the run over the rise (inverse of m) and p is the x-intercept (the original b is the y-intercept)
	// "y in function of x" would cause problems with vertical lines since 'm' in that case would result in division by zero
	// so we use "x in function of x" version cause the lines that we won't need to calculate the top horizontal edge of the triangle

	// run over rise for left and right sides
	const float lw = (v2.pos.x - v0.pos.x) / (v2.pos.y - v0.pos.y);
	const float rw = (v2.pos.x - v1.pos.x) / (v2.pos.y - v1.pos.y);

	// following Microsoft DirectX10 rasterization "top-edge" rule
	const int yStart = (int)std::ceil(v0.pos.y - 0.5f);
	const int yEnd = (int)std::ceil(v2.pos.y - 0.5f);

	// init text coord edges
	Vec2 tcEdgeL = v0.tc;
	Vec2 tcEdgeR = v1.tc;

	// calculate tex coord edge unit steps
	const Vec2 tcEdgeStepL = (v2.tc - v0.tc) / (v2.pos.y - v0.pos.y);
	const Vec2 tcEdgeStepR = (v2.tc - v1.tc) / (v2.pos.y - v1.pos.y);

	// do tex coord pre step
	tcEdgeL += tcEdgeStepL * (float(yStart) - v0.pos.y + 0.5f);
	tcEdgeR += tcEdgeStepR * (float(yStart) - v1.pos.y + 0.5f);

	for (int y = yStart; y < yEnd; y++,
		 tcEdgeL += tcEdgeStepL, tcEdgeR += tcEdgeStepR) {
		// add 0.5 to y value because we are making calculations based on the pixel center
		const float px0 = lw * (y + 0.5f - v0.pos.y) + v0.pos.x;
		const float px1 = rw * (y + 0.5f - v1.pos.y) + v1.pos.x;
		// following Microsoft DirectX10 rasterization "left-edge" rule
		const int xStart = (int)std::ceil(px0 - 0.5f);
		const int xEnd = (int)std::ceil(px1 - 0.5f);

		// calculate text coord scanline unit step
		const Vec2 tcScanStep = (tcEdgeR - tcEdgeL) / (px1 - px0);
		Vec2 tcScanPos = tcEdgeL + tcScanStep * ((float)xStart - px0 + 0.5f);

		// get texture info
		const int surfaceWidth = surf.GetWidth();
		const int surfaceHeight = surf.GetHeight();
		const int sufaceMaxWidth = surfaceWidth - 1;
		const int surfaceMaxHeight = surfaceHeight - 1;

		for (int x = xStart; x < xEnd; x++, tcScanPos += tcScanStep) {
			const int surfaceX = std::min(int(tcScanPos.x * surfaceWidth), sufaceMaxWidth);
			const int surfaceY = std::min(int(tcScanPos.y * surfaceHeight), surfaceMaxHeight);
			PutPixel(x, y, surf.GetPixel(surfaceX, surfaceY));
		}
	}
}
