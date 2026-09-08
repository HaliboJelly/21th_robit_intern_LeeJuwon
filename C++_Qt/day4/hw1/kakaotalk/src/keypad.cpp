#include "keypad.h"

static const char *keys[10] = {"0", ".,?!", "abc", "def", "ghi", "jkl", "mno", "pqrs", "tuv", "wxyz"};

keypad::keypad(QLineEdit *e, QPushButton *s, QObject *parent) : QObject(parent)
{
    edit = e;
    shift_btn = s;
    last_key = -1;
    tap = 0;
    shift = 0;
    cur_upper = false;
    timer = new QTimer(this);
    timer->setSingleShot(true);
    connect(timer, &QTimer::timeout, this, &keypad::commit);
}

void keypad::set_buttons(QPushButton *num[10], QPushButton *back, QPushButton *sp, QPushButton *ent)
{
    for (int k = 0; k < 10; k++)
    {
        connect(num[k], &QPushButton::clicked, this, [=]{ key_press(k); });
    }
    connect(back, &QPushButton::clicked, this, &keypad::backspace);
    connect(sp, &QPushButton::clicked, this, &keypad::space);
    connect(ent, &QPushButton::clicked, this, &keypad::enter);
    connect(shift_btn, &QPushButton::clicked, this, &keypad::toggle_shift);
}

void keypad::key_press(int k)
{
    QString set = keys[k];
    if (k == last_key && timer->isActive())	// 같은 키 연속 -> 다음 글자로 교체
    {
        tap = (tap + 1) % set.length();
        text.chop(1);
    }
    else
    {
        commit();
        tap = 0;
        last_key = k;
        cur_upper = (shift > 0);	// 조합 시작 시점에 대소문자 고정
    }

    QChar c = set[tap];
    if (cur_upper)
    {
        c = c.toUpper();
    }
    text += c;

    if (shift == 1)		// 한 글자만 대문자면 바로 해제
    {
        shift = 0;
        shift_btn->setText("↑ abc");
    }

    timer->start(700);
    update_display();
}

void keypad::commit()	// 조합 종료, 다음 입력은 새 글자
{
    timer->stop();
    last_key = -1;
    tap = 0;
}

void keypad::backspace()
{
    commit();
    text.chop(1);
    update_display();
}

void keypad::space()
{
    commit();
    text += ' ';
    update_display();
}

void keypad::enter()
{
    commit();
    if (text.isEmpty())
    {
        return;
    }
    emit enter_pressed();	// 지우기 전에 알림 - mainwindow가 lineEdit 읽어감
    text.clear();
    update_display();
}

void keypad::toggle_shift()
{
    shift = (shift + 1) % 3;
    if (shift == 0)
    {
        shift_btn->setText("↑ abc");
    }
    else if (shift == 1)
    {
        shift_btn->setText("↑ Abc");
    }
    else
    {
        shift_btn->setText("↑ ABC");
    }
}

void keypad::update_display()
{
    edit->setText(text);
}
