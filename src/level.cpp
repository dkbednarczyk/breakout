#include "level.h"

#include "constants.h"

Level::Level()
    : Level(GetRandomValue(3, MAX_ROWS), GetRandomValue(3, MAX_COLUMNS)) {}

Level::Level(int rowCount, int columnCount) {
    rows = rowCount;
    columns = columnCount;
    activeBlockCount = rows * columns;

    layout.assign(rows, std::vector<Block>(columns));

    const float blockWidth = (float)SCREEN_WIDTH / columns;

    for (int r = 0; r < rows; r++) {
        for (int c = 0; c < columns; c++) {
            Block block;

            block.rect = {c * blockWidth, (r * BLOCK_HEIGHT) + BLOCK_HEIGHT,
                          blockWidth - 2.0f, BLOCK_HEIGHT - 2.0f};
            block.color = BLOCK_COLORS[r];
            block.active = true;

            layout[r][c] = block;
        }
    }
}

void Level::DeactivateBlock(Block& block) {
    block.active = false;
    activeBlockCount--;
}