#ifndef __SPRITE_H__
#define __SPRITE_H__

#include "commdef.h"

/**
 * @brief 定義動畫幀的結構，包含紋理和網格信息
 */
typedef struct AnimFrame {
    Texture2D tex; // Raylib 的紋理對象
    int cellW; // 單個網格單元的寬度 (像素)
    int cellH; // 單個網格單元的高度 (像素)
    int centerW; // 網格單元中心的 X 座標 (相對於單元左上角)
    int centerH; // 網格單元中心的 Y 座標 (相對於單元左上角)
    int xCellCount; // 紋理在 X 軸方向上的網格單元數量
    int yCellCount; // 紋理在 Y 軸方向上的網格單元數量
} AnimFrame;

/**
 * @brief 定義 Sprite (精靈) 結構，包含動畫幀數據和動畫狀態
 */
typedef struct {
    int id; // Sprite 的 ID (目前在此代碼中未使用)
    AnimFrame af; // 包含紋理和網格信息的動畫幀數據
    int frame_counter; // 幀計數器 (目前在此代碼中未使用，可能是冗餘的)
    int curr_frame; // 當前顯示的動畫幀索引 (從 0 開始)
    float next_frame_time; // 切換到下一幀所需的時間間隔 (秒)
    float frame_time; // 當前幀已經顯示的時間 (秒)
    bool animate; // 是否播放動畫
    bool centor_cord; // 繪製時是否以中心點為座標基準
} Sprite;

// --- 函數原型宣告 ---

/**
 * @brief 從文件加載紋理並創建 AnimFrame
 * @param fname 紋理圖片文件的路徑
 * @param cell_width Sprite Sheet 中每個單元的寬度
 * @param cell_height Sprite Sheet 中每個單元的高度
 * @return 初始化後的 AnimFrame 結構。如果加載失敗，返回的 AnimFrame.tex.id 為 0。
 */
AnimFrame AnimFrameLoad(const char* fname, uint16_t cell_width, uint16_t cell_height);

/**
 * @brief 卸載 AnimFrame 中的紋理資源
 * @param af 指向要卸載的 AnimFrame 的指標
 */
void AnimFrameUnload(AnimFrame* af);

/**
 * @brief 使用 AnimFrame 初始化 Sprite
 * @param s 指向要初始化的 Sprite 的指標
 * @param af 用於初始化的 AnimFrame 結構
 */
void SpriteLoad(Sprite* s, AnimFrame af);

/**
 * @brief 更新 Sprite 的動畫狀態 (幀切換)
 * @param s 指向要更新的 Sprite 的指標
 */
void SpriteUpdate(Sprite* s);

/**
 * @brief 在指定位置繪製 Sprite 的當前幀
 * @param s 指向要繪製的 Sprite 的指標
 * @param position 繪製的目標位置 (螢幕座標)
 */
void SpriteDraw(Sprite* s, Vector2 position);

#endif
