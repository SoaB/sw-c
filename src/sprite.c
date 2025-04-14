#include "sprite.h"
#include "raylib.h"
#include "timer.h"
#include <stdio.h>

/**
 * @brief 從文件加載紋理並創建 AnimFrame
 */
AnimFrame AnimFrameLoad(const char* fname, uint16_t cell_width, uint16_t cell_height)
{
    AnimFrame a = { 0 }; // 初始化結構體

    a.tex = LoadTexture(fname); // 使用 Raylib 加載紋理
    // 檢查紋理是否加載成功 (Raylib 中，失敗時 id 為 0)
    if (a.tex.id == 0) {
        printf("Error: Failed to load texture from %s\n", fname);
        // 加載失敗，直接返回包含無效紋理 ID 的結構
        // 調用者應檢查返回的 a.tex.id
        return a;
    }
    // 檢查傳入的單元格尺寸是否有效
    if (cell_width <= 0 || cell_height <= 0) {
        // 如果尺寸無效，將相關尺寸設為 0，避免後續計算出錯
        a.cellW = 0;
        a.cellH = 0;
        a.centerW = 0;
        a.centerH = 0;
        a.xCellCount = 0;
        a.yCellCount = 0;
        printf("Warning: Invalid cell width or height (%d, %d) for texture %s\n", cell_width, cell_height, fname);
    } else {
        // 尺寸有效，計算相關屬性
        a.cellW = cell_width;
        // 計算中心點 (整數除法，對於奇數寬高可能會有 0.5px 的偏差)
        a.centerW = a.cellW / 2;
        a.cellH = cell_height;
        a.centerH = a.cellH / 2;
        // 計算 X 和 Y 方向上的單元格數量
        // POTENTIAL ISSUE: 如果紋理尺寸不能被 cellW/cellH 整除，這裡會取整，可能導致最後一行/列不完整或計算錯誤
        a.xCellCount = a.tex.width / a.cellW;
        a.yCellCount = a.tex.height / a.cellH;
    }
    return a; // 返回初始化後的 AnimFrame
}
/**
 * @brief 卸載 AnimFrame 中的紋理資源
 */
void AnimFrameUnload(AnimFrame* af)
{
    // 使用 Raylib 的函數卸載紋理，釋放 GPU 內存
    UnloadTexture(af->tex);
    // 可以選擇性地將 af->tex.id 設為 0，表示已卸載
    af->tex.id = 0;
}
/**
 * @brief 使用 AnimFrame 初始化 Sprite
 */
void SpriteLoad(Sprite* s, AnimFrame af)
{
    s->af = af; // 將 AnimFrame 數據複製到 Sprite 結構中
    // 設定動畫的預設值
    s->next_frame_time = 0.1F; // 預設每 0.1 秒切換一幀 (10 FPS)
    s->centor_cord = true; // 預設繪製時以中心點為基準
    s->curr_frame = 0; // 從第 0 幀開始
    // s->frame_counter = 0;   // 這個變數在 Update 和 Draw 中似乎未使用，可能是冗餘的
    s->frame_time = 0.0F; // 初始化當前幀顯示時間
    s->animate = true; // 預設啟用動畫
}
/**
 * @brief 更新 Sprite 的動畫狀態
 */
void SpriteUpdate(Sprite* s)
{
    // 只有在啟用動畫時才更新
    if (s->animate) {
        // 累加當前幀顯示的時間 (依賴外部 gTimer 提供 DeltaTime)
        s->frame_time += gTimer.DeltaTime();
        // 如果顯示時間超過了設定的下一幀時間間隔
        if (s->frame_time >= s->next_frame_time) {
            s->frame_time = 0.0F; // 重置計時器 (或 s->frame_time -= s->next_frame_time; 可能更精確)
            s->curr_frame++; // 切換到下一幀
            // 計算總幀數
            int total_frames = s->af.xCellCount * s->af.yCellCount;
            // 如果總幀數大於 0 (避免除以零) 且當前幀超過總幀數，則循環回第 0 幀
            if (total_frames > 0 && s->curr_frame >= total_frames) {
                s->curr_frame = 0; // 動畫循環
            } else if (total_frames <= 0) {
                s->curr_frame = 0; // 如果沒有有效的幀，則保持在第0幀
            }
        }
    }
}
/**
 * @brief 在指定位置繪製 Sprite 的當前幀
 */
void SpriteDraw(Sprite* s, Vector2 position)
{
    // 確保 AnimFrame 中的紋理是有效的 (已成功加載)
    if (s->af.tex.id > 0 && s->af.xCellCount > 0 && s->af.yCellCount > 0) { // 增加檢查 xCellCount/yCellCount > 0
        // 計算當前幀在 Sprite Sheet 中的行和列
        int frame_col = s->curr_frame % s->af.xCellCount;
        // BUG FIX: 原本這裡固定為 0，無法讀取第一行以外的幀。修正為計算行號。
        int frame_row = s->curr_frame / s->af.xCellCount;
        // 計算來源矩形 (source rectangle)，即從紋理的哪個區域取圖像
        Rectangle sourceRec = {
            (float)(frame_col * s->af.cellW), // X 座標 = 列號 * 單元寬度
            (float)(frame_row * s->af.cellH), // Y 座標 = 行號 * 單元高度 (修正 Bug)
            (float)s->af.cellW, // 寬度 = 單元寬度
            (float)s->af.cellH // 高度 = 單元高度
        };
        // 計算目標矩形 (destination rectangle)，即在螢幕上繪製的區域
        // BUG FIX: DrawTexturePro 的 origin 參數會處理中心點，無需手動調整 destRec 的 x, y
        Rectangle destRec = {
            position.x,          // 目標 X 座標
            position.y,          // 目標 Y 座標
            (float)s->af.cellW,  // 繪製寬度
            (float)s->af.cellH   // 繪製高度
        };
        // 設定繪圖的原點 (origin)，用於旋轉和定位
        Vector2 origin = { 0, 0 }; // 預設原點為左上角
        if (s->centor_cord) {
            // 如果設定為中心座標，將原點設為單元的中心
            origin = (Vector2) { (float)s->af.centerW, (float)s->af.centerH };
        }
        // 使用 Raylib 的 DrawTexturePro 函數進行繪製
        // 參數：紋理, 來源矩形, 目標矩形, 原點, 旋轉角度 (0), 顏色濾鏡 (WHITE=無)
        DrawTexturePro(s->af.tex, sourceRec, destRec, origin, 0.0F, WHITE);
    } else if (s->af.tex.id <= 0) {
        printf("Warning: Attempting to draw sprite with invalid texture ID.\n");
    }
}
