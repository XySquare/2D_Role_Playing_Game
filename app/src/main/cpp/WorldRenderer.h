//
// Created by Xyy on 2018/12/20.
//

#ifndef RPG2D_WORLDRENDERER_H
#define RPG2D_WORLDRENDERER_H


#include "World.h"
#include "SpriteBatcher.h"
#include "Assets.h"
#include "Log.h"

class WorldRenderer {

private:

    World *world;

    SpriteBatcher *spriteBatcher;

public:

    WorldRenderer(World *world, SpriteBatcher *spriteBatcher) : world(world),
                                                                spriteBatcher(spriteBatcher) {}

    void resume() {

        LOGI("WorldRenderer","Resume.");
        // Set the backgroundcolor of this map
        glClearColor(world->map->backgroundColor.x, world->map->backgroundColor.y, world->map->backgroundColor.z, 1.0f);
    }

    void render(float offsetX, float offsetY) {

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

            TileSet *tileSet = world->map->tileSets[mapLayer->tileSetIndex];

            spriteBatcher->beginBatch(Assets::tileSetTextures[mapLayer->tileSetIndex]);

            if (mapLayer->layerType == MapLayer::TILE_LAYER) {
                TileLayer *tileLayer = (TileLayer *) mapLayer;

                Tile **tiles = tileLayer->tiles;

                int width = world->map->width;
                int height = world->map->height;

                int startIndexX = (int) (-offsetX / world->map->tileWidth);
                int startIndexY = (int) (-offsetY / world->map->tileHeight);

                for (int y = startIndexY; y < startIndexY + 13 && y < height; y++) {
                    for (int x = startIndexX; x < startIndexX + 21 && x < width; x++) {
                        Tile *tile = tiles[y * width + x];
                        if (tile) {
                            int gid = tile->getGId(world->timer);
                            int y2 = (gid / tileSet->columns);
                            int x1 = (gid % tileSet->columns);
                            spriteBatcher->drawSprite(x * world->map->tileWidth + offsetX,
                                                      y * world->map->tileHeight + offsetY,
                                                      world->map->tileWidth, world->map->tileHeight,
                                                      TextureRegion(
                                                              (float) (x1 * tileSet->tileWidth) /
                                                              tileSet->imageWidth,
                                                              (float) (y2 * tileSet->tileHeight) /
                                                              tileSet->imageHeight,
                                                              (float) tileSet->tileWidth /
                                                              tileSet->imageWidth,
                                                              (float) tileSet->tileHeight /
                                                              tileSet->imageHeight));
                        }
                    }
                }
            } else if (mapLayer->layerType == MapLayer::OBJECT_LAYER) {
                ObjectLayer *objectLayer = (ObjectLayer *) mapLayer;

                for (int j = 0; j < objectLayer->objectCount; j++) {
                    MapObject *object = objectLayer->objects[j];
                    Tile *tile = object->tile;
                    if (tile) {
                        int index = tile->getGId(world->timer);
                        int y2 = (index / tileSet->columns);
                        int x1 = (index % tileSet->columns);
                        spriteBatcher->drawSprite(object->position.x + offsetX,
                                                  object->position.y + offsetY, object->width,
                                                  object->height,
                                                  TextureRegion((float) (x1 * tileSet->tileWidth) /
                                                                tileSet->imageWidth,
                                                                (float) (y2 * tileSet->tileHeight) /
                                                                tileSet->imageHeight,
                                                                (float) tileSet->tileWidth /
                                                                tileSet->imageWidth,
                                                                (float) tileSet->tileHeight /
                                                                tileSet->imageHeight));
                    }
                }
            }

            spriteBatcher->endBatch();
        }

        // Draw Objects
        /*spriteBatcher->beginBatch(uiTexture);

        for(MapObject object:world->map->objects){
            spriteBatcher->drawSprite(object.position.x + offsetX, object.position.y + offsetY, 64, 64,
                                      getObjTextureRegionFromIndex(object.gid));
        }

        spriteBatcher->endBatch(gvPositionHandle, gvCoordinateHandle);*/

        spriteBatcher->beginBatch(Assets::player);

        //Draw Player
        int frame = ((int) (world->player.stateTime / 0.125f)) % 4;
        float x = frame * 90 / 512.f;
        float y = (world->player.state) * 120 / 512.f;
        TextureRegion playerRegine(x, y, 90 / 512.f, 120 / 512.f);
        spriteBatcher->drawSprite(
                world->player.position.x + offsetX - (90 - Player::PLAYER_WIDTH) / 2,
                world->player.position.y + offsetY - (120 - Player::PLAYER_HEIGHT), 90, 120,
                playerRegine);

        spriteBatcher->endBatch();
    }

    void render() {

        float offsetX = 640 - world->player.position.x - Player::PLAYER_WIDTH / 2;
        float offsetY = 360 - world->player.position.y - Player::PLAYER_HEIGHT / 2;

        render(offsetX, offsetY);
    }
};


#endif //RPG2D_WORLDRENDERER_H
