#ifndef DimmerModule_h
#define DimmerModule_h

class DimmerModule {
public:
  bool isLightOn = false;
  void init(int initialBrightness);
  void setLightIntensity(int percentage);
  int getBrightness();
private:
  int brightness = 0;
  void setBrightness(int percentage);
};

#endif  // DimmerModule_h
