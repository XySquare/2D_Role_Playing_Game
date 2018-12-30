//
// Created by Xyy on 2018/12/24.
//

#ifndef RPG2D_ASSETS_H
#define RPG2D_ASSETS_H


#include "Texture.h"
#include "Map.h"
#include "UnitTexture.h"

class Assets {

public:

    static JNIEnv *jniEnv;

    static UnitTexture *unitTexture;

    static Texture *ui;

    static Texture *player;

    static int tileSetCount;

    static Texture **tileSetTextures;

    static void setup(JNIEnv *env){

        jniEnv = env;
        unitTexture = new UnitTexture();
        ui = new Texture(env, "ui.png");
        player = new Texture(env, "player.png");
        reload();
    }

    static void reload(){

        unitTexture->load();
        ui->load();
        player->load();
        for(int i=0;i<tileSetCount;i++){
            tileSetTextures[i]->load();
        }
        LOGI("Assets Loaded.");
    }

    static void load(Map *map){

        if(tileSetTextures){
            for(int i=0;i<tileSetCount;i++){
                delete tileSetTextures[i];
            }
            delete[] tileSetTextures;
        }

        tileSetCount = map->tileSetCount;
        tileSetTextures = new Texture*[tileSetCount];
        for(int i=0;i<tileSetCount;i++){
            tileSetTextures[i] = new Texture(jniEnv, map->tileSets[i]->image);
            tileSetTextures[i]->load();
        }
        LOGI("Map Assets Loaded.");
    }
};

JNIEnv *Assets::jniEnv = NULL;

UnitTexture *Assets::unitTexture = NULL;

Texture *Assets::ui = NULL;

Texture *Assets::player = NULL;

int Assets::tileSetCount = 0;

Texture ** Assets::tileSetTextures = NULL;

#endif //RPG2D_ASSETS_H
