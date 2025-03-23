#pragma once

/*** Actionクラス ***/
#pragma once
#include <cmath>
#include "DxLib.h"
#include <vector>
#include <random> 


/*** Actionクラス ***/
class ACTION
{
public:
    // 発射する玉（弾）の最大数
    static const int MAX_BULLETS = 10;

    // 弾の構造体
    struct BULLET {
        int X = 0;           // x座標
        int Y = 0;           // y座標
        int InitialX = 0;    // 発射時の初期x座標
        int Dir = DIR_RI;    // 進行方向（DIR_RIまたはDIR_LE）
        bool Active = false; // 使用中フラグ
    };

    // 敵キャラ構造体
    struct ENEMY {
        struct {
            int X = 0; // x座標
            int Y = 0; // y座標
        } Pos;
        int MinX = 0;    // 移動範囲の左端
        int MaxX = 0;    // 移動範囲の右端
        int Dir = DIR_RI;    // 進行方向（DIR_RIまたはDIR_LE）
        int Touch = DIR_NONE; // 接触方向
        bool Alive = true;   // 生存状態
        int Speed = 2;    // 敵の移動速度（独自の値を設定）
    };

    // メインキャラ構造体
    struct CHARACTER
    {
        struct {
            int X = CHA_POS_X_INI;
            int Y = CHA_POS_Y_INI;
            int Yin = CHA_POS_Y_INI;  // 着地前Y初期位置
        } Pos;
        struct {
            struct { int Ri = 0, Up = 0, Ce = 0; } RiUp;
            struct { int Le = 0, Up = 0, Ce = 0; } LeUp;
            struct { int Ri = 0, Do = 0, Ce = 0; } RiDo;
            struct { int Le = 0, Do = 0, Ce = 0; } LeDo;
        } Cor;
        int Dir = DIR_NONE;       // 入力による移動方向
        int PicDir = DIR_RI;      // 描画用向き
        int Touch = DIR_NONE;     // 接触している方向
        int Fall = FALSE;         // 落下状態
    };

    // 移動用構造体
    struct MOVEMENT
    {
        int X = MOVEX;          // X方向移動量
        int Y = 0;              // Y方向移動量
        int JumpState = JUMP_OFF;  // ジャンプ状態
        bool Dash = OFF;        // ダッシュフラグ
    };

public:
    // コンストラクタ
    ACTION() : jumpCount(0), maxJump(2)
    {
        Sta_PosX = STG_X_MIN;
        InitEnemies();
        MainChar.Pos.X = CHA_POS_X_INI;
        MainChar.Pos.Y = CHA_POS_Y_INI;
        MainChar.Pos.Yin = CHA_POS_Y_INI;
        MainChar.Dir = DIR_NONE;
        MainChar.PicDir = DIR_RI;
        MainChar.Touch = DIR_NONE;
        MainChar.Fall = FALSE;
        Mov.JumpState = JUMP_OFF;
        Mov.Y = 0;
        Cou = 0;



        Goal = FALSE;
        EndFlag = FALSE;

        for (int i = 0; i < MAX_BULLETS; i++) {
            Bullets[i].Active = false;
        }
    }

    // 更新処理：メインキャラの四角形座標情報を更新
    void Update()
    {
        MainChar.Dir = DIR_NONE;
        MainChar.Touch = DIR_NONE;

        MainChar.Cor.RiUp.Ce = Sta.Cood.Pix[MainChar.Pos.X - Sta_PosX + CELL - 1][MainChar.Pos.Y];
        MainChar.Cor.RiDo.Ce = Sta.Cood.Pix[MainChar.Pos.X - Sta_PosX + CELL - 1][MainChar.Pos.Y + CELL - 1];
        MainChar.Cor.LeUp.Ce = Sta.Cood.Pix[MainChar.Pos.X - Sta_PosX][MainChar.Pos.Y];
        MainChar.Cor.LeDo.Ce = Sta.Cood.Pix[MainChar.Pos.X - Sta_PosX][MainChar.Pos.Y + CELL - 1];
    }

    // 判定処理：接触、入力、各種衝突判定（ジャンプ・左右移動・玉発射など）
    void Judge()
    {
        // ダッシュ判定
        if ((Key.input[KEY_INPUT_LSHIFT] > 0)
#ifdef DEF_JOYPAD_VALID
            || (JPad.input.Buttons[0] == 128)
#endif
            )
        {
            Mov.Dash = ON;
            Mov.X = MOVEX_D;
        }
        else
        {
            Mov.Dash = OFF;
            Mov.X = MOVEX;
        }

        // 左右の接触予測（X方向）
        MainChar.Cor.RiUp.Ri = Sta.Cood.Pix[MainChar.Pos.X - Sta_PosX + CELL - 1 + Mov.X][MainChar.Pos.Y];
        MainChar.Cor.RiDo.Ri = Sta.Cood.Pix[MainChar.Pos.X - Sta_PosX + CELL - 1 + Mov.X][MainChar.Pos.Y + CELL - 1];
        MainChar.Cor.LeUp.Le = Sta.Cood.Pix[MainChar.Pos.X - Sta_PosX - Mov.X][MainChar.Pos.Y];
        MainChar.Cor.LeDo.Le = Sta.Cood.Pix[MainChar.Pos.X - Sta_PosX - Mov.X][MainChar.Pos.Y + CELL - 1];

        // 上下の接触予測（Y方向）
        MainChar.Cor.RiUp.Up = Sta.Cood.Pix[MainChar.Pos.X - Sta_PosX + CELL - 1][MainChar.Pos.Y - 1];
        MainChar.Cor.LeUp.Up = Sta.Cood.Pix[MainChar.Pos.X - Sta_PosX][MainChar.Pos.Y - 1];
        MainChar.Cor.RiDo.Do = Sta.Cood.Pix[MainChar.Pos.X - Sta_PosX + CELL - 1][MainChar.Pos.Y + CELL];
        MainChar.Cor.LeDo.Do = Sta.Cood.Pix[MainChar.Pos.X - Sta_PosX][MainChar.Pos.Y + CELL];

        if (MainChar.Cor.RiUp.Ri != PIC_BACK || MainChar.Cor.RiDo.Ri != PIC_BACK)
            MainChar.Touch |= DIR_RI;
        if (MainChar.Cor.LeUp.Le != PIC_BACK || MainChar.Cor.LeDo.Le != PIC_BACK)
            MainChar.Touch |= DIR_LE;
        if (MainChar.Cor.RiUp.Up != PIC_BACK || MainChar.Cor.LeUp.Up != PIC_BACK)
            MainChar.Touch |= DIR_UP;
        if (MainChar.Cor.RiDo.Do != PIC_BACK || MainChar.Cor.LeDo.Do != PIC_BACK)
            MainChar.Touch |= DIR_DO;

        // 移動入力判定
        if ((Key.input[KEY_INPUT_SPACE] == 1 || Key.input[KEY_INPUT_UP] == 1)
#ifdef DEF_JOYPAD_VALID
            || ((JPad.input.Buttons[1] == 128) && (JPad.input_X_Z1 == 0))
#endif
            )
        {
            if ((Mov.JumpState == JUMP_OFF) && ((MainChar.Touch & DIR_DO) == DIR_DO))
            {
                Mov.JumpState = JUMP_UP;
                jumpCount = 1;
#ifdef DEF_SOUND_VALID
                Snd.PlayJumpSound();
#endif
            }
            // 空中での追加ジャンプ（例：二段ジャンプ）
            else if (Mov.JumpState == JUMP_GRAVITY && jumpCount < maxJump)
            {
                Mov.JumpState = JUMP_UP;
                jumpCount++;
#ifdef DEF_SOUND_VALID
                Snd.PlayJumpSound();
#endif
            }
        }
        else if ((Key.input[KEY_INPUT_D] > 0 || Key.input[KEY_INPUT_RIGHT] > 0)
#ifdef DEF_JOYPAD_VALID
            || (JPad.input.POV[0] > 0 && JPad.input.POV[0] < 18000)
#endif
            )
        {
            MainChar.Dir = DIR_RI;
            MainChar.PicDir = DIR_RI;
        }
        else if ((Key.input[KEY_INPUT_A] > 0 || Key.input[KEY_INPUT_LEFT] > 0)
#ifdef DEF_JOYPAD_VALID
            || (JPad.input.POV[0] > 18000 && JPad.input.POV[0] < 36000)
#endif
            )
        {
            MainChar.Dir = DIR_LE;
            MainChar.PicDir = DIR_LE;
        }

        // ゴール判定
        if (abs(Sta_PosX) > GOAL_POS_X)
        {
            Goal = TRUE;
        }

        // 終了判定（Escキー、落下、敵との接触、ゴールなど）
        if ((Key.input[KEY_INPUT_ESCAPE] > 0) ||
            (MainChar.Fall == TRUE) ||

            (Goal == TRUE)
#ifdef DEF_JOYPAD_VALID
            || (JPad.input.Buttons[12] == 128)
#endif
            )
        {
            InitEnemies();
            EndFlag = TRUE;
        }

        // 玉（弾）発射判定（スペースキー）
        if (Key.input[KEY_INPUT_P] == 1) {
            for (int i = 0; i < MAX_BULLETS; i++) {
                if (!Bullets[i].Active) {
                    Snd.PlayBulletSound();
                    Bullets[i].Active = true;
                    Bullets[i].X = MainChar.Pos.X + CELL / 2;
                    Bullets[i].Y = MainChar.Pos.Y + CELL / 2;
                    Bullets[i].Dir = MainChar.PicDir; // キャラの向きに合わせて発射
                    Bullets[i].InitialX = Bullets[i].X; // 発射時の初期位置を記録
                    break;
                }
            }
        }
    }
    void CheckCollision() {
        // プレイヤーと敵の当たり判定
        for (auto& enemy : Enemies) {
            if (enemy.Alive) {
                if ((MainChar.Pos.X + CELL >= enemy.Pos.X) &&
                    (MainChar.Pos.X < enemy.Pos.X + CELL) &&
                    (MainChar.Pos.Y + CELL > enemy.Pos.Y) &&
                    (MainChar.Pos.Y < enemy.Pos.Y + CELL)) {
                    // プレイヤーが敵に接触した場合の処理（例: ダメージやゲームオーバー）
                    MainChar.Fall = TRUE; // ダメージ状態にする
                    InitEnemies();
                }
            }
        }

        // 玉（弾）と敵キャラの当たり判定
        for (auto& bullet : Bullets) {
            if (bullet.Active) {
                for (auto& enemy : Enemies) {
                    if (enemy.Alive &&
                        bullet.X >= enemy.Pos.X &&
                        bullet.X <= enemy.Pos.X + CELL &&
                        bullet.Y >= enemy.Pos.Y &&
                        bullet.Y <= enemy.Pos.Y + CELL) {
                        // 玉と敵の衝突処理
                        Snd.PlayDamageSound();
                        enemy.Alive = false;    // 敵を非アクティブ化
                        bullet.Active = false; // 玉を非アクティブ化
                    }
                }
            }
        }
    }
    const int MAX_BULLET_DISTANCE = 250;
    /*** 移動計算 ***/
    void Cal()
    {
        UpdateEnemies();   // 敵キャラの移動処理
        CheckCollision();
        SpawnEnemyNearPlayer();

        // X方向移動計算（キャラ移動orステージスクロール）
        if (MainChar.Dir == DIR_RI)
        {
            if ((MainChar.Touch & DIR_RI) != DIR_RI)
            {
                if (MainChar.Pos.X < WIN_MAX_X / 2)
                    MainChar.Pos.X += Mov.X;
                else if ((MainChar.Pos.X >= WIN_MAX_X / 2) && (abs(Sta_PosX) < STG_X_MAX - WIN_MAX_X))
                    Sta_PosX -= Mov.X;
            }
        }
        else if (MainChar.Dir == DIR_LE)
        {
            if ((MainChar.Touch & DIR_LE) != DIR_LE)
            {
                if (MainChar.Pos.X > STG_X_MIN)
                    MainChar.Pos.X -= Mov.X;
                else if ((MainChar.Pos.X <= STG_X_MIN) && (Sta_PosX < STG_X_MIN))
                    Sta_PosX += Mov.X;
            }
        }

        // Y方向移動計算（ジャンプ上昇／重力落下）
        if (Mov.JumpState == JUMP_UP)
        {
            Cou++;
            T = T_k * ((double)Cou / 60.0);
            Mov.Y = (int)(pow(T, 2.0));
            for (int y = 1; y <= Mov.Y; y++)
            {
                if ((MOVEY_H_MAX == MainChar.Pos.Yin - MainChar.Pos.Y) ||
                    (Sta.Cood.Pix[MainChar.Pos.X - Sta_PosX][MainChar.Pos.Y - 1] != PIC_BACK) ||
                    (Sta.Cood.Pix[MainChar.Pos.X - Sta_PosX + CELL - 1][MainChar.Pos.Y - 1] != PIC_BACK))
                {
                    Mov.JumpState = JUMP_GRAVITY;
                    Cou = 0;
                    break;
                }
                else
                {
                    MainChar.Pos.Y--;
                }
            }
        }
        else if ((Mov.JumpState == JUMP_GRAVITY) || ((MainChar.Touch & DIR_DO) != DIR_DO))
        {
            if (MainChar.Pos.Y < STG_FALL_Y)
            {
                Cou++;
                T = T_k * ((double)Cou / 60.0);
                Mov.Y = (int)(pow(T, 2.0));
                for (int y = 1; y <= Mov.Y; y++)
                {
                    if ((Sta.Cood.Pix[MainChar.Pos.X - Sta_PosX][MainChar.Pos.Y + CELL] != PIC_BACK) ||
                        (Sta.Cood.Pix[MainChar.Pos.X - Sta_PosX + CELL - 1][MainChar.Pos.Y + CELL] != PIC_BACK))
                    {
                        MainChar.Pos.Yin = MainChar.Pos.Y;
                        Mov.JumpState = JUMP_OFF;
                        jumpCount = 0;
                        Cou = 0;
                        break;
                    }
                    else
                    {
                        MainChar.Pos.Y++;
                    }
                }
            }
            else
            {
                MainChar.Fall = TRUE;
            }
        }

        // 玉（弾）の更新処理
        const int BULLET_SPEED = 8;
        for (int i = 0; i < MAX_BULLETS; i++) {
            if (Bullets[i].Active) {
                // 弾の移動
                if (Bullets[i].Dir == DIR_RI)
                    Bullets[i].X += BULLET_SPEED;
                else if (Bullets[i].Dir == DIR_LE)
                    Bullets[i].X -= BULLET_SPEED;

                // 飛距離を計算
                int distanceTravelled = abs(Bullets[i].X - Bullets[i].InitialX);

                // 一定距離を超えたら非アクティブ化
                if (distanceTravelled > MAX_BULLET_DISTANCE) {
                    Bullets[i].Active = false;
                }
            }
        }
    }

    // メインキャラ描画
    void Cha()
    {
        if (MainChar.PicDir == DIR_RI)
            DrawGraph(MainChar.Pos.X, MainChar.Pos.Y, Pic.MChara, TRUE);
        else if (MainChar.PicDir == DIR_LE)
            DrawTurnGraph(MainChar.Pos.X, MainChar.Pos.Y, Pic.MChara, TRUE);
    }

    void InitEnemies() {
        Enemies.clear();

        // ランダムエンジンの準備
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> distSpeed(1, 3); // 敵の速度範囲

        for (int i = 0; i < 3; i++) { // 5体の敵を生成
            ENEMY enemy;
            enemy.Pos.X = 300 + i * 150;   // 初期位置
            enemy.Pos.Y = ENE1_POS_Y_INI; // 初期Y座標
            enemy.MinX = 300 + i * 200 - 50; // 固定の移動範囲
            enemy.MaxX = 300 + i * 200 + 50; // 固定の移動範囲
            enemy.Dir = DIR_RI;
            enemy.Alive = true;
            enemy.Speed = distSpeed(gen); // ランダムな速度を設定
            Enemies.push_back(enemy);
        }
    }

    void UpdateEnemies() {
        // ランダムエンジンの準備（往復距離と速度のランダム範囲を設定）
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> distRange(150, 500);  // 往復距離のランダム範囲
        std::uniform_int_distribution<> distSpeed(1, 5);    // 速度のランダム範囲

        for (auto& enemy : Enemies) {
            if (enemy.Alive) {
                if (enemy.Dir == DIR_RI) { // 右方向移動
                    enemy.Pos.X += enemy.Speed;
                    if (enemy.Pos.X >= enemy.MaxX) {
                        // 左方向に切り替え
                        enemy.Dir = DIR_LE;

                        // 新しい移動範囲を生成
                        int newRange = distRange(gen);
                        enemy.MinX = enemy.Pos.X - newRange;
                        enemy.MaxX = enemy.Pos.X;

                        // 新しい速度を生成
                        enemy.Speed = distSpeed(gen);
                    }
                }
                else if (enemy.Dir == DIR_LE) { // 左方向移動
                    enemy.Pos.X -= enemy.Speed;
                    if (enemy.Pos.X <= enemy.MinX) {
                        // 右方向に切り替え
                        enemy.Dir = DIR_RI;

                        // 新しい移動範囲を生成
                        int newRange = distRange(gen);
                        enemy.MinX = enemy.Pos.X;
                        enemy.MaxX = enemy.Pos.X + newRange;

                        // 新しい速度を生成
                        enemy.Speed = distSpeed(gen);
                    }
                }
            }
        }
    }

    void SpawnEnemyNearPlayer() {
        // ランダムエンジンの準備（X座標のランダム範囲を設定）
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> xDist(MainChar.Pos.X - 150, MainChar.Pos.X + 150); // プレイヤー周囲のランダム範囲

        if (spawnTimer >= SPAWN_INTERVAL) {
            ENEMY enemy;

            // 一定距離離れるまでランダム座標を再生成
            do {
                enemy.Pos.X = xDist(gen); // X座標をランダム生成
            } while (abs(enemy.Pos.X - MainChar.Pos.X) < 100); // プレイヤーから100px以上離れる

            enemy.Pos.Y = MainChar.Pos.Y;    // Y座標はプレイヤーと同じ
            enemy.MinX = enemy.Pos.X - 50;   // 移動範囲（固定）
            enemy.MaxX = enemy.Pos.X + 50;   // 移動範囲（固定）
            enemy.Dir = DIR_RI;              // 初期方向を右
            enemy.Alive = true;

            // ランダムな速度を設定
            std::uniform_int_distribution<> distSpeed(1, 3);
            enemy.Speed = distSpeed(gen);

            // 敵をリストに追加
            Enemies.push_back(enemy);

            // タイマーをリセット
            spawnTimer = 0;
        }
        else {
            // タイマーを進める
            spawnTimer++;
        }
    }






    // 敵キャラの描画
    void DrawEnemies() {
        for (const auto& enemy : Enemies) {
            if (enemy.Alive) {
                DrawGraph(enemy.Pos.X, enemy.Pos.Y, Pic.Enemy1, TRUE); // 敵キャラ画像を描画
            }
        }
    }
    // 玉（弾）の描画
    const int BULLET_WIDTH = 30;
    const int BULLET_HEIGHT = 30;

    void BulletDraw()
    {
        for (int i = 0; i < MAX_BULLETS; i++)
        {
            if (Bullets[i].Active)
            {
                DrawGraph(Bullets[i].X - BULLET_WIDTH / 2, Bullets[i].Y - BULLET_HEIGHT / 2, Pic.Bullet, TRUE);
            }
        }
    }

    // 表示系描画（タイム、コマンド説明等）
    void Disp()
    {
        for (int i = 0; i < RANK_DISP_NUM; i++)
        {
            DrawFormatStringFToHandle(RANK_POS_X, static_cast<float>(RANK_POS_Y + i * 10),
                Col.Black, Fon.FH[10], "No.%d:%6.2f s", i + 1, Sta.Rank[i]);
        }
        DrawFormatStringFToHandle(TIME_POS_X, TIME_POS_Y, Col.Black, Fon.FH[10],
            "Time:%6.2f s", static_cast<float>((GetNowCount() - Sta.StartCount) / MillSecond));

        DrawFormatStringFToHandle(COMD_POS_X, COMD_POS_Y, Col.Black, Fon.FH[10], "右移動：[D]");
        DrawFormatStringFToHandle(COMD_POS_X, COMD_POS_Y + 10, Col.Black, Fon.FH[10], "左移動：[A]");
        DrawFormatStringFToHandle(COMD_POS_X, COMD_POS_Y + 20, Col.Black, Fon.FH[10], "ジャンプ：[Space]");
        DrawFormatStringFToHandle(COMD_POS_X, COMD_POS_Y + 30, Col.Black, Fon.FH[10], "ダッシュ：[左SHIFT]");
        DrawFormatStringFToHandle(COMD_POS_X, COMD_POS_Y + 40, Col.Black, Fon.FH[10], "Titleに戻る：[Esc]");
        DrawFormatStringFToHandle(COMD_POS_X, COMD_POS_Y + 50, Col.Black, Fon.FH[10], "手裏剣を投げる：[P]");
    }

    // Actシーン終了時の初期化
    void EndInit()
    {
        Sta_PosX = STG_X_MIN;

        MainChar.Pos.X = CHA_POS_X_INI;
        MainChar.Pos.Y = CHA_POS_Y_INI;
        MainChar.Pos.Yin = CHA_POS_Y_INI;
        MainChar.Dir = DIR_NONE;
        MainChar.PicDir = DIR_RI;
        MainChar.Touch = DIR_NONE;
        MainChar.Fall = FALSE;
        Mov.JumpState = JUMP_OFF;
        Mov.Y = 0;
        Cou = 0;



        Goal = FALSE;
        EndFlag = FALSE;

        for (int i = 0; i < MAX_BULLETS; i++) {
            Bullets[i].Active = false;
        }
    }

    // Actシーン本処理
    int Out()
    {
        int ret = SCE::SCE_01_ACT;

        Update();
        Judge();
        Cal();

        Sta.Out(&Sta_PosX);
        Cha();
        DrawEnemies();
        BulletDraw();
        Disp();

        if (EndFlag == TRUE)
        {
            if (Goal == TRUE)
                Sta.UpdateTime();
            EndInit();
            ret = SCE::SCE_00_TIT;
        }

        return ret;
    }

    // --- メンバ変数 ---

    CHARACTER MainChar;


    BULLET Bullets[MAX_BULLETS];

private:
    int Goal = FALSE;
    int EndFlag = FALSE;
    int Cou = 0;
    double T = 0.0;
    const double T_k = 20.0;
    std::vector<ENEMY> Enemies; // 複数の敵を管理するリスト
    int Sta_PosX = STG_X_MIN;   // ステージ位置オフセット
    MOVEMENT Mov;
    int jumpCount;      // 現在のジャンプ回数
    const int maxJump;  // 最大ジャンプ回数（例：2）
    int spawnTimer = 0; // 敵のスポーンタイマー
    const int SPAWN_INTERVAL = 150; // スポーン間隔（フレーム数）
} Act;