//
// Created by Xyy on 2018/12/24.
//

#ifndef RPG2D_ASSETS_H
#define RPG2D_ASSETS_H


#include "Texture.h"
#include "Map.h"
#include "UnitTexture.h"
#include "Log.h"

class Assets {

public:

    static JNIEnv *jniEnv;

    static UnitTexture *unitTexture;

    static Texture *ui;

    static Texture *player;

    static unsigned char tileSetCount;

    static Texture **tileSetTextures;

    static void setup(JNIEnv *env) {

        jniEnv = env;
        unitTexture = new UnitTexture();
        ui = new Texture("ui.png", 512, 1024);
        player = new Texture("player.png", 512, 512);
        reload();
    }

    static void reload() {

        unitTexture->load();
        ui->load(jniEnv);
        player->load(jniEnv);
        for (unsigned char i = 0; i < tileSetCount; i++) {
            tileSetTextures[i]->load(jniEnv);
        }
        LOGI("Assets", "(Re)Loaded.");
    }

    static void load(Map *map) {

        unsigned char c = map->tileSetCount;
        Texture **s = new Texture *[c];
        // If corresponding textures have already been loaded, reuse it.
        for (unsigned char i = 0; i < c; i++) {
            Texture *t = findTexture(map->tileSets[i]->image);
            if (t) {
                s[i] = t;
            } else {
                s[i] = new Texture( map->tileSets[i]->image, map->tileSets[i]->imageWidth,
                                   map->tileSets[i]->imageHeight);
                s[i]->load(jniEnv);
            }
        }

        if (tileSetTextures) {
            for (unsigned char i = 0; i < tileSetCount; i++) {
                if (tileSetTextures[i] != nullptr)
                    delete tileSetTextures[i];
            }
            delete[] tileSetTextures;
        }
        tileSetCount = c;
        tileSetTextures = s;

        LOGI("Assets", "Map Assets Loaded.");
    }

private:

    static Texture *findTexture(std::string name) {
        for (unsigned char i = 0; i < tileSetCount; i++) {
            if (tileSetTextures[i] && tileSetTextures[i]->fileName == name) {
                Texture *t = tileSetTextures[i];
                tileSetTextures[i] = nullptr;
                return t;
            }
        }
        return nullptr;
    }
};

JNIEnv *Assets::jniEnv = nullptr;

UnitTexture *Assets::unitTexture = nullptr;

Texture *Assets::ui = nullptr;

Texture *Assets::player = nullptr;

unsigned char Assets::tileSetCount = 0;

Texture **Assets::tileSetTextures = nullptr;

#endif //RPG2D_ASSETS_H
