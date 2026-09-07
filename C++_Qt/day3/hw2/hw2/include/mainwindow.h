#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include <QMainWindow>
#include <vector>
#include <queue>
QT_BEGIN_NAMESPACE
namespace Ui
{
class MainWindow;
}
QT_END_NAMESPACE

class map_widget : public QWidget
{
    Q_OBJECT
public:
    map_widget(QWidget *parent = nullptr);
    void set_mode(int m);
    void run_dijkstra();
    void run_astar();
    void set_wall(int i);		// 다른 위젯에서 벽 복사
    void set_start(int i);
    void set_goal(int i);
    int get_visited();		// 결과 읽기
    int get_cost();
    map_widget *twin = nullptr;	// 짝 위젯
    void set_size(int s);
protected:
    void paintEvent(QPaintEvent *) override;
    void mousePressEvent(QMouseEvent *e) override;
private:
    int heuristic(int i);
    int w = 20;			// 가로 칸 수
    int h = 15;			// 세로 칸 수
    std::vector<int> map;		// 0=벽, 1=길
    std::vector<int> g;		// 시작점부터 실제 비용
    std::vector<int> parent;	// 어디서 왔는지
    std::vector<bool> closed;	// 확정된 칸
    std::vector<int> path;		// 최종 경로
    int start = -1;
    int goal = -1;
    int mode = 0;			// 0=벽, 1=시작점, 2=도착점
    int visited = 0;		// 탐색한 노드 수
};

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
private:
    Ui::MainWindow *ui;
};
#endif
