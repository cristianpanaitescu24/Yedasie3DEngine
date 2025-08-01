#include "ObjLoader.h"
#include <fstream>
#include <iostream>
#include <algorithm>
#include <map>

static bool CompareOBJIndexPtr(const OBJIndex* a, const OBJIndex* b);
static inline UINT32 FindNextChar(UINT32 start, const char* str, UINT32 length, char token);
static inline UINT32 ParseOBJIndexValue(const std::string& token, UINT32 start, UINT32 end);
static inline float ParseOBJFloatValue(const std::string& token, UINT32 start, UINT32 end);
static inline std::vector<std::string> SplitString(const std::string &s, char delim);

OBJModel::OBJModel(const std::string& fileName)
{
	hasUVs = false;
	hasNormals = false;
    std::ifstream file;
    file.open(fileName.c_str());

    std::string line;
    if(file.is_open())
    {
        while(file.good())
        {
            getline(file, line);
        
            UINT32 lineLength = (UINT32)line.length();
            
            if(lineLength < 2)
                continue;
            
            const char* lineCStr = line.c_str();
            
            switch(lineCStr[0])
            {
                case 'v':
                    if(lineCStr[1] == 't')
                        this->uvs.push_back(ParseOBJVec2(line));
                    else if(lineCStr[1] == 'n')
                        this->normals.push_back(ParseOBJVec3(line));
                    else if(lineCStr[1] == ' ' || lineCStr[1] == '\t')
                        this->vertices.push_back(ParseOBJVec3(line));
                break;
                case 'f':
                    CreateOBJFace(line);
                break;
                default: break;
            };
        }
    }
    else
    {
        std::cerr << "Unable to load mesh: " << fileName << std::endl;
    }
}

void IndexedModel::CalcNormals()
{
    for(UINT32 i = 0; i < indices.size(); i += 3)
    {
        int i0 = indices[i];
        int i1 = indices[i + 1];
        int i2 = indices[i + 2];

        Vector3f v1 = positions[i1] - positions[i0];
        Vector3f v2 = positions[i2] - positions[i0];
        
        Vector3f normal = glm::normalize(glm::cross(v1, v2));
            
        normals[i0] += normal;
        normals[i1] += normal;
        normals[i2] += normal;
    }
    
    for(UINT32 i = 0; i < positions.size(); i++)
        normals[i] = glm::normalize(normals[i]);
}

IndexedModel OBJModel::ToIndexedModel()
{
    IndexedModel result;
    IndexedModel normalModel;
    
    UINT32 numIndices = (UINT32)OBJIndices.size();
    
    std::vector<OBJIndex*> indexLookup;
    
    for(UINT32 i = 0; i < numIndices; i++)
        indexLookup.push_back(&OBJIndices[i]);
    
    std::sort(indexLookup.begin(), indexLookup.end(), CompareOBJIndexPtr);
    
    std::map<OBJIndex, UINT32> normalModelIndexMap;
    std::map<UINT32, UINT32> indexMap;
    
    for(UINT32 i = 0; i < numIndices; i++)
    {
        OBJIndex* currentIndex = &OBJIndices[i];
        
        Vector3f currentPosition = vertices[currentIndex->vertexIndex];
        Vector2f currentTexCoord;
        Vector3f currentNormal;
        
        if(hasUVs)
            currentTexCoord = uvs[currentIndex->uvIndex];
        else
            currentTexCoord = Vector2f(0,0);
            
        if(hasNormals)
            currentNormal = normals[currentIndex->normalIndex];
        else
            currentNormal = Vector3f(0,0,0);
        
        UINT32 normalModelIndex;
        UINT32 resultModelIndex;
        
        //Create model to properly generate normals on
        std::map<OBJIndex, UINT32>::iterator it = normalModelIndexMap.find(*currentIndex);
        if(it == normalModelIndexMap.end())
        {
            normalModelIndex = (UINT32)normalModel.positions.size();
        
            normalModelIndexMap.insert(std::pair<OBJIndex, UINT32>(*currentIndex, normalModelIndex));
            normalModel.positions.push_back(currentPosition);
            normalModel.texCoords.push_back(currentTexCoord);
            normalModel.normals.push_back(currentNormal);
        }
        else
            normalModelIndex = it->second;
        
        //Create model which properly separates texture coordinates
        UINT32 previousVertexLocation = FindLastVertexIndex(indexLookup, currentIndex, result);
        
        if(previousVertexLocation == (UINT32)-1)
        {
            resultModelIndex = (UINT32)result.positions.size();
        
            result.positions.push_back(currentPosition);
            result.texCoords.push_back(currentTexCoord);
            result.normals.push_back(currentNormal);
        }
        else
            resultModelIndex = previousVertexLocation;
        
        normalModel.indices.push_back(normalModelIndex);
        result.indices.push_back(resultModelIndex);
        indexMap.insert(std::pair<UINT32, UINT32>(resultModelIndex, normalModelIndex));
    }
    
    if(!hasNormals)
    {
        normalModel.CalcNormals();
        
        for(UINT32 i = 0; i < result.positions.size(); i++)
            result.normals[i] = normalModel.normals[indexMap[i]];
    }
    
    return result;
};

UINT32 OBJModel::FindLastVertexIndex(const std::vector<OBJIndex*>& indexLookup, const OBJIndex* currentIndex, const IndexedModel& result)
{
    UINT32 start = 0;
    UINT32 end = (UINT32)indexLookup.size();
    UINT32 current = (end - start) / 2 + start;
    UINT32 previous = start;
    
    while(current != previous)
    {
        OBJIndex* testIndex = indexLookup[current];
        
        if(testIndex->vertexIndex == currentIndex->vertexIndex)
        {
            UINT32 countStart = current;
        
            for(UINT32 i = 0; i < current; i++)
            {
                OBJIndex* possibleIndex = indexLookup[current - i];
                
                if(possibleIndex == currentIndex)
                    continue;
                    
                if(possibleIndex->vertexIndex != currentIndex->vertexIndex)
                    break;
                    
                countStart--;
            }
            
            for(UINT32 i = countStart; i < indexLookup.size() - countStart; i++)
            {
                OBJIndex* possibleIndex = indexLookup[current + i];
                
                if(possibleIndex == currentIndex)
                    continue;
                    
                if(possibleIndex->vertexIndex != currentIndex->vertexIndex)
                    break;
                else if((!hasUVs || possibleIndex->uvIndex == currentIndex->uvIndex) 
                    && (!hasNormals || possibleIndex->normalIndex == currentIndex->normalIndex))
                {
                    Vector3f currentPosition = vertices[currentIndex->vertexIndex];
                    Vector2f currentTexCoord;
                    Vector3f currentNormal;
                    
                    if(hasUVs)
                        currentTexCoord = uvs[currentIndex->uvIndex];
                    else
                        currentTexCoord = Vector2f(0,0);
                        
                    if(hasNormals)
                        currentNormal = normals[currentIndex->normalIndex];
                    else
                        currentNormal = Vector3f(0,0,0);
                    
                    for(UINT32 j = 0; j < result.positions.size(); j++)
                    {
                        if(currentPosition == result.positions[j] 
                            && ((!hasUVs || currentTexCoord == result.texCoords[j])
                            && (!hasNormals || currentNormal == result.normals[j])))
                        {
                            return j;
                        }
                    }
                }
            }
        
            return -1;
        }
        else
        {
            if(testIndex->vertexIndex < currentIndex->vertexIndex)
                start = current;
            else
                end = current;
        }
    
        previous = current;
        current = (end - start) / 2 + start;
    }
    
    return -1;
}

void OBJModel::CreateOBJFace(const std::string& line)
{
    std::vector<std::string> tokens = SplitString(line, ' ');

    this->OBJIndices.push_back(ParseOBJIndex(tokens[1], &this->hasUVs, &this->hasNormals));
    this->OBJIndices.push_back(ParseOBJIndex(tokens[2], &this->hasUVs, &this->hasNormals));
    this->OBJIndices.push_back(ParseOBJIndex(tokens[3], &this->hasUVs, &this->hasNormals));

    if((int)tokens.size() > 4)
    {
        this->OBJIndices.push_back(ParseOBJIndex(tokens[1], &this->hasUVs, &this->hasNormals));
        this->OBJIndices.push_back(ParseOBJIndex(tokens[3], &this->hasUVs, &this->hasNormals));
        this->OBJIndices.push_back(ParseOBJIndex(tokens[4], &this->hasUVs, &this->hasNormals));
    }
}

OBJIndex OBJModel::ParseOBJIndex(const std::string& token, bool* hasUVs, bool* hasNormals)
{
    UINT32 tokenLength = (UINT32)token.length();
    const char* tokenString = token.c_str();
    
    UINT32 vertIndexStart = 0;
    UINT32 vertIndexEnd = FindNextChar(vertIndexStart, tokenString, tokenLength, '/');
    
    OBJIndex result;
    result.vertexIndex = ParseOBJIndexValue(token, vertIndexStart, vertIndexEnd);
    result.uvIndex = 0;
    result.normalIndex = 0;
    
    if(vertIndexEnd >= tokenLength)
        return result;
    
    vertIndexStart = vertIndexEnd + 1;
    vertIndexEnd = FindNextChar(vertIndexStart, tokenString, tokenLength, '/');
    
    result.uvIndex = ParseOBJIndexValue(token, vertIndexStart, vertIndexEnd);
    *hasUVs = true;
    
    if(vertIndexEnd >= tokenLength)
        return result;
    
    vertIndexStart = vertIndexEnd + 1;
    vertIndexEnd = FindNextChar(vertIndexStart, tokenString, tokenLength, '/');
    
    result.normalIndex = ParseOBJIndexValue(token, vertIndexStart, vertIndexEnd);
    *hasNormals = true;
    
    return result;
}

Vector3f OBJModel::ParseOBJVec3(const std::string& line) 
{
    UINT32 tokenLength = (UINT32)line.length();
    const char* tokenString = line.c_str();
    
    UINT32 vertIndexStart = 2;
    
    while(vertIndexStart < tokenLength)
    {
        if(tokenString[vertIndexStart] != ' ')
            break;
        vertIndexStart++;
    }
    
    UINT32 vertIndexEnd = FindNextChar(vertIndexStart, tokenString, tokenLength, ' ');
    
    float x = ParseOBJFloatValue(line, vertIndexStart, vertIndexEnd);
    
    vertIndexStart = vertIndexEnd + 1;
    vertIndexEnd = FindNextChar(vertIndexStart, tokenString, tokenLength, ' ');
    
    float y = ParseOBJFloatValue(line, vertIndexStart, vertIndexEnd);
    
    vertIndexStart = vertIndexEnd + 1;
    vertIndexEnd = FindNextChar(vertIndexStart, tokenString, tokenLength, ' ');
    
    float z = ParseOBJFloatValue(line, vertIndexStart, vertIndexEnd);
    
    return Vector3f(x,y,z);

    //Vector3f(atof(tokens[1].c_str()), atof(tokens[2].c_str()), atof(tokens[3].c_str()))
}

Vector2f OBJModel::ParseOBJVec2(const std::string& line)
{
    UINT32 tokenLength = (UINT32)line.length();
    const char* tokenString = line.c_str();
    
    UINT32 vertIndexStart = 3;
    
    while(vertIndexStart < tokenLength)
    {
        if(tokenString[vertIndexStart] != ' ')
            break;
        vertIndexStart++;
    }
    
    UINT32 vertIndexEnd = FindNextChar(vertIndexStart, tokenString, tokenLength, ' ');
    
    float x = ParseOBJFloatValue(line, vertIndexStart, vertIndexEnd);
    
    vertIndexStart = vertIndexEnd + 1;
    vertIndexEnd = FindNextChar(vertIndexStart, tokenString, tokenLength, ' ');
    
    float y = ParseOBJFloatValue(line, vertIndexStart, vertIndexEnd);
    
    return Vector2f(x,y);
}

static bool CompareOBJIndexPtr(const OBJIndex* a, const OBJIndex* b)
{
    return a->vertexIndex < b->vertexIndex;
}

static inline UINT32 FindNextChar(UINT32 start, const char* str, UINT32 length, char token)
{
    UINT32 result = start;
    while(result < length)
    {
        result++;
        if(str[result] == token)
            break;
    }
    
    return result;
}

static inline UINT32 ParseOBJIndexValue(const std::string& token, UINT32 start, UINT32 end)
{
    return atoi(token.substr(start, end - start).c_str()) - 1;
}

static inline float ParseOBJFloatValue(const std::string& token, UINT32 start, UINT32 end)
{
    return (float)atof(token.substr(start, end - start).c_str());
}

static inline std::vector<std::string> SplitString(const std::string &s, char delim)
{
    std::vector<std::string> elems;
        
    const char* cstr = s.c_str();
    UINT32 strLength = (UINT32)s.length();
    UINT32 start = 0;
    UINT32 end = 0;
        
    while(end <= strLength)
    {
        while(end <= strLength)
        {
            if(cstr[end] == delim)
                break;
            end++;
        }
            
        elems.push_back(s.substr(start, end - start));
        start = end + 1;
        end = start;
    }
        
    return elems;
}
