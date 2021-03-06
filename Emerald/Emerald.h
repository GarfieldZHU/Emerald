#pragma once
#ifndef _EE_EMERALD_H_
#define _EE_EMERALD_H_

#include "EECore.h"
#include "EEMath.h"
#include "EEColor.h"
#include "EESystem.h"
#include "EEInput.h"
#include "EETimer.h"
#include "EED3D.h"
#include "EEGeometry.h"
#include "EEGUI.h"
#include "EEAction.h"
#include "EEMusic.h"
#include "EERecorder.h"
#include "EEWaveCoder.h"
#include "EEParticle.h"
#include "EEFont.h"
#include "EETCP.h"
#include "EEUDP.h"
#include "EEDNS.h"
#include "EEAnimation.h"

#include "EEAverage.h"
#include "EERipple.h"
#include "EEBlur.h"
#include "EEGray.h"
#include "EEContrastAndBright.h"
#include "EEErode.h"
#include "EEDilate.h"
#include "EECanny.h"

using namespace Emerald;

/*
//EECore_APIs
//----------------------------------------------------------------------------------------------------
bool EEInitialize();
bool EEInitialize(const EEDesc& _EEDesc);
void EEShutdown();

//EEInput_APIs
//----------------------------------------------------------------------------------------------------
bool EEIsKeyDown(UINT _para);
Point EEGetMousePosition();
int EEGetOnDeltaX();
int EEGetOnDeltaY();
void EEClearOnDeltaX();
void EEClearOnDeltaY();
void EEClearOnDeltaXY();

//EETimer_APIs
//----------------------------------------------------------------------------------------------------
void EETimerStart();
void EETimerStop();
void EETimerReset();
void EETimerTick();
double EEGetTotalTime();
double EEGetDeltaTime();
void EEShowFPSInTitle();
void EEShowFPSInTitle(LPCWSTR _title);

//EED3D_APIs
//----------------------------------------------------------------------------------------------------
void EEBeginScene(float, float, float, float);
void EEBeginScene(const FLOAT4&);
void EEEndScene();

*/

#endif