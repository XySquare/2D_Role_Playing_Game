//
// Created by Xyy on 2018/12/26.
//

#ifndef RPG2D_BATTLE_H
#define RPG2D_BATTLE_H


#include "Property.h"

class Battle {

private:
    //最大回合数
    static const int MAX_ROUND = 999;
    //n越大，防御力对伤害的减免就越大
    static const float n;
    //参与战斗的玩家与怪物
    Property *monster;
    Property *player;
    //伤害缓存
    int defenderDamage;
    //回合计数器
    int mRound;

    bool mIsPlayerTurn;
    //攻击类型（1=普通， 2=暴击）
//int atkType;

    int mAgiBalance;

//int attackerDamage;

public:
    Battle() {

    }

    /**
     * Start new Battle / 开始新战斗
     *
     */
    void newBattle(Property *monster, Property *player) {
        mAgiBalance = player->agi - monster->agi;
        mIsPlayerTurn = mAgiBalance >= 0;
        this->monster = monster;
        this->player = player;
        mRound = 0;
        monster->hp = monster->maxHp;
    }

    /**
     * Next round / 下一轮
     *
     * @return true=还有下一轮； false=战斗结束
     */
    bool next() {
        //回合计数器累加
        mRound++;
        //超过指定回合数
        if (mRound > MAX_ROUND || player->hp <= 0 || monster->hp <= 0)
            return false;
        Property *atker, *defer;
        //决定攻击方与防御方
        if (mAgiBalance >= 0) {
            atker = player;
            defer = monster;
            mIsPlayerTurn = true;
        } else {
            atker = monster;
            defer = player;
            mIsPlayerTurn = false;
        }
        //防御减伤
        defenderDamage = (int) (atker->atk * (1 - (defer->def * n) / (1 + n * defer->def)));
        // Minimum damage 1 / 除去闪避，伤害值最低为1
        if (defenderDamage < 1)
            defenderDamage = 1;

        //累加敏捷计数器
        if (mAgiBalance >= 0) {
            mAgiBalance -= defer->agi;
        } else {
            mAgiBalance += defer->agi;
        }

        // Apply damage / 扣血
        if(defenderDamage > defer->hp){
            defer->hp = 0;
        }else{
            defer->hp -= defenderDamage;
        }
        //atker->hp -= attackerDamage;
        return true;
    }

    /**
     * @return 该轮是否为玩家攻击
     */
    bool isPlayerTurn() {
        return mIsPlayerTurn;
    }

    int getDamage() {
        return defenderDamage;
    }

    /**
     * 跳过战斗
     */
    void skip() {
        while (next());
    }

    bool isWin() {
        return monster->hp <= 0;
    }

    /*public int getRound() {
        return mRound;
    }*/

};

const float Battle::n = 0.1f;

#endif //RPG2D_BATTLE_H
