#include "../include/class.hpp"
#include <iostream>
using namespace std;

//몬스터 생성자
Monster::Monster()
{
	HP = 0;
	x = 0;
	y = 0;
}

Monster::Monster(int _x,int _y,int _HP)
{
	this->x = _x;
	this->y = _y;
	this->HP = _HP;
}

//플레이어 생성자
Player::Player()
{
	HP = 50;
	MP = 10;
	x = 0;
	y = 0;
}

Player::Player(int _x,int _y)
{
	this->x = _x;
	this->y = _y;
	HP = 50;
	MP = 10;
}



int Monster::Be_Attacked() {   //몬스터 공격받음
    HP -= 10;
    cout << "공격 성공!" << endl;
    cout << "남은 체력: " << HP << endl;
    return HP;
}


void Player::X_move(int move)	// 플레이어 x좌표 이동
{
	x += move;
}

void Player::Y_move(int move)	// 플레이어 y좌표 이동
{
	y += move;
}

void Player::Show_Status()	// 플레이어 상태 출력
{
    cout << "HP: " << HP << endl;
    cout << "MP: " << MP << endl;
    cout << "Position: " << x << "," << y << endl;
}

void Player::Attack(Monster &target) 
{
    
    if(x == target.x && y == target.y && target.HP > 0 && MP > 0) // 플레이어와 몬스터의 좌표가 같으면 공격 가능
    {
        target.Be_Attacked(); // 몬스터 공격받음
        if(target.HP <= 0) // 몬스터 체력이 0 이하이면 프로그램 종료
        {
            cout << "Monster Die!!" << endl;
            exit(0);
        }
    }
    
    else if(MP <= 0) // 플레이어 MP가 0 이하이면 프로그램 종료
    {
        cout << "MP 부족!" << endl;
        exit(0);
    }
    else
    {
        cout << "공격 실패!" << endl;
    }
    MP -= 1; // 플레이어 MP 감소
}



int main()
{
	char input;
	Player player(0,0);
	Monster monster(5,4,50);

	while(1)
	{
		cout << "Type Command(A/U/D/R/L/S)" << endl;
		cin >> input;   //입력받기

		if(input=='R')
		{
			player.X_move(1);
			cout << "X Position 1 moved!" << endl;
		}
		else if(input=='L')
		{
			player.X_move(-1);
			cout << "X Position -1 moved!" << endl;
		}
		else if(input=='U')
		{
			player.Y_move(1);
			cout << "Y Position 1 moved!" << endl;
		}
		else if(input=='D')
		{
			player.Y_move(-1);
			cout << "Y Position -1 moved!" << endl;
		}
        else if(input=='S')
        {
            player.Show_Status();
        }
        else if(input=='A')
        {
            player.Attack(monster);
        }
        else
        {
            cout << "잘못된 입력입니다." << endl;
        }


	}
}