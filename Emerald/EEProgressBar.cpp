#include "EEProgressBar.h"
#include "EECore.h"

//----------------------------------------------------------------------------------------------------
namespace Emerald
{
	//EEProgressBar
	//----------------------------------------------------------------------------------------------------
	EEProgressbar::EEProgressbar(const Rect_Float& _progressRect, const EETexture& _progressTex, const EETexture& _frameTex, std::function<void(float)> _funcPtr)
		:
		EEQuad2D(_progressRect, _progressTex),
		m_progressFrame(Rect_Float(0.0f, 0.0f, _progressRect.z - _progressRect.x, _progressRect.w - _progressRect.y) - FLOAT2(GetWidht() / 2, GetHeight() / 2), _frameTex),
		m_progress(0.0f),
		m_isProgressDirty(false),
		m_callbackFunc(_funcPtr)
	{
		m_progressFrame.SetParent(this);
		SetIsFocusable(true);
	}

	//----------------------------------------------------------------------------------------------------
	EEProgressbar::EEProgressbar(const Rect_Float& _progressRect, const Rect_Float& _frameRect, const EETexture& _progressTex, const EETexture& _frameTex, std::function<void(float)> _funcPtr)
		:
		EEQuad2D(_progressRect, _progressTex),
		m_progressFrame(_frameRect - FLOAT2(GetWidht() / 2, GetHeight() / 2), _frameTex),
		m_progress(0.0f),
		m_isProgressDirty(false),
		m_callbackFunc(_funcPtr)
	{
		m_progressFrame.SetParent(this);
		SetIsFocusable(true);
	}

	//----------------------------------------------------------------------------------------------------
	EEProgressbar::EEProgressbar(const EEProgressbar& _progressbar)
		:
		EEQuad2D(_progressbar),
		m_progressFrame(_progressbar.m_progressFrame),
		m_progress(_progressbar.m_progress),
		m_isProgressDirty(_progressbar.m_isProgressDirty),
		m_callbackFunc(_progressbar.m_callbackFunc)
	{
		m_progressFrame.SetParent(this);
		SetIsFocusable(true);
	}

	//----------------------------------------------------------------------------------------------------
	EEProgressbar::~EEProgressbar()
	{

	}

	//----------------------------------------------------------------------------------------------------
	bool EEProgressbar::Update()
	{
		if (!EEObject::Update())
			return false;

		UpdateObjectState();

		if (m_isPositionDirty)
		{
			m_isPositionDirty = false;
		}

		if (m_isScaleDirty || m_isLocalZOrderDirty || m_isProgressDirty)
		{
			Rect_Float rect(
				-m_quadWidth / 2,
				-m_quadHeight / 2,
				m_quadWidth / 2,
				m_quadHeight / 2
				);

			//the value of the z depends on the progress (the scaled end - the scaled width * (1.0f - the progress)
			rect.z -= (rect.z - rect.x) * (1.0f - m_progress);

			EEQuad2DVertex vertices[4];
			vertices[0].pos = FLOAT3(rect.x, rect.y, m_localZOrder * 0.0001f);
			vertices[0].tex = FLOAT2(0, 0);
			vertices[1].pos = FLOAT3(rect.z, rect.y, m_localZOrder * 0.0001f);
			vertices[1].tex = FLOAT2(m_progress, 0);
			vertices[2].pos = FLOAT3(rect.x, rect.w, m_localZOrder * 0.0001f);
			vertices[2].tex = FLOAT2(0, 1);
			vertices[3].pos = FLOAT3(rect.z, rect.w, m_localZOrder * 0.0001f);
			vertices[3].tex = FLOAT2(m_progress, 1);

			ID3D11DeviceContext *deviceContext = EECore::s_EECore->GetDeviceContext();
			D3D11_MAPPED_SUBRESOURCE mappedResource;
			ZeroMemory(&mappedResource, sizeof(D3D11_MAPPED_SUBRESOURCE));
			deviceContext->Map(m_quadVB, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
			memcpy(mappedResource.pData, vertices, sizeof(vertices));
			deviceContext->Unmap(m_quadVB, 0);

			if (m_isProgressDirty)
			{
				if (m_callbackFunc)
				{
					m_callbackFunc(m_progress);
				}
			}

			m_isScaleDirty = false;
			m_isLocalZOrderDirty = false;
			m_isProgressDirty = false;
		}

		m_progressFrame.Update();

		return true;
	}

	//----------------------------------------------------------------------------------------------------
	bool EEProgressbar::Render()
	{
		m_progressFrame.Render();
		EEQuad2D::Render();

		return true;
	}

	//----------------------------------------------------------------------------------------------------
	void EEProgressbar::SetLocalZOrder(float _localZOrder)
	{
		EEQuad2D::SetLocalZOrder(_localZOrder);
		m_progressFrame.SetLocalZOrder(_localZOrder);
	}

	//----------------------------------------------------------------------------------------------------
	bool EEProgressbar::SetProgress(float _progress)
	{
		m_progress = _progress;
		m_isProgressDirty = true;

		return true;
	}

	//----------------------------------------------------------------------------------------------------
	bool EEProgressbar::SetCallbackFunc(std::function<void(float)> _funcPtr)
	{
		m_callbackFunc = _funcPtr;

		return true;
	}

	//----------------------------------------------------------------------------------------------------
	float EEProgressbar::GetProgress()
	{
		return m_progress;
	}

	//----------------------------------------------------------------------------------------------------
	EETexture* EEProgressbar::GetProgressTex()
	{
		return &m_tex;
	}

	//----------------------------------------------------------------------------------------------------
	EETexture* EEProgressbar::GetFrameTex()
	{
		return m_progressFrame.GetTexture();
	}

	//----------------------------------------------------------------------------------------------------
	void EEProgressbar::OnMouseTriggered(const Point& _pos)
	{
		EEObject::OnMouseTriggered(_pos);

		SetProgress((_pos.x - m_position.x) / m_quadWidth + 0.5f);
	}
}