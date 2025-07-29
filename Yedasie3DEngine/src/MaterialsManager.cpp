#include "MaterialsManager.h"

MaterialsManager* gMaterialsManager = nullptr;

void MaterialsManager::Create()
{
    gMaterialsManager = new MaterialsManager();
}

void MaterialsManager::Destroy()
{
    ASSERT(gMaterialsManager);
    delete gMaterialsManager;
}

MaterialsManager::MaterialsManager()
{
    ASSERT(!gMaterialsManager);
}

MaterialsManager::~MaterialsManager()
{
    for (std::pair<std::string, Material *> it : mMaterials)
        delete it.second;
    mMaterials.clear();
}

void MaterialsManager::AddMaterial(Material *material, std::string materialId)
{
    ASSERT(mMaterials.find(materialId) == mMaterials.end());

    if (material)
    {
        mMaterials[materialId] = material;
    }
}

Material* MaterialsManager::GetMaterial(std::string materialId)
{
    ASSERT(mMaterials.find(materialId) != mMaterials.end());

    return mMaterials[materialId];

}