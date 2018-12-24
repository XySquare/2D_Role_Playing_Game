//
// Created by Xyy on 2018/12/20.
//

#ifndef RPG2D_WORLDRENDERER_H
#define RPG2D_WORLDRENDERER_H


#include "World.h"
#include "SpriteBatcher.h"

class WorldRenderer {

private:

    World *world;

    SpriteBatcher *spriteBatcher;

    //Texture *mapTexture;

    Texture *uiTexture;

    Texture *playerTexture;

    int tileSetCount;

    Texture **tileSetTextures;

    TextureRegion getObjTextureRegionFromIndex(int index) {
        int y = (index / 2);
        int x = (index % 2);
        return TextureRegion((float) (x * 64) / 128.f, (float) (y * 64) / 128.f,
                             (float) 64 / 128.f, (float) 64 / 128.f);
    }

public:

    WorldRenderer(JNIEnv *env, World *world, SpriteBatcher *spriteBatcher) : world(world),
                                                                             spriteBatcher(
                                                                                     spriteBatcher) {
        tileSetCount = world->map->tileSetsCount;
        tileSetTextures = new Texture*[tileSetCount];
        for(int i=0;i<tileSetCount;i++){
            tileSetTextures[i] = new Texture(env, world->map->tileSets[i]->image);
        }
        //mapTexture = new Texture(env, "GoP_1.png");
        uiTexture = new Texture(env, "obj.png");
        playerTexture = new Texture(env, "player.png");
    }

    void render(GLuint gvPositionHandle, GLuint gvCoordinateHandle, float offsetX, float offsetY) {

        // Adjust offsets
        int mapWidth = world->map->width * world->map->tileWidth;
        if (mapWidth < 1280)
            offsetX = (1280 - mapWidth) / 2.f;
        else if (offsetX > 0) offsetX = 0;
        else if (offsetX < 1280 - mapWidth)
            offsetX = 1280 - mapWidth;

        int mapHeight = world->map->height * world->map->tileHeight;
        if (mapHeight < 720)
            offsetX = (720 - mapHeight) / 2.f;
        else if (offsetY > 0) offsetY = 0;
        else if (offsetY < 720 - mapHeight)
            offsetY = 720 - mapHeight;

        // Draw Map

        int layerCount = world->map->layerCount;
        for (int i = 0; i < layerCount; i++) {
            MapLayer *mapLayer = world->map->layers[i];

            if(mapLayer->getLayerType() == MapLayer::TILE_LAYER){
                TileLayer *tileLayer = (TileLayer*)mapLayer;

                TileSet *tileSet = world->map->tileSets[tileLayer->tileSetIndex];

                spriteBatcher->beginBatch(tileSetTextures[tileLayer->tileSetIndex]);

                Tile **tiles = tileLayer->tiles;

                int width = world->map->width;
                int height = world->map->height;

                int startIndexX = (int) (-offsetX / world->map->tileWidth);
                int startIndexY = (int) (-offsetY / world->map->tileHeight);

                for (int y = startIndexY; y < startIndexY + 13 && y < height; y++) {
                    for (int x = startIndexX; x < startIndexX + 21 && x < width; x++) {
                        Tile *tile = tiles[y * width + x];
                        if(tile){
                            int gid = tile->getGId(world->timer);
                            int y2 = (gid / tileSet->columns);
                            int x1 = (gid % tileSet->columns);
                            spriteBatcher->drawSprite(x * world->map->tileWidth + offsetX, y * world->map->tileHeight + offsetY, world->map->tileWidth, world->map->tileHeight,
                                                      TextureRegion((float) (x1 * tileSet->tileWidth) / tileSet->imageWidth, (float) (y2 * tileSet->tileHeight) / tileSet->imageHeight,
                                                                           (float) tileSet->tileWidth / tileSet->imageWidth, (float) tileSet->tileHeight / tileSet->imageHeight));
                        }
                    }
                }

                spriteBatcher->endBatch(gvPositionHandle, gvCoordinateHandle);
            }
        }

        // Draw Objects
        spriteBatcher->beginBatch(uiTexture);

        for(MapObject object:world->map->objects){
            spriteBatcher->drawSprite(object.position.x + offsetX, object.position.y + offsetY, 64, 64,
                                      getObjTextureRegionFromIndex(object.gid));
        }

        spriteBatcher->endBatch(gvPositionHandle, gvCoordinateHandle);

        spriteBatcher->beginBatch(playerTexture);

        //Draw Player
        int frame = ((int)(world->player.stateTime/0.125f))%4;
        float x = frame*90/512.f;
        float y = (world->player.state - 1)*120/512.f;
        TextureRegion playerRegine(x,y,90/512.f,120/512.f);
        spriteBatcher->drawSprite(world->player.position.x + offsetX - (90 - Player::PLAYER_WIDTH)/2,
                                  world->player.position.y + offsetY - (120 - Player::PLAYER_HEIGHT), 90, 120,
                                  playerRegine);

        spriteBatcher->endBatch(gvPositionHandle, gvCoordinateHandle);
    }

    void render(GLuint gvPositionHandle, GLuint gvCoordinateHandle) {

        float offsetX = 640 - world->player.position.x - Player::PLAYER_WIDTH/2;
        float offsetY = 360 - world->player.position.y - Player::PLAYER_HEIGHT/2;

        render(gvPositionHandle, gvCoordinateHandle, offsetX, offsetY);
    }

    void reload() {
        uiTexture->reload();
        playerTexture->reload();
        for(int i=0;i<tileSetCount;i++){
            tileSetTextures[i]->reload();
        }
    }
};


#endif //RPG2D_WORLDRENDERER_H
