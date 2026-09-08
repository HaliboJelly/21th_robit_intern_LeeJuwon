#ifndef KEYPAD_H
#define KEYPAD_H
#include <QObject>
#include <QLineEdit>
#include <QPushButton>
#include <QTimer>
class keypad : public QObject
{
    Q_OBJECT
public:
    keypad(QLineEdit *e, QPushButton *s, QObject *parent = nullptr);
    void set_buttons(QPushButton *num[10], QPushButton *back, QPushButton *sp, QPushButton *ent);
signals:
    void enter_pressed();
private:
    void key_press(int k);
    void commit();
    void backspace();
    void space();
    void enter();
    void toggle_shift();
    void update_display();

    QLineEdit *edit;
    QPushButton *shift_btn;
    QTimer *timer;
    QString text;
    int last_key;	// 직전에 누른 키, 없으면 -1
    int tap;		// 같은 키를 몇 번째 눌렀는지
    int shift;		// 0:소문자 1:한글자만 2:고정
    bool cur_upper;	// 조합 중인 글자의 대소문자
};
#endif
