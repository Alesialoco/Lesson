#include <iostream>
using namespace std;

void stars(unsigned long long num){
    for (int i = 0; i < num; i++){
        for (int j = 0; j <= i; j++){
            cout << '*';
        }
        cout << endl;
    }
    for (int k = 0; k < num - 1; k++){
        for (int m = num - 1 - k; m > 0; m--){
            cout << '*';
        }
        cout << endl;
    }
}

int main(){
    unsigned long long n;
    cin >> n;
    stars(n);
    return 0;
}