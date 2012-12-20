#ifndef _CONFIG_H_
#define _CONFIG_H_

class Configurator
{
private:
    void setDefaults();
    char path_[256];
    bool invertX_;
    bool invertY_;
    int imageWidth_;
    int imageHeight_;
    bool showGUI_;
    double deadzone_;
    int maxY_;
    int minY_;
    int maxX_;
    int minX_;
    int servo1_;
    int servo2_;
    char serialportpath_[256];
    int nofacethreshold_;
    int nofacefreamecount_;
    int CAM_;
    char haarcascade_[256];

public:
    Configurator (const char * pathname);
    virtual ~Configurator() {};
    void setLimits();
    void saveLimits(int maxY, int minY, int maxX, int minX);
    bool get_invertX();
    bool get_invertY();
    int get_imageWidth();
    int get_imageHeight();
    bool get_showGUI();
    double get_deadzone();
    int get_maxY();
    int get_minY();
    int get_maxX();
    int get_minX();
    int get_servo1();
    int get_servo2();
    char* get_serialportpath();
    int get_nofacethreshold();
    int get_nofacefreamecount();
    int get_CAM();
    char* get_haarcascade();
};


inline bool Configurator::get_invertX()
{
    return invertX_;
}

inline bool Configurator::get_invertY()
{
    return invertY_;
}

inline int Configurator::get_imageWidth()
{
    return imageWidth_;
}

inline int Configurator::get_imageHeight()
{
    return imageHeight_;
}

inline bool Configurator::get_showGUI()
{
    return showGUI_;
}

inline double Configurator::get_deadzone()
{
    return deadzone_;
}

inline int Configurator::get_maxY()
{
    return maxY_;
}

inline int Configurator::get_minY()
{
    return minY_;
}

inline int Configurator::get_maxX()
{
    return maxX_;
}

inline int Configurator::get_minX()
{
    return minX_;
}

inline int Configurator::get_servo1()
{
    return servo1_;
}

inline int Configurator::get_servo2()
{
    return servo2_;
}

inline char* Configurator::get_serialportpath()
{
    return serialportpath_;
}

inline int Configurator::get_nofacethreshold()
{
    return nofacethreshold_;
}

inline int Configurator::get_nofacefreamecount()
{
    return nofacefreamecount_;
}

inline int Configurator::get_CAM()
{
    return CAM_;
}

inline char* Configurator::get_haarcascade()
{
    return haarcascade_;
}
    

#endif
