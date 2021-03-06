#include "stdafx.h"
#include "Renderer.h"


Renderer::Renderer()
{
	m_iLightChannel = 0;
}


Renderer::~Renderer()
{
}

bool Renderer::Init()
{
	m_D3D = Direct3DCreate9(D3D_SDK_VERSION);

	D3DPRESENT_PARAMETERS pp = {};
	pp.Windowed = (Application::GetInstance()->GetWindowed());
	pp.BackBufferWidth = Application::GetInstance()->GetWindowWidth();
	pp.BackBufferHeight = Application::GetInstance()->GetWindowHeight();
	pp.BackBufferFormat = D3DFMT_A8R8G8B8;
	pp.SwapEffect = D3DSWAPEFFECT_DISCARD;
	pp.AutoDepthStencilFormat = D3DFMT_D16;
	pp.EnableAutoDepthStencil = true;

	if (FAILED(m_D3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, Application::GetInstance()->GetHandle(), D3DCREATE_SOFTWARE_VERTEXPROCESSING, &pp, &m_D3DDevice)))
	{
		MessageBox(nullptr, L"Devide Create Arror", L"Arror", MB_OK);
		return false;
	}

	m_D3DDevice->SetRenderState(D3DRS_ZENABLE, true); //깊이 테스트 
	m_D3DDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE); //컬링 
	m_D3DDevice->SetRenderState(D3DRS_LIGHTING, true); //라이팅 
	m_D3DDevice->SetRenderState(D3DRS_AMBIENT, 0x00202020);



	m_D3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, true); //알파 블렌딩
	m_D3DDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA); //텍스쳐가 가진 알파 그대로 씀
	m_D3DDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA); //텍스쳐가 가진 알파를 1 - srcalpha

	//m_D3DDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	//m_D3DDevice->SetRenderState(D3DRS_ALPHAREF, 0x00);
	//m_D3DDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

	m_D3DDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR); //멀리 있는 텍스쳐 축소
	m_D3DDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR); //가까이 있는 텍스쳐 확대
	m_D3DDevice->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_LINEAR); //자연스러운 보간

	ZeroMemory(&Light, sizeof(D3DLIGHT9));

	Light.Type = D3DLIGHT_DIRECTIONAL;

	Light.Direction = D3DXVECTOR3(0.f, -1.f, 1.f);

	Light.Diffuse = D3DXCOLOR(1.f, 1.f, 1.f, 1.f); //난반사광 (다수의 방향으로 반사됨)
	Light.Ambient = D3DXCOLOR(0.2f, 0.2f, 0.2f, 1.f); //주변광 (특별한 방향없이 주변을 덮고 있는 빛)
	Light.Specular = D3DXCOLOR(1.f, 1.f, 1.f, 1.f); //정반사광 (한 방향으로만 반사되는 빛)

	SetLights(&Light);

	return true;
}

bool Renderer::Release()
{
	m_D3D->Release();
	m_D3DDevice->Release();

	return true;
}

bool Renderer::Begin()
{
	if (m_bWireFrame)
		m_D3DDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
	else
		m_D3DDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);

	if (Input::GetInstance()->GetKeyState(VK_SPACE) == KeyState::Up)
	{
		m_bWireFrame ^= true;
	}

	m_D3DDevice->BeginScene();
	m_D3DDevice->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_XRGB(0, 125, 125), 1.f, 0.f);

	return true;
}

bool Renderer::End()
{
	m_D3DDevice->EndScene();
	m_D3DDevice->Present(NULL, NULL, NULL, NULL);

	return true;
}

void Renderer::SetLights(D3DLIGHT9 * light)
{
	m_D3DDevice->SetLight(m_iLightChannel, light);
	m_D3DDevice->LightEnable(m_iLightChannel, true);

	m_iLightChannel++;
}
