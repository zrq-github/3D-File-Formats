#include "FbxSdkHelp.h"

#include <fbxsdk.h>

#include <Windows.h>
#include <string>

FbxSdkHelp::FbxSdkHelp()
{
    InitializeSdkObjects(m_pSdkManager, m_pScene);
}

FbxSdkHelp::~FbxSdkHelp()
{
    m_pSdkManager->Destroy();
}

void FbxSdkHelp::RunMyFirstProgrammer()
{
    // Change the following filename to a suitable filename value.
    auto lFilename = R"(C:\Users\zrq\Desktop\FBX\FbxTest\rvt_square_property.fbx)";

    // Initialize the SDK manager. This object handles all our memory management.
    FbxManager* lSdkManager = FbxManager::Create();

    // Create the IO settings object.
    FbxIOSettings* ios = FbxIOSettings::Create(lSdkManager, IOSROOT);
    lSdkManager->SetIOSettings(ios);

    // Create an importer using the SDK manager.
    FbxImporter* lImporter = FbxImporter::Create(lSdkManager, "");

    // Use the first argument as the filename for the importer.
    if (!lImporter->Initialize(lFilename, -1, lSdkManager->GetIOSettings()))
    {
        printf("Call to FbxImporter::Initialize() failed.\n");
        printf("Error returned: %s\n\n", lImporter->GetStatus().GetErrorString());
        exit(-1);
    }

    // Create a new scene so that it can be populated by the imported file.
    FbxScene* lScene = FbxScene::Create(lSdkManager, "myScene");

    // Import the contents of the file into the scene.
    lImporter->Import(lScene);

    // The file is imported; so get rid of the importer.
    lImporter->Destroy();

    // Print the nodes of the scene and their attributes recursively.
    // Note that we are not printing the root node because it should
    // not contain any attributes.
    FbxNode* lRootNode = lScene->GetRootNode();
    if (lRootNode)
    {
        for (int i = 0; i < lRootNode->GetChildCount(); i++)
            PrintNode(lRootNode->GetChild(i));
    }
    // Destroy the SDK manager and all the other objects it was handling.
    lSdkManager->Destroy();
}

void FbxSdkHelp::RunAddFbxNode()
{
    // Create the scene.
    FbxScene* pScene = FbxScene::Create(m_pSdkManager, "Scene Name");
    // Get the root node of the scene.
    FbxNode* pRootNode = pScene->GetRootNode();
    // Create a child node.
    FbxNode* pChild = FbxNode::Create(pScene, "child");
    // Add the child to the root node.
    pRootNode->AddChild(pChild);

    ExportScene(pScene);
}

void FbxSdkHelp::RunFbxProperty()
{
    FbxScene* pScene = FbxScene::Create(m_pSdkManager, "Scene FbxProperty");
    FbxNode* pRootNode = pScene->GetRootNode();
    FbxNode* pNode = FbxNode::Create(pScene, "node_property");
    pRootNode->AddChild(pNode);

    FbxProperty p = FbxProperty::Create(pScene, FbxDouble3DT, "Vector3Property");
    p.Set(FbxDouble3(1.1, 2.2, 3.3));

    ExportScene(pScene);
}

void FbxSdkHelp::RunCreateMeshes()
{
    FbxScene* pScene = FbxScene::Create(m_pSdkManager, "Scene CreateMeshes");
    FbxNode* pRootNode = pScene->GetRootNode();
    FbxNode* lMeshNode = FbxNode::Create(pScene, "meshNode");
    pRootNode->AddChild(lMeshNode);

    // Create a mesh.
    FbxMesh* lMesh = FbxMesh::Create(pScene, "mesh");
    // Set the node attribute of the mesh node.
    lMeshNode->SetNodeAttribute(lMesh);

    CreateOctahedron(lMesh);
    ExportScene(pScene);
}

void FbxSdkHelp::RunCreateMaterials()
{
    FbxNode* pNode = CreatePyramid(m_pScene, "mesh_materials");
    CreatePyramidMaterials(m_pScene, pNode->GetMesh());

    m_pScene->GetRootNode()->AddChild(pNode);

    auto saveFilePath = GetExePath().append("\\").append(m_PyramidWithMaterials);
    ExportScene(m_pScene, saveFilePath);
}

void FbxSdkHelp::RunReadMaterials()
{
    auto filePath = GetExePath().append("\\").append(m_PyramidWithMaterials);
    const char* lFilename = filePath.c_str();

    //std::string filePath = R"(D:\source\repos\Hongwa\Dazzle\Public\ModelExchangeDirector\branches\v3.0.0\bin\RelWithDebInfo\models\c9214798-0f48-4622-ae2a-39121fb06c6e\PyramidWithMaterials.fbx)";
    //const char* lFilename = filePath.c_str();

    FbxImporter* lImporter = FbxImporter::Create(m_pSdkManager, "");
    bool lImportStatus = lImporter->Initialize(lFilename, -1, m_pSdkManager->GetIOSettings());
    if (!lImportStatus)
    {
        printf("Call to FbxImporter::Initialize() failed.\n");
        printf("Error returned: %s\n\n", lImporter->GetStatus().GetErrorString());
        exit(-1);
    }

    // Import the contents of the file into the scene.
    lImporter->Import(m_pScene);
    // The file has been imported; we can get rid of the importer.
    lImporter->Destroy();

    auto sceneNodeCount = m_pScene->GetNodeCount();
    auto rootNode = m_pScene->GetRootNode();
    for (auto i = 0; i < rootNode->GetChildCount(); i++)
    {
        auto node = rootNode->GetChild(i);
        if (!IsMeshNode(node))
            continue;

        auto nodeChildCount = node->GetChildCount();
        auto pNodeMesh = node->GetMesh();
        auto polygonCount = pNodeMesh->GetPolygonCount();

        // 遍历Mesh节点的多边形
        for (int polygonIndex = 0; polygonIndex < polygonCount; ++polygonIndex)
        {
            auto elementMaterial0 = pNodeMesh->GetElementMaterial(0);
            auto elementMaterial1 = pNodeMesh->GetElementMaterial(1);
            auto elementMaterial2 = pNodeMesh->GetElementMaterial(2);
            auto elementMaterial3 = pNodeMesh->GetElementMaterial(3);

            // 获取当前多边形的材质索引
            int materialIndex = pNodeMesh->GetElementMaterial()->GetIndexArray().GetAt(polygonIndex);
            FbxSurfaceMaterial* pMaterial = node->GetMaterial(materialIndex);

            auto aa = pMaterial;
        }

        // 该节点所拥有的全部材质
        auto materialCount = node->GetMaterialCount();
        for (int i = 0; i < materialCount; ++i)
        {
            FbxSurfaceMaterial* pMaterial = node->GetMaterial(i);

            // 一种取法
            auto lBlack = pMaterial->FindProperty(FbxSurfaceMaterial::sEmissive).Get<FbxDouble3>();
            auto lRed = pMaterial->FindProperty(FbxSurfaceMaterial::sAmbient).Get<FbxDouble3>();
            auto iColor = pMaterial->FindProperty(FbxSurfaceMaterial::sDiffuse).Get<FbxDouble3>();
            auto transparencyFactor = pMaterial->FindProperty(FbxSurfaceMaterial::sTransparencyFactor).Get<FbxDouble>();
            auto shadingModel = pMaterial->FindProperty(FbxSurfaceMaterial::sShadingModel).Get<FbxString>();
            auto shininess = pMaterial->FindProperty(FbxSurfaceMaterial::sShininess).Get<FbxDouble>();

            auto pSurfacePhong = FbxCast<FbxSurfacePhong>(pMaterial);
            if (pSurfacePhong == nullptr)
            {
                continue;
            }
            auto emissivePhong = pSurfacePhong->Emissive.Get();
            auto ambientPhong = pSurfacePhong->Ambient.Get();
            auto diffusePhong = pSurfacePhong->Diffuse.Get();
            auto transparencyFactorPhong = pSurfacePhong->TransparencyFactor.Get();
            auto shadingModelPhong = pSurfacePhong->ShadingModel.Get();
            auto shininessPhone = pSurfacePhong->Shininess.Get();

            auto nothing = pSurfacePhong->DisplacementColor.Get();
        }
    }
}

void FbxSdkHelp::RunTriangulateScene()
{
    auto filePath = GetExePath().append("\\").append(m_PyramidWithMaterials);

    // Create the FBX SDK manager
    FbxManager* lSdkManager = m_pSdkManager;
    // Create an IOSettings object.
    FbxIOSettings* ios = FbxIOSettings::Create(lSdkManager, IOSROOT);
    lSdkManager->SetIOSettings(ios);
    // Create an importer.
    FbxImporter* lImporter = FbxImporter::Create(lSdkManager, "");
    // Declare the path and filename of the file containing the scene.
    // In this case, we are assuming the file is in the same directory as the executable.
    const char* lFilename = filePath.c_str();
    // Initialize the importer.
    bool lImportStatus = lImporter->Initialize(lFilename, -1, lSdkManager->GetIOSettings());
    if (!lImportStatus)
    {
        printf("Call to FbxImporter::Initialize() failed.\n");
        printf("Error returned: %s\n\n", lImporter->GetStatus().GetErrorString());
        exit(-1);
    }

    // Create a new scene so it can be populated by the imported file.
    FbxScene* lScene = FbxScene::Create(lSdkManager, "myScene");
    // Import the contents of the file into the scene.
    lImporter->Import(lScene);
    lImporter->Destroy();

    FindMaterials(lScene, 0);

    FbxGeometryConverter geometryConverter(lScene->GetFbxManager());
    if (lScene)
    {
        geometryConverter.Triangulate(lScene, true);
        //geometryConverter.SplitMeshesPerMaterial(lScene, true);
    }

    auto sceneNodeCount = lScene->GetNodeCount();
    auto rootNode = lScene->GetRootNode();
    for (auto i = 0; i < rootNode->GetChildCount(); i++)
    {
        auto node = rootNode->GetChild(i);

        auto nodeChildCount = node->GetChildCount();
        auto materialCount = node->GetMaterialCount();
        auto pNodeMesh = node->GetMesh();
        if (nullptr == pNodeMesh)
        {
            continue;
        }

        auto meshNodeCount = pNodeMesh->GetNodeCount();

        // 遍历Mesh节点的多边形
        for (int polygonIndex = 0; polygonIndex < pNodeMesh->GetPolygonCount(); ++polygonIndex)
        {
            // 获取当前多边形的材质索引
            int materialIndex = pNodeMesh->GetElementMaterial()->GetIndexArray().GetAt(polygonIndex);
            FbxSurfaceMaterial* pMaterial = node->GetMaterial(materialIndex);
            auto materialUniqueId = pMaterial->GetUniqueID();
            printf(std::to_string(materialUniqueId).c_str());
        }
    }

    ExportScene(lScene, GetExePath() + "\\PyramidWithMaterials_Triangulate_SplitMeshesPerMaterial.fbx");
}

void FbxSdkHelp::RunCreateMeshNoPlane()
{
    FbxScene* pScene = FbxScene::Create(m_pSdkManager, "Scene CreateMeshes");
    FbxNode* pRootNode = pScene->GetRootNode();
    FbxNode* lMeshNode = FbxNode::Create(pScene, "meshNode");
    pRootNode->AddChild(lMeshNode);

    // Create a mesh.
    FbxMesh* lMesh = FbxMesh::Create(pScene, "mesh");
    // Set the node attribute of the mesh node.
    lMeshNode->SetNodeAttribute(lMesh);

    CreateMesh_NoPlane(lMesh);

    ExportScene(pScene, GetExePath().append("MeshNoPlane.fbx"));
}

void FbxSdkHelp::RunCreateCubeWithTexture()
{
    FbxNode* pFbxNode = CreateCube(m_pScene, (char*)"cube");
    CreateCubeTexture(m_pScene, pFbxNode->GetMesh());

    auto pRootNode = m_pScene->GetRootNode();
    pRootNode->AddChild(pFbxNode);

    // 三角化后再执行输出
    FbxGeometryConverter geometryConverter(m_pScene->GetFbxManager());
    if (m_pScene)
    {
        geometryConverter.Triangulate(m_pScene, true);
    }
    ExportScene(m_pScene, GetExePath().append("\\CubeWithTexture.fbx"));
}

void FbxSdkHelp::RunReadCubeWithTexture()
{
    auto filePath = GetExePath().append("\\CubeWithTexture.fbx");
    ImportFbxFile((char*)filePath.c_str());

    FbxGeometryConverter geometryConverter(m_pScene->GetFbxManager());
    try
    {
        geometryConverter.Triangulate(m_pScene, true);
    }
    catch (std::runtime_error)
    {
    }

    auto rootNode = m_pScene->GetRootNode();
    for (auto i = 0; i < rootNode->GetChildCount(); i++)
    {
        auto node = rootNode->GetChild(i);
        if (!IsMeshNode(node))
            continue;

        auto pNodeMesh = node->GetMesh();
        auto elementNormalCount = pNodeMesh->GetElementNormalCount();
        if(elementNormalCount != 0)
        {
            auto elementNormal = pNodeMesh->GetElementNormal(0);
            FbxLayerElement::EMappingMode  mappingMode = elementNormal->GetMappingMode();
        }

        auto polygonCount = pNodeMesh->GetPolygonCount();
        auto polygonVertexCount = pNodeMesh->GetPolygonVertexCount();
        auto controlPointsCount = pNodeMesh->GetControlPointsCount();
        auto textureUVCount = pNodeMesh->GetTextureUVCount();

        // 该节点所拥有的全部材质
        auto materialCount = node->GetMaterialCount();
        for (int i = 0; i < materialCount; ++i)
        {
            FbxSurfaceMaterial* pMaterial = node->GetMaterial(i);
            if (nullptr == pMaterial) continue;

            FbxProperty diffuseMaterial = pMaterial->FindProperty(FbxSurfaceMaterial::sDiffuse);
            if (diffuseMaterial.IsValid())
            {
                int numTextures = diffuseMaterial.GetSrcObjectCount<FbxFileTexture>();
                for (int j = 0; j < numTextures; ++j)
                {
                    FbxFileTexture* texture = diffuseMaterial.GetSrcObject<FbxFileTexture>(j);
                    if(!texture)
                    {
                        continue;
                    }

                    // 设置Unifiedbitmap_Invert属性
                    auto Unifiedbitmap_Invert = texture->GetSwapUV();

                    // 设置Texture_URepeat和Texture_VRepeat属性
                    auto Texture_URepeat = texture->GetWrapModeU() == FbxTexture::eRepeat;
                    auto Texture_VRepeat = texture->GetWrapModeV() == FbxTexture::eRepeat;

                    // 设置其他属性
                    auto Texture_UScale = texture->GetScaleU();
                    auto Texture_VScale = texture->GetScaleV();
                    auto Texture_UOffset = texture->GetTranslationU();
                    auto Texture_VOffset = texture->GetTranslationV();
                    auto Texture_WAngle = texture->GetRotationW();

                    // 处理纹理数据
                    const char* textureFileName = texture->GetFileName();
                    // 在这里执行你的操作，比如输出文件名
                    printf("Texture File Name: %s\n", textureFileName);
                }
            }

            // 一种取法
            auto lBlack = pMaterial->FindProperty(FbxSurfaceMaterial::sEmissive).Get<FbxDouble3>();
            auto lRed = pMaterial->FindProperty(FbxSurfaceMaterial::sAmbient).Get<FbxDouble3>();
            auto iColor = pMaterial->FindProperty(FbxSurfaceMaterial::sDiffuse).Get<FbxDouble3>();
            auto transparencyFactor = pMaterial->FindProperty(FbxSurfaceMaterial::sTransparencyFactor).Get<FbxDouble>();
            auto shadingModel = pMaterial->FindProperty(FbxSurfaceMaterial::sShadingModel).Get<FbxString>();
            auto shininess = pMaterial->FindProperty(FbxSurfaceMaterial::sShininess).Get<FbxDouble>();

            auto pSurfacePhong = FbxCast<FbxSurfacePhong>(pMaterial);
            if (pSurfacePhong == nullptr)
            {
                continue;
            }
            auto emissivePhong = pSurfacePhong->Emissive.Get();
            auto ambientPhong = pSurfacePhong->Ambient.Get();
            auto diffusePhong = pSurfacePhong->Diffuse.Get();
            auto transparencyFactorPhong = pSurfacePhong->TransparencyFactor.Get();
            auto shadingModelPhong = pSurfacePhong->ShadingModel.Get();
            auto shininessPhone = pSurfacePhong->Shininess.Get();

            auto nothing = pSurfacePhong->DisplacementColor.Get();
        }

        // 获取UV坐标
        FbxLayerElementUV* uvElement = pNodeMesh->GetElementUV();
        if (uvElement)
        {
            FbxLayerElement::EMappingMode mappingMode = uvElement->GetMappingMode();
            FbxLayerElement::EReferenceMode referenceMode = uvElement->GetReferenceMode();

            // 获取UV坐标数组
            FbxLayerElementArrayTemplate<FbxVector2>& uvArray = uvElement->GetDirectArray();
            int numUVs = uvArray.GetCount();
            for (int k = 0; k < numUVs; ++k)
            {
                FbxVector2 uvCoord = uvArray[k];
                // 在这里执行你的操作，比如输出UV坐标
                printf("UV Coord: (%f, %f)\n", uvCoord[0], uvCoord[1]);
            }
        }
        // 获取UV坐标, 另外一种取法
        FbxLayerElementArrayTemplate<FbxVector2>* vector2Array = new FbxLayerElementArrayTemplate<FbxVector2>(EFbxType::eFbxDouble2);
        FbxLayerElementArrayTemplate<FbxVector2>** arrayPtr = &vector2Array;
        pNodeMesh->GetTextureUV(arrayPtr);
        if (arrayPtr && *arrayPtr)
        {
            FbxLayerElementArrayTemplate<FbxVector2>* arrayTemplate = *arrayPtr;

            int count = arrayTemplate->GetCount();
            for (int i = 0; i < count; ++i)
            {
                FbxVector2 element = arrayTemplate->GetAt(i);

                // 在这里执行你的操作，比如输出元素值
                printf("Element %d: (%f, %f)\n", i, element[0], element[1]);
            }
        }

        for (int polygonIndex=0;polygonIndex<polygonCount;++polygonIndex)
        {
            auto polygonSize = pNodeMesh->GetPolygonSize(polygonIndex);
            auto uvIndex = pNodeMesh->GetTextureUVIndex(polygonIndex, polygonIndex);
            auto fbxVector2 = vector2Array->GetAt(uvIndex);
            PrintFbxVector(fbxVector2);
        }
    }
}

void FbxSdkHelp::ExportScene(FbxScene* pFbxScene, std::string saveFilePath)
{
    if (saveFilePath.empty())
    {
        saveFilePath = GetExePath() + "export_scene.fbx";
    }

    auto pSdkManager = m_pSdkManager;
    FbxExporter* pExporter = FbxExporter::Create(pSdkManager, "");
    pSdkManager->GetIOSettings()->SetBoolProp(EXP_FBX_EMBEDDED, false);

    // pFileFormat = 1 是文本文件
    auto pFileFormat = -1;
    if (pFileFormat < 0 || pFileFormat >= pSdkManager->GetIOPluginRegistry()->GetWriterFormatCount())
    {
        // Write in fall back format in less no ASCII format found
        pFileFormat = pSdkManager->GetIOPluginRegistry()->GetNativeWriterFormat();

        //Try to export in ASCII if possible
        int lFormatIndex, lFormatCount = pSdkManager->GetIOPluginRegistry()->GetWriterFormatCount();

        for (lFormatIndex = 0; lFormatIndex < lFormatCount; lFormatIndex++)
        {
            if (pSdkManager->GetIOPluginRegistry()->WriterIsFBX(lFormatIndex))
            {
                FbxString lDesc = pSdkManager->GetIOPluginRegistry()->GetWriterFormatDescription(lFormatIndex);
                auto lASCII = "ascii";
                if (lDesc.Find(lASCII) >= 0)
                {
                    pFileFormat = lFormatIndex;
                    break;
                }
            }
        }
    }

    // Initialize the exporter by providing a filename.
    if (pExporter->Initialize(saveFilePath.c_str(), -1, pSdkManager->GetIOSettings()) == false)
    {
        FBXSDK_printf("Call to FbxExporter::Initialize() failed.\n");
        FBXSDK_printf("Error returned: %s\n\n", pExporter->GetStatus().GetErrorString());
    }
    // Export the scene to the file.
    bool isExport = pExporter->Export(pFbxScene);

    pExporter->Destroy();
}

void FbxSdkHelp::RunReadSelfMaterials()
{
    //std::string filePath = "D:\\source\\repos\\Hongwa\\Dazzle\\Public\\ModelExchangeDirector\\branches\\v3.0.0\\bin\\RelWithDebInfo\\models\\291b2401-3811-464f-9528-cfa0513551fd\\FBX_ALL.fbx";
    //std::string filePath = "D:\\source\\repos\\FBXSDK\\MyTest\\FbxTest\\x64\\Debug\\CubeWithTexture.fbx";

    std::string filePath = "D:\\source\\repos\\FBXSDK\\MyTest\\FbxTest\\x64\\Debug\\PyramidWithMaterials.fbx";

    ImportFbxFile((char*)filePath.c_str());

    FbxGeometryConverter geometryConverter(m_pScene->GetFbxManager());
    try
    {
        geometryConverter.Triangulate(m_pScene, true);
    }
    catch (std::runtime_error)
    {
    }

    FbxNode* rootNode = m_pScene->GetRootNode();
    int childCount = rootNode->GetChildCount();
    for (auto i = 0; i < childCount; i++)
    {
        auto node = rootNode->GetChild(i);
        if (!IsMeshNode(node))
            continue;

        auto pMesh = node->GetMesh();


        int nodeMaterialCount = node->GetMaterialCount();
        auto elementMaterialCount = pMesh->GetElementMaterialCount();

        bool lIsAllSame = true;
        for (int l = 0; l < pMesh->GetElementMaterialCount(); l++)
        {
            FbxGeometryElementMaterial* lMaterialElement = pMesh->GetElementMaterial(l);
            if (lMaterialElement->GetMappingMode() == FbxGeometryElement::eByPolygon)
            {
                lIsAllSame = false;
                break;
            }
        }

        int lPolygonCount = pMesh->GetPolygonCount();

        //For eAllSame mapping type, just out the material and texture mapping info once
        if (lIsAllSame)
        {
            int l = 0;

            for (int l = 0; l < pMesh->GetElementMaterialCount(); l++)
            {

                FbxGeometryElementMaterial* lMaterialElement = pMesh->GetElementMaterial(l);
                if (lMaterialElement->GetMappingMode() == FbxGeometryElement::eAllSame)
                {
                    FbxSurfaceMaterial* lMaterial = pMesh->GetNode()->GetMaterial(lMaterialElement->GetIndexArray().GetAt(0));
                    int lMatId = lMaterialElement->GetIndexArray().GetAt(0);
                    if (lMatId >= 0)
                    {
     
                    }
                }
            }

            //no material
            if (l == 0)
                printf("        no material applied");
        }


        //For eByPolygon mapping type, just out the material and texture mapping info once
        else
        {
            for (i = 0; i < lPolygonCount; i++)
            {
                for (int l = 0; l < pMesh->GetElementMaterialCount(); l++)
                {

                    FbxGeometryElementMaterial* lMaterialElement = pMesh->GetElementMaterial(l);
                    FbxSurfaceMaterial* lMaterial = NULL;
                    int lMatId = -1;
                    lMaterial = pMesh->GetNode()->GetMaterial(lMaterialElement->GetIndexArray().GetAt(i));
                    lMatId = lMaterialElement->GetIndexArray().GetAt(i);

                    printf_s(std::to_string(lMatId).c_str());

                    if (lMatId >= 0)
                    {
                        //DisplayMaterialTextureConnections(lMaterial, header, lMatId, l);
                    }
                }
            }
        }
    }
}

void FbxSdkHelp::RunTestFbxMatrixMajor()
{
    // 定义平移、旋转、缩放向量
    FbxVector4 translation(0, 0, 0);
    FbxVector4 rotation(-90.0, 0, 0.0);  // 绕Y轴旋转90度
    FbxVector4 scaling(1.0, 1.0, 2.0);

    FbxAMatrix matrix;
    // 设置平移部分
    matrix.SetT(translation);
    // 设置旋转部分（绕Y轴旋转）
    matrix.SetR(rotation);
    // 设置缩放部分
    matrix.SetS(scaling);

    FbxVector4 vertex0(1, 1, 1);
    
    FbxVector4 row0 = matrix.GetRow(0);
    FbxVector4 row1 = matrix.GetRow(1);
    FbxVector4 row2 = matrix.GetRow(1);
    FbxVector4 row3 = matrix.GetRow(3);

    FbxVector4 column0 = matrix.GetColumn(0);
    FbxVector4 column1 = matrix.GetColumn(1);
    FbxVector4 column2 = matrix.GetColumn(2);
    FbxVector4 column3 = matrix.GetColumn(3);

    for(int i=0;i<4;i++)
    {
        FbxVector4 row = matrix.GetRow(i);

        for (int j=0;j<4;j++)
        {
            printf("%f ", row[j]);
        }
        printf("\n");
    }
}

void FbxSdkHelp::RunCreateNode_ParentAndParentWithMesh()
{
    // 获取根节点
    FbxNode* rootNode = m_pScene->GetRootNode();

    FbxNode* cubeNode = CreateCube(m_pScene, "Cube_Center");
    FbxNode* pyramidNode = CreatePyramid(m_pScene, "Pyramid");
    FbxNode* triangle = CreateTriangle(m_pScene, "Triangle");

    rootNode->AddChild(cubeNode);
    cubeNode->AddChild(pyramidNode);
    cubeNode->AddChild(triangle);
    pyramidNode->LclTranslation.Set(FbxVector4(150, 0, 0));
    triangle->LclTranslation.Set(FbxVector4(300, 0, 0));

    FbxNode* cubeNode0 = CreateCube(m_pScene, "Cube_0");
    FbxNode* cubeNode1 = CreateCube(m_pScene, "Cube_1");
    cubeNode->AddChild(cubeNode0);
    cubeNode0->AddChild(cubeNode1);
    cubeNode0->LclTranslation.Set(FbxVector4(0, 150, 0));
    cubeNode1->LclTranslation.Set(FbxVector4(0, 300, 0));

    ExportScene(m_pScene, GetExePath() + "\\ParentAndParentWithMesh.fbx");
}

FbxMesh* FbxSdkHelp::CreateFbxMeshNode()
{
    FbxNode* pRootNode = m_pScene->GetRootNode();

    FbxNode* pNode = FbxNode::Create(m_pScene, "node:" + pRootNode->GetChildCount());
    pRootNode->AddChild(pNode);

    FbxMesh* pMesh = FbxMesh::Create(m_pScene, "mesh");
    pNode->SetNodeAttribute(pMesh);

    return pMesh;
}

FbxNode* FbxSdkHelp::CreateFbxNodeWithChildNode(FbxScene* pFbxScene)
{
    auto pRotNode = pFbxScene->GetRootNode();
    auto pChild = FbxNode::Create(pFbxScene, "child");
    pRotNode->AddChild(pChild);
    return pChild;
}

void FbxSdkHelp::CreateOctahedron(FbxMesh* pMesh)
{
    // 创建八面体的顶点和索引
    FbxVector4 vertices[12] = {
        FbxVector4(0, 0, 0),
        FbxVector4(10, 0, 0),
        FbxVector4(15, 5, 0),
        FbxVector4(10, 10, 0),
        FbxVector4(0, 10, 0),
        FbxVector4(-5, 5, 0),

        FbxVector4(0, 0, 30),
        FbxVector4(10, 0, 30),
        FbxVector4(15, 5, 30),
        FbxVector4(10, 10, 30),
        FbxVector4(0, 10, 30),
        FbxVector4(-5, 5, 30),
    };

    // 设置顶点和索引
    pMesh->InitControlPoints(12);
    for (int i = 0; i < 12; ++i)
    {
        pMesh->SetControlPointAt(vertices[i], i);
    }

    pMesh->BeginPolygon();
    pMesh->AddPolygon(0);
    pMesh->AddPolygon(1);
    pMesh->AddPolygon(2);
    pMesh->AddPolygon(3);
    pMesh->AddPolygon(4);
    pMesh->AddPolygon(5);
    pMesh->EndPolygon();

    pMesh->BeginPolygon();
    pMesh->AddPolygon(6);
    pMesh->AddPolygon(7);
    pMesh->AddPolygon(8);
    pMesh->AddPolygon(9);
    pMesh->AddPolygon(10);
    pMesh->AddPolygon(11);
    pMesh->EndPolygon();

    pMesh->BeginPolygon();
    pMesh->AddPolygon(0);
    pMesh->AddPolygon(1);
    pMesh->AddPolygon(7);
    pMesh->AddPolygon(6);
    pMesh->EndPolygon();

    pMesh->BeginPolygon();
    pMesh->AddPolygon(1);
    pMesh->AddPolygon(2);
    pMesh->AddPolygon(8);
    pMesh->AddPolygon(7);
    pMesh->EndPolygon();

    pMesh->BeginPolygon();
    pMesh->AddPolygon(2);
    pMesh->AddPolygon(3);
    pMesh->AddPolygon(9);
    pMesh->AddPolygon(8);
    pMesh->EndPolygon();

    pMesh->BeginPolygon();
    pMesh->AddPolygon(3);
    pMesh->AddPolygon(4);
    pMesh->AddPolygon(10);
    pMesh->AddPolygon(9);
    pMesh->EndPolygon();

    pMesh->BeginPolygon();
    pMesh->AddPolygon(4);
    pMesh->AddPolygon(5);
    pMesh->AddPolygon(11);
    pMesh->AddPolygon(10);
    pMesh->EndPolygon();

    pMesh->BeginPolygon();
    pMesh->AddPolygon(5);
    pMesh->AddPolygon(0);
    pMesh->AddPolygon(6);
    pMesh->AddPolygon(11);
    pMesh->EndPolygon();
}

void FbxSdkHelp::CreatePyramidMaterials(FbxScene* pScene, FbxMesh* pMesh)
{
    int i;
    int defaultIndex = 5;
    for (i = 0; i < 5; i++)
    {
        FbxString lMaterialName = "material";
        FbxString lShadingName = "Phong";
        lMaterialName += i;
        FbxDouble3 lBlack(0.0, 0.0, 0.0);
        FbxDouble3 lRed(1.0, 0.0, 0.0);
        FbxDouble3 lColor;
        FbxSurfacePhong* lMaterial = FbxSurfacePhong::Create(pScene, lMaterialName.Buffer());

        // Generate primary and secondary colors.
        lMaterial->Emissive.Set(lBlack);
        lMaterial->Ambient.Set(lRed);
        lColor = FbxDouble3(i > 2 ? 1.0 : 0.0,
                            i > 0 && i < 4 ? 1.0 : 0.0,
                            i % 2 ? 0.0 : 1.0);
        lMaterial->Diffuse.Set(lColor);
        lMaterial->TransparencyFactor.Set(0.0);
        lMaterial->ShadingModel.Set(lShadingName);
        lMaterial->Shininess.Set(0.5);

        //get the node of mesh, add material for it.
        FbxNode* lNode = pMesh->GetNode();
        if (lNode)
            lNode->AddMaterial(lMaterial);
    }
}

FbxNode* FbxSdkHelp::CreateTriangle(FbxScene* pScene, const char* pName)
{
    FbxMesh* lMesh = FbxMesh::Create(pScene, pName);

    // The three vertices
    FbxVector4 lControlPoint0(-50, 0, 50);
    FbxVector4 lControlPoint1(50, 0, 50);
    FbxVector4 lControlPoint2(0, 50, -50);

    // Create control points.
    lMesh->InitControlPoints(3);
    FbxVector4* lControlPoints = lMesh->GetControlPoints();

    lControlPoints[0] = lControlPoint0;
    lControlPoints[1] = lControlPoint1;
    lControlPoints[2] = lControlPoint2;

    // Create the triangle's polygon
    lMesh->BeginPolygon();
    lMesh->AddPolygon(0); // Control point 0
    lMesh->AddPolygon(1); // Control point 1
    lMesh->AddPolygon(2); // Control point 2
    lMesh->EndPolygon();

    FbxNode* lNode = FbxNode::Create(pScene, pName);
    lNode->SetNodeAttribute(lMesh);

    return lNode;
}

FbxNode* FbxSdkHelp::CreatePyramid(FbxScene* pScene, const char* pName)
{
    int i, j;
    FbxMesh* lMesh = FbxMesh::Create(pScene, std::string(pName).append("_mesh").c_str());

    FbxVector4 vertex0(-50, 0, 50);
    FbxVector4 vertex1(50, 0, 50);
    FbxVector4 vertex2(50, 0, -50);
    FbxVector4 vertex3(-50, 0, -50);
    FbxVector4 vertex4(0, 100, 0);

    FbxVector4 lNormalP0(0, 1, 0);
    FbxVector4 lNormalP1(0, 0.447, 0.894);
    FbxVector4 lNormalP2(0.894, 0.447, 0);
    FbxVector4 lNormalP3(0, 0.447, -0.894);
    FbxVector4 lNormalP4(-0.894, 0.447, 0);

    // Create control points.
    lMesh->InitControlPoints(16);
    FbxVector4* lControlPoints = lMesh->GetControlPoints();

    lControlPoints[0] = vertex0;
    lControlPoints[1] = vertex1;
    lControlPoints[2] = vertex2;
    lControlPoints[3] = vertex3;
    lControlPoints[4] = vertex0;
    lControlPoints[5] = vertex1;
    lControlPoints[6] = vertex4;
    lControlPoints[7] = vertex1;
    lControlPoints[8] = vertex2;
    lControlPoints[9] = vertex4;
    lControlPoints[10] = vertex2;
    lControlPoints[11] = vertex3;
    lControlPoints[12] = vertex4;
    lControlPoints[13] = vertex3;
    lControlPoints[14] = vertex0;
    lControlPoints[15] = vertex4;

    // specify normals per control point.

    FbxGeometryElementNormal* lNormalElement = lMesh->CreateElementNormal();
    lNormalElement->SetMappingMode(FbxGeometryElement::eByControlPoint);
    lNormalElement->SetReferenceMode(FbxGeometryElement::eDirect);

    lNormalElement->GetDirectArray().Add(lNormalP0);
    lNormalElement->GetDirectArray().Add(lNormalP0);
    lNormalElement->GetDirectArray().Add(lNormalP0);
    lNormalElement->GetDirectArray().Add(lNormalP0);
    lNormalElement->GetDirectArray().Add(lNormalP1);
    lNormalElement->GetDirectArray().Add(lNormalP1);
    lNormalElement->GetDirectArray().Add(lNormalP1);
    lNormalElement->GetDirectArray().Add(lNormalP2);
    lNormalElement->GetDirectArray().Add(lNormalP2);
    lNormalElement->GetDirectArray().Add(lNormalP2);
    lNormalElement->GetDirectArray().Add(lNormalP3);
    lNormalElement->GetDirectArray().Add(lNormalP3);
    lNormalElement->GetDirectArray().Add(lNormalP3);
    lNormalElement->GetDirectArray().Add(lNormalP4);
    lNormalElement->GetDirectArray().Add(lNormalP4);
    lNormalElement->GetDirectArray().Add(lNormalP4);
    // Array of polygon vertices.
    int lPolygonVertices[] = {
        0, 3, 2, 1,
        4, 5, 6,
        7, 8, 9,
        10, 11, 12,
        13, 14, 15
    };

    // Set material mapping.
    FbxGeometryElementMaterial* lMaterialElement = lMesh->CreateElementMaterial();
    auto mappingMode = lMaterialElement->GetMappingMode();
    auto referenceMode = lMaterialElement->GetReferenceMode();
    lMaterialElement->SetMappingMode(FbxGeometryElement::eByPolygon);
    lMaterialElement->SetReferenceMode(FbxGeometryElement::eIndexToDirect);

    // Create polygons. Assign material indices.

    // Pyramid base.
    lMesh->BeginPolygon(0); // Material index.

    for (j = 0; j < 4; j++)
    {
        lMesh->AddPolygon(lPolygonVertices[j]); // Control point index.
    }

    lMesh->EndPolygon();

    // Pyramid sides.
    for (i = 1; i < 5; i++)
    {
        lMesh->BeginPolygon(i); // Material index.

        for (j = 0; j < 3; j++)
        {
            lMesh->AddPolygon(lPolygonVertices[4 + 3 * (i - 1) + j]); // Control point index.
        }

        lMesh->EndPolygon();
    }


    FbxNode* lNode = FbxNode::Create(pScene, std::string(pName).append("_node").c_str());
    lNode->SetNodeAttribute(lMesh);
    return lNode;
}

void FbxSdkHelp::ImportFbxFile(char* filePath)
{
    FbxImporter* lImporter = FbxImporter::Create(m_pSdkManager, "");
    bool lImportStatus = lImporter->Initialize(filePath, -1, m_pSdkManager->GetIOSettings());
    if (!lImportStatus)
    {
        printf("Call to FbxImporter::Initialize() failed.\n");
        printf("Error returned: %s\n\n", lImporter->GetStatus().GetErrorString());
        exit(-1);
    }

    // Import the contents of the file into the scene.
    lImporter->Import(m_pScene);
    // The file has been imported; we can get rid of the importer.
    lImporter->Destroy();
}

void FbxSdkHelp::InitializeSdkObjects(FbxManager*& pManager, FbxScene*& pScene)
{
    //The first thing to do is to create the FBX Manager which is the object allocator for almost all the classes in the SDK
    pManager = FbxManager::Create();
    if (!pManager)
    {
        FBXSDK_printf("Error: Unable to create FBX Manager!\n");
        exit(1);
    }
    FBXSDK_printf("Autodesk FBX SDK version %s\n", pManager->GetVersion());

    //Create an IOSettings object. This object holds all import/export settings.
    FbxIOSettings* ios = FbxIOSettings::Create(pManager, IOSROOT);
    pManager->SetIOSettings(ios);

    //Load plugins from the executable directory (optional)
    //FbxString lPath = FbxGetApplicationDirectory();
    //pManager->LoadPluginsDirectory(lPath.Buffer());

    //Create an FBX scene. This object holds most objects imported/exported from/to files.
    pScene = FbxScene::Create(pManager, "My Scene");
    if (!pScene)
    {
        FBXSDK_printf("Error: Unable to create FBX scene!\n");
        exit(1);
    }
}

void FbxSdkHelp::CreateMesh_NoPlane(FbxMesh* pMesh)
{
    FbxVector4 vertices[4] = {
        FbxVector4(0, 0, 0),
        FbxVector4(0, 10, 0),
        FbxVector4(10, 0, 0),
        FbxVector4(10, 10, 10)
    };

    // 设置顶点和索引
    pMesh->InitControlPoints(4);
    for (int i = 0; i < 4; ++i)
    {
        pMesh->SetControlPointAt(vertices[i], i);
    }

    pMesh->BeginPolygon();
    pMesh->AddPolygon(0);
    pMesh->AddPolygon(1);
    pMesh->AddPolygon(2);
    pMesh->AddPolygon(3);
    pMesh->EndPolygon();
}

void FbxSdkHelp::PrintTabs()
{
    for (int i = 0; i < numTabs; i++)
        printf("\t");
}

FbxString FbxSdkHelp::GetAttributeTypeName(FbxNodeAttribute::EType type)
{
    switch (type)
    {
    case FbxNodeAttribute::eUnknown: return "unidentified";
    case FbxNodeAttribute::eNull: return "null";
    case FbxNodeAttribute::eMarker: return "marker";
    case FbxNodeAttribute::eSkeleton: return "skeleton";
    case FbxNodeAttribute::eMesh: return "mesh";
    case FbxNodeAttribute::eNurbs: return "nurbs";
    case FbxNodeAttribute::ePatch: return "patch";
    case FbxNodeAttribute::eCamera: return "camera";
    case FbxNodeAttribute::eCameraStereo: return "stereo";
    case FbxNodeAttribute::eCameraSwitcher: return "camera switcher";
    case FbxNodeAttribute::eLight: return "light";
    case FbxNodeAttribute::eOpticalReference: return "optical reference";
    case FbxNodeAttribute::eOpticalMarker: return "marker";
    case FbxNodeAttribute::eNurbsCurve: return "nurbs curve";
    case FbxNodeAttribute::eTrimNurbsSurface: return "trim nurbs surface";
    case FbxNodeAttribute::eBoundary: return "boundary";
    case FbxNodeAttribute::eNurbsSurface: return "nurbs surface";
    case FbxNodeAttribute::eShape: return "shape";
    case FbxNodeAttribute::eLODGroup: return "lodgroup";
    case FbxNodeAttribute::eSubDiv: return "subdiv";
    default: return "unknown";
    }
}

void FbxSdkHelp::PrintAttribute(FbxNodeAttribute* pAttribute)
{
    if (!pAttribute) return;

    FbxString typeName = GetAttributeTypeName(pAttribute->GetAttributeType());
    FbxString attrName = pAttribute->GetName();

    PrintTabs();
    // Note: to retrieve the character array of a FbxString, use its Buffer() method.
    printf("<attribute type='%s' name='%s'/>\n", typeName.Buffer(), attrName.Buffer());
}

void FbxSdkHelp::PrintNode(FbxNode* pNode)
{
    PrintTabs();
    const char* nodeName = pNode->GetName();
    FbxDouble3 translation = pNode->LclTranslation.Get();
    FbxDouble3 rotation = pNode->LclRotation.Get();
    FbxDouble3 scaling = pNode->LclScaling.Get();

    // Print the contents of the node.
    printf("<node name='%s' translation='(%f, %f, %f)' rotation='(%f, %f, %f)' scaling='(%f, %f, %f)'>\n",
           nodeName,
           translation[0], translation[1], translation[2],
           rotation[0], rotation[1], rotation[2],
           scaling[0], scaling[1], scaling[2]
    );
    numTabs++;

    // Print the node's attributes.
    for (int i = 0; i < pNode->GetNodeAttributeCount(); i++)
        PrintAttribute(pNode->GetNodeAttributeByIndex(i));

    // Recursively print the children.
    for (int j = 0; j < pNode->GetChildCount(); j++)
        PrintNode(pNode->GetChild(j));

    numTabs--;
    PrintTabs();
    printf("</node>\n");
}

void FbxSdkHelp::PrintFbxVector(FbxVector2& fbxVector2)
{
    printf("fbvVector2,0:'%f' 1:'%f' \n", fbxVector2[0], fbxVector2[1]);
}

std::string FbxSdkHelp::GetExePath()
{
    char szFilePath[MAX_PATH + 1] = {0};
    GetModuleFileNameA(nullptr, szFilePath, MAX_PATH);
    (strrchr(szFilePath, '\\'))[0] = 0; // 删除文件名，只获得路径字符串
    std::string path = szFilePath;
    return path;
}

bool FbxSdkHelp::IsMeshNode(FbxNode* pFbxNode)
{
    if (!pFbxNode)
    {
        return false;
    }

    // 获取节点上属性的数量
    const auto attributeCount = pFbxNode->GetNodeAttributeCount();

    // 遍历节点上的属性
    for (int i = 0; i < attributeCount; ++i)
    {
        const auto attribute = pFbxNode->GetNodeAttributeByIndex(i);

        // 检查属性是否是Mesh属性
        if (attribute && attribute->GetAttributeType() == FbxNodeAttribute::eMesh)
        {
            return true; // 节点包含Mesh属性，是Mesh节点
        }
    }

    return false; // 节点不包含Mesh属性，不是Mesh节点
}

void FbxSdkHelp::TriangulateScene(FbxScene* pScene)
{
    if (pScene)
    {
        FbxGeometryConverter geometryConverter(pScene->GetFbxManager());
        geometryConverter.Triangulate(pScene, true);
    }
}

void FbxSdkHelp::CreateCubeTexture(FbxScene* pScene, FbxMesh* pMesh)
{
    // A texture need to be connected to a property on the material,
// so let's use the material (if it exists) or create a new one
    FbxSurfacePhong* lMaterial = NULL;

    //get the node of mesh, add material for it.
    FbxNode* lNode = pMesh->GetNode();
    if (lNode)
    {
        lMaterial = lNode->GetSrcObject<FbxSurfacePhong>(0);
        if (lMaterial == NULL)
        {
            FbxString lMaterialName = "toto";
            FbxString lShadingName = "Phong";
            FbxDouble3 lBlack(0.0, 0.0, 0.0);
            FbxDouble3 lRed(1.0, 0.0, 0.0);
            FbxDouble3 lDiffuseColor(0.75, 0.75, 0.0);
            lMaterial = FbxSurfacePhong::Create(pScene, lMaterialName.Buffer());

            // Generate primary and secondary colors.
            lMaterial->Emissive.Set(lBlack);
            lMaterial->Ambient.Set(lRed);
            lMaterial->AmbientFactor.Set(1.);
            // Add texture for diffuse channel
            lMaterial->Diffuse.Set(lDiffuseColor);
            lMaterial->DiffuseFactor.Set(1.);
            lMaterial->TransparencyFactor.Set(0.4);
            lMaterial->ShadingModel.Set(lShadingName);
            lMaterial->Shininess.Set(0.5);
            lMaterial->Specular.Set(lBlack);
            lMaterial->SpecularFactor.Set(0.3);

            lNode->AddMaterial(lMaterial);
        }
    }

    FbxFileTexture* lTexture = FbxFileTexture::Create(pScene, "Diffuse Texture");

    // Set texture properties.
    lTexture->SetFileName("scene03.jpg"); // Resource file is in current directory.
    lTexture->SetTextureUse(FbxTexture::eStandard);
    lTexture->SetMappingType(FbxTexture::eUV);
    lTexture->SetMaterialUse(FbxFileTexture::eModelMaterial);
    lTexture->SetSwapUV(false);
    lTexture->SetTranslation(0.0, 0.0);
    lTexture->SetScale(1.0, 1.0);
    lTexture->SetRotation(0.0, 0.0);

    // don't forget to connect the texture to the corresponding property of the material
    if (lMaterial)
        lMaterial->Diffuse.ConnectSrcObject(lTexture);

    lTexture = FbxFileTexture::Create(pScene, "Ambient Texture");

    // Set texture properties.
    lTexture->SetFileName("gradient.jpg"); // Resource file is in current directory.
    lTexture->SetTextureUse(FbxTexture::eStandard);
    lTexture->SetMappingType(FbxTexture::eUV);
    lTexture->SetMaterialUse(FbxFileTexture::eModelMaterial);
    lTexture->SetSwapUV(false);
    lTexture->SetTranslation(0.0, 0.0);
    lTexture->SetScale(1.0, 1.0);
    lTexture->SetRotation(0.0, 0.0);

    // don't forget to connect the texture to the corresponding property of the material
    if (lMaterial)
        lMaterial->Ambient.ConnectSrcObject(lTexture);

    lTexture = FbxFileTexture::Create(pScene, "Emissive Texture");

    // Set texture properties.
    lTexture->SetFileName("spotty.jpg"); // Resource file is in current directory.
    lTexture->SetTextureUse(FbxTexture::eStandard);
    lTexture->SetMappingType(FbxTexture::eUV);
    lTexture->SetMaterialUse(FbxFileTexture::eModelMaterial);
    lTexture->SetSwapUV(false);
    lTexture->SetTranslation(0.0, 0.0);
    lTexture->SetScale(1.0, 1.0);
    lTexture->SetRotation(0.0, 0.0);

    // don't forget to connect the texture to the corresponding property of the material
    if (lMaterial)
        lMaterial->Emissive.ConnectSrcObject(lTexture);
}

FbxNode* FbxSdkHelp::CreateCube(FbxScene* pScene,const char* pName)
{
    int i, j;
    FbxMesh* lMesh = FbxMesh::Create(pScene, std::string(pName).append("_mesh").c_str());

    FbxVector4 lControlPoint0(-50, 0, 50);
    FbxVector4 lControlPoint1(50, 0, 50);
    FbxVector4 lControlPoint2(50, 100, 50);
    FbxVector4 lControlPoint3(-50, 100, 50);
    FbxVector4 lControlPoint4(-50, 0, -50);
    FbxVector4 lControlPoint5(50, 0, -50);
    FbxVector4 lControlPoint6(50, 100, -50);
    FbxVector4 lControlPoint7(-50, 100, -50);

    FbxVector4 lNormalXPos(1, 0, 0);
    FbxVector4 lNormalXNeg(-1, 0, 0);
    FbxVector4 lNormalYPos(0, 1, 0);
    FbxVector4 lNormalYNeg(0, -1, 0);
    FbxVector4 lNormalZPos(0, 0, 1);
    FbxVector4 lNormalZNeg(0, 0, -1);

    // Create control points.
    lMesh->InitControlPoints(24);
    FbxVector4* lControlPoints = lMesh->GetControlPoints();

    lControlPoints[0] = lControlPoint0;
    lControlPoints[1] = lControlPoint1;
    lControlPoints[2] = lControlPoint2;
    lControlPoints[3] = lControlPoint3;
    lControlPoints[4] = lControlPoint1;
    lControlPoints[5] = lControlPoint5;
    lControlPoints[6] = lControlPoint6;
    lControlPoints[7] = lControlPoint2;
    lControlPoints[8] = lControlPoint5;
    lControlPoints[9] = lControlPoint4;
    lControlPoints[10] = lControlPoint7;
    lControlPoints[11] = lControlPoint6;
    lControlPoints[12] = lControlPoint4;
    lControlPoints[13] = lControlPoint0;
    lControlPoints[14] = lControlPoint3;
    lControlPoints[15] = lControlPoint7;
    lControlPoints[16] = lControlPoint3;
    lControlPoints[17] = lControlPoint2;
    lControlPoints[18] = lControlPoint6;
    lControlPoints[19] = lControlPoint7;
    lControlPoints[20] = lControlPoint1;
    lControlPoints[21] = lControlPoint0;
    lControlPoints[22] = lControlPoint4;
    lControlPoints[23] = lControlPoint5;


    // We want to have one normal for each vertex (or control point),
    // so we set the mapping mode to eByControlPoint.
    FbxGeometryElementNormal* lGeometryElementNormal = lMesh->CreateElementNormal();

    lGeometryElementNormal->SetMappingMode(FbxGeometryElement::eByControlPoint);

    // Here are two different ways to set the normal values.
    bool firstWayNormalCalculations = true;
    if (firstWayNormalCalculations)
    {
        // The first method is to set the actual normal value
        // for every control point.
        lGeometryElementNormal->SetReferenceMode(FbxGeometryElement::eDirect);

        lGeometryElementNormal->GetDirectArray().Add(lNormalZPos);
        lGeometryElementNormal->GetDirectArray().Add(lNormalZPos);
        lGeometryElementNormal->GetDirectArray().Add(lNormalZPos);
        lGeometryElementNormal->GetDirectArray().Add(lNormalZPos);
        lGeometryElementNormal->GetDirectArray().Add(lNormalXPos);
        lGeometryElementNormal->GetDirectArray().Add(lNormalXPos);
        lGeometryElementNormal->GetDirectArray().Add(lNormalXPos);
        lGeometryElementNormal->GetDirectArray().Add(lNormalXPos);
        lGeometryElementNormal->GetDirectArray().Add(lNormalZNeg);
        lGeometryElementNormal->GetDirectArray().Add(lNormalZNeg);
        lGeometryElementNormal->GetDirectArray().Add(lNormalZNeg);
        lGeometryElementNormal->GetDirectArray().Add(lNormalZNeg);
        lGeometryElementNormal->GetDirectArray().Add(lNormalXNeg);
        lGeometryElementNormal->GetDirectArray().Add(lNormalXNeg);
        lGeometryElementNormal->GetDirectArray().Add(lNormalXNeg);
        lGeometryElementNormal->GetDirectArray().Add(lNormalXNeg);
        lGeometryElementNormal->GetDirectArray().Add(lNormalYPos);
        lGeometryElementNormal->GetDirectArray().Add(lNormalYPos);
        lGeometryElementNormal->GetDirectArray().Add(lNormalYPos);
        lGeometryElementNormal->GetDirectArray().Add(lNormalYPos);
        lGeometryElementNormal->GetDirectArray().Add(lNormalYNeg);
        lGeometryElementNormal->GetDirectArray().Add(lNormalYNeg);
        lGeometryElementNormal->GetDirectArray().Add(lNormalYNeg);
        lGeometryElementNormal->GetDirectArray().Add(lNormalYNeg);
    }
    else
    {
        // The second method is to the possible values of the normals
        // in the direct array, and set the index of that value
        // in the index array for every control point.
        lGeometryElementNormal->SetReferenceMode(FbxGeometryElement::eIndexToDirect);

        // Add the 6 different normals to the direct array
        lGeometryElementNormal->GetDirectArray().Add(lNormalZPos);
        lGeometryElementNormal->GetDirectArray().Add(lNormalXPos);
        lGeometryElementNormal->GetDirectArray().Add(lNormalZNeg);
        lGeometryElementNormal->GetDirectArray().Add(lNormalXNeg);
        lGeometryElementNormal->GetDirectArray().Add(lNormalYPos);
        lGeometryElementNormal->GetDirectArray().Add(lNormalYNeg);

        // Now for each control point, we need to specify which normal to use
        lGeometryElementNormal->GetIndexArray().Add(0); // index of lNormalZPos in the direct array.
        lGeometryElementNormal->GetIndexArray().Add(0); // index of lNormalZPos in the direct array.
        lGeometryElementNormal->GetIndexArray().Add(0); // index of lNormalZPos in the direct array.
        lGeometryElementNormal->GetIndexArray().Add(0); // index of lNormalZPos in the direct array.
        lGeometryElementNormal->GetIndexArray().Add(1); // index of lNormalXPos in the direct array.
        lGeometryElementNormal->GetIndexArray().Add(1); // index of lNormalXPos in the direct array.
        lGeometryElementNormal->GetIndexArray().Add(1); // index of lNormalXPos in the direct array.
        lGeometryElementNormal->GetIndexArray().Add(1); // index of lNormalXPos in the direct array.
        lGeometryElementNormal->GetIndexArray().Add(2); // index of lNormalZNeg in the direct array.
        lGeometryElementNormal->GetIndexArray().Add(2); // index of lNormalZNeg in the direct array.
        lGeometryElementNormal->GetIndexArray().Add(2); // index of lNormalZNeg in the direct array.
        lGeometryElementNormal->GetIndexArray().Add(2); // index of lNormalZNeg in the direct array.
        lGeometryElementNormal->GetIndexArray().Add(3); // index of lNormalXNeg in the direct array.
        lGeometryElementNormal->GetIndexArray().Add(3); // index of lNormalXNeg in the direct array.
        lGeometryElementNormal->GetIndexArray().Add(3); // index of lNormalXNeg in the direct array.
        lGeometryElementNormal->GetIndexArray().Add(3); // index of lNormalXNeg in the direct array.
        lGeometryElementNormal->GetIndexArray().Add(4); // index of lNormalYPos in the direct array.
        lGeometryElementNormal->GetIndexArray().Add(4); // index of lNormalYPos in the direct array.
        lGeometryElementNormal->GetIndexArray().Add(4); // index of lNormalYPos in the direct array.
        lGeometryElementNormal->GetIndexArray().Add(4); // index of lNormalYPos in the direct array.
        lGeometryElementNormal->GetIndexArray().Add(5); // index of lNormalYNeg in the direct array.
        lGeometryElementNormal->GetIndexArray().Add(5); // index of lNormalYNeg in the direct array.
        lGeometryElementNormal->GetIndexArray().Add(5); // index of lNormalYNeg in the direct array.
        lGeometryElementNormal->GetIndexArray().Add(5); // index of lNormalYNeg in the direct array.
    }

    // Array of polygon vertices.
    int lPolygonVertices[] = { 0, 1, 2, 3,
        4, 5, 6, 7,
        8, 9, 10, 11,
        12, 13, 14, 15,
        16, 17, 18, 19,
        20, 21, 22, 23 };

    // Create UV for Diffuse channel
    FbxGeometryElementUV* lUVDiffuseElement = lMesh->CreateElementUV(gDiffuseElementName);
    FBX_ASSERT(lUVDiffuseElement != NULL);
    lUVDiffuseElement->SetMappingMode(FbxGeometryElement::eByPolygonVertex);
    lUVDiffuseElement->SetReferenceMode(FbxGeometryElement::eIndexToDirect);

    FbxVector2 lVectors0(0, 0);
    FbxVector2 lVectors1(1, 0);
    FbxVector2 lVectors2(1, 1);
    FbxVector2 lVectors3(0, 1);

    lUVDiffuseElement->GetDirectArray().Add(lVectors0);
    lUVDiffuseElement->GetDirectArray().Add(lVectors1);
    lUVDiffuseElement->GetDirectArray().Add(lVectors2);
    lUVDiffuseElement->GetDirectArray().Add(lVectors3);


    // Create UV for Ambient channel
    FbxGeometryElementUV* lUVAmbientElement = lMesh->CreateElementUV(gAmbientElementName);

    lUVAmbientElement->SetMappingMode(FbxGeometryElement::eByPolygonVertex);
    lUVAmbientElement->SetReferenceMode(FbxGeometryElement::eIndexToDirect);

    lVectors0.Set(0, 0);
    lVectors1.Set(1, 0);
    lVectors2.Set(0, 0.418586879968643);
    lVectors3.Set(1, 0.418586879968643);

    lUVAmbientElement->GetDirectArray().Add(lVectors0);
    lUVAmbientElement->GetDirectArray().Add(lVectors1);
    lUVAmbientElement->GetDirectArray().Add(lVectors2);
    lUVAmbientElement->GetDirectArray().Add(lVectors3);

    // Create UV for Emissive channel
    FbxGeometryElementUV* lUVEmissiveElement = lMesh->CreateElementUV(gEmissiveElementName);

    lUVEmissiveElement->SetMappingMode(FbxGeometryElement::eByPolygonVertex);
    lUVEmissiveElement->SetReferenceMode(FbxGeometryElement::eIndexToDirect);

    lVectors0.Set(0.2343, 0);
    lVectors1.Set(1, 0.555);
    lVectors2.Set(0.333, 0.999);
    lVectors3.Set(0.555, 0.666);

    lUVEmissiveElement->GetDirectArray().Add(lVectors0);
    lUVEmissiveElement->GetDirectArray().Add(lVectors1);
    lUVEmissiveElement->GetDirectArray().Add(lVectors2);
    lUVEmissiveElement->GetDirectArray().Add(lVectors3);

    //Now we have set the UVs as eIndexToDirect reference and in eByPolygonVertex  mapping mode
    //we must update the size of the index array.
    lUVDiffuseElement->GetIndexArray().SetCount(24);
    lUVAmbientElement->GetIndexArray().SetCount(24);
    lUVEmissiveElement->GetIndexArray().SetCount(24);

    // Create polygons. Assign texture and texture UV indices.
    for (i = 0; i < 6; i++)
    {
        //we won't use the default way of assigning textures, as we have
        //textures on more than just the default (diffuse) channel.
        lMesh->BeginPolygon(-1, -1, false);

        for (j = 0; j < 4; j++)
        {
            //this function points 
            lMesh->AddPolygon(lPolygonVertices[i * 4 + j] // Control point index. 
            );
            //Now we have to update the index array of the UVs for diffuse, ambient and emissive
            lUVDiffuseElement->GetIndexArray().SetAt(i * 4 + j, j);
            lUVAmbientElement->GetIndexArray().SetAt(i * 4 + j, j);
            lUVEmissiveElement->GetIndexArray().SetAt(i * 4 + j, j);

        }

        lMesh->EndPolygon();
    }

    FbxNode* lNode = FbxNode::Create(pScene, std::string(pName).append("_node").c_str());

    lNode->SetNodeAttribute(lMesh);
    lNode->SetShadingMode(FbxNode::eTextureShading);

    return lNode;
}

void FbxSdkHelp::FindMaterials(FbxScene* pScene, FbxUInt64 fbxUInt64)
{
    auto materialCount = pScene->GetMaterialCount();
    for (auto i = 1; i < materialCount; i++)
    {
        auto material = pScene->GetMaterial(i);
    }
    FbxArray<FbxSurfaceMaterial*> fbxSurfaceMaterial;
    pScene->FillMaterialArray(fbxSurfaceMaterial);
    auto size = fbxSurfaceMaterial.Size();
}
