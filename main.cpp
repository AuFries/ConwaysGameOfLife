#include <SFML/Graphics.hpp>
#include <time.h>

#define SCREEN_HEIGHT 800
#define SCREEN_WIDTH 800
#define DELAY_TIME 100

const int numCells = 32;

struct Cell{
int red;
int blue;
int green;
int isAlive = 0;
sf::RectangleShape rect;
};

struct Cell cells[numCells][numCells];

sf::VertexArray verticalGridLines[numCells];
sf::VertexArray horizontalGridLines[numCells];

int numNeighborCells(int r, int c) {
    int numNeighbors = 0;
    for(int i = r-1; i <= r+1; i++) {
        for(int j = c-1; j <= c+1; j++) {
            if(i >= 0 && i < numCells && j >= 0 && j < numCells && (i != r || j != c) && cells[i][j].isAlive) {
                numNeighbors++;
            }
        }
    }
    return numNeighbors;
}
void gameOfLife() {
    struct Cell nextCells[numCells][numCells];

    for(int i = 0; i < numCells; i++) {
        for(int j = 0; j < numCells; j++) {
            nextCells[i][j] = cells[i][j];
        }
    }

    for(int i = 0; i < numCells; i++) {
        for(int j = 0; j < numCells; j++) {
            int numNeighbors = numNeighborCells(i,j);
            if(!cells[i][j].isAlive && numNeighbors == 3) { //Make cell alive
                nextCells[i][j].isAlive = 1;
            } else if (cells[i][j].isAlive && numNeighbors != 2 && numNeighbors != 3) { //Make cell dead
                nextCells[i][j].isAlive = 0;
            }
        }
    }

    for(int i = 0; i < numCells; i++) {
        for(int j = 0; j < numCells; j++) {
            cells[i][j] = nextCells[i][j];
        }
    }
}

int main()
{
    srand(time(0));

    for(int i = 0; i < numCells; i++) {
        for(int j = 0; j < numCells; j++) {
            cells[i][j].red = 255;
            cells[i][j].isAlive = rand() % 2;
            sf::RectangleShape rectangle(sf::Vector2f(SCREEN_HEIGHT/numCells,SCREEN_WIDTH/numCells));
            rectangle.setPosition(i*(SCREEN_HEIGHT/numCells),j*(SCREEN_WIDTH/numCells));
            rectangle.setFillColor(sf::Color(cells[i][j].red,cells[i][j].green,cells[i][j].blue));
            cells[i][j].rect = rectangle;
        }
    }

    //cells[1][2].isAlive = 1;
    //cells[2][2].isAlive = 1;
    //cells[3][2].isAlive = 1;
    for(int i = 0; i < numCells; i++) {
        sf::VertexArray verticalLine(sf::Lines, 2);
        verticalLine[0].position = sf::Vector2f(i*(SCREEN_HEIGHT/numCells),0);
        verticalLine[1].position = sf::Vector2f(i*(SCREEN_HEIGHT/numCells),SCREEN_HEIGHT);
        verticalGridLines[i] = verticalLine;

        sf::VertexArray horizontalLine(sf::Lines, 2);
        horizontalLine[0].position = sf::Vector2f(0,i*(SCREEN_WIDTH/numCells));
        horizontalLine[1].position = sf::Vector2f(SCREEN_WIDTH,i*(SCREEN_WIDTH/numCells));
        horizontalGridLines[i] = horizontalLine;
    }

    sf::RenderWindow window(sf::VideoMode(SCREEN_WIDTH, SCREEN_HEIGHT), "Conway's Game of Life");
    sf::Clock Clock;

    while (window.isOpen())
    {

        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        window.clear();
        for(int i = 0; i < numCells; i++) {
            for(int j = 0; j < numCells; j++) {
                    if(cells[i][j].isAlive) {
                        window.draw(cells[i][j].rect);
                    }
            }
        }
        for(int i = 0; i < numCells; i++) {
            window.draw(horizontalGridLines[i]);
            window.draw(verticalGridLines[i]);
        }
        window.display();

        if(Clock.getElapsedTime().asMilliseconds() >= DELAY_TIME) {
            gameOfLife();
            Clock.restart();
        }

    }

    return 0;
}
