#ifndef CLASS_MYEVENTHANDLER
#define CLASS_MYEVENTHANDLER

#include "IAppEventHandler.h"
#include "IInputEventHandler.h"
#include "StaticArray.h"

namespace ncine {

class AppConfiguration;
class String;
class Texture;
class Sprite;
class Font;
class TextNode;

}

namespace nc = ncine;

/// My nCine event handler
class MyEventHandler :
	public nc::IAppEventHandler,
	public nc::IInputEventHandler
{
  public:
	void onPreInit(nc::AppConfiguration &config) override;
	void onInit() override;
	void onFrameStart() override;
	void onShutdown() override;

#ifdef __ANDROID__
	void handleEvent(const nc::TouchEvent &event, nc::String *string, const char *eventName);
	void onTouchDown(const nc::TouchEvent &event) override;
	void onTouchUp(const nc::TouchEvent &event) override;
	void onTouchMove(const nc::TouchEvent &event) override;
	void onPointerDown(const nc::TouchEvent &event) override;
	void onPointerUp(const nc::TouchEvent &event) override;
#else
	void onKeyReleased(const nc::KeyboardEvent &event) override;
#endif

  private:
	static const unsigned int MaxNumChars = 1024;
	nc::String *multitouchString_;

#ifdef __ANDROID__
	nc::Texture *texture_;
	nc::StaticArray<nc::Sprite *, nc::TouchEvent::MaxPointers> sprites_;
#endif
	nc::Font *font_;
	nc::TextNode *textNode_;
};

#endif
