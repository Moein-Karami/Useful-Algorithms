//https://codeforces.com/problemset/problem/7/C
#include<bits/stdc++.h>

using namespace std;

pair<long long,long long> find_euc(long long a, long long b)
{
    if(a == 0)
        return make_pair(0, 1);
    pair<long long, long long> answer_rec = find_euc(b % a, a);
    pair<long long, long long> answer;
    answer.first = answer_rec.second - ((b / a) * answer_rec.first);
    answer.second = answer_rec.first;
    return answer;
}

int main()
{
    long long a, b, c;
    cin >> a >> b >> c;
    long long gcd_numbers = __gcd(a, b);
    if(c % gcd_numbers != 0)
    {
        cout << -1;
        return 0;
    }
    bool flag = 0;
    if(a > b)
    {
        swap(a, b);
        flag = 1;
    }
    pair<long long,long long> answer = find_euc(a, b);
    long long mult = (-1 * c) / (answer.first * a + answer.second * b);
    if(flag)
        swap(answer.first, answer.second);
    cout << answer.first * mult << " " << answer.second * mult;
}