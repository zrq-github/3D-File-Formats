#pragma once
#include <GLTFSDK/GLTF.h>
#include <GLTFSDK/GLTFResourceReader.h>
#include <GLTFSDK/GLBResourceReader.h>
#include <GLTFSDK/Deserialize.h>
#include <fstream>
#include <sstream>
#include <iostream>

#include <cassert>
#include <cstdlib>
#include <filesystem>
namespace fs = std::filesystem;

class GltfHelper
{
    // The glTF SDK is decoupled from all file I/O by the IStreamReader (and IStreamWriter)
    // interface(s) and the C++ stream-based I/O library. This allows the glTF SDK to be used in
    // sandboxed environments, such as WebAssembly modules and UWP apps, where any file I/O code
    // must be platform or use-case specific.
    class StreamReader : public Microsoft::glTF::IStreamReader
    {
    public:
        StreamReader(fs::path pathBase) : m_pathBase(std::move(pathBase))
        {
            assert(m_pathBase.has_root_path());
        }

        // Resolves the relative URIs of any external resources declared in the glTF manifest
        std::shared_ptr<std::istream> GetInputStream(const std::string& filename) const override
        {
            // In order to construct a valid stream:
            // 1. The filename argument will be encoded as UTF-8 so use filesystem::u8path to
            //    correctly construct a path instance.
            // 2. Generate an absolute path by concatenating m_pathBase with the specified filename
            //    path. The filesystem::operator/ uses the platform's preferred directory separator
            //    if appropriate.
            // 3. Always open the file stream in binary mode. The glTF SDK will handle any text
            //    encoding issues for us.
            auto streamPath = m_pathBase / fs::u8path(filename);
            auto stream = std::make_shared<std::ifstream>(streamPath, std::ios_base::binary);

            // Check if the stream has no errors and is ready for I/O operations
            if (!stream || !(*stream))
            {
                throw std::runtime_error("Unable to create a valid input stream for uri: " + filename);
            }

            return stream;
        }

    private:
        fs::path m_pathBase;
    };

    // mesh
public:
    static void PrintMeshInfo(const Microsoft::glTF::Document& document, const Microsoft::glTF::GLTFResourceReader& resourceReader);
    static void PrintMesh_ACCESSOR_POSITION(const std::vector<float>& positions);

    // Uses the Document class to print some basic information about various top-level glTF entities
    static void PrintDocumentInfo(const Microsoft::glTF::Document& document);
    // Uses the Document and GLTFResourceReader classes to print information about various glTF binary resources
    static void PrintResourceInfo(const Microsoft::glTF::Document& document, const Microsoft::glTF::GLTFResourceReader& resourceReader);
    static void PrintInfo(const fs::path& path);
};

