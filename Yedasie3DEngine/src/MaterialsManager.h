#pragma once

#include "Utils.h"
#include "Material.h"

class MaterialsManager
{
public:
    static void Create();
    static void Destroy();

    MaterialsManager();
	~MaterialsManager();
	
    void            AddMaterial(Material *material, std::string materialId);

    Material*       GetMaterial(std::string materialId);

    UINT32          GetMaterialsCount() { return mMaterials.size(); }

protected:
private:

    std::unordered_map<std::string, Material *>     mMaterials;
};

extern MaterialsManager* gMaterialsManager;