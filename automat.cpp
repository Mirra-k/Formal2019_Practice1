#include <iostream>
#include <vector>
#include <string>
#include <stack>

class Automat
{
private:
    struct Vertex
    {
        std::vector< std::pair<Vertex*, char> > to;
        bool final;
        bool visit = false;
    };
    Vertex *start;
    std::vector<Vertex*> final;

private:
    Automat (char symbol);
    Automat mult (Automat &second, Automat &first);
    Automat add (Automat &second, Automat &first);
    Automat star (Automat &first);
    void goForAutomat (Vertex* vertex, const std::string &word, int &iter, int &len, int &longLen);

public:
    Automat (std::string reg);
    int FindLongestPreffix (std::string word);
    std::string CheckRegular(std::string regular);
};

std::string Automat:: CheckRegular(std::string regular)
{
    std::stack<std::string> st;
    for (auto symb : regular)
    {
        if (symb == 'a' || symb == 'b' || symb == 'c' || symb == '1')
        {
            std::string new_reg = std::to_string(symb);
            st.push(new_reg);
        }
        else if (symb == '.')
        {
            if(st.size() < 2)
            {
                return "ERROR";
            }
            std::string new_reg = st.top() + ".";
            st.pop();
            new_reg += st.top();
            st.pop();
            st.push (new_reg);
        }
        else if (symb == '+')
        {
            if(st.size() < 2)
            {
                return "ERROR";
            }
            std::string new_reg = st.top() + "+";
            st.pop();
            new_reg += st.top();
            st.pop();
            st.push (new_reg);
        }
        else if (symb == '*')
        {
            if(!st.size())
            {
                return "ERROR";
            }
            std::string new_reg = st.top() + "*";
            st.pop();
            st.push (new_reg);
        }
        else
        {
            return "ERROR";
        }
    }
    if(st.size() != 1)
    {
        return "ERROR";
    }
    return "OK";
}

Automat Automat::mult (Automat &second, Automat &first)
{
    for (auto finish_vertex_1 : first.final)
    {
        finish_vertex_1->final = false;
        for (auto start_vertex_2 : second.start->to)
        {
            finish_vertex_1->to.emplace_back(start_vertex_2.first, start_vertex_2.second);
        }
        if (second.final[0]->to.size()){
            int final_size_2 = second.final.size();
            for (int i = 0; i < final_size_2; ++i)
            {
                if (second.final[i] != finish_vertex_1)
                {
                    if (second.final[i]->to[0].first == second.start)
                    {
                        second.final[i]->to[0] = std::make_pair(finish_vertex_1, '1');
                    }
                    else
                    {
                        second.final[i]->to.emplace_back(finish_vertex_1, '1');
                    }
                    finish_vertex_1->final = true;
                    if (second.final[second.final.size() - 1] != finish_vertex_1)
                    {
                        if (second.final[second.final.size() - 1] == second.start)
                        {
                            second.final[second.final.size() - 1] = finish_vertex_1;
                        }
                        else
                        {
                            second.final.emplace_back(finish_vertex_1);
                        }
                    }
                }
            }
        }
    }
    delete second.start;
    first.final = second.final;
    return first;
}

Automat Automat::add (Automat &second, Automat &first)
{
    for (auto &start_vertex_2 : second.start->to)
    {
        first.start->to.emplace_back(start_vertex_2);
    }
    for (auto &sf : second.final)
    {
        first.final.emplace_back(sf);
    }
    delete second.start;
    return first;
}

Automat Automat::star (Automat &first)
{
    for (auto &final : first.final)
    {
        final->to.emplace_back(first.start, '1');
    }
    first.start->final = true;
    first.final.emplace_back(first.start);
    return first;
}

Automat::Automat (char symbol)
{
    start = new Vertex;
    start->final = false;
    Vertex *nowFinish = new Vertex;
    nowFinish->final = true;
    start->to.emplace_back(nowFinish, symbol);
    final.push_back(nowFinish);
}

Automat::Automat (std::string reg)
{
    if(CheckRegular(reg) == "OK")
    {
        std::stack<Automat> st;
        for (auto symb : reg)
        {
            if (symb == 'a' || symb == 'b' || symb == 'c' || symb == '1')
            {
                Automat aut((char)symb);
                st.push((aut));;
            }
            else if (symb == '.')
            {
                Automat a1 = st.top();
                st.pop();
                Automat a2 = st.top();
                st.pop();
                st.push (mult (a1, a2));
            }
            else if (symb == '+')
            {
                Automat a1 = st.top();
                st.pop();
                Automat a2 = st.top();
                st.pop();
                st.push (add (a1, a2));
            }
            else if (symb == '*')
            {
                Automat a1 = st.top();
                st.pop();
                st.push (star (a1));
            }
        }
        Automat me = st.top();
        st.pop();
        start = me.start;
        final = me.final;
    } 
    else
    {
        std::cout << "ERROR" << std::endl;
    }
    
}

void Automat::goForAutomat (Vertex* vertex, const std::string &word, int &iter, int &len, int &longLen)
{
    if (iter > word.size())
    {
        return;
    }
    for (auto &curent_vertex : vertex->to)
    {
        if (word[iter] == curent_vertex.second)
        {
            len += 1;
            iter += 1;
            if (len > longLen && curent_vertex.first->final)
            {
                longLen = len;
            }
            goForAutomat(curent_vertex.first, word, iter, len, longLen);
            len -= 1;
            iter -= 1;
        }
        if (curent_vertex.second == '1')
        {
            goForAutomat(curent_vertex.first, word, iter, len, longLen);
        }

    }
}

int Automat::FindLongestPreffix (std::string word)
{
    int len = 0, iter = 0, res = 0;
    goForAutomat(start, std::move(word), iter, len, res);
    return res;
}

std::string ForTest(std::string reg, std::string word)
{
    Automat automat(reg);
    if(automat.CheckRegular(reg) == "OK")
    {
        Automat automat(reg);
        std::string answer = std::to_string(automat.FindLongestPreffix(word));
        return answer;
    }
    else
    {
        return "ERROR";
    }
}