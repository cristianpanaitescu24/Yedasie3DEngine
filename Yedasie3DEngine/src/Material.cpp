#include "Material.h"

Material::Material(const std::string& fileName)
{
    FILE *f;
    fopen_s(&f, (MATERIALS_DIR + fileName).c_str(), "r");
    
    if (f)
    {
        
        fscanf_s(f, "ka %f %f %f\n", &mAmbiental.r, &mAmbiental.g, &mAmbiental.b);
        fscanf_s(f, "kd %f %f %f\n", &mDiffuse.r, &mDiffuse.g, &mDiffuse.b);
        fscanf_s(f, "ks %f %f %f\n", &mSpecular.r, &mSpecular.g, &mSpecular.b);
        fscanf_s(f, "sh %f\n", &mShininess);

        fclose(f);
    }
}


Material::~Material()
{
}

void Material::SetMaterial()
{
}