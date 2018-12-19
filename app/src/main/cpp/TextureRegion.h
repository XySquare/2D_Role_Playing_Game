//
// Created by Xyy on 2018/12/18.
//

#ifndef RPG2D_TEXTUREREGION_H
#define RPG2D_TEXTUREREGION_H


struct TextureRegion {
    float u1, v1;
    float u2, v2;

    TextureRegion(float x, float y, float width, float height) : u1(x), v1(y),
                                                                                    u2(x+width),
                                                                                    v2(y+height) {}
};


#endif //RPG2D_TEXTUREREGION_H
