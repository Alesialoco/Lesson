#include <iostream>

void hannoy_tower(unsigned int num, int start, int end, int another){
    if (num == 1){
        std::cout << "Переложить диск радиуса 1 со стержня " << start << " на стержень " << end << std::endl;
        return;
    }
    hannoy_tower(num - 1, start, another, end);
    std::cout << "Переложить диск радиуса " << num << " со стержня " << start << " на стержень " << end << std::endl;
    hannoy_tower(num - 1, another, end, start);
}

int main(){
    unsigned int n;
    int start, end;
    std::cin >> n >> start >> end;
    int another;
    if (start == 1){
        if (end == 2) another = 3;
        else another = 2;
    }
    else if (start == 2){
        if (end == 1) another = 3;
        else another = 1;
    }
    else {
        if (end == 1) another = 2;
        else another = 1;
    }
    hannoy_tower(n, start, end, another);
    return 0;
}