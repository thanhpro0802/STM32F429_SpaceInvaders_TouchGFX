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
    int16_t x;
    int16_t y;
    bool active;
    uint8_t type; // 0: normal, 1: boss2_mid, 2: boss2_side
};

// Struct dung chung de trao doi du lieu giua Logic, Ngoai vi va GUI
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

    EnemyBullet enemyBullets[5];

    // Thong tin vu no
    int16_t explosionX;
    int16_t explosionY;
    int8_t explosionTimer; // Neu > 0 thi vu no dang dien ra

    // Thong tin vat pham roi (Items/Power-ups)
    int16_t itemX;
    int16_t itemY;
    bool itemActive;
    uint8_t itemType;      // 0: Shield, 1: Star, 2: Bolt, 3: Missile

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
    int8_t bossDirection; // 1: Phải, -1: Trái
    int16_t bossTimer;
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
    int32_t highScores[3]; // Luu top 3 diem cao nhat
};

#endif // MODEL_HPP
