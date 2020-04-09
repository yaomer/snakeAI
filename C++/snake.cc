#include "snake.h"

void Snake::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);
    if (inithead) initSnake();
    showFood();
    if (findMinPath(head->s, food)) {
        auto s = nextstep;
        findtail = true;
        if (findMinPath(s, tail->s)) {
            growhead(s);
        } else {
            if (findMinPath(head->s, tail->s))
                growhead(nextstep);
        }
    } else {
        findtail = true;
        if (findMinPath(head->s, tail->s))
            growhead(nextstep);
    }
    findtail = false;
    if (isEatFood()) creatFood();
    else decrtail();
}

void Snake::timerEvent(QTimerEvent *event)
{
    Q_UNUSED(event);
    this->repaint();
}

bool Snake::isCrash(Pos pos)
{
    auto p = tail;
    if (findtail) {
        while (p) {
            if (p != tail && pos == p->s) return true;
            p = p->next;
        }
    } else {
        while (p) {
            if (pos == p->s) return true;
            p = p->next;
        }
    }
    return false;
}

bool Snake::isEatFood()
{
    return head->s == food;
}

void Snake::initSnake()
{
    inithead = false;
    qsrand(::clock());
    head = tail = new Node;
    head->s.x = qrand() % (X2 - X1) + X1;
    head->s.y = qrand() % (Y2 - Y1) + Y1;
    showSnake();
    creatFood();
}

void Snake::creatFood()
{
    qsrand(::clock());
    food.x = qrand() % (X2 - X1) + X1;
    food.y = qrand() % (Y2 - Y1) + Y1;
    if (isCrash(food)) creatFood();
    showFood();
}

void Snake::showFood()
{
    QPainter painter(this);
    QPen pen;
    pen.setColor(Qt::green);
    pen.setWidth(P);
    painter.setPen(pen);
    painter.drawPoint(food.x, food.y);
}

void Snake::showSnake()
{
    QPainter painter(this);
    QPen pen;
    pen.setColor(Qt::red);
    pen.setWidth(P);
    painter.setPen(pen);
    for (auto p = tail; p; p = p->next)
        painter.drawPoint(p->s.x, p->s.y);
}

void Snake::growhead(Pos pos)
{
    Node *p = new Node(pos);
    head->next = p;
    head = p;
    showSnake();
}

void Snake::decrtail()
{
    Node *p = tail->next;
    delete tail;
    tail = p;
}

void Snake::clear()
{
    while (tail) {
        Node *p = tail->next;
        delete tail;
        tail = p;
    }
}

bool Snake::findMinPath(Pos p1, Pos p2)
{
    return AStar(p1, p2);
}

#define isOk(x, y)  ((x) >= X1 && (x) <= X2 && y >= Y1 && (y) <= Y2)

bool Snake::AStar(Pos p1, Pos p2)
{
    static bool open[X2][Y2];
    static bool close[X2][Y2];
    bzero(open, sizeof(open));
    bzero(close, sizeof(close));

    AStarPos s(p1.x, p1.y);
    AStarPos d(p2.x, p2.y);
    openlist.push(s);
    open[s.x][s.y] = true;

    static int dx[] = { 0, 0, 1, -1 };
    static int dy[] = { 1, -1, 0, 0 };

    bool found = false;
    while (!openlist.empty()) {
        auto p = openlist.top();
        openlist.pop();
        close[p.x][p.y] = true;
        for (int i = 0; i < 4; i++) {
            int tx = p.x + dx[i];
            int ty = p.y + dy[i];
            Pos pos(tx, ty);
            if (!close[tx][ty] && isOk(tx, ty) && !isCrash(pos)) {
                if (!open[tx][ty]) {
                    AStarPos ts(tx, ty);
                    ts.g = p.g + 1;
                    ts.h = ::abs(tx - d.x) + ::abs(ty - d.y);
                    ts.f = ts.g + ts.h;
                    parent.emplace(ts, p);
                    openlist.push(ts);
                    open[tx][ty] = true;
                }
            }
            if (p.x == d.x && p.y == d.y) {
                backpath(s, d);
                found = true;
                goto end;
            }
        }
    }
end:
    while (!openlist.empty())
        openlist.pop();
    parent.clear();
    return found;
}

void Snake::backpath(const AStarPos &s, const AStarPos &d)
{
    auto lastpos = d;
    while (true) {
        auto it = parent.find(lastpos);
        if (it == parent.end()) break;
        if (it->second == s) {
            nextstep.x = it->first.x;
            nextstep.y = it->first.y;
            break;
        }
        lastpos = it->second;
    }
}
