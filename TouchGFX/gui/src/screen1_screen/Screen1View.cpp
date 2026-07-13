#include <gui/screen1_screen/Screen1View.hpp> // trigger re-parse 2
#include <touchgfx/Utils.hpp>
#include <images/BitmapDatabase.hpp>
#include <touchgfx/Color.hpp>

// Dummy defines fallback just in case TouchGFX Generator hasn't been run yet
#ifndef BITMAP_UFO_BOSS_2_ID
#define BITMAP_UFO_BOSS_2_ID BITMAP_UFO_BOSS_ID
#endif
#ifndef BITMAP_BOSS_2_LAZER_ID
#define BITMAP_BOSS_2_LAZER_ID BITMAP_LASER_ENEMY_ID
#endif
#ifndef BITMAP_BOSS_3_LAZER_ID
#define BITMAP_BOSS_3_LAZER_ID BITMAP_LASER_ENEMY_ID
#endif

Screen1View::Screen1View() : lastScore(-1), lastLives(-1), lastLevel(0)
{
    levelTensDigit.setXY(109, 144);
    levelTensDigit.setBitmap(touchgfx::Bitmap(BITMAP_NUM_0_ID));
    levelTensDigit.setVisible(false);
    add(levelTensDigit);

    levelOnesDigit.setXY(121, 144);
    levelOnesDigit.setBitmap(touchgfx::Bitmap(BITMAP_NUM_1_ID));
    levelOnesDigit.setVisible(false);
    add(levelOnesDigit);

    for (int i = 0; i < 20; i++)
    {
        enemyBullets[i].setXY(0, 0);
        touchgfx::Bitmap bmp(BITMAP_LASER_ENEMY_ID);
        enemyBullets[i].setBitmap(bmp);
        enemyBullets[i].setWidth(bmp.getWidth());
        enemyBullets[i].setHeight(bmp.getHeight());
        enemyBullets[i].setBitmapPosition(0.0f, 0.0f);
        enemyBullets[i].setOrigo(bmp.getWidth() / 2.0f, bmp.getHeight() / 2.0f);
        enemyBullets[i].setCameraDistance(1000.0f);
        enemyBullets[i].updateAngles(0.0f, 0.0f, 0.0f);
        enemyBullets[i].setVisible(false);
        add(enemyBullets[i]);
    }

    playerBullet2.setBitmap(touchgfx::Bitmap(BITMAP_LASER_PLAYER_ID));
    playerBullet2.setVisible(false);
    add(playerBullet2);

    bossImage.setXY(0, 0);
    bossImage.setBitmap(touchgfx::Bitmap(BITMAP_UFO_BOSS_ID));
    bossImage.setVisible(false);
    add(bossImage);

    bossHpBarBg.setPosition(40, 5, 160, 6);
    bossHpBarBg.setColor(touchgfx::Color::getColorFromRGB(100, 100, 100));
    bossHpBarBg.setVisible(false);
    add(bossHpBarBg);

    bossHpBar.setPosition(40, 5, 160, 6);
    bossHpBar.setColor(touchgfx::Color::getColorFromRGB(255, 0, 0));
    bossHpBar.setVisible(false);
    add(bossHpBar);
}

// Tra ve BitmapId tuong ung voi chu so 0-9
touchgfx::BitmapId Screen1View::getNumBitmap(uint8_t digit)
{
    const touchgfx::BitmapId numBitmaps[10] = {
        BITMAP_NUM_0_ID, BITMAP_NUM_1_ID, BITMAP_NUM_2_ID,
        BITMAP_NUM_3_ID, BITMAP_NUM_4_ID, BITMAP_NUM_5_ID,
        BITMAP_NUM_6_ID, BITMAP_NUM_7_ID, BITMAP_NUM_8_ID,
        BITMAP_NUM_9_ID
    };
    if (digit > 9) digit = 0;
    return numBitmaps[digit];
}

touchgfx::BitmapId Screen1View::getEnemyBitmap(uint8_t type)
{
    if (type == 0)
        return BITMAP_ENEMY_RED_ID;
    if (type == 1)
        return BITMAP_ENEMY_BLUE_ID;
    return BITMAP_ENEMY_SPIKEY_ID;
}

// Cap nhat 6 widget anh so dua tren gia tri Score (0-999999)
void Screen1View::setScoreDisplay(int32_t score)
{
    if (score < 0) score = 0;
    if (score > 999999) score = 999999;

    uint8_t d[6];
    d[0] = score % 10;
    d[1] = (score / 10) % 10;
    d[2] = (score / 100) % 10;
    d[3] = (score / 1000) % 10;
    d[4] = (score / 10000) % 10;
    d[5] = (score / 100000) % 10;

    scoreD0.setBitmap(touchgfx::Bitmap(getNumBitmap(d[0]))); scoreD0.invalidate();
    scoreD1.setBitmap(touchgfx::Bitmap(getNumBitmap(d[1]))); scoreD1.invalidate();
    scoreD2.setBitmap(touchgfx::Bitmap(getNumBitmap(d[2]))); scoreD2.invalidate();
    scoreD3.setBitmap(touchgfx::Bitmap(getNumBitmap(d[3]))); scoreD3.invalidate();
    scoreD4.setBitmap(touchgfx::Bitmap(getNumBitmap(d[4]))); scoreD4.invalidate();
    scoreD5.setBitmap(touchgfx::Bitmap(getNumBitmap(d[5]))); scoreD5.invalidate();
}

// Cap nhat widget anh so mang song (0-9)
void Screen1View::setLivesDisplay(int8_t lives)
{
    if (lives < 0) lives = 0;
    if (lives > 9) lives = 9;
    livesDigit.setBitmap(touchgfx::Bitmap(getNumBitmap(lives)));
    livesDigit.invalidate();
}

void Screen1View::setupScreen()
{
    Screen1ViewBase::setupScreen();

    // Reset game truoc khi bat dau de khoi tao lai diem so, mang song va vi tri quai vat
    presenter->resetGame();

    // Dong bo trang thai ban dau tu Model
    const GameState& state = presenter->getGameState();
    playerShip.setXY(state.playerX, state.playerY);
    playerShip.invalidate();

    // Hien thi Score va Lives ban dau
    setScoreDisplay(state.score);
    setLivesDisplay(state.lives);
    lastScore = state.score;
    lastLives = state.lives;

    // Khoi tao va Add quai vat dong vao man hinh
    enemyImages[0] = &enemy0;
    enemyImages[1] = &enemy1;
    enemyImages[2] = &enemy2;
    enemyImages[3] = &enemy3;
    enemyImages[4] = &enemy4;
    enemyImages[5] = &enemy5;
    enemyImages[6] = &enemy6;
    enemyImages[7] = &enemy7;
    enemyImages[8] = &enemy8;
    enemyImages[9] = &enemy9;

    for (int i = 0; i < MAX_ENEMIES; i++)
    {
        enemyImages[i]->setXY(state.enemies[i].x, state.enemies[i].y);
        enemyImages[i]->setBitmap(touchgfx::Bitmap(getEnemyBitmap(state.enemies[i].type)));
        enemyImages[i]->setVisible(state.enemies[i].alive);
        enemyImages[i]->invalidate();
    }

    playerBulletLeft.setBitmap(touchgfx::Bitmap(BITMAP_LASER_PLAYER_ID));
    playerBulletLeft.setVisible(false);
    add(playerBulletLeft);
    
    playerBulletRight.setBitmap(touchgfx::Bitmap(BITMAP_LASER_PLAYER_ID));
    playerBulletRight.setVisible(false);
    add(playerBulletRight);

    updateLevelIntro(state);
}

void Screen1View::tearDownScreen()
{
    Screen1ViewBase::tearDownScreen();
}

void Screen1View::handleKeyEvent(uint8_t key)
{
    if (key == 18 || key == 'a' || key == 'A')
    {
        presenter->setPlayerMoveDirection(-1);
    }
    else if (key == 19 || key == 'd' || key == 'D')
    {
        presenter->setPlayerMoveDirection(1);
    }
    else if (key == 16 || key == 'w' || key == 'W')
    {
        presenter->setPlayerMoveDirectionY(-1);
    }
    else if (key == 17 || key == 's' || key == 'S')
    {
        presenter->setPlayerMoveDirectionY(1);
    }
    else if (key == 32 || key == ' ') // Phím Space để bắn đạn
    {
        presenter->fireBullet();
    }

}

void Screen1View::updateGameState(const GameState& state)
{
    updateLevelIntro(state);

    // Cap nhat vi tri tau nguoi choi
    if (playerShip.getX() != state.playerX || playerShip.getY() != state.playerY)
    {
        playerShip.invalidate();
        playerShip.setXY(state.playerX, state.playerY);
        playerShip.invalidate();
    }

    // Cap nhat dan cua nguoi choi
    if (playerBullet.isVisible() != state.bulletActive)
    {
        playerBullet.setVisible(state.bulletActive);
        playerBullet.invalidate();
    }
    if (state.bulletActive)
    {
        if (playerBullet.getX() != state.bulletX || playerBullet.getY() != state.bulletY)
        {
            playerBullet.invalidate();
            playerBullet.setXY(state.bulletX, state.bulletY);
            playerBullet.invalidate();
        }
    }

    // Cap nhat dan chéo trái
    if (playerBulletLeft.isVisible() != state.bulletLeftActive)
    {
        playerBulletLeft.setVisible(state.bulletLeftActive);
        playerBulletLeft.invalidate();
    }
    if (state.bulletLeftActive)
    {
        if (playerBulletLeft.getX() != state.bulletLeftX || playerBulletLeft.getY() != state.bulletLeftY)
        {
            playerBulletLeft.invalidate();
            playerBulletLeft.setXY(state.bulletLeftX, state.bulletLeftY);
            playerBulletLeft.invalidate();
        }
    }

    // Cap nhat dan chéo phải
    if (playerBulletRight.isVisible() != state.bulletRightActive)
    {
        playerBulletRight.setVisible(state.bulletRightActive);
        playerBulletRight.invalidate();
    }
    if (state.bulletRightActive)
    {
        if (playerBulletRight.getX() != state.bulletRightX || playerBulletRight.getY() != state.bulletRightY)
        {
            playerBulletRight.invalidate();
            playerBulletRight.setXY(state.bulletRightX, state.bulletRightY);
            playerBulletRight.invalidate();
        }
    }

    // Cap nhat dan thu 2 (DoubleShot)
    if (playerBullet2.isVisible() != state.bullet2Active)
    {
        playerBullet2.setVisible(state.bullet2Active);
        playerBullet2.invalidate();
    }
    if (state.bullet2Active)
    {
        if (playerBullet2.getX() != state.bullet2X || playerBullet2.getY() != state.bullet2Y)
        {
            playerBullet2.invalidate();
            playerBullet2.setXY(state.bullet2X, state.bullet2Y);
            playerBullet2.invalidate();
        }
    }

    for (int i = 0; i < 20; i++)
    {
        bool showBullet = state.enemyBullets[i].active;

        if (enemyBullets[i].isVisible() != showBullet)
        {
            enemyBullets[i].setVisible(showBullet);
            enemyBullets[i].invalidate();
        }
        if (state.enemyBullets[i].active)
        {
            touchgfx::BitmapId bmpId = BITMAP_LASER_ENEMY_ID;
            if (state.enemyBullets[i].type == 1) bmpId = BITMAP_BOSS_2_LAZER_ID;
            else if (state.enemyBullets[i].type == 2) bmpId = BITMAP_BOSS_3_LAZER_ID;
            
            touchgfx::Bitmap bmp(bmpId);
            enemyBullets[i].setBitmap(bmp);
            enemyBullets[i].setWidth(bmp.getWidth());
            enemyBullets[i].setHeight(bmp.getHeight());
            enemyBullets[i].setBitmapPosition(0.0f, 0.0f);
            enemyBullets[i].setOrigo(bmp.getWidth() / 2.0f, bmp.getHeight() / 2.0f);
            
            // Set rotation angle (negated to match TouchGFX coordinate system rotation direction)
            enemyBullets[i].updateAngles(0.0f, 0.0f, -state.enemyBullets[i].angle);
            
            if (enemyBullets[i].getX() != (int16_t)state.enemyBullets[i].x || enemyBullets[i].getY() != (int16_t)state.enemyBullets[i].y)
            {
                enemyBullets[i].invalidate();
                enemyBullets[i].setXY((int16_t)state.enemyBullets[i].x, (int16_t)state.enemyBullets[i].y);
                enemyBullets[i].invalidate();
            }
        }
    }

    // Cap nhat vat pham roi (Items)
    if (itemDrop.isVisible() != state.itemActive)
    {
        itemDrop.setVisible(state.itemActive);
        itemDrop.invalidate();
    }
    if (state.itemActive)
    {
        // Thay doi anh bitmap tuong ung voi loai vat pham
        touchgfx::BitmapId itemBmp;
        if (state.itemType == 0)
            itemBmp = BITMAP_ITEM_SHIELD_ID;
        else if (state.itemType == 1)
            itemBmp = BITMAP_ITEM_STAR_ID;
        else if (state.itemType == 2)
            itemBmp = BITMAP_ITEM_BOLT_ID;
        else if (state.itemType == 3)
            itemBmp = BITMAP_ITEM_MISSILE_ID;
        else if (state.itemType == 4)
            itemBmp = BITMAP_DOUBLESHOT_ID;
        else // itemType == 5: TripleShot
            itemBmp = BITMAP_TRIPLESHOT_ID;
            
        itemDrop.setBitmap(touchgfx::Bitmap(itemBmp));

        if (itemDrop.getX() != state.itemX || itemDrop.getY() != state.itemY)
        {
            itemDrop.invalidate();
            itemDrop.setXY(state.itemX, state.itemY);
            itemDrop.invalidate();
        }
    }

    // Cap nhat dan ten lua cua nguoi choi
    if (playerMissile.isVisible() != state.missileActive)
    {
        playerMissile.setVisible(state.missileActive);
        playerMissile.invalidate();
    }
    if (state.missileActive)
    {
        if (playerMissile.getX() != state.missileX || playerMissile.getY() != state.missileY)
        {
            playerMissile.invalidate();
            playerMissile.setXY(state.missileX, state.missileY);
            playerMissile.invalidate();
        }
    }

    // Cap nhat hieu ung vu no
    bool isExploding = (state.explosionTimer > 0);
    if (explosionEffect.isVisible() != isExploding)
    {
        explosionEffect.setVisible(isExploding);
        explosionEffect.invalidate();
    }
    if (isExploding)
    {
        explosionEffect.setXY(state.explosionX, state.explosionY);
        explosionEffect.invalidate();
    }

    // Cap nhat hieu ung vu no lon (Ten lua)
    bool isLargeExploding = (state.largeExplosionTimer > 0);
    if (explosionLarge.isVisible() != isLargeExploding)
    {
        explosionLarge.setVisible(isLargeExploding);
        explosionLarge.invalidate();
    }
    if (isLargeExploding)
    {
        explosionLarge.setXY(state.largeExplosionX, state.largeExplosionY);
        explosionLarge.invalidate();
    }

    // Cap nhat tung con quai vat
    for (int i = 0; i < MAX_ENEMIES; i++)
    {
        // Neu quai vat truoc do song ma bay gio chet (hoac nguoc lai)
        if (enemyImages[i]->isVisible() != state.enemies[i].alive)
        {
            enemyImages[i]->setVisible(state.enemies[i].alive);
            enemyImages[i]->invalidate();
        }

        // Cap nhat toa do neu quai vat con song va co su thay doi vi tri
        if (state.enemies[i].alive)
        {
            enemyImages[i]->setBitmap(touchgfx::Bitmap(getEnemyBitmap(state.enemies[i].type)));
            if (enemyImages[i]->getX() != state.enemies[i].x || enemyImages[i]->getY() != state.enemies[i].y)
            {
                enemyImages[i]->invalidate();
                enemyImages[i]->setXY(state.enemies[i].x, state.enemies[i].y);
                enemyImages[i]->invalidate();
            }
        }
    }

    // Cap nhat Boss
    if (bossImage.isVisible() != state.bossActive)
    {
        bossImage.setVisible(state.bossActive);
        bossImage.invalidate();
    }
    if (state.bossActive)
    {
        touchgfx::BitmapId bossBmp = (state.bossType == 1) ? BITMAP_UFO_BOSS_ID : BITMAP_UFO_BOSS_2_ID;
        bossImage.setBitmap(touchgfx::Bitmap(bossBmp));

        if (bossImage.getX() != state.bossX || bossImage.getY() != state.bossY)
        {
            bossImage.invalidate();
            bossImage.setXY(state.bossX, state.bossY);
            bossImage.invalidate();
        }
    }

    // Cap nhat Boss HP Bar
    bool showBossHp = state.bossActive;
    if (bossHpBarBg.isVisible() != showBossHp)
    {
        bossHpBarBg.setVisible(showBossHp);
        bossHpBar.setVisible(showBossHp);
        bossHpBarBg.invalidate();
        bossHpBar.invalidate();
    }
    if (showBossHp)
    {
        int16_t newWidth = (160 * state.bossHp) / state.bossMaxHp;
        if (newWidth < 0) newWidth = 0;
        if (bossHpBar.getWidth() != newWidth)
        {
            bossHpBar.invalidate();
            bossHpBar.setWidth(newWidth);
            bossHpBar.invalidate();
        }
    }

    // Cap nhat Score chi khi co thay doi (tranh invalidate lien tuc)
    if (state.score != lastScore)
    {
        setScoreDisplay(state.score);
        lastScore = state.score;
    }

    // Cap nhat Lives chi khi co thay doi
    if (state.lives != lastLives)
    {
        setLivesDisplay(state.lives);
        lastLives = state.lives;
    }
}

void Screen1View::updateLevelIntro(const GameState& state)
{
    bool showLevelIntro = (state.levelIntroTimer > 0);

    if (state.level != lastLevel)
    {
        uint8_t shownLevel = state.level;
        if (shownLevel > 99) shownLevel = 99;

        levelTensDigit.setBitmap(touchgfx::Bitmap(getNumBitmap(shownLevel / 10)));
        levelOnesDigit.setBitmap(touchgfx::Bitmap(getNumBitmap(shownLevel % 10)));
        levelTensDigit.invalidate();
        levelOnesDigit.invalidate();
        lastLevel = state.level;
    }

    if (levelTensDigit.isVisible() != showLevelIntro)
    {
        levelTensDigit.setVisible(showLevelIntro);
        levelOnesDigit.setVisible(showLevelIntro);
        levelTensDigit.invalidate();
        levelOnesDigit.invalidate();
    }
}

void Screen1View::gotoGameOverScreen()
{
    // Chuyển sang GameOverScreen
    application().gotoGameOverScreenScreenNoTransition();
}


