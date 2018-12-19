package xyy.game.rpg2d;

import android.os.Bundle;

import xyy.game.rpg2d.framework.impl.AndroidGame;

public class MainActivity extends AndroidGame {

    @Override
    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        AssetsLoader.setAssetManager(getAssets());
    }
}
