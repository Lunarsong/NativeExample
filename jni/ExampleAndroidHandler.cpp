#include "ExampleAndroidHandler.h"
#include <AndroidLog.h>

ExampleAndroidHandler::ExampleAndroidHandler()
{
	// State variables
	m_bShouldQuit 	= false;
	m_bIsVisible 	= false;
	m_bIsPaused		= true;

	// Egl
	m_Display = EGL_NO_DISPLAY;
	m_Surface = EGL_NO_SURFACE;
	m_Context = NULL;
}

ExampleAndroidHandler::~ExampleAndroidHandler()
{
	DestroyOpenGL();
}

void ExampleAndroidHandler::Run()
{
	// Example asset read
	Android::Asset* pAsset = Android::GetAssetManager().GetAsset( "test.txt" );
	if ( pAsset )
	{
		// Create a buffer to read the content into,
		// [ Size() + 1 ] for null terminator character for LOV usage
		char* pBuffer = new char[ pAsset->Size() + 1 ];

		// Read the buffer
		pAsset->Read( pBuffer, pAsset->Size() );

		// Delete the asset file
		delete pAsset;

		// Set null terminating for LOGV
		pBuffer[ pAsset->Size() ] = 0;

		// Show us the file's content!
		LOGV( "File content: %s", pBuffer );

		// Delete the buffer
		delete [] pBuffer;
	}

	// Create time measurement
	timespec timeNow;
	clock_gettime( CLOCK_MONOTONIC, &timeNow );
	uint64_t uPreviousTime = timeNow.tv_sec * 1000000000ull + timeNow.tv_nsec;

	// While application is alive...
	while ( !m_bShouldQuit )
	{
		// Handle Android events
		Android::PollEvents();

		// Calculate delta time
		clock_gettime( CLOCK_MONOTONIC, &timeNow ); // query time now
		uint64_t uNowNano = timeNow.tv_sec * 1000000000ull + timeNow.tv_nsec; // get time in nanoseconds
		float fDeltaSeconds = float( uNowNano - uPreviousTime ) * 0.000000001f; // 1 second = 1,000,000,000 nanoseconds
		uPreviousTime = uNowNano; // set previous time to new time

		// If not paused...
		if ( !m_bIsPaused )
		{
			// Update logic
			Update( fDeltaSeconds );
		}

		// If visible
		if ( m_bIsVisible )
		{
			// Draw
			Draw();
		}

	}
}

void ExampleAndroidHandler::Update( float fDeltaSeconds )
{
	// Do stuff here
}

void ExampleAndroidHandler::Draw()
{
	// Draw things here
	glClearColor( 0.0f, 0.0f, 1.0f, 1.0f );
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

	if ( !eglSwapBuffers( m_Display, m_Surface ) )
	{
		LOGE( "eglSwapBuffers() returned error %d", eglGetError() );
	}
}

bool ExampleAndroidHandler::InitOpenGL()
{
	const EGLint attribs[] =
	{
		EGL_SURFACE_TYPE, EGL_WINDOW_BIT,
		EGL_BLUE_SIZE, 8,
		EGL_GREEN_SIZE, 8,
		EGL_RED_SIZE, 8,
		EGL_NONE
	};

	EGLDisplay display;
	EGLConfig config;
	EGLint numConfigs;
	EGLint format;
	EGLSurface surface;
	EGLContext context;
	EGLint width;
	EGLint height;
	GLfloat ratio;

	LOGV( "[Example]: Initializing context" );

	if ( ( display = eglGetDisplay( EGL_DEFAULT_DISPLAY ) ) == EGL_NO_DISPLAY )
	{
		LOGE( "[Example]: eglGetDisplay() returned error %d", eglGetError() );
		return false;
	}

	if ( !eglInitialize( display, 0, 0 ) )
	{
		LOGE( "[Example]: eglInitialize() returned error %d", eglGetError() );
		return false;
	}

	if ( !eglChooseConfig(display, attribs, &config, 1, &numConfigs) )
	{
		LOGE( "[Example]: eglChooseConfig() returned error %d", eglGetError() );
		DestroyOpenGL();
		return false;
	}

	if ( !eglGetConfigAttrib(display, config, EGL_NATIVE_VISUAL_ID, &format) )
	{
		LOGE( "[Example]: eglGetConfigAttrib() returned error %d", eglGetError() );
		DestroyOpenGL();
		return false;
	}

	// Set buffer geometry using our window which is saved in Android
	ANativeWindow_setBuffersGeometry( Android::GetWindow(), 0, 0, format );

	if ( !( surface = eglCreateWindowSurface( display, config, Android::GetWindow(), 0 ) ) )
	{
		LOGE( "[Example]: eglCreateWindowSurface() returned error %d", eglGetError() );
		DestroyOpenGL();
		return false;
	}

	if ( !( context = eglCreateContext( display, config, 0, 0 ) ) )
	{
		LOGE( "[Example]: eglCreateContext() returned error %d", eglGetError() );
		DestroyOpenGL();
		return false;
	}

	if ( !eglMakeCurrent(display, surface, surface, context ) )
	{
		LOGE( "[Example]: eglMakeCurrent() returned error %d", eglGetError() );
		DestroyOpenGL();
		return false;
	}

	if ( !eglQuerySurface( display, surface, EGL_WIDTH, &width ) ||
		!eglQuerySurface( display, surface, EGL_HEIGHT, &height ) )
	{
		LOGE( "[Example]: eglQuerySurface() returned error %d", eglGetError() );
		DestroyOpenGL();
		return false;
	}

	m_Display = display;
	m_Surface = surface;
	m_Context = context;

	glDisable( GL_DITHER );
	glHint( GL_PERSPECTIVE_CORRECTION_HINT, GL_FASTEST );
	glClearColor(0, 0, 0, 0);
	glEnable( GL_CULL_FACE );
	glEnable( GL_DEPTH_TEST );

	return true;
}

void ExampleAndroidHandler::DestroyOpenGL()
{
	if ( m_Display )
	{
		LOGV( "[Example]: Shutting down OpenGL" );

		eglMakeCurrent( m_Display, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT );
		eglDestroyContext( m_Display, m_Context );
		eglDestroySurface( m_Display, m_Surface);
		eglTerminate( m_Display );

		m_Display = EGL_NO_DISPLAY;
		m_Surface = EGL_NO_SURFACE;
		m_Context = NULL;
	}
}

// Application
void ExampleAndroidHandler::OnShutdown()
{
	LOGV( "[Example]: Shutting down!" );
	m_bShouldQuit = true;
}

// Surface
void ExampleAndroidHandler::OnSurfaceCreated()
{
	LOGV( "[Example]: Creating surface!" );
	if ( InitOpenGL() == false )
	{
		m_bShouldQuit = true;
	}
}

void ExampleAndroidHandler::OnSurfaceChanged( int iPixelFormat, int iWidth, int iHeight )
{
	LOGV( "[Example]: Setting viewports!" );

	// Set new viewport
	glViewport( 0, 0, iWidth, iHeight );
}

void ExampleAndroidHandler::OnSurfaceDestroyed()
{
	LOGV( "[Example]: Destroying egl!" );
	DestroyOpenGL();
}

// States
void ExampleAndroidHandler::OnPause()
{
	LOGV( "[Example]: Paused!" );
	m_bIsPaused = true;
}

void ExampleAndroidHandler::OnResume()
{
	LOGV( "[Example]: Resumed!" );
	m_bIsPaused = false;
}

void ExampleAndroidHandler::OnVisible()
{
	LOGV( "[Example]: Visible!" );
	m_bIsVisible = true;
}

void ExampleAndroidHandler::OnHidden()
{
	LOGV( "[Example]: Hidden!" );
	m_bIsVisible = false;
}

void ExampleAndroidHandler::OnLowMemory()
{
	LOGV( "[Example]: Clearing some memory to stay alive..." );

	// BigMemoryObject->Release();
}

// Input
void ExampleAndroidHandler::OnKey( int iKeyCode, wchar_t iUnicodeChar )
{
	LOGV( "[Example]: Got key! %i %c", iKeyCode, iUnicodeChar );
}

void ExampleAndroidHandler::OnTouch( int iPointerID, float fPosX, float fPosY, int iAction )
{
	LOGV( "[Example]: Touch: %i, x: %f y:, %f action:, %i.", iPointerID, fPosX, fPosY, iAction );

	if ( iAction == 0 )
	{
		// On touch start show keyboard!
		Android::ShowKeyboard();
	}

	else if ( iAction == 1 )
	{
		// On touch up, hide keyboard...
		Android::HideKeyboard();
	}
}
