//
// Created by Xyy on 2018/12/19.
//

#ifndef RPG2D_WORLD_H
#define RPG2D_WORLD_H


#include "Player.h"
#include "Map.h"
#include "MapLoader.h"

class World {

private:

    bool isAccessible(int landform) {
        return landform != 3;
    }

public:

    Map *map;

    Player player;

    bool animeMap = false;

    float timer = 0;

    World() : player(Player(200,200)) {

        MapLoader mapLoader;
        map = mapLoader.loadMap("Dummy");//new Map00();
    }

    void setPlayerVelocity(Vector v) {

        player.velocity = v;
    }

    void update(float deltaTime) {

        timer += deltaTime;
        if(timer >= 1.f){
            animeMap = !animeMap;
            timer -= 1.f;
        }

        float prevRecttop = player.position.y;
        float prevRectbottom = player.position.y + Player::PLAYER_HEIGHT;
        float prevRectleft = player.position.x;
        float prevRectright = player.position.x + Player::PLAYER_WIDTH;

        player.update(deltaTime);

        const int tileWidth = 64;

        /*
  Calculate player current position
 */
        float curRecttop = player.position.y;
        float curRectbottom = player.position.y + Player::PLAYER_HEIGHT;
        float curRectleft = player.position.x;
        float curRectright = player.position.x + Player::PLAYER_WIDTH;

        /*
          Collision Detection
        */

        float offsetX = 0;
        float offsetY = 0;

        const int *layer = map->collisionLayer;
        int width = map->width;

        if (layer != NULL) {
            int minIndexY = (int) (curRecttop / tileWidth);
            int maxIndexY = (int) (curRectbottom / tileWidth);
            int minIndexX = (int) (curRectleft / tileWidth);
            int maxIndexX = (int) (curRectright / tileWidth);

            if (curRecttop > prevRecttop) {
                for (int ix = minIndexX; ix <= maxIndexX; ix++) {
                    if (!isAccessible(layer[width * maxIndexY + ix]) && isAccessible(layer[width * (maxIndexY - 1) + ix])) {
                        int edge = maxIndexY * tileWidth;
                        if (curRectbottom >= edge && prevRectbottom <= edge) {
                            offsetY = edge - curRectbottom;
                            break;
                        }
                    }
                }
            } else {
                for (int ix = minIndexX; ix <= maxIndexX; ix++) {
                    if (!isAccessible(layer[width * minIndexY + ix]) && isAccessible(layer[width * (minIndexY + 1) + ix])) {
                        int edge = (minIndexY + 1) * tileWidth;
                        if (curRecttop <= edge && prevRecttop >= edge) {
                            offsetY = edge - curRecttop;
                            break;
                        }
                    }
                }
            }

            if (curRectleft > prevRectleft) {
                for (int iy = minIndexY; iy <= maxIndexY; iy++) {
                    if (!isAccessible(layer[width * iy + maxIndexX]) && isAccessible(layer[width * iy + maxIndexX - 1])) {
                        int edge = maxIndexX * tileWidth;
                        if (curRectright >= edge && prevRectright <= edge) {
                            offsetX = edge - curRectright;
                            break;
                        }
                    }
                }
            } else {
                for (int iy = minIndexY; iy <= maxIndexY; iy++) {
                    if (!isAccessible(layer[width * iy + minIndexX]) && isAccessible(layer[width * iy + minIndexX + 1])) {
                        int edge = (minIndexX + 1) * tileWidth;
                        if (curRectleft <= edge && prevRectleft >= edge) {
                            offsetX = edge - curRectleft;
                            break;
                        }
                    }
                }
            }
        }

        player.position.x += offsetX;
        player.position.y += offsetY;
    }
};


#endif //RPG2D_WORLD_H
