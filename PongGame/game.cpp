#define isDown(x) input->buttons[x].isDown
#define pressed(x) input->buttons[x].isDown && input->buttons[x].changed
#define relised(x) (!input->buttons[x].isDown && input->buttons[x].changed)



class HalfSize {
public:
    float halfSizeX;
    float halfSizeY;

    HalfSize() : halfSizeX(0.0), halfSizeY(0.0) {}
};

class Player : public HalfSize {
public:
    float p;
    float dp;
    float ddp;
    float score;

    Player() : HalfSize(), score(1), p(0.0), dp(0.0), ddp(0.0) {
        halfSizeX = 1.5;
        halfSizeY = 10;
    }
};

class Ball : public HalfSize {
public:
    float pX, pY;
    float dpX, dpY;

    Ball() {
        halfSizeX = 1;
        halfSizeY = 1;
        pX = 0;
        pY = 0;
        dpX = 100;

    }
};

class Arena : public HalfSize {
public:
    Arena() {
        halfSizeX = 85;
        halfSizeY = 45;
    }
};

Arena arena;
Player playerA;
Player playerB;
Ball ball;


static void simulate_player(Player& player, float dt) {
    player.ddp -= player.dp * 10.f;

    player.p = player.p + player.dp * dt + player.ddp * dt * dt * .5f;
    player.dp = player.dp + player.ddp * dt;

    if (player.p + player.halfSizeY > arena.halfSizeY) {
        player.p = arena.halfSizeY - player.halfSizeY;
        player.dp = 0;
    }
    else if (player.p - player.halfSizeY < -arena.halfSizeY) {
        player.p = -arena.halfSizeY + player.halfSizeY;
        player.dp = 0;
    }
}


static bool aabb_vs_aabb(float p1x, float p1y, float hs1x, float hs1y,
    float p2x, float p2y, float hs2x, float hs2y) {
    return (p1x + hs1x > p2x - hs2x &&
        p1x - hs1x < p2x + hs2x &&
        p1y + hs1y > p2y - hs2y &&
        p1y + hs1y < p2y + hs2y);
}

void drawNumber(float x, float y, int number) 
{
    // Sayý resimlerini çizmek için bir dizi oluþturabilirsiniz.
    const char* numbers[] = {
        "111101101101111", // 0
        "010010010010010", // 1
        "111001111100111", // 2
        "111001111001111", // 3
        "101101111001001", // 4
        "111100111001111", // 5
        "111100111101111", // 6
        "111001001001001", // 7
        "111101111101111", // 8
        "111101111001111"  // 9
    };

    float halfSize = 0.5f; // Sayýlarýn yarý geniþliði

    for (int i = 0; i < 15; i++) 
    {
        float px = x + (i % 3) * (2 * halfSize);
        float py = y - (i / 3) * (2 * halfSize);
        int index = number * 3 + i;

        int digitA = number / 10;
        int digitB = number % 10;

        if (number >= 10)
        {
            if (numbers[digitA][i] == '1') {
                resizeRectangle(px - 4, py, halfSize, halfSize, 0x000000);
            }
        }

        if (numbers[digitB][i] == '1') {
            resizeRectangle(px, py, halfSize, halfSize, 0x000000);
        }
    }
}

void clearNumber(float x, float y)
{
    float halfSize = 1.0f;

    for (int i = 0; i < 15; i++) {
        float px = x + (i % 3) * (2 * halfSize);
        float py = y - (i / 3) * (2 * halfSize);
        resizeRectangle(px, py, halfSize, halfSize, 0xEEEEEE);
    }
}

void drawLetter(float x, float y, char letter)
{
    // Harf resimlerini çizmek için bir dizi oluþturabilirsiniz.
    const char* letters[] = {
        "111101111101101", // A
        "110101111101110", // B
        "111100100100111", // C
        "110101101101110", // D
        "111100111100111", // E
        "111100111100100", // F
        "111100101101111", // G
        "101101111101101", // H
        "111010010010111", // I
        "001001001001111", // J
        "101101110101101", // K
        "100100100100111", // L
        "101111111101101", // M
        "101101111111101", // N
        "010101101101010", // O
        "111101111100100", // P
        "010101101101110", // Q
        "111101111110101", // R
        "111100111001111", // S
        "111010010010010", // T
        "101101101101111", // U
        "101101101101010", // V
        "101101111111101", // W
        "101101010101101", // X
        "101101010010010", // Y
        "111001010100111"  // Z
    };

    float halfSize = 0.5f; // Harflerin yarý geniþliði

    int index = 0;

    if (letter >= 'A' && letter <= 'Z') {
        index = letter - 'A';
    }

    for (int i = 0; i < 15; i++)
    {
        float px = x + (i % 3) * (2 * halfSize);
        float py = y - (i / 3) * (2 * halfSize);

        if (letters[index][i] == '1') {
            resizeRectangle(px, py, halfSize, halfSize, 0x000000);
        }
    }
}





enum GameMode {
    GM_MENU,
    GM_GAMEPLAY,
};

GameMode gameMode;

bool hotButton;
bool enemyIsAI;

static void simulateGame(Input* input, float dt)
{



    if (gameMode == GM_GAMEPLAY)
    {
        clearScreen(0x068FFF);
        draw_arena_borders(arena.halfSizeX, arena.halfSizeY, 0x068FFF);
        resizeRectangle(0, 0, arena.halfSizeX, arena.halfSizeY, 0xEEEEEE);

        playerA.ddp = 0.f;
        if (!enemyIsAI)
        {
            if (isDown(buttonUp)) playerA.ddp += 2000;
            if (isDown(buttonDown)) playerA.ddp -= 2000;
        }
        else
        {
            if (ball.pY > playerA.p + 2.f)
            {
                playerA.ddp += 2000;
            }
            else if (ball.pY < playerA.p - 2.f)
            {
                playerA.ddp -= 2000;
            }
        }

        playerB.ddp = 0.f;
        #if 1
        if (isDown(buttonW)) playerB.ddp += 2000;
        if (isDown(buttonS)) playerB.ddp -= 2000;
        #else
        if (ball.pY > playerB.p)
        {
            playerB.ddp += 2000;
        }
        else
        {
            playerB.ddp -= 2000;
        }
        #endif
        simulate_player(playerA, dt);
        simulate_player(playerB, dt);

        ball.pY += ball.dpY * dt;
        ball.pX += ball.dpX * dt;



        if (aabb_vs_aabb(ball.pX, ball.pY, ball.halfSizeX, ball.halfSizeY, 80, playerA.p, playerA.halfSizeX, playerA.halfSizeY))
        {
            ball.pX = 80 - playerA.halfSizeX - ball.halfSizeX;
            ball.dpX *= -1;
            ball.dpY = (ball.pY - playerA.p) * 2 + playerA.dp * .75f;
        }
        else if (aabb_vs_aabb(ball.pX, ball.pY, ball.halfSizeX, ball.halfSizeY, -80, playerB.p, playerA.halfSizeX, playerA.halfSizeY))
        {
            ball.pX = -80 + playerA.halfSizeX + ball.halfSizeX;
            ball.dpX *= -1;
            ball.dpY = (ball.pY - playerB.p) * 2 + playerB.dp * .75f;
        }

        if (ball.pY + ball.halfSizeY > arena.halfSizeY)
        {
            ball.pY = arena.halfSizeY - ball.halfSizeY;
            ball.dpY *= -1;
        }
        else if (ball.pY - ball.halfSizeY < -arena.halfSizeY)
        {
            ball.pY = -arena.halfSizeY + ball.halfSizeY;
            ball.dpY *= -1;
        }

        if (ball.pX + ball.halfSizeX > arena.halfSizeX) {
            ball.dpX *= -1;
            ball.dpY = 0;
            ball.pX = 0;
            ball.pY = 0;
            playerA.score++;
            ;
        }
        else if (ball.pX - ball.halfSizeX < -arena.halfSizeX) {
            ball.dpX *= -1;
            ball.dpY = 0;
            ball.pX = 0;
            ball.pY = 0;
            playerB.score++;

        }

        float at_x = -75;

        for (int i = 0; i < playerA.score; i++)
        {
            clearNumber(at_x - 4, 40.f);
            clearNumber(at_x, 40.f);
            drawNumber(at_x, 40.f, i);

        }

        float bt_x = 75;

        for (int i = 0; i < playerB.score; i++)
        {
            clearNumber(bt_x - 4, 40.f);
            clearNumber(bt_x, 40.f);
            drawNumber(bt_x, 40.f, i);
        }


        resizeRectangle(ball.pX, ball.pY, ball.halfSizeX, ball.halfSizeY, 0x000000);

        resizeRectangle(0, 0, 0.1, 50, 0x068FFF);
        resizeRectangle(80, playerA.p, playerA.halfSizeX, playerA.halfSizeY, 0x4E4FEB);
        resizeRectangle(-80, playerB.p, playerB.halfSizeX, playerB.halfSizeY, 0x4E4FEB);
    }
    else if (gameMode == GM_MENU)
    {


        if (pressed(buttonLeft))
        {
            hotButton = true;
        }
        else if (pressed(buttonRight))
        {
            hotButton = false;
        }
        if (pressed(buttonEnter))
        {
            gameMode = GM_GAMEPLAY;
            enemyIsAI = hotButton ? 1 : 0;
        }

        if (hotButton == true)
        {
            resizeRectangle(-20, 20, 10, 10, 0x000000);
            resizeRectangle(20, 20, 10, 10, 0xEEEEEE);
        } 
        else
        {
            resizeRectangle(-20, 20, 10, 10, 0xEEEEEE);
            resizeRectangle(20, 20, 10, 10, 0x000000);
        }

        const char* letter = "SINGLE PLAYER";

        for (int i = 0; i < strlen(letter); i++)
        {
            if (letter[i] == ' ')
            {
                continue;
            }
            drawLetter(-62 + i * 4, 38, letter[i]);
        }

        letter = "MULTIPLAYER";

        for (int i = 0; i < strlen(letter); i++)
        {
            drawLetter(10 + i * 4, 38, letter[i]);
        }



        resizeRectangle(0, 20, 10, 1, 0xEEEEEE);
        resizeRectangle(0, 0, 1, 5, 0xEEEEEE);
        resizeRectangle(0, -20, 40, 1, 0xEEEEEE);
        resizeRectangle(-40, -15, 1, 5, 0xEEEEEE);
        resizeRectangle(40, -15, 1, 5, 0xEEEEEE);
    }

}