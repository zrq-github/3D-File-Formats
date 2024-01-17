#pragma once

#include <fbxsdk.h>

static const char* gDiffuseElementName = "DiffuseUV";
static const char* gAmbientElementName = "AmbientUV";
static const char* gEmissiveElementName = "EmissiveUV";

class FbxSdkHelp
{
public:
    FbxSdkHelp();
    ~FbxSdkHelp();
    /// @brief 我的第一个程序
    void RunMyFirstProgrammer();
    /// @brief 插入节点测试
    void RunAddFbxNode();
    /// @brief 插入节点属性测试
    void RunFbxProperty();
    /// @brief 创建一个Mesh
    void RunCreateMeshes();
    /// @brief 创建材质测试
    void RunCreateMaterials();
    /// @brief 读取材质
    void RunReadMaterials();
    /// @brief 三角化测试
    void RunTriangulateScene();
    /// @brief 测试非平面多边形
    void RunCreateMeshNoPlane();
    /// @brief 创建带有材质的正方形
    /// @details 
    void RunCreateCubeWithTexture();
    /// @brief 读取带有材质的Cube
    void RunReadCubeWithTexture();
    /// @brief 导出场景
    void ExportScene(FbxScene* pFbxScene, std::string path = "");
    /// @brief 读取指定文件的材质
    void RunReadSelfMaterials();
    /// @brief 测试FBX.SDK中矩阵运算的主序
    /// @details 
    void RunTestFbxMatrixMajor();
    /// @brief 创建子父节点
    /// @details 创建的场景中，父节点具有几何信息，子节点也具有几何信息
    void RunCreateNode_ParentAndParentWithMesh();

public:
    /// @brief 获取EXE路径
    static std::string GetExePath();
    /// @breif 该节点是不是Mehs片元节点
    static bool IsMeshNode(FbxNode* pFbxNode);
    /// @brief 三角化
    static void TriangulateScene(FbxScene* pScene);

private: // Creating a material

    /// @brief Create a cube 
    /// @details 创建一个正方形
    FbxNode* CreateCube(FbxScene* pScene, const char* pName);
    /// @brief Create texture for cube.
    /// @details 为一个正方体创建一个材质
    void CreateCubeTexture(FbxScene* pScene, FbxMesh* pMesh);

    /// @brief Create a pyramid with materials.
    /// @details 创建一个金字塔
    FbxNode* CreatePyramid(FbxScene* pScene, const char* pName);
    /// @brief Create materials for pyramid.
    /// @details 为金字塔创建一个材质
    void CreatePyramidMaterials(FbxScene* pScene, FbxMesh* pMesh);

    /// @brief 创建一个三角形
    /// @details 
    FbxNode* CreateTriangle(FbxScene* pScene, const char* pName);

    /// @brief 创建八面体
    /// @details 
    void CreateOctahedron(FbxMesh* pMesh);

    /// @brief 导入Fbx文件
    /// @details 
    void ImportFbxFile(char* filePath);
    /// @brief 初始化SDK
    /// @details 
    void InitializeSdkObjects(FbxManager*& pManager, FbxScene*& pScene);
    /// @brief 创建非平面的几何
    /// @details 
    void CreateMesh_NoPlane(FbxMesh* pMesh);
    /// @brief 创建Mesh节点
    /// @details 创建一个节点属性是Mesh的节点
    FbxMesh* CreateFbxMeshNode();
    /// @brief 创建跟结点的子节点
    /// @details 创建一个带有子节点的节点
    FbxNode* CreateFbxNodeWithChildNode(FbxScene* pFbxScene);
    /// @brief 创建材质
    /// @details Print the required number of tabs.
    void PrintTabs();
    /// @brief Return a string-based representation based on the attribute type.
    /// @details 
    FbxString GetAttributeTypeName(FbxNodeAttribute::EType type);
    /// @brief Print an attribute.
    void PrintAttribute(FbxNodeAttribute* pAttribute);
    /// @brief Print a node, its attributes, and all its children recursively.
    void PrintNode(FbxNode* pNode);
    void PrintFbxVector(FbxVector2& fbxVector2);

private: // static

    /// @brief 如何查找材质
    static void FindMaterials(FbxScene* pScene, FbxUInt64 fbxUInt64);

private:
    std::string m_PyramidWithMaterials = "PyramidWithMaterials.fbx";

    int         numTabs = 0;
    FbxManager* m_pSdkManager;
    FbxScene*   m_pScene;
};
