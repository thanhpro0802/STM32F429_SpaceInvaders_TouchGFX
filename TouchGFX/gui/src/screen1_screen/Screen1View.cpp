#include <gui/screen1_screen/Screen1View.hpp>
#include <touchgfx/Utils.hpp>
#include <images/BitmapDatabase.hpp>

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

    enemyBullet.setXY(0, 0);
    enemyBullet.setBitmap(touchgfx::Bitmap(BITMAP_LASER_ENEMY_ID));
    enemyBullet.setVisible(false);
    add(enemyBullet);
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
    playerShip.setX(state.playerX);
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
    else if (key == 32 || key == ' ') // Phím Space để bắn đạn
    {
        presenter->fireBullet();
    }

}

void Screen1View::updateGameState(const GameState& state)
{
    updateLevelIntro(state);

    // Cap nhat vi tri tau nguoi choi
    if (playerShip.getX() != state.playerX)
    {
        playerShip.invalidate();
        playerShip.setX(state.playerX);
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

    if (enemyBullet.isVisible() != state.enemyBulletActive)
    {
        enemyBullet.setVisible(state.enemyBulletActive);
        enemyBullet.invalidate();
    }
    if (state.enemyBulletActive)
    {
        if (enemyBullet.getX() != state.enemyBulletX || enemyBullet.getY() != state.enemyBulletY)
        {
            enemyBullet.invalidate();
            enemyBullet.setXY(state.enemyBulletX, state.enemyBulletY);
            enemyBullet.invalidate();
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
        else
            itemBmp = BITMAP_ITEM_MISSILE_ID; // Item ten lua moi
            
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


