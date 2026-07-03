#include <gui/model/Model.hpp>
#include <gui/model/ModelListener.hpp>

Model::Model() : modelListener(0), playerMoveDirection(0), playerMoveTimer(0), enemyShootCooldown(90), nextEnemyShooterIndex(0), missileAmmo(0)
{
    // Khoi tao gia tri mac dinh cho game
    state.playerX = 105;
    state.score = 0;
    state.lives = 3;
    state.level = 1;
    state.levelIntroTimer = 60;
    state.isGameOver = false;
    state.playBuzzerBeep = false;
    highScores[0] = 0;
    highScores[1] = 0;
    highScores[2] = 0;
    
    // Khoi tao dan cua nguoi choi
    state.bulletX = 0;
    state.bulletY = 0;
    state.bulletActive = false;
    state.enemyBulletX = 0;
    state.enemyBulletY = 0;
    state.enemyBulletActive = false;

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
    
    state.enemyDirection = 1; // Mac dinh di qua phai
    initializeEnemiesForLevel(state.level);
}

void Model::initializeEnemiesForLevel(uint8_t level)
{
    if (level == 0)
    {
        level = 1;
    }

    const int16_t startY = 40 + (((level - 1) / 2) > 5 ? 5 : ((level - 1) / 2)) * 3;
    const uint8_t pattern = (level - 1) % 5;
    for (int i = 0; i < MAX_ENEMIES; i++)
    {
        int row = i / 5;
        int col = i % 5;
        int16_t x = 15 + col * 35;
        int16_t y = startY + row * 25;

        if (pattern == 1)
        {
            x = 28 + i * 27;
            y = startY + 16;
        }
        else if (pattern == 2)
        {
            const int16_t diamondX[MAX_ENEMIES] = {105, 82, 128, 59, 105, 151, 82, 128, 105, 105};
            const int16_t diamondY[MAX_ENEMIES] = {0, 20, 20, 40, 40, 40, 60, 60, 80, 100};
            x = diamondX[i];
            y = startY + diamondY[i];
        }
        else if (pattern == 3)
        {
            x = 12 + col * 40 + ((row == 0) ? 0 : 18);
            y = startY + row * 32 + ((col % 2) * 10);
        }
        else if (pattern == 4)
        {
            const int16_t arrowX[MAX_ENEMIES] = {90, 118, 90, 118, 38, 64, 90, 116, 142, 90};
            const int16_t arrowY[MAX_ENEMIES] = {0, 0, 24, 24, 56, 56, 56, 56, 56, 86};
            x = arrowX[i];
            y = startY + arrowY[i];
        }

        state.enemies[i].x = x;
        state.enemies[i].y = y;
        state.enemies[i].alive = !(pattern == 1 && i >= 7);
        state.enemies[i].type = (row + col + level + pattern) % 3;
        
        if (state.enemies[i].type == 0)
            state.enemies[i].hp = 1;
        else if (state.enemies[i].type == 1)
            state.enemies[i].hp = (level < 4) ? 1 : 2;
        else
            state.enemies[i].hp = (level < 7) ? 2 : 3;
    }
}

bool Model::areAllEnemiesDefeated() const
{
    for (int i = 0; i < MAX_ENEMIES; i++)
    {
        if (state.enemies[i].alive)
        {
            return false;
        }
    }
    return true;
}

void Model::startNextLevel()
{
    if (state.level < 99)
    {
        state.level++;
    }

    state.enemyDirection = 1;
    state.bulletActive = false;
    state.enemyBulletActive = false;
    state.missileActive = false;
    state.itemActive = false;
    state.explosionTimer = 0;
    state.largeExplosionTimer = 0;
    state.levelIntroTimer = 60;
    enemyShootCooldown = 90;
    nextEnemyShooterIndex = 0;
    missileAmmo = 0;
    state.score += 250;
    if (state.score > 999999) state.score = 999999;

    initializeEnemiesForLevel(state.level);
}

void Model::updateHighScores()
{
    int32_t curScore = state.score;
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

void Model::fireBullet()
{
    // Chi cho phep ban 1 vien dan tren man hinh tai mot thoi diem
    if (!state.bulletActive && !state.missileActive && !state.isGameOver)
    {
        if (missileAmmo > 0)
        {
            missileAmmo--;
            state.missileActive = true;
            state.missileX = state.playerX + 9;
            state.missileY = 260;
        }
        else
        {
            state.bulletActive = true;
            state.bulletX = state.playerX + 13; // Canh giua tau (tau rong 30px, dan rong 4px -> 30/2 - 4/2 = 13px)
            state.bulletY = 264; // Dau tau (tau o Y=280, dan cao 16px -> 280 - 16 = 264px)
        }
    }
}

void Model::setPlayerMoveDirection(int8_t direction)
{
    if (direction < -1) direction = -1;
    if (direction > 1) direction = 1;

    playerMoveDirection = direction;
    playerMoveTimer = (direction == 0) ? 0 : 2;
}

static int tickCount = 0;
void Model::tick()
{
    const uint8_t levelBoost = (state.level > 12) ? 12 : state.level;
    const int16_t bulletSpeed = 6 + (levelBoost / 6);
    const int16_t itemFallSpeed = 2 + (levelBoost / 8);
    const int16_t enemyStep = 3 + (levelBoost / 5);
    const int16_t enemyDropStep = 6 + (levelBoost / 4);
    const uint8_t currentPattern = (state.level - 1) % 5;
    const int enemyMoveInterval = 18 - (levelBoost / 2) + ((currentPattern == 1) ? 5 : 0);
    const int16_t enemyBulletSpeed = 3 + (levelBoost / 6);

    if (state.levelIntroTimer > 0)
    {
        state.levelIntroTimer--;
    }

    if (!state.isGameOver && enemyShootCooldown > 0)
    {
        enemyShootCooldown--;
    }

    if (!state.isGameOver && playerMoveTimer > 0)
    {
        state.playerX += playerMoveDirection * 8;
        if (state.playerX < 0) state.playerX = 0;
        if (state.playerX > 210) state.playerX = 210;
        playerMoveTimer--;
        if (playerMoveTimer == 0)
        {
            playerMoveDirection = 0;
        }
    }

    // 0. Giam timer vu no neu dang dien ra
    if (state.explosionTimer > 0)
    {
        state.explosionTimer--;
    }

    if (state.largeExplosionTimer > 0)
    {
        state.largeExplosionTimer--;
    }

    if (state.enemyBulletActive && !state.isGameOver)
    {
        state.enemyBulletY += enemyBulletSpeed;

        if (state.enemyBulletY > 320)
        {
            state.enemyBulletActive = false;
            enemyShootCooldown = 70 - (levelBoost * 2);
            if (enemyShootCooldown < 35) enemyShootCooldown = 35;
        }
        else
        {
            const int px = state.playerX;
            const int py = 280;
            if (state.enemyBulletX + 4 >= px && state.enemyBulletX <= px + 30 &&
                state.enemyBulletY + 14 >= py && state.enemyBulletY <= py + 26)
            {
                state.enemyBulletActive = false;
                enemyShootCooldown = 80;
                if (state.lives > 0)
                {
                    state.lives--;
                }
            }
        }
    }
    else if (!state.isGameOver && enemyShootCooldown <= 0)
    {
        int shooterIndex = -1;
        for (int offset = 0; offset < MAX_ENEMIES; offset++)
        {
            int i = (nextEnemyShooterIndex + offset) % MAX_ENEMIES;
            if (state.enemies[i].alive && state.enemies[i].type == 2)
            {
                shooterIndex = i;
                nextEnemyShooterIndex = (i + 1) % MAX_ENEMIES;
                break;
            }
        }

        if (shooterIndex >= 0)
        {
            state.enemyBulletActive = true;
            state.enemyBulletX = state.enemies[shooterIndex].x + 11;
            state.enemyBulletY = state.enemies[shooterIndex].y + 20;
        }
        else
        {
            enemyShootCooldown = 50;
        }
    }

    // 0b. Logic vat pham roi tu tu xuong duoi man hinh
    if (state.itemActive && !state.isGameOver)
    {
        state.itemY += itemFallSpeed; // Vat pham roi nhanh dan theo level
        
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
                    if (state.score > 999999) state.score = 999999;
                }
                else if (state.itemType == 2) // Bolt
                {
                    // Cong them 1000 diem thuong
                    state.score += 1000;
                    if (state.score > 999999) state.score = 999999;
                }
                else if (state.itemType == 3) // Missile
                {
                    missileAmmo = 1;
                }
            }
        }
    }

    if (state.missileActive && !state.isGameOver)
    {
        state.missileY -= 4 + (levelBoost / 5);

        for (int i = 0; i < MAX_ENEMIES; i++)
        {
            if (state.enemies[i].alive)
            {
                int ex = state.enemies[i].x;
                int ey = state.enemies[i].y;

                if (state.missileX + 12 >= ex && state.missileX <= ex + 26 &&
                    state.missileY + 20 >= ey && state.missileY <= ey + 22)
                {
                    state.missileActive = false;
                    state.largeExplosionX = ex - 11;
                    state.largeExplosionY = ey - 13;
                    state.largeExplosionTimer = 12;

                    const int16_t blastCenterX = ex + 13;
                    const int16_t blastCenterY = ey + 11;
                    for (int j = 0; j < MAX_ENEMIES; j++)
                    {
                        if (state.enemies[j].alive)
                        {
                            int16_t enemyCenterX = state.enemies[j].x + 13;
                            int16_t enemyCenterY = state.enemies[j].y + 11;
                            int16_t dx = enemyCenterX - blastCenterX;
                            int16_t dy = enemyCenterY - blastCenterY;
                            if (dx * dx + dy * dy <= 55 * 55)
                            {
                                state.enemies[j].alive = false;
                                state.score += 60 + (state.enemies[j].type * 30) + (state.level * 10);
                                if (state.score > 999999) state.score = 999999;
                            }
                        }
                    }
                    break;
                }
            }
        }

        if (state.missileY < -20)
        {
            state.missileActive = false;
        }
    }

    // 1. Logic cap nhat dan bay va check va cham
    if (state.bulletActive && !state.isGameOver)
    {
        state.bulletY -= bulletSpeed; // Dan bay len nhanh hon o level cao
        
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
                    state.bulletActive = false; // Huy dan
                    state.enemies[i].hp--;
                    if (state.enemies[i].hp <= 0)
                    {
                        state.enemies[i].alive = false;
                    
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
                            state.itemType = (state.score / 100 + state.level + i) % 4; // Luan phien cac loai vat pham
                        }
                    }
                    
                        // Cong diem so
                        state.score += 80 + (state.enemies[i].type * 40) + (state.level * 10);
                        if (state.score > 999999) state.score = 999999;
                    }
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
    if (tickCount >= enemyMoveInterval && !state.isGameOver)
    {
        tickCount = 0;
        bool changeDir = false;

        // Kiem tra xem co con quai nao cham bien trai/phai chua
        for (int i = 0; i < MAX_ENEMIES; i++)
        {
            if (state.enemies[i].alive)
            {
                int nextX = state.enemies[i].x + state.enemyDirection * enemyStep;
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
                    state.enemies[i].y += enemyDropStep; // Di xuong 8px
                    // Kiem tra neu quai vat di xuong qua gan tau nguoi choi (Y = 280)
                    if (state.enemies[i].y >= 260)
                    {
                        state.isGameOver = true;
                        updateHighScores();
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
                    state.enemies[i].x += state.enemyDirection * enemyStep;
                }
            }
        }
    }

    if (!state.isGameOver && areAllEnemiesDefeated())
    {
        startNextLevel();
        tickCount = 0;
    }

    // Kiem tra them truong hop het mang song cung ket thuc game
    if (state.lives <= 0 && !state.isGameOver)
    {
        state.isGameOver = true;
        updateHighScores();
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
    state.level = 1;
    state.levelIntroTimer = 60;
    state.isGameOver = false;
    state.playBuzzerBeep = false;
    
    state.bulletX = 0;
    state.bulletY = 0;
    state.bulletActive = false;
    state.enemyBulletX = 0;
    state.enemyBulletY = 0;
    state.enemyBulletActive = false;
    
    state.explosionX = 0;
    state.explosionY = 0;
    state.explosionTimer = 0;
    
    state.itemX = 0;
    state.itemY = 0;
    state.itemActive = false;
    state.itemType = 0;

    state.missileX = 0;
    state.missileY = 0;
    state.missileActive = false;

    state.largeExplosionX = 0;
    state.largeExplosionY = 0;
    state.largeExplosionTimer = 0;
    
    state.enemyDirection = 1;
    playerMoveDirection = 0;
    playerMoveTimer = 0;
    enemyShootCooldown = 90;
    nextEnemyShooterIndex = 0;
    missileAmmo = 0;
    tickCount = 0;
    initializeEnemiesForLevel(state.level);
}
