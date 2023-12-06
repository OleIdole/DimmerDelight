#ifndef DimmerModule_h
#define DimmerModule_h

class DimmerModule {
public:
  void init(int initialBrightness);
  void setLightIntensity(int percentage);
  int getCurrentBrightness();
private:
  int currentBrightness = 0;
  void setCurrentBrightness(int percentage);
};

#endif  // DimmerModule_h
