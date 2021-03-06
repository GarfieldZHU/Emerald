#include "EECore.h"
#include "EEThread.h"
#include "EESystem.h"
#include "EEInput.h"
#include "EETimer.h"
#include "EED3D.h"
#include "EECamera.h"
#include "EEShaderState.h"
#include "EEGUI.h"

namespace Emerald
{
	//EECore
	//----------------------------------------------------------------------------------------------------
	EECore *EECore::s_EECore = NULL;

	//----------------------------------------------------------------------------------------------------
	bool EECore::Initialize_All(const EEDesc& _desc)
	{
		srand((unsigned)time(NULL));

		//EECore_ThreadSystem
		m_EEThreadSystem = NULL;
		m_EEThreadSystem = new EEThreadSystem;
		if (!m_EEThreadSystem->Initialize())
			return false;

		//EECore_System
		m_EESystem = NULL;
		m_EESystem = new EESystem;
		if (!m_EESystem->Initialize(_desc.applicationName, _desc.isFullScreen, _desc.width, _desc.height))
			return false;

		//EECore_Input
		m_EEInput = NULL;
		m_EEInput = new EEInput;
		if (!m_EEInput->Initialize())
			return false;
		m_inputPro = &EEInput::MessageHandler;

		//EECore_Timer
		m_EETimer = NULL;
		m_EETimer = new EETimer;
		if (!m_EETimer->Initialize())
			return false;

		//EECore_D3D
		m_EED3D = NULL;
		m_EED3D = new EED3D;
		if (!m_EED3D->Initialize(_desc.isSSAA, _desc.isVsync))
			return false;

		//EECore_CameraSystem
		m_EECameraSystem = NULL;
		m_EECameraSystem = new EECameraSystem;
		if (!m_EECameraSystem->Initialize())
			return false;

		//EECore_ShaderState
		m_EEShaderState = NULL;
		m_EEShaderState = new EEShaderState;
		if (!m_EEShaderState->Initialize())
			return false;

		//EECore_GUI
		m_control_mutex = NULL;

		return true;
	}

	//----------------------------------------------------------------------------------------------------
	void EECore::Shutdown()
	{
		SAFE_DELETE(m_EEShaderState);
		SAFE_DELETE(m_EECameraSystem);
		SAFE_DELETE(m_EED3D);
		SAFE_DELETE(m_EETimer);
		SAFE_DELETE(m_EEInput);
		SAFE_DELETE(m_EESystem);
		SAFE_DELETE(m_EEThreadSystem);
	}

	//----------------------------------------------------------------------------------------------------
	int EECore::Run()
	{
		Start();
		Tick();
		if (PeekMessageW(&m_msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&m_msg);
			DispatchMessageW(&m_msg);
		}

		if (m_runTime > 0)
		{
			m_runTime -= (float)GetDeltaTime();
			if (m_runTime < 0.0f)
				m_runTime = 0.0f;
		}


		if (m_runTime == 0.0f || m_msg.message == WM_QUIT || (m_msg.message == WM_KEYDOWN && m_msg.wParam == VK_ESCAPE))
		{
			m_runTime = -1.0f;
			Stop();
			return false;
		}

		return true;
	}

	//----------------------------------------------------------------------------------------------------
	void EECore::SetRunTime(float _time)
	{
		m_runTime = _time;
	}

	//EECore_ThreadSystem
	//----------------------------------------------------------------------------------------------------
	EEThreadSystem* EECore::GetEEThreadSystem() const { return m_EEThreadSystem; }

	bool EECore::AddThread(boost::thread* _thread) { return m_EEThreadSystem->AddThread(_thread); }
	bool EECore::RemoveThread(boost::thread* _thread) { return m_EEThreadSystem->RemoveThread(_thread); }
	void EECore::JoinAll() { m_EEThreadSystem->JoinAll(); }

	//EECore_System
	//----------------------------------------------------------------------------------------------------
	EESystem* EECore::GetEESystem() const { return m_EESystem; };

	bool EECore::SetHInstance(HINSTANCE _para) { m_EESystem->SetHInstance(_para); return true; };
	bool EECore::SetHWnd(HWND _para) { m_EESystem->SetHWnd(_para); return true; };
	bool EECore::SetApplicationName(LPCWSTR _para) { m_EESystem->SetApplicationName(_para); return true; };
	bool EECore::SetIsFullScreen(bool _para) { m_EESystem->SetIsFullScreen(_para); return true; };
	bool EECore::SetWidth(int _para) { m_EESystem->SetWidth(_para); return true; };
	bool EECore::SetHeight(int _para) { m_EESystem->SetHeight(_para); return true; };
	bool EECore::SetRect(const RECT& _para) { m_EESystem->SetRect(_para); return true; }
	bool EECore::SetInsideCentre(const INT2& _para) { m_EESystem->SetInsideCentre(_para); return true; }
	bool EECore::SetOutsideCentre(const INT2& _para) { m_EESystem->SetOutsideCentre(_para); return true; }

	HINSTANCE EECore::GetHInstance() const { return m_EESystem->GetHInstance(); };
	HWND EECore::GetHWnd() const { return m_EESystem->GetHWnd(); };
	LPCWSTR EECore::GetApplicationName() const { return m_EESystem->GetApplicationName(); };
	bool EECore::GetIsFullScreen() const { return m_EESystem->GetIsFullScreen(); };
	int EECore::GetWidth() const { return m_EESystem->GetWidth(); };
	int EECore::GetHeight() const { return m_EESystem->GetHeight(); };
	RECT EECore::GetRect() const { return m_EESystem->GetRect(); };
	INT2 EECore::GetInsideCentre() const { return m_EESystem->GetInsideCentre(); };
	INT2 EECore::GetOutsideCentre() const { return m_EESystem->GetOutsideCentre(); };

	//EECore_Input
	//----------------------------------------------------------------------------------------------------
	EEInput* EECore::GetEEInput() { return m_EEInput; };
	LRESULT(EEInput::*EECore::GetInputPro())(HWND, UINT, WPARAM, LPARAM) { return m_inputPro; };

	bool EECore::IsKeyDown(UINT _key) { return m_EEInput->IsKeyDown(_key); }
	UINT EECore::GetKey() { return m_EEInput->GetKey(); }
	bool EECore::IsKeyInput() { return m_EEInput->IsKeyInput(); }
	Point EECore::GetMousePosition() const { return m_EEInput->GetMousePosition(); }
	int EECore::GetMouseDeltaX() const { return m_EEInput->GetMouseDeltaX(); }
	int EECore::GetMouseDeltaY() const { return m_EEInput->GetMouseDeltaY(); }
	int EECore::GetMouseDeltaM() const { return m_EEInput->GetMouseDeltaM(); }
	void EECore::ClearMouseDeltaX() { m_EEInput->ClearMouseDeltaX(); }
	void EECore::ClearMouseDeltaY() { m_EEInput->ClearMouseDeltaY(); }
	void EECore::ClearMouseDeltaXY() { m_EEInput->ClearMouseDeltaXY(); };
	void EECore::ClearMouseDeltaM() { m_EEInput->ClearMouseDeltaM(); };

	//EECore_Timer
	//----------------------------------------------------------------------------------------------------
	EETimer* EECore::GetEETimer() { return m_EETimer; };

	void EECore::Start() { return m_EETimer->Start(); }
	void EECore::Stop() { return m_EETimer->Stop(); }
	void EECore::Reset() { return m_EETimer->Reset(); }
	void EECore::Tick() { return m_EETimer->Tick(); }
	double EECore::GetTotalTime() const { return m_EETimer->GetTotalTime(); }
	double EECore::GetDeltaTime() const { return m_EETimer->GetDeltaTime(); }
	int EECore::GetFPS(){ return m_EETimer->GetFPS(); }
	void EECore::ShowFPSInTitle() { return m_EETimer->ShowFPSInTitle(); }
	void EECore::ShowFPSInTitle(LPCWSTR _title) { return m_EETimer->ShowFPSInTitle(_title); }

	//EECore_D3D
	//----------------------------------------------------------------------------------------------------
	EED3D* EECore::GetEED3D() { return m_EED3D; };

	bool EECore::SetDevice(ID3D11Device* _para) { m_EED3D->SetDevice(_para); return true; }
	bool EECore::SetDeviceeContext(ID3D11DeviceContext* _para) { m_EED3D->SetDeviceeContext(_para); return true; }
	bool EECore::SetScreenNear(float _para) { m_EED3D->SetScreenNear(_para); return true; }
	bool EECore::SetScreenDeep(float _para) { m_EED3D->SetScreenDeep(_para); return true; }
	bool EECore::SetIsSSAA(bool _para) { m_EED3D->SetIsSSAA(_para); return true; }
	bool EECore::SetIsVsync(bool _para) { m_EED3D->SetIsVsync(_para); return true; }

	ID3D11Device* EECore::GetDevice() { return m_EED3D->GetDevice(); }
	ID3D11DeviceContext* EECore::GetDeviceContext() { return m_EED3D->GetDeviceContext(); }
	float EECore::GetScreenNear() const { return m_EED3D->GetScreenNear(); }
	float EECore::GetScreenDeep() const { return m_EED3D->GetScreenDeep(); }
	bool EECore::GetIsSSAA() const { return m_EED3D->GetIsSSAA(); }
	bool EECore::GetIsVsync() const { return m_EED3D->GetIsVsync(); }
	void EECore::GetVideoCardInfo(char* _cardName, int& _memory) { m_EED3D->GetVideoCardInfo(_cardName, _memory); }
	ID3D11RenderTargetView* EECore::GetRenderTargetView() { return m_EED3D->GetRenderTargetView(); }
	ID3D11DepthStencilView* EECore::GetDepthStencilView() { return m_EED3D->GetDepthStencilView(); }

	//EECore_CameraSystem
	//----------------------------------------------------------------------------------------------------
	EECameraSystem* EECore::GetEECameraSystem() { return m_EECameraSystem; }

	bool EECore::MapCameraBuffer() { return m_EECameraSystem->MapCameraBuffer(); }
	EEHCamera EECore::CreateCamera(const EECameraDesc& _desc) { return m_EECameraSystem->CreateCamera(_desc); }
	bool EECore::DeleteCamera(EEHCamera _camera) { return m_EECameraSystem->DeleteCamera(_camera); }
	void EECore::ClearCamera() { m_EECameraSystem->ClearCamera(); }
	bool EECore::SetCamera(EEHCamera _camera) { return m_EECameraSystem->SetCamera(_camera); }
	EEHCamera EECore::GetCamera() { return m_EECameraSystem->GetCamera(); }
	const MATRIX& EECore::GetViewMatrix() { return m_EECameraSystem->GetViewMatrix(); }
	const MATRIX& EECore::GetProjectionMatrix() { return m_EECameraSystem->GetProjectionMatrix(); }
	const MATRIX& EECore::GetOrthoLHMatrix() { return m_EECameraSystem->GetOrthoLHMatrix(); }

	//EECore_ShaderState
	//----------------------------------------------------------------------------------------------------
	EEShaderState* EECore::GetEEShaderState() { return m_EEShaderState; }

	//EECore_GUI
	//----------------------------------------------------------------------------------------------------
	boost::shared_mutex* EECore::GetControlMutex() { return m_control_mutex; }

	//EECore_APIs
	//----------------------------------------------------------------------------------------------------
	bool EEInitialize(const EEDesc& _EEDesc)
	{
		EECore::s_EECore = new EECore;
		if (!EECore::s_EECore->Initialize_All(_EEDesc))
			return false;

		return true;
	}

	//----------------------------------------------------------------------------------------------------
	void EEShutdown()
	{
		SAFE_DELETE(EECore::s_EECore);
	}

	//----------------------------------------------------------------------------------------------------
	int EERun()
	{
		return EECore::s_EECore->Run();
	}

	//----------------------------------------------------------------------------------------------------
	void EESetRuntime(float _time)
	{
		EECore::s_EECore->SetRunTime(_time);
	}
}