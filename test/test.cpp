#include <bits/stdc++.h>
using namespace std;

#include <windows.h>

int power(int x, int exp) {
    if (exp == 0) {
        return 1;
    }
    else if (exp == 1) {
        return x;
    }
    else if (exp % 2 != 0) { 
        int y = power(x * x, exp / 2) * x;
        return y;
    }
    else {
        int y = power(x * x, exp / 2);
        return y;
    }
}

int getDigit(int x) {
    int ans = 0;
    while (x) {
        ans += x % 10;
        x = x / 10;
    }
    return ans;
}

signed main() {
    int i = 0;
    while (1)
        {
            cout << i++ << '\n';
        }
    return 0;
}

