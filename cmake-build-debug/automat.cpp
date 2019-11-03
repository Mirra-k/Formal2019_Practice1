// ab+c.aba.*.bac.+.+* abacb 4
// acb..bab.c.*.ab.ba.+.+*a. cb 0
#include <iostream>
#include <vector>
#include <string>
#include <stack>
using namespace std;

class Automat
{
private:
    struct Vertex
    {
        vector< pair<Vertex*, char> > to;
        bool finish;
        bool visit = false;
    };
    Vertex *start;
    vector<Vertex*> finish;

private:
    Automat (char symbol);
    Automat mult (Automat &second, Automat &first);
    Automat add (Automat &second, Automat &first);
    Automat star (Automat &first);
    void goForAutomat (Vertex* vertex, const string &u, int &iter, int &len, int &longLen);
    void View (Vertex* vertex);

public:
    Automat (string& reg);
    int FindLongestPreffix (string u);
};

Automat Automat::mult (Automat &second, Automat &first)
{
    for (auto ff : first.finish)
    {
        ff->finish = false;
        for (auto sst : second.start->to)
        {
            ff->to.emplace_back(sst.first, sst.second);
        }
        if (second.finish[0]->to.size()){
            int secFinSize = second.finish.size();
            for (int i = 0; i < secFinSize; ++i)
            {
                if (second.finish[i] != ff)
                {
                    if (second.finish[i]->to[0].first == second.start)
                    {
                        second.finish[i]->to[0] = make_pair(ff, '1');
                    }
                    else
                    {
                        second.finish[i]->to.emplace_back(ff, '1');
                    }
                    ff->finish = true;
                    if (second.finish[second.finish.size() - 1] != ff)
                    {
                        if (second.finish[second.finish.size() - 1] == second.start)
                        {
                            second.finish[second.finish.size() - 1] = ff;
                        }
                        else
                        {
                            second.finish.emplace_back(ff);
                        }
                    }
                }
            }
        }
    }
    delete second.start;
    first.finish = second.finish;
    return first;
}

Automat Automat::add (Automat &second, Automat &first)
{
    for (auto &sst : second.start->to)
    {
        first.start->to.emplace_back(sst);
    }
    for (auto &sf : second.finish)
    {
        first.finish.emplace_back(sf);
    }
    delete second.start;
    return first;
}

Automat Automat::star (Automat &first)
{
    for (auto &fin : first.finish)
    {
        fin->to.emplace_back(first.start, '1');
    }
    first.start->finish = true;
    first.finish.emplace_back(first.start);
    return first;
}

Automat::Automat (char symbol)
{
    start = new Vertex;
    start->finish = false;
    Vertex *nowFinish = new Vertex;
    nowFinish->finish = true;
    start->to.emplace_back(nowFinish, symbol);
    finish.push_back(nowFinish);
}

Automat::Automat (string& reg)
{
    stack<Automat> st;
    for (auto symb : reg)
    {
        if (symb == 'a' || symb == 'b' || symb == 'c' || symb == '1')
        {
            Automat aut((char)symb);
            st.push((aut));;
        }
        else if (symb == '.')
        {
            if(st.size() < 2)
            {
                std::cout << "ERROR";
                exit(3);
            }
            Automat a1 = st.top();
            st.pop();
            Automat a2 = st.top();
            st.pop();
            st.push (mult (a1, a2));
        }
        else if (symb == '+'){
            if(st.size() < 2){
                std::cout << "ERROR";
                exit(3);
            }
            Automat a1 = st.top();
            st.pop();
            Automat a2 = st.top();
            st.pop();
            st.push (add (a1, a2));
        }
        else if (symb == '*')
        {
            if(!st.size())
            {
                std::cout << "ERROR";
                exit(3);
            }
            Automat a1 = st.top();
            st.pop();
            st.push (star (a1));
        }
        else
        {
            std::cout << "ERROR";
            exit(3);
        }
    }
    if(st.size() != 1)
    {
        std::cout << "ERROR";
        exit(3);
    }
    Automat me = st.top();
    st.pop();
    start = me.start;
    finish = me.finish;
    View(start);
}

void Automat::goForAutomat (Vertex* vertex, const string &u, int &iter, int &len, int &longLen)
{
    if (iter > u.size())
    {
        return;
    }
    for (auto &v : vertex->to)
    {
        if (u[iter] == v.second)
        {
            len += 1;
            iter += 1;
            if (len > longLen && v.first->finish)
            {
                longLen = len;
            }
            goForAutomat(v.first, u, iter, len, longLen);
            len -= 1;
            iter -= 1;
        }
        if (v.second == '1')
        {
            goForAutomat(v.first, u, iter, len, longLen);
        }

    }
}

int Automat::FindLongestPreffix (string u)
{
    int len = 0, iter = 0, res = 0;
    goForAutomat(start, std::move(u), iter, len, res);
    return res;
}

void Automat::View (Vertex* vertex)
{
    cout << vertex << " " << vertex->finish << ": ";
    for(auto &v : vertex->to)
    {
        cout << v.second << " " << v.first << "; ";
    }
    cout << endl;
    for(auto &v : vertex->to)
    {
        if(v.second != '1' && !v.first->visit)
        {
            View(v.first);
            v.first->visit = true;
        }
    }
}
int main() {
    string reg;
    string u;
    cin >> reg >> u;
    Automat automat(reg);
    cout << automat.FindLongestPreffix(u) << endl;
    return 0;
}