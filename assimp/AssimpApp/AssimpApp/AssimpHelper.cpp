#include "AssimpHelper.h"

const aiScene* AssimpHelper::ImportScene(Assimp::Importer& importer, std::string& filePath)
{
    unsigned int flags = 0;
    // 导入的时候就三角化
    flags = flags | aiProcess_Triangulate;
    flags = flags | aiProcess_FlipUVs;
    flags = flags | aiProcess_GenNormals;

    // 不知道这个标记干什么，参考 aiFace
    // If the #AI_SCENE_FLAGS_NON_VERBOSE_FORMAT is NOT set each face references an unique set of vertices.
    //flags = flags | AI_SCENE_FLAGS_NON_VERBOSE_FORMAT;


    // 先试用默认的测试
    const aiScene* scene = importer.ReadFile(filePath, flags);

    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
        std::cerr << "Error loading model: " << importer.GetErrorString() << std::endl;
        return nullptr;
    }

    return scene;
}

void AssimpHelper::PrintScene(const aiScene* scene)
{
    // 节点数量
    aiNode* rootNode = scene->mRootNode;
    auto numChildren = rootNode->mNumChildren;
    std::cout << "numChildren: " << numChildren << std::endl;
    // 获取网格
    aiMesh** meshes = scene->mMeshes;
    auto numMeshes = scene->mNumMeshes;
    std::cout << "numMeshes: " << numMeshes << std::endl;
    // 获取材质
    aiMaterial** materials = scene->mMaterials;
    auto numMaterials = scene->mNumMaterials;
    std::cout << "numMaterials: " << numMaterials << std::endl;
}

void AssimpHelper::PrintNodes(Assimp::Importer& importer, aiNode* node)
{
    PrintNode(importer, node);

    auto& numChildren = node->mNumChildren;
    if(numChildren == 0)
        return;

    for (int i=0;i<numChildren;++i)
    {
        aiNode* childNode = node->mChildren[i];
        PrintNodes(importer, childNode);
    }
}

void AssimpHelper::PrintNode(Assimp::Importer& importer,aiNode* node)
{
    aiString nodeName = node->mName;
    std::cout << "nodeName: " << nodeName.C_Str() << std::endl;

    auto numChildren = node->mNumChildren;
    std::cout << "numChildren: " << numChildren << std::endl;

    auto numMeshes = node->mNumMeshes;
    std::cout << "numMeshes: " << numMeshes << std::endl;

    const aiMatrix4x4& matrix4X4 = node->mTransformation;
    PrintMatrix4x4(matrix4X4);

    const aiScene* scene = importer.GetScene();
    PrintNodeMeshes(scene, node);
}

void AssimpHelper::PrintNodeMeshes(const aiScene* scene, aiNode* node)
{
    aiMesh** meshes = scene->mMeshes;

    auto& numMeshes = node->mNumMeshes;
    unsigned int* meshIndexes = node->mMeshes;

    for (int i=0;i<numMeshes;++i)
    {
        unsigned int meshIndex = meshIndexes[i];
        aiMesh* mesh = meshes[meshIndex];
        AssimpHelper::PrintNodeMesh(scene,mesh);
    }
}

void AssimpHelper::PrintNodeMesh(const aiScene* scene, aiMesh* mesh)
{
    aiString meshName = mesh->mName;
    std::cout << "meshName: " << meshName.C_Str() << std::endl;

    if (!mesh->HasPositions())
    {
        std::cerr << "meh is no has positions" << std::endl;
        return;
    }

    auto& numVertices = mesh->mNumVertices;
    auto& vertices = mesh->mVertices;
    std::cout << "numVertices: " << numVertices << std::endl;

    aiFace* faces = mesh->mFaces;
    auto& numFaces = mesh->mNumFaces;
    std::cout << "numFaces: " << numFaces << std::endl;

    for (int i=0;i<numFaces;++i)
    {
        aiFace& face = faces[i];

        auto& faceIndices = face.mIndices;
        auto& faceNumIndices = face.mNumIndices;

        auto& faceIndex0 = faceIndices[0];
        auto& faceIndex1 = faceIndices[1];
        auto& faceIndex2 = faceIndices[2];

        auto& vertex0 = vertices[faceIndex0];
        auto& vertex1 = vertices[faceIndex1];
        auto& vertex2 = vertices[faceIndex2];
        AssimpHelper::PrintVector3D(vertex0);
        AssimpHelper::PrintVector3D(vertex1);
        AssimpHelper::PrintVector3D(vertex2);
    }
}

void AssimpHelper::PrintMatrix4x4(const aiMatrix4x4& matrix4)
{
    std::cout << "Matrix" << std::endl;

    auto& a1 = matrix4.a1;
    auto& a2 = matrix4.a2;
    auto& a3 = matrix4.a3;
    auto& a4 = matrix4.a4;
    std::cout << a1 << "  " << a2 << "  " << a3 << "  " << a4 << std::endl;

    auto& b1 = matrix4.b1;
    auto& b2 = matrix4.b2;
    auto& b3 = matrix4.b3;
    auto& b4 = matrix4.b4;
    std::cout << b1 << "  " << b2 << "  " << b3 << "  " << b4 << std::endl;

    auto& c1 = matrix4.c1;
    auto& c2 = matrix4.c2;
    auto& c3 = matrix4.c3;
    auto& c4 = matrix4.c4;
    std::cout << c1 << "  " << c2 << "  " << c3 << "  " << c4 << std::endl;

    auto& d1 = matrix4.d1;
    auto& d2 = matrix4.d2;
    auto& d3 = matrix4.d3;
    auto& d4 = matrix4.d4;
    std::cout << d1 << "  " << d2 << "  " << d3 << "  " << d4 << std::endl;
}

void AssimpHelper::PrintVector3D(const aiVector3D& vector3D)
{
    std::cout << vector3D.x << "  " << vector3D.y << "  " << vector3D.z << std::endl;
}
