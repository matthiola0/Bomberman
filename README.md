# 爆爆王之孤獨搖滾版 (Bomberman: Bocchi the Rock Edition)

這是一款使用 C++ 和 Allegro 5 遊戲函式庫開發的單機版爆爆王（Bomberman-like）遊戲，其主題靈感來自於人氣動漫《孤獨搖滾！》。

玩家將操控「團結 Band」的成員，使用她們的樂器作為炸彈，在舞台上盡情揮灑，透過爆炸將地盤染上自己的顏色。在時間結束時，佔有最廣闊地盤的玩家將獲得勝利！

<!-- ![Game Screenshot](https://i.imgur.com/gK6wZJ1.png)
*(這是一個範例圖片，您可以替換成您的遊戲截圖)* -->

## 遊戲特色

* **經典玩法，全新體驗**：融合了經典的爆爆王玩法與創新的「佔地為王」計分模式。
* **孤獨搖滾主題**：可選擇四位主角（後藤一里、伊地知虹夏、山田涼、喜多郁代）進行遊戲。
* **多人對戰**：支援 1 位或 2 位玩家與多名電腦 AI 進行對戰。
* **多樣地圖**：包含三張精心設計的地圖：學校、STARRY 展演廳、波奇的房間。
* **動態場景**：地圖中的部分障礙物可以被炸毀，並有機會掉落增益道具。

## 如何遊玩

### 遊戲目標
在時間限制內，藉由炸彈爆炸將最多數量的地板區塊染成自己的代表色。時間到點後，地盤面積最廣的玩家獲勝。

### 操作方式
* **玩家 1**
    * **移動**: `↑` `↓` `←` `→` (方向鍵)
    * **放置炸彈**: `Space` (空白鍵)

* **玩家 2**
    * **移動**: `W` `A` `S` `D`
    * **放置炸彈**: `Left Shift`

<!-- ## 安裝與編譯

本專案使用 Allegro 5 函式庫，請確保您已安裝 C++ 編譯器 (如 GCC/G++) 與對應的 Allegro 5 開發套件。

### 依賴項目
* Allegro 5
* Allegro 5 Addons:
    * `allegro_main`
    * `allegro_primitives`
    * `allegro_font`
    * `allegro_ttf`
    * `allegro_image`
    * `allegro_audio`
    * `allegro_acodec`
* `algif` (已包含在專案 `algif5` 目錄中)

### 編譯指令

打開您的終端機 (Terminal) 或命令提示字元 (Command Prompt)，進入專案的根目錄，然後執行以下指令來編譯遊戲：

```bash
g++ -o bocchi_bomber \
    Main.cpp \
    GameWindow.cpp \
    Menu.cpp \
    Level.cpp \
    BandMember.cpp \
    Bomb.cpp \
    Circle.cpp \
    global.cpp \
    algif5/src/algif.cpp \
    algif5/src/bitmap.cpp \
    algif5/src/gif.cpp \
    algif5/src/lzw.cpp \
    -lallegro -lallegro_main -lallegro_primitives -lallegro_font -lallegro_ttf -lallegro_image -lallegro_audio -lallegro_acodec
```
> **注意**: 上述指令適用於 Linux/macOS 或 Windows 上的 MinGW/MSYS2 環境。如果您使用 Visual Studio，請在專案屬性中設定對應的標頭檔與函式庫路徑。

編譯成功後，會生成一個名為 `bocchi_bomber` (或 `bocchi_bomber.exe`) 的執行檔。 -->

## 專案結構
```
.
├── Main.cpp               # 程式進入點
├── GameWindow.cpp/.h      # 主遊戲視窗與遊戲迴圈管理
├── Menu.cpp/.h            # 遊戲選單邏輯
├── Level.cpp/.h           # 關卡場景，管理所有角色、炸彈與地圖物件
├── BandMember.cpp/.h      # 角色的基礎類別，包含玩家輸入與 AI 邏輯
├── bocchi.h, kita.h, ...  # 各角色的衍生類別
├── Bomb.cpp/.h            # 炸彈物件的邏輯
├── global.cpp/.h          # 全域變數與設定
├── LEVEL1.txt, ...        # 地圖佈局檔案
└── algif5/                  # 用於播放 GIF 動畫的函式庫
```

---
**開發者**: 章竣雲, 潘柏育