#pragma once

#include <Android.h>
#include <EGL/egl.h> // requires ndk r5 or newer
#include <GLES/gl.h>

class ExampleAndroidHandler : public IAndroidHandler
{
public:
	ExampleAndroidHandler();
	~ExampleAndroidHandler();

	void Run();
	void Update( float fDeltaSeconds );
	void Draw();

	bool InitOpenGL();
	void DestroyOpenGL();

	// Application
	virtual void OnShutdown();

	// Surface
	virtual void OnSurfaceCreated();
	virtual void OnSurfaceChanged( int iPixelFormat, int iWidth, int iHeight );
	virtual void OnSurfaceDestroyed();

	// States
	virtual void OnPause();
	virtual void OnResume();

	virtual void OnVisible();
	virtual void OnHidden();

	virtual void OnLowMemory();

	// Input
	virtual void OnKey( int iKeyCode, wchar_t iUnicodeChar );
	virtual void OnTouch( int iPointerID, float fPosX, float fPosY, int iAction );

private:
	// State variables
	bool m_bShouldQuit;
	bool m_bIsVisible;
	bool m_bIsPaused;

	// EGL
	EGLDisplay m_Display;
	EGLSurface m_Surface;
	EGLContext m_Context;

};
