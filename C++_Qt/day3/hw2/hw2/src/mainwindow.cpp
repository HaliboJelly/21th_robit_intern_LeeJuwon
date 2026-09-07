#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QPainter>
#include <QMouseEvent>
#include <QPushButton>
#include <QElapsedTimer>
#include <cstdlib>
#include <QSpinBox>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->map_area->twin = ui->map_area2;	// 서로 짝 지어주기
    ui->map_area2->twin = ui->map_area;

    connect(ui->btn_wall, &QPushButton::clicked, this, [this]()
            {
                ui->map_area->set_mode(0);
                ui->map_area2->set_mode(0);
            });
    connect(ui->btn_start, &QPushButton::clicked, this, [this]()
            {
                ui->map_area->set_mode(1);
                ui->map_area2->set_mode(1);
            });
    connect(ui->btn_goal, &QPushButton::clicked, this, [this]()
            {
                ui->map_area->set_mode(2);
                ui->map_area2->set_mode(2);
            });
    connect(ui->btn_run, &QPushButton::clicked, this, [this]()
            {
                QElapsedTimer t;
                t.start();
                ui->map_area->run_astar();
                double ta = t.nsecsElapsed() / 1000000.0;
                t.restart();
                ui->map_area2->run_dijkstra();
                double td = t.nsecsElapsed() / 1000000.0;
                ui->lbl_astar->setText(QString("A*  cost: %1  visited: %2  %3 ms")
                                           .arg(ui->map_area->get_cost()).arg(ui->map_area->get_visited()).arg(ta));
                ui->lbl_dijk->setText(QString("Dijkstra  cost: %1  visited: %2  %3 ms")
                                          .arg(ui->map_area2->get_cost()).arg(ui->map_area2->get_visited()).arg(td));
            });
    connect(ui->btn_apply, &QPushButton::clicked, this, [this]()
            {
                int s = ui->spin_size->value();
                ui->map_area->set_size(s);
                ui->map_area2->set_size(s);
            });
}

MainWindow::~MainWindow()
{
    delete ui;
}

map_widget::map_widget(QWidget *parent) : QWidget(parent)
{
    map.assign(w * h, 1);
}
void map_widget::set_size(int s)
{
    w = s;
    h = s;			// 정사각형
    map.assign(w * h, 1);
    g.clear();
    parent.clear();
    closed.clear();
    path.clear();
    start = -1;		// 인덱스가 무효가 되니 리셋
    goal = -1;
    visited = 0;
    update();
}
void map_widget::set_mode(int m)
{
    mode = m;
}

void map_widget::set_wall(int i)
{
    if (map[i] == 0)
    {
        map[i] = 1;
    }
    else
    {
        map[i] = 0;
    }
    update();
}

void map_widget::set_start(int i)
{
    start = i;
    map[i] = 1;
    update();
}

void map_widget::set_goal(int i)
{
    goal = i;
    map[i] = 1;
    update();
}

int map_widget::get_visited()
{
    return visited;
}

int map_widget::get_cost()
{
    if (goal == -1 || g.empty())
    {
        return -1;
    }
    return g[goal];
}

int map_widget::heuristic(int i)
{
    int x1 = i % w;
    int y1 = i / w;
    int x2 = goal % w;
    int y2 = goal / w;
    return abs(x1 - x2) + abs(y1 - y2);	// 맨해튼 거리
}

void map_widget::paintEvent(QPaintEvent *)
{
    QPainter p(this);
    int cell = qMin(width() / w, height() / h);
    for (int y = 0; y < h; y++)
    {
        for (int x = 0; x < w; x++)
        {
            int i = y * w + x;  //칸별로 인덱스 번호를 매긴것, 예를들어 (0,0)은 0, (10,10)은 99이다.
            QRect r(x * cell, y * cell, cell, cell);
            if (map[i] == 0)
            {
                p.fillRect(r, Qt::black);	// 벽
            }
            else
            {
                p.fillRect(r, Qt::white);	// 길
            }
            if (closed.size() == map.size() && closed[i])
            {
                p.fillRect(r, QColor(180, 180, 255));	// 방문
            }
            p.drawRect(r);	// 격자선
        }
    }
    for (int k = 0; k < (int)path.size(); k++)
    {
        if (path[k] == start || path[k] == goal)
        {
            continue;
        }
        int px = path[k] % w;
        int py = path[k] / w;
        p.fillRect(QRect(px * cell, py * cell, cell, cell), Qt::yellow);
    }
    if (start != -1)
    {
        p.fillRect(QRect(start % w * cell, start / w * cell, cell, cell), Qt::green);
    }
    if (goal != -1)
    {
        p.fillRect(QRect(goal % w * cell, goal / w * cell, cell, cell), Qt::red);
    }
}

void map_widget::mousePressEvent(QMouseEvent *e)
{
    int cell = qMin(width() / w, height() / h);
    int x = e->position().x() / cell;	// 픽셀 -> 칸 번호
    int y = e->position().y() / cell;
    if (x < 0 || y < 0 || x >= w || y >= h)
    {
        return;
    }
    int i = y * w + x;
    if (mode == 0)
    {
        if (i == start || i == goal)
        {
            return;		// 시작/도착점엔 벽 못 씀
        }
        set_wall(i);
        twin->set_wall(i);	// 짝 위젯에도 똑같이
    }
    else if (mode == 1)
    {
        set_start(i);
        twin->set_start(i);
    }
    else
    {
        set_goal(i);
        twin->set_goal(i);
    }
}

void map_widget::run_dijkstra()     //다익스트라 계산
{
    if (start == -1 || goal == -1)
    {
        return;
    }
    int n = w * h;
    g.assign(n, 1000000);
    parent.assign(n, -1);
    closed.assign(n, false);
    path.clear();
    visited = 0;

    std::priority_queue<std::pair<int, int>, std::vector<std::pair<int, int>>, std::greater<>> open;
    g[start] = 0;
    open.push({0, start});

    int dx[4] = {1, -1, 0, 0};
    int dy[4] = {0, 0, 1, -1};
    while (!open.empty())
    {
        int cur = open.top().second;
        open.pop();
        if (closed[cur])
        {
            continue;
        }
        closed[cur] = true;
        visited++;
        if (cur == goal)
        {
            break;
        }
        for (int d = 0; d < 4; d++)
        {
            int nx = cur % w + dx[d];
            int ny = cur / w + dy[d];
            if (nx < 0 || ny < 0 || nx >= w || ny >= h)
            {
                continue;
            }
            int nxt = ny * w + nx;
            if (map[nxt] == 0 || closed[nxt])
            {
                continue;
            }
            int ng = g[cur] + 1;	// 한 칸 이동 비용 1
            if (ng < g[nxt])
            {
                g[nxt] = ng;
                parent[nxt] = cur;
                open.push({ng, nxt});	// f = g
            }
        }
    }

    int cur = goal;
    while (cur != -1)
    {
        path.push_back(cur);
        cur = parent[cur];
    }
    update();
}

void map_widget::run_astar()    //a*계산
{
    if (start == -1 || goal == -1)
    {
        return;
    }
    int n = w * h;
    g.assign(n, 1000000);
    parent.assign(n, -1);
    closed.assign(n, false);
    path.clear();
    visited = 0;

    std::priority_queue<std::pair<int, int>, std::vector<std::pair<int, int>>, std::greater<>> open;
    g[start] = 0;
    open.push({heuristic(start), start});

    int dx[4] = {1, -1, 0, 0};
    int dy[4] = {0, 0, 1, -1};
    while (!open.empty())
    {
        int cur = open.top().second;
        open.pop();
        if (closed[cur])
        {
            continue;
        }
        closed[cur] = true;
        visited++;
        if (cur == goal)
        {
            break;
        }
        for (int d = 0; d < 4; d++)
        {
            int nx = cur % w + dx[d];
            int ny = cur / w + dy[d];
            if (nx < 0 || ny < 0 || nx >= w || ny >= h)
            {
                continue;
            }
            int nxt = ny * w + nx;
            if (map[nxt] == 0 || closed[nxt])
            {
                continue;
            }
            int ng = g[cur] + 1;
            if (ng < g[nxt])
            {
                g[nxt] = ng;
                parent[nxt] = cur;
                open.push({ng + heuristic(nxt), nxt});	// f = g + h
            }
        }
    }

    int cur = goal;
    while (cur != -1)
    {
        path.push_back(cur);
        cur = parent[cur];
    }
    update();
}
