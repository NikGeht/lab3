#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <bitset>
#include <math.h>

using namespace std;

// Алгоритм RSA. Параметры: p = 173, q = 263, e = 49.


long long fast_pow_mod(int a, int x, int p)   //Алгоритм быстрого возведения в степень по модулю m^e mod n
{
    long long res = 1;
    vector<int> bin_a, t;
    bitset<32> b(x); //перевод степени в строку бит
    int mark = log2(x) + 1; 
    for (int i = 0; i < mark; i++)
        bin_a.push_back(b[i]);
    vector<int> massive;
    massive.push_back(a);
    for (int i = 1; i < mark; i++)
        massive.push_back((massive[i - 1] * massive[i - 1]) % p);
    for (int i = 0; i < mark; i++)
        res = (long long)(res * pow(massive[i], bin_a[i])) % p;
    return res;
}

int euclid(int a, int b) //Алгоритм евклида
{
    while (true)
    {
        if (a > b)
        {
            a = a - b;
        }
        else
        {
            b = b - a;
        }
        if (a == b)
            break;
    }
    return a; //НОД(a, b)
}

//a*x + b*y = нод(a,b);
pair<int, int> expand_euclid(int a, int b) //Расширенный алгоритм евклида для получения ключа d
{
    int s0 = 1, s1 = 0, t0 = 0, t1 = 1;
    int div, mod = 1, s, t, x, y;
    if (a > b)
    {
        x = a;
        y = b;
    }
    else
    {
        x = b;
        y = a;
    }
    while (mod != 0)
    {
        div = x / y;
        mod = x - (div * y);
        s = s0 - div * s1;
        t = t0 - div * t1;
        s0 = s1;
        s1 = s;
        t0 = t1;
        t1 = t;
        x = y;
        y = mod;
    }
    return make_pair(s0, t0);
}

void encode(int e, int n) //шифрование
{
    string text, decimal_alp; //алфавит по таблице
    ifstream S("input.txt");
    getline(S, text, '\0');
    S.close();
    cout << text << endl;
    for (int i = 0; i < text.size(); i++) // перевод символов в числовое значение
    {
        if (int(text[i]) >= 'А' && int(text[i]) <= 'Я')
            decimal_alp = decimal_alp + to_string(int(text[i]) + 74);
        if (int(text[i]) >= 'A' && int(text[i]) <= 'Z')
            decimal_alp = decimal_alp + to_string(int(text[i]) - 23);
        if (text[i] == ' ')
            decimal_alp = decimal_alp + to_string(int(text[i]) + 36);
        if (text[i] == ',')
            decimal_alp = decimal_alp + to_string(int(text[i]) + 25);
        if (text[i] == '.')
            decimal_alp = decimal_alp + to_string(int(text[i]) + 24);
    }
    cout << decimal_alp << endl;
    ofstream R("output.txt");
    string buf;
    for (int i = 0; i < decimal_alp.size(); i++) //шифрование
    {
        buf = buf + decimal_alp[i];
        if (stoi(buf) > n && i != decimal_alp.size() - 1)
        {
            if (decimal_alp[i] - 48 == 0)
            {
                buf = buf.assign(buf, 0, buf.size() - 2);
                cout << buf << endl;
                R << fast_pow_mod(stoi(buf), e, n);
                R << " ";
                buf = "";
                i = i - 2;
            }
            else
            {
                buf = buf.assign(buf, 0, buf.size() - 1);
                //cout << buf << endl;
                R << fast_pow_mod(stoi(buf), e, n);
                R << " ";
                buf = "";
                i--;
            }
        }
        if (i == decimal_alp.size() - 1)
        {
            if (stoi(buf) < n)
            {
                buf = buf.assign(buf, 0, buf.size());
                //cout << buf << endl;
                R << fast_pow_mod(stoi(buf), e, n);
            }
            else
            {
                if (decimal_alp[i] - 48 == 0 || decimal_alp[i] - 48 == 1)
                {
                    buf = buf.assign(buf, 0, buf.size() - 2);
                    //cout << buf << endl;
                    R << fast_pow_mod(stoi(buf), e, n);
                    R << " ";
                    buf = "";
                    i = i - 2;
                }
                else
                {
                    buf = buf.assign(buf, 0, buf.size() - 1);
                    //cout << buf << endl;
                    R << fast_pow_mod(stoi(buf), e, n);
                    R << " ";
                    buf = "";
                    i--;
                }
            }

        }
    }
    R.close();
}

void decode(int d, int n) //дешифрование
{
    int num;
    string text, t;
    ifstream S("output.txt");
    while (!S.eof())
    {
        S >> num;
        text = text + to_string(fast_pow_mod(num, d, n));
    }
    for (int i = 0; i < text.size(); i = i + 2)
    {
        t.assign(text, i, 2);
        num = stoi(t);
        if (num > 9 && num < 42)
            cout << char(num - 74);
        if (num > 41 && num < 68)
            cout << char(num + 23);
        if (num == 68)
            cout << " ";
        if (num == 69)
            cout << ",";
        if (num == 70)
            cout << ".";
    }
    S.close();
}

int main()
{
    setlocale(LC_ALL, "rus");
    int p = 173, q = 263, e = 49;   //исходные данные p, q, e
    int n = p * q;                  //mod n
    int f = (p - 1) * (q - 1);      //функция эйлера
    int d;                          //закрытый ключ d
    if (euclid(e, f) == 1)
    {
        auto kf = expand_euclid(e, f);
        d = f + kf.first;
        if ((d * e) % f != 1) //условие выполнения d -> d * e mod f = 1
            d = f + kf.second;
        encode(e, n);
        decode(d, n);
    }
    //cout << d * e % f;
    cout << "\nn = " << n << "\nf = " << f << "\nd = " << d << endl;
}

//c = m^e mod N; шифрование
//m = c^d mod N; дешифрование