#ifndef _SNAKE_H
#define _SNAKE_H

#include <QWidget>
#include <QApplication>
#include <QPainter>

#include <queue>
#include <vector>
#include <unordered_map>

struct Pos {
    int x, y;
    Pos() = default;
    Pos(int x, int y)
    {
        this->x = x;
        this->y = y;
    }
    bool operator==(const Pos& pos) const
    {
        return x == pos.x && y == pos.y;
    }
};

struct Node {
    Node() = default;
    Node(const Pos& pos) : s(pos)
    {
    }
    Pos s;
    Node *next = nullptr;
};

struct AStarPos {
    int x, y; // 坐标
    int f, g, h; // for A*
    AStarPos() = default;
    AStarPos(int x, int y)
    {
        this->x = x;
        this->y = y;
        f = g = h = 0;
    }
    AStarPos& operator=(const AStarPos& pos)
    {
        memcpy(this, &pos, sizeof(pos));
        return *this;
    }
    bool operator==(const AStarPos& pos) const
    {
        return x == pos.x && y == pos.y;
    }
};

struct  ASPCompare {
    bool operator()(const AStarPos& p1, const AStarPos& p2) const
    {
        return p1.f > p2.f;
    }
};

struct ASPHash {
    size_t operator()(const AStarPos& p) const
    {
        return p.x * 31 + p.y * 13;
    }
};

#define P 18
#define XW 300
#define YW 300
#define X1 (P / 2)
#define Y1 (P / 2)
#define X2 (XW - P / 2)
#define Y2 (YW - P / 2)

// 整个蛇的构造是这样的：
// O -> O -> O -> O -> O
// tail             head
// 运动时会在head后添加一个新节点，从tail处移除一个节点
class Snake : public QWidget {
    Q_OBJECT
public:
    Snake() : head(nullptr), tail(nullptr), findtail(false), inithead(true)
    {
        this->setWindowTitle("snakeAI");
        this->resize(XW, YW);
        this->startTimer(10);
    }

protected:
    // void keyPressEvent(QKeyEvent *event) override ;
    void paintEvent(QPaintEvent *event) override ;
    void timerEvent(QTimerEvent *event) override ;

private:
    void initSnake();
    void creatFood();
    void showFood();
    void showSnake();
    void growhead(Pos pos);
    void decrtail();
    void clear();

    bool isCrash(Pos pos);
    bool isEatFood();

    bool AStar(Pos p1, Pos p2);
    bool findMinPath(Pos p1, Pos p2);
    void backpath(const AStarPos& s, const AStarPos& d);
    // bool findMaxPath(Pos p1, Pos p2);
    std::priority_queue<AStarPos, std::vector<AStarPos>, ASPCompare> openlist;
    std::unordered_map<AStarPos, AStarPos, ASPHash> parent;

    Node *head, *tail;
    Pos nextstep, food;
    bool findtail;
    bool inithead;
};

#endif // _SNAKE_H
