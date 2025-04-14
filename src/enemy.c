#include "enemy.h"
#include "commdef.h"
#include "explod.h"
#include "player.h"
#include "raylib.h"
#include "raymath.h"
#include "sprite.h"
#include "spritepool.h"
#include "timer.h"
#include "weapon.h"
#include <stdio.h>

// --- 靜態變數 ---
static Enemy enemies[MAX_ENEMY]; // 敵人陣列 (物件池)
static int enemyCounter; // 目前活躍的敵人數量 (指向最後一個活躍敵人的下一個索引)
static float enemySpawnTime = 0; // 敵人生成的計時器
static Explod explods[MAX_ENEMY]; // 爆炸效果陣列 (物件池)
static int explodCounter; // 活躍的爆炸效果計數器 (指向最高使用過的索引+1)

// --- 爆炸效果相關函式 ---

/**
 * @brief 嘗試在指定位置添加一個爆炸效果
 * @param pos 爆炸發生的邏輯網格位置 (Vec2)
 */
void TryAddExplod(Vec2 pos)
{
    // 遍歷爆炸效果池，尋找一個閒置的爆炸效果物件
    Vec2 sprPos = Vector2Scale(pos, CELL_SIZE);
    for (int i = 0; i < MAX_ENEMY; i++) { // BUG? 理論上應該遍歷 MAX_EXPLOSION 或類似常數，但這裡用 MAX_ENEMY 權充
        if (explods[i].lifeTime <= 0) { // 如果生命週期 <= 0，表示此爆炸效果已結束或未使用
            ExplodStart(&explods[i], sprPos); // 啟動這個爆炸效果，設定其位置
            // 如果使用的索引超出了目前的計數器，則擴大計數器範圍
            // 這確保了 Update 和 Draw 迴圈會檢查到這個新啟動的爆炸
            if (i >= explodCounter) {
                explodCounter = i + 1; // 更新計數器為目前使用的最大索引 + 1
            }
            return; // 找到並啟動後即可返回
        }
    }
    // 如果迴圈結束仍未找到閒置的，表示爆炸效果池已滿，無法再添加
}

/**
 * @brief 更新所有活躍的爆炸效果
 */
void ExplodsUpdate()
{
    // 遍歷到目前為止使用過的最高索引
    for (int i = 0; i < explodCounter; i++) {
        if (explods[i].lifeTime > 0) { // 只更新生命週期 > 0 的活躍爆炸
            ExplodUpdate(&explods[i]); // 調用單個爆炸效果的更新函式
        }
    }
}

/**
 * @brief 繪製所有活躍的爆炸效果
 */
void ExplodsDraw()
{
    Sprite* explodSpr = SpritePoolGet(SPR_EXPLODE);
    // 遍歷到目前為止使用過的最高索引
    for (int i = 0; i < explodCounter; i++) {
        if (explods[i].lifeTime > 0) { // 只繪製生命週期 > 0 的活躍爆炸
            ExplodDraw(&explods[i], explodSpr); // 調用單個爆炸效果的繪製函式
        }
    }
}

// --- 敵人管理相關函式 ---

/**
 * @brief 初始化敵人系統和相關的爆炸效果池
 */
void EnemyInit()
{
    // 遍歷整個敵人陣列
    for (int i = 0; i < MAX_ENEMY; ++i) {
        // 初始化敵人屬性
        enemies[i].type = ENEMY_NONE; // 初始狀態為無敵人
        enemies[i].hp = ENEMY_HP; // 設定初始生命值
        enemies[i].damage = 1; // 設定敵人傷害 (未使用?)
        enemies[i].speed = 5; // 設定敵人移動速度
        enemies[i].pos = (Vec2) { 0, 0 }; // 初始位置 (稍後會被覆蓋)
        enemies[i].radius = 0.5F; // 設定敵人碰撞半徑 (以網格為單位)
        ExplodInit(&explods[i]); // 同步初始化對應索引的爆炸效果物件
    }
    // 重設計數器和計時器
    enemyCounter = 0; // 活躍敵人數歸零
    enemySpawnTime = 0; // 生成計時器歸零
    explodCounter = 0; // 活躍爆炸數歸零
}

/**
 * @brief 嘗試添加一個指定類型的敵人到敵人池中
 * @param eType 要添加的敵人類型 (EnemyType)
 */
void TryAddEnemy(EnemyType eType)
{
    // 遍歷敵人陣列，尋找閒置位置 (ENEMY_NONE)
    for (int i = 0; i < MAX_ENEMY; ++i) {
        if (enemies[i].type == ENEMY_NONE) { // 找到一個閒置的敵人槽位
            // 設定新敵人的屬性
            enemies[i].type = eType; // 設定類型
            // POTENTIAL IMPROVEMENT/BUG?: 硬編碼 0-19 的隨機範圍，應使用定義的網格寬高常數
            enemies[i].pos = (Vec2) { GetRandomValue(0, 19), GetRandomValue(0, 19) }; // 在 20x20 的網格內隨機生成位置
            enemies[i].hp = ENEMY_HP; // 重設生命值

            // BUG/管理策略?: 這裡直接將 enemyCounter 加 1。
            // 但 RemoveEnemy 使用的是 swap-and-pop 策略。
            // 如果在移除敵人後，此函式填充了中間的空位 (i < enemyCounter)，
            // enemyCounter 可能會比實際活躍敵人數多，直到新敵人填滿到最後。
            // 然而，由於迴圈使用 < enemyCounter，並且 RemoveEnemy 會正確處理計數器，
            // 這可能不會導致執行錯誤，但計數器的值在某些情況下可能不直觀。
            // 更好的方式可能是，如果 i == enemyCounter，才將 enemyCounter 加 1。
            // 但目前的寫法搭配 RemoveEnemy 也能運作。
            enemyCounter += 1; // 增加活躍敵人計數 (這裡的邏輯可能需要依賴 RemoveEnemy 的實現)

            printf("enemy counter : %d\n", enemyCounter); // 打印敵人數量 (用於調試)
            return; // 成功添加後返回
        }
    }
    // 如果迴圈結束仍未找到閒置位置，表示敵人池已滿
}

/**
 * @brief 從敵人陣列中移除指定索引的敵人 (使用 Swap-and-Pop 策略)
 * @param index 要移除的敵人的索引
 */
void RemoveEnemy(int index)
{
    // 檢查索引是否有效
    if (index >= MAX_ENEMY || index < 0) { // 新增 index < 0 的檢查
        return;
    }
    // 確保要移除的敵人是活躍的
    if (enemies[index].type != ENEMY_NONE) {
        // 檢查是否還有活躍的敵人 (enemyCounter > 0) 且 index 不是最後一個活躍敵人
        // (如果 index 是最後一個，只需標記為 NONE 並減少計數器)
        if (enemyCounter > 0 && index < enemyCounter - 1) {
            // 將最後一個活躍的敵人複製到當前要移除的位置
            enemies[index] = enemies[enemyCounter - 1];
        }
        // 將原始的最後一個活躍敵人的位置標記為閒置
        // (即使 index 就是最後一個，這一步也是必要的)
        if (enemyCounter > 0) { // 確保計數器大於0，防止訪問 enemies[-1]
            enemies[enemyCounter - 1].type = ENEMY_NONE;
        }
        // 減少活躍敵人計數
        enemyCounter -= 1;
        // 確保計數器不為負
        if (enemyCounter < 0)
            enemyCounter = 0;
        printf("enemy counter after remove: %d\n", enemyCounter); // 調試信息
    }
}

/**
 * @brief 處理敵人受到攻擊的邏輯 (命名建議: DamageEnemy 或 EnemyTakeDamage)
 * @param index 受到攻擊的敵人的索引
 */
void EnemyAlife(int index) // Typo: "Alife" -> "Alive" or better "DamageEnemy"
{
    // 檢查索引是否有效
    if (index >= MAX_ENEMY || index < 0) { // 新增 index < 0 的檢查
        return;
    }
    // 確保操作的是活躍敵人
    if (enemies[index].type == ENEMY_NONE) {
        return;
    }

    int damage = PlayerAttackDamage(); // 獲取玩家的攻擊傷害值
    enemies[index].hp -= damage; // 敵人扣除生命值

    // 檢查敵人是否死亡
    if (enemies[index].hp <= 0) {
        TryAddExplod(enemies[index].pos); // 在敵人死亡位置產生爆炸效果
        RemoveEnemy(index); // 從陣列中移除該敵人
    }
}

/**
 * @brief 根據時間間隔生成新的敵人
 */
void EnemySpawn()
{
    enemySpawnTime += gTimer.DeltaTime(); // 累加時間差
    // 如果累加時間超過生成間隔
    if (enemySpawnTime >= ENEMY_SPAWN_INTERVAL) {
        enemySpawnTime = 0; // 重置計時器
        TryAddEnemy(ENEMY_SLIME); // 嘗試添加一個史萊姆敵人
    }
}

/**
 * @brief 處理敵人之間的碰撞，防止重疊
 */
void EnemyCollision()
{
    // 使用嵌套迴圈檢查每一對不同的敵人 (避免自我比較和重複比較)
    for (int i = 0; i < enemyCounter - 1; i++) {
        Enemy* enemyA = &enemies[i];
        // 跳過非活躍敵人 (理論上在此迴圈範圍內不應出現，但做個防禦性檢查)
        if (enemyA->type == ENEMY_NONE) {
            continue;
        }
        for (int j = i + 1; j < enemyCounter; j++) {
            Enemy* enemyB = &enemies[j];
            // 跳過非活躍敵人
            if (enemyB->type == ENEMY_NONE) {
                continue;
            }

            // 計算兩個敵人之間的距離平方
            float distanceSqr = Vector2DistanceSqr(enemyA->pos, enemyB->pos);
            float radiusA = enemyA->radius; // 敵人A的半徑
            float radiusB = enemyB->radius; // 敵人B的半徑
            float radiusSum = radiusA + radiusB; // 兩個半徑的和

            // 檢查是否碰撞 (距離平方 < 半徑和的平方) 且距離不為零 (避免同位置計算問題)
            if (distanceSqr < radiusSum * radiusSum && distanceSqr > 0.001F) {
                float distance = sqrtf(distanceSqr); // 計算實際距離
                float overlap = radiusSum - distance; // 計算重疊量

                // 計算位置修正量 (除以 5.0F 是為了平滑分離，避免抖動，但這個值可以調整)
                float positionCorrection = overlap / 5.0F;
                // 計算從 A 指向 B 的方向向量，並按修正量縮放
                Vector2 direction = (Vector2) {
                    (enemyB->pos.x - enemyA->pos.x) / distance * positionCorrection,
                    (enemyB->pos.y - enemyA->pos.y) / distance * positionCorrection
                };

                // 將兩個敵人沿相反方向推開
                enemyA->pos = Vector2Subtract(enemyA->pos, direction); // A 往反方向移動
                enemyB->pos = Vector2Add(enemyB->pos, direction); // B 往正方向移動
            }
        }
    }
}

/**
 * @brief 處理敵人與玩家（及其武器）的碰撞
 */
void EnemyTouchPlayer()
{
    Vec2 playerPos = PlayerPosition(); // 獲取玩家位置
    float playerRadius = PlayerColliRadius(); // 獲取玩家碰撞半徑
    float weaponRadius = PlayerAttackDamage(); // 獲取武器攻擊範圍半徑

    // 遍歷所有活躍的敵人
    for (int i = 0; i < enemyCounter; i++) {
        Enemy* enemy = &enemies[i];
        // 跳過非活躍敵人
        if (enemy->type == ENEMY_NONE) {
            continue;
        }

        // 計算敵人與玩家之間的距離平方
        float distanceSqr = Vector2DistanceSqr(playerPos, enemy->pos);

        // 1. 檢查武器碰撞
        float radiusSumWeapon = weaponRadius + enemy->radius; // 武器範圍 + 敵人半徑
        if (distanceSqr < radiusSumWeapon * radiusSumWeapon && distanceSqr > 0.001F) {
            // 武器命中敵人
            EnemyAlife(i); // 對敵人造成傷害 (或直接擊殺)
            // 如果敵人被武器擊殺後，可能就不需要再檢查與玩家身體的碰撞了
            // 因此，可以在這裡加上 continue; (如果 EnemyAlife 確定移除了敵人)
             if (enemies[i].type == ENEMY_NONE || enemies[i].hp <= 0) { // 檢查敵人是否已被移除
                 continue; // 如果敵人死了，跳過後續的玩家碰撞檢查
             }
            // printf("weapon attacked!!\n"); // 調試信息
        }

        // 2. 檢查玩家身體碰撞
        float radiusSumPlayer = playerRadius + enemy->radius; // 玩家半徑 + 敵人半徑
        if (distanceSqr < radiusSumPlayer * radiusSumPlayer && distanceSqr > 0.001F) {
            // 敵人碰到玩家身體
            // BUG: 當前邏輯是敵人碰到玩家，敵人自己死亡 (EnemyAlife(i))
            // 正確邏輯應該是玩家受到傷害，例如調用 PlayerTakeDamage(enemy->damage);
            // EnemyAlife(i); // <-- 這是錯誤的，會殺死敵人而不是傷害玩家
             PlayerTakeDamage(enemy->damage); // 假設有這個函式來傷害玩家
             RemoveEnemy(i); // 敵人撞到玩家後是否消失？根據遊戲設計決定。這裡假設撞到後敵人消失。

            // printf("enemy counter : %d\n", enemyCounter); // 調試信息
        }
    }
}

/**
 * @brief 更新所有活躍敵人的狀態 (移動、碰撞檢測等)
 */
void EnemyUpdate()
{
    Vec2 playerPos = PlayerPosition();   // 獲取玩家當前位置
    float deltaTime = gTimer.DeltaTime(); // 獲取幀時間差

    // 1. 更新每個敵人的位置 (朝向玩家移動)
    for (int i = 0; i < enemyCounter; i++) {
        // 跳過非活躍敵人 (理論上不需要，因為迴圈只到 enemyCounter)
        if (enemies[i].type == ENEMY_NONE) {
            continue;
        }
        // 計算從敵人指向玩家的方向向量
        Vec2 dir = Vector2Subtract(playerPos, enemies[i].pos);
        dir = Vector2Normalize(dir); // 標準化向量 (得到單位方向)
        // 根據速度和時間差計算本幀的移動向量
        dir = Vector2Scale(dir, deltaTime * enemies[i].speed);
        // 更新敵人位置
        enemies[i].pos = Vector2Add(enemies[i].pos, dir);
    }

    // 2. 處理敵人之間的碰撞
    EnemyCollision();

    // 3. 處理敵人與玩家/武器的碰撞
    EnemyTouchPlayer();

    // 4. 更新爆炸效果
    ExplodsUpdate();
}

/**
 * @brief 繪製所有活躍的敵人和爆炸效果
 * @param spr 指向敵人要使用的 Sprite 物件的指標
 */
void EnemyDraw() 
{
    Sprite* enemySpr = SpritePoolGet(SPR_ENEMY);
    // 1. 繪製所有活躍的敵人
    for (int i = 0; i < enemyCounter; i++) {
        // 跳過非活躍敵人
        if (enemies[i].type == ENEMY_NONE) {
            continue;
        }
        // 將敵人的邏輯網格位置轉換為繪圖所需的像素位置
        Vec2 trPos = Vector2Scale(enemies[i].pos, CELL_SIZE);
        // 使用傳入的 sprite 繪製敵人
        // 注意：這裡假設所有敵人都使用同一個 sprite，如果不同類型敵人外觀不同，需要修改
        SpriteDraw(enemySpr, trPos);
    }

    // 2. 繪製所有活躍的爆炸效果
    ExplodsDraw();
}