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
};

// Struct dung chung de trao doi du lieu giua Logic, Ngoai vi va GUI
struct GameState
{
    int16_t playerX;       // Toa do X cua tau nguoi choi (0 - 210)
    int16_t score;         // Diem so hien tai
    int8_t lives;          // So mang con lai (3, 2, 1, 0)
    bool isGameOver;       // Trang thai ket thuc game
    bool playBuzzerBeep;   // Trigger coi buzzer keu bip (TV3 doc tu day)
    
    Enemy enemies[MAX_ENEMIES];
    int8_t enemyDirection; // Hướng di chuyển đàn quái (1: Phải, -1: Trái)

    // Thong tin vien dan cua nguoi choi
    int16_t bulletX;
    int16_t bulletY;
    bool bulletActive;

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
    void setScore(int16_t s) { state.score = s; }
    void setLives(int8_t l) { state.lives = l; }
    void setGameOver(bool gameOver) { state.isGameOver = gameOver; }
    void fireBullet();
    void resetGame();
    
    int16_t getHighScore() const { return highScores[0]; }
    int16_t getHighScoreAt(uint8_t index) const { return (index < 3) ? highScores[index] : 0; }

protected:
    ModelListener* modelListener;
    GameState state; // Bien luu tru trang thai game
    int16_t highScores[3]; // Lưu top 3 điểm cao nhất
};

#endif // MODEL_HPP
