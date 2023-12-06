#ifndef ButtonModule_h
#define ButtonModule_h

typedef void (*ButtonPressCallback)();

class ButtonModule {
public:
  void init();
  void monitorState();
  void onShortPress(ButtonPressCallback callback);
  void onLongPress(ButtonPressCallback callback);
private:
  ButtonPressCallback shortPressCallback;
  ButtonPressCallback longPressCallback;
};

#endif  // ButtonModule_h
