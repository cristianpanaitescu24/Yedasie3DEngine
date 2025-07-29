#pragma once
#include "Utils.h"

#define MATERIALS_DIR "../../Resources/materials/"

class Material
{
public:
    Material(const std::string& fileName);
    ~Material();
    
    void            SetMaterial();

    float*        GetAmbientalRef() { return &mAmbiental.x; }
    float*        GetDiffuseRef()   { return &mDiffuse.x; }
    float*        GetSpecularRef()  { return &mSpecular.x; }
    float         GetShininess()    { return mShininess; }

protected:
private:

    Vector3f        mAmbiental;
    Vector3f        mDiffuse;
    Vector3f        mSpecular;
    float           mShininess;
};

