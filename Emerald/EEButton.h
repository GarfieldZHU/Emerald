#pragma once
#ifndef _EE_BUTTON_H_
#define _EE_BUTTON_H_

#include "EEGeometry.h"
//#include <boost/any.hpp>
//----------------------------------------------------------------------------------------------------
namespace Emerald
{
	//EEButtonType
	//----------------------------------------------------------------------------------------------------
	typedef enum
	{
		EE_BUTTON_THREE = 1,
		EE_BUTTON_SCALE = 2,
	} EEButtonType;

	//EEButton
	//----------------------------------------------------------------------------------------------------
	class EEButton : public EEQuad2D
	{
	public:
		EEButton(EEButtonType _type, const Rect_Float& _rect, const EETexture& _upTex, const EETexture& _overTex, const EETexture& _downTex, std::function<void(void)> _funcPtr = []{});
		EEButton(EEButtonType _type, const Rect_Float& _rect, ID3D11ShaderResourceView* _upTex, ID3D11ShaderResourceView* _overTex, ID3D11ShaderResourceView* _downTex, std::function<void(void)> _funcPtr = []{});
		EEButton(EEButtonType _type, const Rect_Float& _rect, float _scale, float _scaleTime, float _fadeTime, const EETexture& _tex, std::function<void(void)> _funcPtr = []{});
		EEButton(EEButtonType _type, const Rect_Float& _rect, float _scale, float _scaleTime, float _fadeTime, ID3D11ShaderResourceView* _tex, std::function<void(void)> _funcPtr = []{});
		EEButton(const EEButton& _button);
		virtual ~EEButton();

		virtual bool Update();
		virtual bool Render();

		Rect_Float GetFinalRect();
		EETexture* GetUpTex();
		EETexture* GetOverTex();
		EETexture* GetDownTex();

		//OnFunction
		virtual void OnMouseOver(const Point& _pos);
		virtual void OnMouseClicked(const Point& _pos);
		virtual void OnMouseTriggered(const Point& _pos);

	protected:
		virtual bool UpdateObjectState();

		bool Update_THREE();
		bool Render_THREE();
		Rect_Float GetFinalRect_THREE();
		bool Update_SCALE();
		bool Render_SCALE();
		Rect_Float GetFinalRect_SCALE();

	protected:
		//type
		EEButtonType m_type;
		//scale
		float m_aimScale;
		float m_currScaleTime;
		float m_aimScaleTime;
		//alpha
		float m_aimAlpha;
		float m_currFadeTime;
		float m_aimFadeTime;
		//callback function
		//DWORD_PTR m_callbackFunc;
		std::function<void(void)> m_callbackFunc;
		//the picture of the button
		EETexture m_overTex, m_downTex;
	};
}

#endif