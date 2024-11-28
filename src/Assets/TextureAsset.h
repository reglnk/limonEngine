//
// Created by Engin Manap on 1.03.2016.
//

#ifndef LIMONENGINE_TEXTURE_ASSET_H
#define LIMONENGINE_TEXTURE_ASSET_H

#include <string>
#include <iostream>
#include <cassert>
#include <SDL2/SDL_image.h>
#include "API/Graphics/GraphicsInterface.h"
#include "Graphics/Texture.h"

#include "Asset.h"
#include "AssetManager.h"

class TextureAsset : public Asset {
    struct TextureMetaData {
        GraphicsInterface::TextureTypes textureType;
        GraphicsInterface::InternalFormatTypes internalFormatType;
        GraphicsInterface::FormatTypes formatType;
        GraphicsInterface::DataTypes dataType;
        uint32_t width;
        uint32_t height;
    };
    TextureMetaData textureMetaData;
    SDL_Surface* cpuSurface = nullptr;//gets deleted after gpu load, don't use
    void loadCPUPart() override;
    void loadGPUPart() override;
protected:
    std::vector<std::string> name;//1) Caller asset filename, used for alternative search, 2) texture filename or texture ID based on 3rd, 3) Optional: model. if filled, second element embedded texture ID, third element model.
    std::shared_ptr<Texture> texture;
public:
    TextureAsset(AssetManager *assetManager, uint32_t assetID, const std::vector<std::string> &files);
#ifdef CEREAL_SUPPORT
    TextureAsset(AssetManager *assetManager, uint32_t assetID, const std::vector<std::string> &fileList, cereal::BinaryInputArchive& binaryArchive) :
    Asset(assetManager, assetID, fileList, binaryArchive) {
        assert(false && "TextureAsset doesn't support Cereal Loading");
    }
#endif
    ~TextureAsset();

    uint32_t getID() const {
        return texture->getTextureID();
    }

    const std::shared_ptr<Texture> &getTexture() const {
        return texture;
    }

    std::shared_ptr<Texture> &getTexture() {
        return texture;
    }

    std::vector<std::string> getName() const {
        return name;
    }

    uint32_t getHeight() const {
        return texture->getHeight();
    }

    uint32_t getWidth() const {
        return texture->getWidth();
    }
};

#endif //LIMONENGINE_TEXTURE_ASSET_H
