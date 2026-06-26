#include <gui/model/Model.hpp>
#include <gui/model/ModelListener.hpp>

Model::Model() : modelListener(0)
{
    // Khoi tao gia tri mac dinh cho game
    state.playerX = 105;
    state.score = 0;
    state.lives = 3;
    state.isGameOver = false;
    state.playBuzzerBeep = false;
    highScores[0] = 0;
    highScores[1] = 0;
    highScores[2] = 0;
    
    // Khoi tao dan cua nguoi choi
    state.bulletX = 0;
    state.bulletY = 0;
    state.bulletActive = false;

    // Khoi tao vu no
    state.explosionX = 0;
    state.explosionY = 0;
    state.explosionTimer = 0;

    // Khoi tao vat pham roi
    state.itemX = 0;
    state.itemY = 0;
    state.itemActive = false;
    state.itemType = 0;

    // Khoi tao ten lua (de thanh vien khac code logic)
    state.missileX = 0;
    state.missileY = 0;
    state.missileActive = false;

    // Khoi tao vu no lon
    state.largeExplosionX = 0;
    state.largeExplosionY = 0;
    state.largeExplosionTimer = 0;
    
    // Khoi tao 10 con quai: 2 hang x 5 cot
    // Moi con cach nhau khoang 35px theo chieu ngang, hang 1 Y=40, hang 2 Y=70
    state.enemyDirection = 1; // Mac dinh di qua phai
    for (int i = 0; i < MAX_ENEMIES; i++)
    {
        int row = i / 5;
        int col = i % 5;
        state.enemies[i].x = 15 + col * 35;
        state.enemies[i].y = 40 + row * 25;
        state.enemies[i].alive = true;
        state.enemies[i].type = (row % 3); // Cung 1 hang (row) se co cung loai quai vat (type)
        
        // Khoi tao HP theo tung loai quai (Thanh vien khac se viet logic tru HP khi va cham)
        if (state.enemies[i].type == 0)
            state.enemies[i].hp = 1;
        else if (state.enemies[i].type == 1)
            state.enemies[i].hp = 2;
        else
            state.enemies[i].hp = 3;
    }
}

void Model::fireBullet()
{
    // Chi cho phep ban 1 vien dan tren man hinh tai mot thoi diem
    if (!state.bulletActive && !state.isGameOver)
    {
        state.bulletActive = true;
        state.bulletX = state.playerX + 13; // Canh giua tau (tau rong 30px, dan rong 4px -> 30/2 - 4/2 = 13px)
        state.bulletY = 264; // Dau tau (tau o Y=280, dan cao 16px -> 280 - 16 = 264px)
    }
}

static int tickCount = 0;
void Model::tick()
{
    // 0. Giam timer vu no neu dang dien ra
    if (state.explosionTimer > 0)
    {
        state.explosionTimer--;
    }

    // 0b. Logic vat pham roi tu tu xuong duoi man hinh
    if (state.itemActive && !state.isGameOver)
    {
        state.itemY += 2; // Vat pham roi voi toc do 2px/tick
        
        // Kiem tra vat pham bay ra khoi man hinh duoi (Y > 320)
        if (state.itemY > 320)
        {
            state.itemActive = false;
        }
        else
        {
            // Kiem tra va cham voi tau nguoi choi (X = playerX, Y = 280, W = 30, H = 26)
            // Kich thuoc vat pham: 16x16 px
            int px = state.playerX;
            int py = 280;
            if (state.itemX + 16 >= px && state.itemX <= px + 30 &&
                state.itemY + 16 >= py && state.itemY <= py + 26)
            {
                // Tau an duoc vat pham!
                state.itemActive = false;
                
                // Gia lap hieu ung an vat pham:
                if (state.itemType == 0) // Shield
                {
                    // Cong them mang song ( lives + 1, toi da 9 mang)
                    if (state.lives < 9)
                    {
                        state.lives++;
                    }
                }
                else if (state.itemType == 1) // Star
                {
                    // Cong them 500 diem thuong
                    state.score += 500;
                    if (state.score > 9999) state.score = 9999;
                }
                else if (state.itemType == 2) // Bolt
                {
                    // Cong them 1000 diem thuong
                    state.score += 1000;
                    if (state.score > 9999) state.score = 9999;
                }
            }
        }
    }

    // 1. Logic cap nhat dan bay va check va cham
    if (state.bulletActive && !state.isGameOver)
    {
        state.bulletY -= 6; // Dan bay len voi van toc 6px/tick
        
        // Kiem tra va cham voi tung con quai
        for (int i = 0; i < MAX_ENEMIES; i++)
        {
            if (state.enemies[i].alive)
            {
                // Toa do quai vat (rong 26, cao 22)
                int ex = state.enemies[i].x;
                int ey = state.enemies[i].y;
                
                // Kiem tra va cham AABB (Box Collision)
                if (state.bulletX + 4 >= ex && state.bulletX <= ex + 26 &&
                    state.bulletY + 16 >= ey && state.bulletY <= ey + 22)
                {
                    // Tieu diet quai
                    state.enemies[i].alive = false;
                    state.bulletActive = false; // Huy dan
                    
                    // Kich hoat vu no
                    state.explosionX = ex + 1; // Canh giua vu no (vu no 24x24, quai 26x22)
                    state.explosionY = ey - 1;
                    state.explosionTimer = 8; // Vu no se ton tai trong 8 tick (khoang 0.13 giay)

                    // Roi vat pham voi ty le 50% neu tren man hinh dang khong co vat pham nao
                    if (!state.itemActive)
                    {
                        // Gia lap ti le roi bang cach dung vi tri/diem so
                        if ((state.score + i) % 2 == 0)
                        {
                            state.itemActive = true;
                            state.itemX = ex + 5; // Căn giữa vật phẩm rơi (quái 26px, vật phẩm 16px)
                            state.itemY = ey;
                            state.itemType = (state.score / 100) % 3; // Luan phien cac loai vat pham
                        }
                    }
                    
                    // Cong diem so
                    state.score += 100;
                    if (state.score > 9999) state.score = 9999;
                    break;
                }
            }
        }

        if (state.bulletY < 0)
        {
            state.bulletActive = false; // Xoa dan khi bay khoi man hinh
        }
    }

    // 2. Logic di chuyen quai vat sau moi 15 ticks de giam toc do di chuyen
    tickCount++;
    if (tickCount >= 15 && !state.isGameOver)
    {
        tickCount = 0;
        bool changeDir = false;

        // Kiem tra xem co con quai nao cham bien trai/phai chua
        for (int i = 0; i < MAX_ENEMIES; i++)
        {
            if (state.enemies[i].alive)
            {
                int nextX = state.enemies[i].x + state.enemyDirection * 4;
                // Gioi han chieu rong man hinh 240px, quai vat rong khoang 26px
                if (nextX < 5 || nextX > 240 - 26 - 5)
                {
                    changeDir = true;
                    break;
                }
            }
        }

        if (changeDir)
        {
            state.enemyDirection = -state.enemyDirection; // Doi huong
            for (int i = 0; i < MAX_ENEMIES; i++)
            {
                if (state.enemies[i].alive)
                {
                    state.enemies[i].y += 8; // Di xuong 8px
                    // Kiem tra neu quai vat di xuong qua gan tau nguoi choi (Y = 280)
                    if (state.enemies[i].y >= 260)
                    {
                        state.isGameOver = true;
                        // Chen diem moi vao top 3
                        int16_t curScore = state.score;
                        if (curScore > highScores[0])
                        {
                            highScores[2] = highScores[1];
                            highScores[1] = highScores[0];
                            highScores[0] = curScore;
                        }
                        else if (curScore > highScores[1])
                        {
                            highScores[2] = highScores[1];
                            highScores[1] = curScore;
                        }
                        else if (curScore > highScores[2])
                        {
                            highScores[2] = curScore;
                        }
                    }
                }
            }
        }
        else
        {
            // Di chuyen ngang binh thuong
            for (int i = 0; i < MAX_ENEMIES; i++)
            {
                if (state.enemies[i].alive)
                {
                    state.enemies[i].x += state.enemyDirection * 4;
                }
            }
        }
    }

    // Kiem tra them truong hop het mang song cung ket thuc game
    if (state.lives <= 0 && !state.isGameOver)
    {
        state.isGameOver = true;
        // Chen diem moi vao top 3
        int16_t curScore = state.score;
        if (curScore > highScores[0])
        {
            highScores[2] = highScores[1];
            highScores[1] = highScores[0];
            highScores[0] = curScore;
        }
        else if (curScore > highScores[1])
        {
            highScores[2] = highScores[1];
            highScores[1] = curScore;
        }
        else if (curScore > highScores[2])
        {
            highScores[2] = curScore;
        }
    }

    if (modelListener != 0)
    {
        modelListener->updateGameState(state);
    }
}

void Model::resetGame()
{
    state.playerX = 105;
    state.score = 0;
    state.lives = 3;
    state.isGameOver = false;
    state.playBuzzerBeep = false;
    
    state.bulletX = 0;
    state.bulletY = 0;
    state.bulletActive = false;
    
    state.explosionX = 0;
    state.explosionY = 0;
    state.explosionTimer = 0;
    
    state.itemX = 0;
    state.itemY = 0;
    state.itemActive = false;
    state.itemType = 0;
    
    state.enemyDirection = 1;
    for (int i = 0; i < MAX_ENEMIES; i++)
    {
        int row = i / 5;
        int col = i % 5;
        state.enemies[i].x = 15 + col * 35;
        state.enemies[i].y = 40 + row * 25;
        state.enemies[i].alive = true;
        state.enemies[i].type = (row % 3);
    }
}
