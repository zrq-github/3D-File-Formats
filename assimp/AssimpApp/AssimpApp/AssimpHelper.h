#pragma once
#include <assimp/Importer.hpp>      // C++ importer interface
#include <assimp/Exporter.hpp>
#include <assimp/scene.h>           // Output data structure
#include <assimp/postprocess.h>     // Post processing flags

#include <algorithm>
#include <iostream>
#include <vector>

class AssimpHelper
{
public:
    /// @brief ���볡��
    /// @details  
    static const aiScene* ImportScene(Assimp::Importer& importer, std::string& filePath);

    /// @brief ��ӡ������һЩ������Ϣ
    /// @details 
    static void PrintScene(const aiScene* scene);

    /// @brief ��ӡ�ڵ���Ϣ�������ӽڵ㣩
    /// @details 
    static void PrintNodes(Assimp::Importer& importer, aiNode* node);

    /// @brief ��ӡ�ڵ��һЩ��Ϣ
    /// @details  
    static void PrintNode(Assimp::Importer& importer, aiNode* node);

    /// @brief ��ӡ�ڵ�������Ϣ
    /// @details 
    static void PrintNodeMeshes(const aiScene* scene, aiNode* node);

    /// @brief ��ӡ������Ϣ
    /// @details  
    static void PrintNodeMesh(const aiScene* scene, aiMesh* mesh);

    /// @brief ��ӡ atrix4x4
    /// @details 
    static void PrintMatrix4x4(const aiMatrix4x4& matrix4);

    /// @brief ��ӡ Vector3D
    /// @details 
    static void PrintVector3D(const aiVector3D& vector3D);

    /// @brief ����ת��
    /// @details 
    void static TransformVertices(aiMesh* mesh, const aiMatrix4x4& transformMatrix) {
        for (unsigned int j = 0; j < mesh->mNumVertices; ++j) {
            aiVector3D& vertex = mesh->mVertices[j];
            vertex = transformMatrix * vertex;
        }
    }

    /// @brief ת��ϵͳ������
    /// @details
    /// In this coordinate system, +X points to the right, +Y points upwards and +Z points out of the screen towards the viewer
    void static ConvertSystemAxis(const aiScene* scene)
    {
        // ����������ϵͳת�����������Է�תZ��Ϊ��
        aiMatrix4x4 coordinateTransform;
        coordinateTransform.a3 = -1.0f;  // ��תZ��

        // ����ģ���е�ÿһ�����񣬶����������ת��
        for (unsigned int i = 0; i < scene->mNumMeshes; ++i) {
            aiMesh* mesh = scene->mMeshes[i];
            TransformVertices(mesh, coordinateTransform);
        }
    }

    /// @brief �������ļ���ʽ 
    /// @details Ŀǰ��֧�ֵ������ļ���ʽ 
    std::vector<std::string> ExportingModels = 
    {
        "collada",
        "x",
        "stp",
        "obj",
        "objnomtl",
        "stl",
        "stlb",
        "ply",
        "plyb",
        "3ds",
        "gltf2",
        "glb2",
        "gltf",
        "glb",
        "assbin",
        "assxml",
        "x3d",
        "fbx",
        "fbxa",
        "m3d",
        "m3da",
        "3mf",
        "pbrt",
        "assjson"
    };

    // �ݹ�����ڵ������
    // https://assimp-docs.readthedocs.io/en/latest/usage/use_the_lib.html#the-node-hierarchy
    //void CopyNodesWithMeshes(aiNode node, SceneObject targetParent, Matrix4x4 accTransform) {
    //    SceneObject parent;
    //    Matrix4x4 transform;

    //    // if node has meshes, create a new scene object for it
    //    if (node.mNumMeshes > 0) {
    //        SceneObjekt newObject = new SceneObject;
    //        targetParent.addChild(newObject);
    //        // copy the meshes
    //        CopyMeshes(node, newObject);

    //        // the new object is the parent for all child nodes
    //        parent = newObject;
    //        transform.SetUnity();
    //    }
    //    else {
    //        // if no meshes, skip the node, but keep its transformation
    //        parent = targetParent;
    //        transform = node.mTransformation * accTransform;
    //    }

    //    // continue for all child nodes
    //    for (all node.mChildren) {
    //        CopyNodesWithMeshes(node.mChildren[a], parent, transform);
    //    }
    //}

    bool exporterTest() {
        ::Assimp::Importer importer;
        ::Assimp::Exporter exporter;
        const aiScene* scene = importer.ReadFile("/OBJ/spider.obj", aiProcess_ValidateDataStructure);
        exporter.Export(scene, "obj","/OBJ/spider_out.obj");
        return true;
    }

    bool DoTheImportThing(const std::string& pFile) {
        // Create an instance of the Importer class
        Assimp::Importer importer;

        // And have it read the given file with some example postprocessing
        // Usually - if speed is not the most important aspect for you - you'll
        // probably to request more postprocessing than we do in this example.
        const aiScene* scene = importer.ReadFile(pFile,
            aiProcess_CalcTangentSpace |
            aiProcess_Triangulate |
            aiProcess_JoinIdenticalVertices |
            aiProcess_SortByPType);

        // If the import failed, report it
        if (nullptr == scene) {
            //DoTheErrorLogging(importer.GetErrorString());
            return false;
        }

        // Now we can access the file's contents.
        //DoTheSceneProcessing(scene);

        // We're done. Everything will be cleaned up by the importer destructor
        return true;
    }
};

