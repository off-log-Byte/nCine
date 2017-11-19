#ifndef CLASS_NCINE_EGLGFXDEVICE
#define CLASS_NCINE_EGLGFXDEVICE

#include <EGL/egl.h>
#include <EGL/eglext.h>

#include "IGfxDevice.h"
#include "Vector2.h"
#include "DisplayMode.h"

struct android_app;

namespace ncine {

/// The EGL based graphics device
class EglGfxDevice : public IGfxDevice
{
  public:
	/// Constructor taking a `DisplayMode` object
	EglGfxDevice(struct android_app *state, const GLContextInfo &contextInfo, const DisplayMode &mode);
	~EglGfxDevice() override;

	void setResolution(int width, int height) override { }
	void setResolution(Vector2i size) override { }

	void toggleFullScreen() override { }

	inline void update() override { eglSwapBuffers(display_, surface_); }

	void setWindowTitle(const char *windowTitle) override { }
	void setWindowIcon(const char *windowIconFilename) override { }

	/// Recreates a surface from a native window
	void createSurface(struct android_app *state);
	/// Binds the current context
	void bindContext();
	/// Unbinds the current context
	void unbindContext();
	/// Queries the size of the current surface
	void querySurfaceSize();

	/// Checks if the desired pixel format is supported
	static bool isModeSupported(struct android_app *state, const GLContextInfo &contextInfo, const DisplayMode &mode);

  private:
	/// The EGL display connection
	EGLDisplay display_;
	/// The EGL drawing surface
	EGLSurface surface_;
	/// The EGL context
	EGLContext context_;
	/// The EGL config used to create the first surface
	EGLConfig config_;

	/// Initializes the OpenGL graphic context
	void initDevice(struct android_app *state);
};

}

#endif
