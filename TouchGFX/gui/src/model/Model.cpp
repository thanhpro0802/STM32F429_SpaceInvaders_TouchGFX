#include <gui/model/Model.hpp>
#include <gui/model/ModelListener.hpp>
#include <cmath>

Model::Model() : modelListener(0), playerMoveDirection(0), playerMoveTimer(0), playerMoveDirectionY(0), playerMoveTimerY(0), enemyShootCooldown(90), nextEnemyShooterIndex(0), missileAmmo(0)
{
    // Khoi tao gia tri mac dinh cho game
    state.playerX = 105;
    state.playerY = 280;
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
    state.boltBuffTimer = 0;
    state.bulletLeftActive = false;
    state.bulletRightActive = false;
    state.doubleShotBuffTimer = 0;
    state.bullet2Active = false;
    state.rapidFireTimer = 0;
    state.rapidFireTimer = 0;
    autoFireCooldown = 0;
    for (int i = 0; i < 20; i++)
    {
        state.enemyBullets[i].active = false;
        state.enemyBullets[i].angle = 0.0f;
    }
    state.bossLaserPhase = 0;
    state.bossLaserTimer = 0;
    state.bossLockedX = 0;

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
    
    // Khoi tao Boss
    state.bossActive = false;
    state.bossType = 1;
    state.bossX = 0;
    state.bossY = 0;
    state.bossHp = 0;
    state.bossMaxHp = 0;
    state.bossDirection = 1;
    state.bossTimer = 0;
    
    state.enemyDirection = 1; // Mac dinh di qua phai
    initializeEnemiesForLevel(state.level);
}

void Model::initializeEnemiesForLevel(uint8_t level)
{
    if (level == 0)
    {
        level = 1;
    }

    if (level == 5 || level == 10)
    {
        state.bossActive = true;
        state.bossType = (level == 5) ? 1 : 2;
        state.bossX = 88; // 240/2 - 64/2
        state.bossY = 30;
        state.bossHp = (level == 5) ? 15 : 20;
        state.bossMaxHp = state.bossHp;
        state.bossDirection = 1;
        state.bossTimer = 0;

        for (int i = 0; i < MAX_ENEMIES; i++)
        {
            state.enemies[i].alive = false;
        }
        return;
    }

    state.bossActive = false;

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
    if (state.bossActive) return false;
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
    state.boltBuffTimer = 0;
    state.bulletLeftActive = false;
    state.bulletRightActive = false;
    state.doubleShotBuffTimer = 0;
    state.bullet2Active = false;
    state.rapidFireTimer = 0;
    state.rapidFireTimer = 0;
    autoFireCooldown = 0;
    for (int i = 0; i < 20; i++)
    {
        state.enemyBullets[i].active = false;
        state.enemyBullets[i].angle = 0.0f;
    }
    state.bossLaserPhase = 0;
    state.bossLaserTimer = 0;
    state.bossLockedX = 0;
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
            state.missileY = state.playerY - 20;
        }
        else
        {
            state.bulletActive = true;
            state.bulletX = state.playerX + 13;
            state.bulletY = state.playerY - 16;

            if (state.doubleShotBuffTimer > 0 && !state.bullet2Active)
            {
                state.bullet2Active = true;
                state.bullet2X = state.bulletX + 8;
                state.bullet2Y = state.bulletY;
            }

            if (state.boltBuffTimer > 0)
            {
                state.bulletLeftActive = true;
                state.bulletLeftX = state.bulletX - 5;
                state.bulletLeftY = state.bulletY;

                state.bulletRightActive = true;
                state.bulletRightX = state.bulletX + 5;
                state.bulletRightY = state.bulletY;
            }
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

void Model::setPlayerMoveDirectionY(int8_t direction)
{
    if (direction < -1) direction = -1;
    if (direction > 1) direction = 1;

    playerMoveDirectionY = direction;
    playerMoveTimerY = (direction == 0) ? 0 : 2;
}

static int tickCount = 0;
void Model::tick()
{
    const uint8_t levelBoost = (state.level > 12) ? 12 : state.level;
    int16_t bulletSpeed = 6 + (levelBoost / 6);
    if (state.rapidFireTimer > 0)
    {
        bulletSpeed = 14;
    }
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

    if (state.boltBuffTimer > 0)
    {
        state.boltBuffTimer--;
    }

    if (state.doubleShotBuffTimer > 0)
    {
        state.doubleShotBuffTimer--;
    }

    if (state.rapidFireTimer > 0)
    {
        state.rapidFireTimer--;
    }

    if (state.rapidFireTimer > 0 && !state.isGameOver)
    {
        if (autoFireCooldown > 0)
        {
            autoFireCooldown--;
        }
        if (autoFireCooldown == 0 && !state.bulletActive && !state.missileActive)
        {
            fireBullet();
            autoFireCooldown = 6;
        }
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

    if (!state.isGameOver && playerMoveTimerY > 0)
    {
        state.playerY += playerMoveDirectionY * 8;
        if (state.playerY < 150) state.playerY = 150;
        if (state.playerY > 294) state.playerY = 294;
        playerMoveTimerY--;
        if (playerMoveTimerY == 0)
        {
            playerMoveDirectionY = 0;
        }
    }

    // Boss Logic
    if (state.bossActive && !state.isGameOver)
    {
        state.bossTimer++;
        
        // Laser skill trigger
        if (state.bossLaserPhase == 0 && state.bossTimer % 300 == 0)
        {
            state.bossLaserPhase = 1;
            state.bossLaserTimer = 120; // 2 seconds
        }

        if (state.bossLaserPhase == 0 && state.bossTimer % 3 == 0)
        {
            state.bossX += state.bossDirection * 2;
            if (state.bossX <= 10)
            {
                state.bossX = 10;
                state.bossDirection = 1;
            }
            else if (state.bossX >= 240 - 64 - 10)
            {
                state.bossX = 240 - 64 - 10;
                state.bossDirection = -1;
            }
        }
        
        // Boss Shoot
        if (state.bossLaserPhase == 0 && state.bossTimer % 60 == 0)
        {
            if (state.bossType == 1)
            {
                // Boss 1 shoots 1 bullet
                for (int j = 0; j < 5; j++)
                {
                    if (!state.enemyBullets[j].active)
                    {
                        state.enemyBullets[j].active = true;
                        state.enemyBullets[j].x = state.bossX + 32;
                        state.enemyBullets[j].y = state.bossY + 60;
                        state.enemyBullets[j].type = 0;
                        break;
                    }
                }
            }
            else if (state.bossType == 2)
            {
                // Boss 2 shoots 2 bullets at a time, alternating between center and arms
                static bool alternate = false;
                alternate = !alternate;
                int spawned = 0;
                for (int j = 0; j < 5 && spawned < 2; j++)
                {
                    if (!state.enemyBullets[j].active)
                    {
                        state.enemyBullets[j].active = true;
                        if (alternate)
                        {
                            if (spawned == 0) { state.enemyBullets[j].x = state.bossX + 20; state.enemyBullets[j].y = state.bossY + 60; state.enemyBullets[j].type = 1; }
                            else if (spawned == 1) { state.enemyBullets[j].x = state.bossX + 44; state.enemyBullets[j].y = state.bossY + 60; state.enemyBullets[j].type = 1; }
                        }
                        else
                        {
                            if (spawned == 0) { state.enemyBullets[j].x = state.bossX + 4; state.enemyBullets[j].y = state.bossY + 40; state.enemyBullets[j].type = 2; }
                            else if (spawned == 1) { state.enemyBullets[j].x = state.bossX + 60; state.enemyBullets[j].y = state.bossY + 40; state.enemyBullets[j].type = 2; }
                        }
                        spawned++;
                    }
                }
            }
        }

        // Handle Aiming Phase (Phase 1)
        if (state.bossLaserPhase == 1)
        {
            if (state.bossLaserTimer > 0)
            {
                state.bossLaserTimer--;
                for (int i = 0; i < 10; i++)
                {
                    int j = 5 + i;
                    state.enemyBullets[j].active = true;
                    
                    float startX = state.bossX + 4.0f + i * 6.0f;
                    float startY = state.bossY + 60.0f;
                    state.enemyBullets[j].x = startX;
                    state.enemyBullets[j].y = startY;
                    state.enemyBullets[j].type = 0;
                    
                    float targetX = state.playerX + 15.0f;
                    float targetY = state.playerY + 13.0f;
                    float dx = targetX - startX;
                    float dy = targetY - startY;
                    state.enemyBullets[j].angle = std::atan2(dx, dy);
                }
            }
            else
            {
                state.bossLaserPhase = 2;
                state.bossLockedX = state.playerX;
            }
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

    bool anyEnemyBulletActive = false;
    for (int i = 0; i < 20; i++)
    {
        if (state.enemyBullets[i].active)
        {
            anyEnemyBulletActive = true;

            // In Phase 1, lasers are tracking and should not move down or hurt the player yet
            if (state.bossLaserPhase == 1 && i >= 5 && i < 15)
            {
                // Managed by aiming tracking logic
            }
            else
            {
                // Phase 2 lasers move slightly faster
                int16_t speed = enemyBulletSpeed;
                if (state.bossLaserPhase == 2 && i >= 5 && i < 15)
                {
                    speed = enemyBulletSpeed + 2;
                }

                if (state.bossLaserPhase == 2 && i >= 5 && i < 15)
                {
                    state.enemyBullets[i].x += speed * std::sin(state.enemyBullets[i].angle);
                    state.enemyBullets[i].y += speed * std::cos(state.enemyBullets[i].angle);
                }
                else
                {
                    state.enemyBullets[i].y += speed;
                }

                if (state.enemyBullets[i].y > 320)
                {
                    state.enemyBullets[i].active = false;
                    enemyShootCooldown = 70 - (levelBoost * 2);
                    if (enemyShootCooldown < 35) enemyShootCooldown = 35;
                }
                else if (!state.isGameOver)
                {
                    const int px = state.playerX;
                    const int py = state.playerY;
                    if (state.enemyBullets[i].x + 4 >= px && state.enemyBullets[i].x <= px + 30 &&
                        state.enemyBullets[i].y + 14 >= py && state.enemyBullets[i].y <= py + 26)
                    {
                        state.enemyBullets[i].active = false;
                        enemyShootCooldown = 80;
                        if (state.lives > 0)
                        {
                            state.lives--;
                        }
                    }
                }
            }
        }
    }

    // Reset Laser Phase to 0 when all active lasers in Firing phase are gone
    if (state.bossLaserPhase == 2)
    {
        bool anyLaserActive = false;
        for (int i = 5; i < 15; i++)
        {
            if (state.enemyBullets[i].active)
            {
                anyLaserActive = true;
                break;
            }
        }
        if (!anyLaserActive)
        {
            state.bossLaserPhase = 0;
        }
    }

    if (!state.isGameOver && enemyShootCooldown <= 0 && !anyEnemyBulletActive && !state.bossActive)
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
            for (int j = 0; j < 5; j++)
            {
                if (!state.enemyBullets[j].active)
                {
                    state.enemyBullets[j].active = true;
                    state.enemyBullets[j].x = state.enemies[shooterIndex].x + 11;
                    state.enemyBullets[j].y = state.enemies[shooterIndex].y + 20;
                    state.enemyBullets[j].type = 0;
                    break;
                }
            }
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
            // Kiem tra va cham voi tau nguoi choi
            int px = state.playerX;
            int py = state.playerY;
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
                else if (state.itemType == 2) // Bolt (Rapid Fire)
                {
                    state.rapidFireTimer = 180; // 3 seconds
                }
                else if (state.itemType == 5) // TripleShot
                {
                    state.boltBuffTimer = 300; // 5 seconds
                }
                else if (state.itemType == 3) // Missile
                {
                    missileAmmo = 1;
                }
                else if (state.itemType == 4) // DoubleShot
                {
                    state.doubleShotBuffTimer = 300;
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
        
        if (state.bossActive)
        {
            if (state.missileX + 12 >= state.bossX && state.missileX <= state.bossX + 64 &&
                state.missileY + 20 >= state.bossY && state.missileY <= state.bossY + 64)
            {
                state.missileActive = false;
                state.bossHp -= 15;
                state.largeExplosionX = state.bossX;
                state.largeExplosionY = state.bossY;
                state.largeExplosionTimer = 12;
                if (state.bossHp <= 0)
                {
                    state.bossActive = false;
                    state.score += 2000;
                    if (state.score > 999999) state.score = 999999;
                }
            }
        }

        if (state.missileY < -20)
        {
            state.missileActive = false;
        }
    }

    // Logic dan DoubleShot (dan thu 2 bay thang)
    if (state.bullet2Active && !state.isGameOver)
    {
        state.bullet2Y -= bulletSpeed;

        for (int i = 0; i < MAX_ENEMIES; i++)
        {
            if (state.enemies[i].alive)
            {
                int ex = state.enemies[i].x;
                int ey = state.enemies[i].y;
                if (state.bullet2X + 4 >= ex && state.bullet2X <= ex + 26 &&
                    state.bullet2Y + 16 >= ey && state.bullet2Y <= ey + 22)
                {
                    state.bullet2Active = false;
                    state.enemies[i].hp--;
                    if (state.enemies[i].hp <= 0)
                    {
                        state.enemies[i].alive = false;
                        state.explosionX = ex + 1;
                        state.explosionY = ey - 1;
                        state.explosionTimer = 8;
                        state.score += 80 + (state.enemies[i].type * 40) + (state.level * 10);
                        if (state.score > 999999) state.score = 999999;
                    }
                    break;
                }
            }
        }

        if (state.bossActive)
        {
            if (state.bullet2X + 4 >= state.bossX && state.bullet2X <= state.bossX + 64 &&
                state.bullet2Y + 16 >= state.bossY && state.bullet2Y <= state.bossY + 64)
            {
                state.bullet2Active = false;
                state.bossHp -= 1;
                state.explosionX = state.bullet2X - 10;
                state.explosionY = state.bullet2Y - 10;
                state.explosionTimer = 8;
                if (state.bossHp <= 0)
                {
                    state.bossActive = false;
                    state.score += 2000;
                    if (state.score > 999999) state.score = 999999;
                }
            }
        }

        if (state.bullet2Y < -16)
        {
            state.bullet2Active = false;
        }
    }

    // Logic cho dan cheo trai/phai
    if (state.bulletLeftActive && !state.isGameOver)
    {
        state.bulletLeftY -= bulletSpeed;
        state.bulletLeftX -= 2;
        
        for (int i = 0; i < MAX_ENEMIES; i++)
        {
            if (state.enemies[i].alive)
            {
                int ex = state.enemies[i].x;
                int ey = state.enemies[i].y;
                if (state.bulletLeftX + 4 >= ex && state.bulletLeftX <= ex + 26 &&
                    state.bulletLeftY + 16 >= ey && state.bulletLeftY <= ey + 22)
                {
                    state.bulletLeftActive = false;
                    state.enemies[i].hp--;
                    if (state.enemies[i].hp <= 0)
                    {
                        state.enemies[i].alive = false;
                        state.explosionX = ex + 1;
                        state.explosionY = ey - 1;
                        state.explosionTimer = 8;
                        state.score += 80 + (state.enemies[i].type * 40) + (state.level * 10);
                        if (state.score > 999999) state.score = 999999;
                    }
                    break;
                }
            }
        }
        
        if (state.bossActive)
        {
            if (state.bulletLeftX + 4 >= state.bossX && state.bulletLeftX <= state.bossX + 64 &&
                state.bulletLeftY + 16 >= state.bossY && state.bulletLeftY <= state.bossY + 64)
            {
                state.bulletLeftActive = false;
                state.bossHp -= 1;
                state.explosionX = state.bulletLeftX - 10;
                state.explosionY = state.bulletLeftY - 10;
                state.explosionTimer = 8;
                if (state.bossHp <= 0)
                {
                    state.bossActive = false;
                    state.score += 2000;
                    if (state.score > 999999) state.score = 999999;
                }
            }
        }

        if (state.bulletLeftY < -16 || state.bulletLeftX < -16)
        {
            state.bulletLeftActive = false;
        }
    }

    if (state.bulletRightActive && !state.isGameOver)
    {
        state.bulletRightY -= bulletSpeed;
        state.bulletRightX += 2;
        
        for (int i = 0; i < MAX_ENEMIES; i++)
        {
            if (state.enemies[i].alive)
            {
                int ex = state.enemies[i].x;
                int ey = state.enemies[i].y;
                if (state.bulletRightX + 4 >= ex && state.bulletRightX <= ex + 26 &&
                    state.bulletRightY + 16 >= ey && state.bulletRightY <= ey + 22)
                {
                    state.bulletRightActive = false;
                    state.enemies[i].hp--;
                    if (state.enemies[i].hp <= 0)
                    {
                        state.enemies[i].alive = false;
                        state.explosionX = ex + 1;
                        state.explosionY = ey - 1;
                        state.explosionTimer = 8;
                        state.score += 80 + (state.enemies[i].type * 40) + (state.level * 10);
                        if (state.score > 999999) state.score = 999999;
                    }
                    break;
                }
            }
        }
        
        if (state.bossActive)
        {
            if (state.bulletRightX + 4 >= state.bossX && state.bulletRightX <= state.bossX + 64 &&
                state.bulletRightY + 16 >= state.bossY && state.bulletRightY <= state.bossY + 64)
            {
                state.bulletRightActive = false;
                state.bossHp -= 1;
                state.explosionX = state.bulletRightX - 10;
                state.explosionY = state.bulletRightY - 10;
                state.explosionTimer = 8;
                if (state.bossHp <= 0)
                {
                    state.bossActive = false;
                    state.score += 2000;
                    if (state.score > 999999) state.score = 999999;
                }
            }
        }

        if (state.bulletRightY < -16 || state.bulletRightX > 240)
        {
            state.bulletRightActive = false;
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
                    state.bulletActive = false;
    state.boltBuffTimer = 0;
    state.bulletLeftActive = false;
    state.bulletRightActive = false; // Huy dan
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
                            state.itemType = (state.score / 100 + state.level + i) % 6; // Luan phien cac loai vat pham
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
        
        if (state.bossActive)
        {
            if (state.bulletX + 4 >= state.bossX && state.bulletX <= state.bossX + 64 &&
                state.bulletY + 16 >= state.bossY && state.bulletY <= state.bossY + 64)
            {
                state.bulletActive = false;
    state.boltBuffTimer = 0;
    state.bulletLeftActive = false;
    state.bulletRightActive = false;
                state.bossHp -= 1;
                state.explosionX = state.bulletX - 10;
                state.explosionY = state.bulletY - 10;
                state.explosionTimer = 8;
                if (state.bossHp <= 0)
                {
                    state.bossActive = false;
                    state.score += 2000;
                    if (state.score > 999999) state.score = 999999;
                }
            }
        }

        if (state.bulletY < 0)
        {
            state.bulletActive = false;
    state.boltBuffTimer = 0;
    state.bulletLeftActive = false;
    state.bulletRightActive = false; // Xoa dan khi bay khoi man hinh
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
                    // Kiem tra neu quai vat di xuong qua gan tau nguoi choi
                    if (state.enemies[i].y >= state.playerY - 20)
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
    state.playerY = 280;
    state.score = 0;
    state.lives = 3;
    state.level = 1;
    state.levelIntroTimer = 60;
    state.isGameOver = false;
    state.playBuzzerBeep = false;
    
    state.bulletX = 0;
    state.bulletY = 0;
    state.bulletActive = false;
    state.boltBuffTimer = 0;
    state.bulletLeftActive = false;
    state.bulletRightActive = false;
    state.doubleShotBuffTimer = 0;
    state.bullet2Active = false;
    state.rapidFireTimer = 0;
    state.rapidFireTimer = 0;
    autoFireCooldown = 0;
    for (int i = 0; i < 20; i++)
    {
        state.enemyBullets[i].active = false;
        state.enemyBullets[i].angle = 0.0f;
    }
    state.bossLaserPhase = 0;
    state.bossLaserTimer = 0;
    state.bossLockedX = 0;
    
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
    
    state.bossActive = false;
    state.bossType = 1;
    state.bossX = 0;
    state.bossY = 0;
    state.bossHp = 0;
    state.bossMaxHp = 0;
    state.bossDirection = 1;
    state.bossTimer = 0;
    
    state.enemyDirection = 1;
    playerMoveDirection = 0;
    playerMoveTimer = 0;
    playerMoveDirectionY = 0;
    playerMoveTimerY = 0;
    enemyShootCooldown = 90;
    nextEnemyShooterIndex = 0;
    missileAmmo = 0;
    tickCount = 0;
    initializeEnemiesForLevel(state.level);
}
