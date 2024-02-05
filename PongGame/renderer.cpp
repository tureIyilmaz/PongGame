void renderBackground()
{
    unsigned int* pixel = (unsigned int*)renderState.memory;

    for (int y = 0; y < renderState.height; y++)
    {
        for (int x = 0; x < renderState.width; x++)
        {
            *pixel++ = 0x7FC7D9;
        }
    }
}

static void clearScreen(unsigned int color)
{
    unsigned int* pixel = (unsigned int*)renderState.memory;

    for (int y = 0; y < renderState.height; y++)
    {
        for (int x = 0; x < renderState.width; x++)
        {
            *pixel++ = color;
        }
    }
}


static void drawRectangle(int x0, int x1, int y0, int y1, unsigned int color)
{
    x0 = clamp(0, x0, renderState.width);
    x1 = clamp(0, x1, renderState.width);
    y0 = clamp(0, y0, renderState.height);
    y1 = clamp(0, y1, renderState.height);

    for (int y = y0; y < y1; y++)
    {
    unsigned int* pixel = (unsigned int*)renderState.memory + x0 + y * renderState.width;
        for (int x = x0; x < x1; x++)
        {
            *pixel++ = color;
        }
    }
}

static float renderScale = 0.01f;


static void resizeRectangle(float x, float y, float halfX, float halfY, unsigned int color)
{
    x *= renderState.height * renderScale;
    y *= renderState.height * renderScale;
    halfX *= renderState.height * renderScale;
    halfY *= renderState.height * renderScale;

    x += renderState.width / 2.f;
    y += renderState.height / 2.f;

    int x0 = x - halfX;
    int x1 = x + halfX;
    int y0 = y - halfY;
    int y1 = y + halfY;
    drawRectangle(x0, x1, y0, y1, color);
}

static void draw_arena_borders(float arena_x, float arena_y, unsigned int color) {
    arena_x *= renderState.height * renderScale;
    arena_y *= renderState.height * renderScale;

    int x0 = static_cast<int>((static_cast<float>(renderState.width) * 0.5f) - arena_x);
    int x1 = static_cast<int>((static_cast<float>(renderState.width) * 0.5f) + arena_x);
    int y0 = static_cast<int>((static_cast<float>(renderState.height) * 0.5f) - arena_y);
    int y1 = static_cast<int>((static_cast<float>(renderState.height) * 0.5f) + arena_y);

    resizeRectangle(0, 0, renderState.width, y0, color);
    resizeRectangle(0, y1, x1, renderState.height, color);
    resizeRectangle(0, y0, x0, y1, color);
    resizeRectangle(x1, y0, renderState.width, renderState.height, color);
}

