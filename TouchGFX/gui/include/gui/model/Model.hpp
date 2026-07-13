#ifndef MODEL_HPP
#define MODEL_HPP

#include <stdint.h>

#define MAX_ENEMIES 10

struct Enemy
{
    int16_t x;
    int16_t y;
    bool alive;
    uint8_t type; // 0: Red, 1: Blue, 2: Spikey
    int8_t hp;    // Mau cua quai vat
};

struct EnemyBullet
{
    float x;
    float y;
    bool active;
    uint8_t type; // 0: normal, 1: boss2_mid, 2: boss2_side
    float angle;
};

struct GameState
{
    int16_t playerX;       // Toa do X cua tau nguoi choi (0 - 210)
    int16_t playerY;       // Toa do Y cua tau nguoi choi
    int32_t score;         // Diem so hien tai
    int8_t lives;          // So mang con lai (3, 2, 1, 0)
    uint8_t level;          // Man choi hien tai, tang sau moi wave
    int8_t levelIntroTimer; // Hien thong bao level moi khi > 0
    bool isGameOver;       // Trang thai ket thuc game
    bool playBuzzerBeep;   // Trigger coi buzzer keu bip (TV3 doc tu day)
    
    Enemy enemies[MAX_ENEMIES];
    int8_t enemyDirection; // Hướng di chuyển đàn quái (1: Phải, -1: Trái)

    // Thong tin vien dan cua nguoi choi
    int16_t bulletX;
    int16_t bulletY;
    bool bulletActive;

    int16_t boltBuffTimer;
    int16_t bulletLeftX;
    int16_t bulletLeftY;
    bool bulletLeftActive;
    int16_t bulletRightX;
    int16_t bulletRightY;
    bool bulletRightActive;

    int16_t doubleShotBuffTimer;
    int16_t bullet2X;
    int16_t bullet2Y;
    bool bullet2Active;

    int16_t rapidFireTimer;

    EnemyBullet enemyBullets[20];

    // Thong tin vu no
    int16_t explosionX;
    int16_t explosionY;
    int8_t explosionTimer; // Neu > 0 thi vu no dang dien ra

    // Thong tin vat pham roi (Items/Power-ups)
    int16_t itemX;
    int16_t itemY;
    bool itemActive;
    uint8_t itemType;      // 0: Shield, 1: Star, 2: Bolt, 3: Missile, 4: DoubleShot, 5: TripleShot

    // Thong tin ten lua cua nguoi choi
    int16_t missileX;
    int16_t missileY;
    bool missileActive;

    // Thong tin vu no lon (AOE Explosion cua ten lua)
    int16_t largeExplosionX;
    int16_t largeExplosionY;
    int8_t largeExplosionTimer;

    // Thong tin Boss
    bool bossActive;
    uint8_t bossType;
    int16_t bossX;
    int16_t bossY;
    int16_t bossHp;
    int16_t bossMaxHp;
    int16_t bossDirection; // 1: Phải, -1: Trái
    int16_t bossTimer;
    int8_t bossLaserPhase;      // 0: Normal, 1: Aiming, 2: Firing, 3: FlickAim, 4: FlickFire, 5: GridAim, 6: GridFire
    int16_t bossLaserTimer;
    int16_t bossLockedX;
    bool bossForcefieldActive;
    bool bossForcefieldTriggered;
    int8_t bossMinionsAlive;
    int8_t bossLaserWavesLeft;
};

class ModelListener;

class Model
{
public:
    Model();

    void bind(ModelListener* listener)
    {
        modelListener = listener;
    }

    void tick();

    // Lay tham chieu GameState de doc/ghi du lieu
    GameState& getGameState() { return state; }
    
    // Cac helper tien ich
    void setPlayerX(int16_t x) { state.playerX = x; }
    void setScore(int32_t s) { state.score = s; }
    void setLives(int8_t l) { state.lives = l; }
    void setGameOver(bool gameOver) { state.isGameOver = gameOver; }
    void setPlayerMoveDirection(int8_t direction);
    void setPlayerMoveDirectionY(int8_t direction);
    void fireBullet();
    void resetGame();
    void initializeEnemiesForLevel(uint8_t level);
    bool areAllEnemiesDefeated() const;
    void startNextLevel();
    void updateHighScores();
    
    int32_t getHighScore() const { return highScores[0]; }
    int32_t getHighScoreAt(uint8_t index) const { return (index < 3) ? highScores[index] : 0; }

protected:
    ModelListener* modelListener;
    GameState state; // Bien luu tru trang thai game
    int8_t playerMoveDirection;
    int8_t playerMoveTimer;
    int8_t playerMoveDirectionY;
    int8_t playerMoveTimerY;
    int16_t enemyShootCooldown;
    uint8_t nextEnemyShooterIndex;
    uint8_t missileAmmo;
    int8_t enemyVelX[MAX_ENEMIES];
    uint8_t enemyBobPhase[MAX_ENEMIES];
    uint8_t enemyMoveDelay[MAX_ENEMIES];
    int32_t highScores[3]; // Luu top 3 diem cao nhat
    int16_t autoFireCooldown;

#ifndef SIMULATOR
    uint8_t btnW_Debounce;
    uint8_t btnS_Debounce;
    uint8_t btnA_Debounce;
    uint8_t btnD_Debounce;
    uint8_t btnSpace_Debounce;
    bool prevSpacePressed;
#endif
};

#endif // MODEL_HPP
