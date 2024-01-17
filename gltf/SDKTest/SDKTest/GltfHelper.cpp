#include "GltfHelper.h"

#include <GLTFSDK/GLTF.h>
#include <GLTFSDK/GLTFResourceReader.h>
#include <GLTFSDK/GLBResourceReader.h>
#include <GLTFSDK/Deserialize.h>

#include <fstream>
#include <sstream>
#include <iostream>

#include <cassert>
#include <cstdlib>

/// @details
/// https://registry.khronos.org/glTF/specs/2.0/glTF-2.0.html#meshes
///
void GltfHelper::PrintMeshInfo(const Microsoft::glTF::Document& document, const Microsoft::glTF::GLTFResourceReader& resourceReader)
{

    // Use the resource reader to get each mesh primitive's position data
    for (const auto& mesh : document.meshes.Elements())
    {
        std::cout << "Mesh: " << mesh.id << "\n";

        for (const auto& meshPrimitive : mesh.primitives)
        {
            std::string accessorPositionId;
            if (meshPrimitive.TryGetAttributeAccessorId(Microsoft::glTF::ACCESSOR_POSITION, accessorPositionId))
            {
                const Microsoft::glTF::Accessor& accessor = document.accessors.Get(accessorPositionId);
                const auto positions = resourceReader.ReadBinaryData<float>(document, accessor);
                const auto dataByteLength = positions.size() * sizeof(float);
                std::cout << "MeshPrimitive: " << dataByteLength << " bytes of position data\n";
            }

            std::string accessorNormalId;
            if (meshPrimitive.TryGetAttributeAccessorId(Microsoft::glTF::ACCESSOR_NORMAL, accessorNormalId))
            {
                const Microsoft::glTF::Accessor& accessor = document.accessors.Get(accessorNormalId);
                const auto data = resourceReader.ReadBinaryData<float>(document, accessor);
                const auto dataByteLength = data.size() * sizeof(float);
                std::cout << "MeshPrimitive: " << dataByteLength << " bytes of normal data\n";
            }

            std::string accessorTangentId;
            if(meshPrimitive.TryGetAttributeAccessorId(Microsoft::glTF::ACCESSOR_TANGENT,accessorTangentId))
            {
                const Microsoft::glTF::Accessor& accessor = document.accessors.Get(accessorTangentId);
                const auto data = resourceReader.ReadBinaryData<float>(document, accessor);
                const auto dataByteLength = data.size() * sizeof(float);
                std::cout << "MeshPrimitive: " << dataByteLength << " bytes of tangent data\n";
            }
        }

        std::cout << "\n";
    }
}

void GltfHelper::PrintMesh_ACCESSOR_POSITION(const std::vector<float>& positions)
{
    auto size = positions.size();
    for (size_t i=0;i<size;++i)
    {
        if(i%3==0)
        {
            std::count << "\n";
        }
        std::cout << positions[i] << "  ";
    }
}

void GltfHelper::PrintDocumentInfo(const Microsoft::glTF::Document& document)
{
    // Asset Info
    std::cout << "Asset Version:    " << document.asset.version << "\n";
    std::cout << "Asset MinVersion: " << document.asset.minVersion << "\n";
    std::cout << "Asset Generator:  " << document.asset.generator << "\n";
    std::cout << "Asset Copyright:  " << document.asset.copyright << "\n\n";

    // Scene Info
    std::cout << "Scene Count: " << document.scenes.Size() << "\n";

    if (document.scenes.Size() > 0U)
    {
        std::cout << "Default Scene Index: " << document.GetDefaultScene().id << "\n\n";
    }
    else
    {
        std::cout << "\n";
    }

    // Entity Info
    std::cout << "Node Count:     " << document.nodes.Size() << "\n";
    std::cout << "Camera Count:   " << document.cameras.Size() << "\n";
    std::cout << "Material Count: " << document.materials.Size() << "\n\n";

    // Mesh Info
    std::cout << "Mesh Count: " << document.meshes.Size() << "\n";
    std::cout << "Skin Count: " << document.skins.Size() << "\n\n";

    // Texture Info
    std::cout << "Image Count:   " << document.images.Size() << "\n";
    std::cout << "Texture Count: " << document.textures.Size() << "\n";
    std::cout << "Sampler Count: " << document.samplers.Size() << "\n\n";

    // Buffer Info
    std::cout << "Buffer Count:     " << document.buffers.Size() << "\n";
    std::cout << "BufferView Count: " << document.bufferViews.Size() << "\n";
    std::cout << "Accessor Count:   " << document.accessors.Size() << "\n\n";

    // Animation Info
    std::cout << "Animation Count: " << document.animations.Size() << "\n\n";

    for (const auto& extension : document.extensionsUsed)
    {
        std::cout << "Extension Used: " << extension << "\n";
    }

    if (!document.extensionsUsed.empty())
    {
        std::cout << "\n";
    }

    for (const auto& extension : document.extensionsRequired)
    {
        std::cout << "Extension Required: " << extension << "\n";
    }

    if (!document.extensionsRequired.empty())
    {
        std::cout << "\n";
    }
}

void GltfHelper::PrintResourceInfo(const Microsoft::glTF::Document& document, const Microsoft::glTF::GLTFResourceReader& resourceReader)
{
    // Use the resource reader to get each mesh primitive's position data
    for (const auto& mesh : document.meshes.Elements())
    {
        std::cout << "Mesh: " << mesh.id << "\n";

        for (const auto& meshPrimitive : mesh.primitives)
        {
            std::string accessorId;

            if (meshPrimitive.TryGetAttributeAccessorId(Microsoft::glTF::ACCESSOR_POSITION, accessorId))
            {
                const Microsoft::glTF::Accessor& accessor = document.accessors.Get(accessorId);

                const auto data = resourceReader.ReadBinaryData<float>(document, accessor);
                const auto dataByteLength = data.size() * sizeof(float);

                std::cout << "MeshPrimitive: " << dataByteLength << " bytes of position data\n";
            }
        }

        std::cout << "\n";
    }

    // Use the resource reader to get each image's data
    for (const auto& image : document.images.Elements())
    {
        std::string filename;

        if (image.uri.empty())
        {
            assert(!image.bufferViewId.empty());

            auto& bufferView = document.bufferViews.Get(image.bufferViewId);
            auto& buffer = document.buffers.Get(bufferView.bufferId);

            filename += buffer.uri; //NOTE: buffer uri is empty if image is stored in GLB binary chunk
        }
        else if (Microsoft::glTF::IsUriBase64(image.uri))
        {
            filename = "Data URI";
        }
        else
        {
            filename = image.uri;
        }

        auto data = resourceReader.ReadBinaryData(document, image);

        std::cout << "Image: " << image.id << "\n";
        std::cout << "Image: " << data.size() << " bytes of image data\n";

        if (!filename.empty())
        {
            std::cout << "Image filename: " << filename << "\n\n";
        }
    }
}

void GltfHelper::PrintInfo(const fs::path& path)
{
    // Pass the absolute path, without the filename, to the stream reader
    auto streamReader = std::make_unique<StreamReader>(path.parent_path());

    fs::path pathFile = path.filename();
    fs::path pathFileExt = pathFile.extension();

    std::string manifest;

    auto MakePathExt = [](const std::string& ext)
    {
        return "." + ext;
    };

    std::unique_ptr<Microsoft::glTF::GLTFResourceReader> resourceReader;

    // If the file has a '.gltf' extension then create a GLTFResourceReader
    if (pathFileExt == MakePathExt(Microsoft::glTF::GLTF_EXTENSION))
    {
        auto gltfStream = streamReader->GetInputStream(pathFile.u8string()); // Pass a UTF-8 encoded filename to GetInputString
        auto gltfResourceReader = std::make_unique<Microsoft::glTF::GLTFResourceReader>(std::move(streamReader));

        std::stringstream manifestStream;

        // Read the contents of the glTF file into a string using a std::stringstream
        manifestStream << gltfStream->rdbuf();
        manifest = manifestStream.str();

        resourceReader = std::move(gltfResourceReader);
    }

    // If the file has a '.glb' extension then create a GLBResourceReader. This class derives
    // from GLTFResourceReader and adds support for reading manifests from a GLB container's
    // JSON chunk and resource data from the binary chunk.
    if (pathFileExt == MakePathExt(Microsoft::glTF::GLB_EXTENSION))
    {
        auto glbStream = streamReader->GetInputStream(pathFile.u8string()); // Pass a UTF-8 encoded filename to GetInputString
        auto glbResourceReader = std::make_unique<Microsoft::glTF::GLBResourceReader>(std::move(streamReader), std::move(glbStream));

        manifest = glbResourceReader->GetJson(); // Get the manifest from the JSON chunk

        resourceReader = std::move(glbResourceReader);
    }

    if (!resourceReader)
    {
        throw std::runtime_error("Command line argument path filename extension must be .gltf or .glb");
    }

    Microsoft::glTF::Document document;

    try
    {
        document = Microsoft::glTF::Deserialize(manifest);
    }
    catch (const Microsoft::glTF::GLTFException& ex)
    {
        std::stringstream ss;

        ss << "Microsoft::glTF::Deserialize failed: ";
        ss << ex.what();

        throw std::runtime_error(ss.str());
    }

    std::cout << "### glTF Info - " << pathFile << " ###\n\n";

    PrintDocumentInfo(document);
    PrintResourceInfo(document, *resourceReader);
}
