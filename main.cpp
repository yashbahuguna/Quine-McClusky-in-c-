#include<iostream>
#include<vector>
#include<cstring>

void showVector(const std::vector<std::pair<int,std::string>> &vec, const int &n, std::vector<std::vector<int>>&minterms) {
    int i=0;
    std::cout<<"------------------------"<<std::endl;
   for (auto& it : vec) {
        for (auto& it1 : minterms[i]) {
             std::cout<<it1<<",";
        }
        std::cout<<"->"<<it.second;
        std::cout<<"->"<<it.first;
        std::cout<<std::endl;
        i++;
    }
    std::cout<<"------------------------"<<std::endl;
}


void showExpression (std::vector<std::pair<int,std::string>> vec, int n) {
    char c;
    int i;
    std::string s;
    if (n <= 26) {

         for (auto& it : vec) {
            c = 'A';
            if (it.first == 1) {

                for (i = 0; i < n; i++) {
                    if(it.second[i] == '1') {
                        s.push_back(c);
                    }
                    else if(it.second[i] == '0') {
                        s.push_back(c);
                        s = s+"\'";
                    }
                    c++;
                }

                s = s + " + ";
            }
        }
    }
    else {
        for (auto& it : vec) {
            s = s + it.second + " + ";
        }
    }
    s.pop_back();
    s.pop_back();
    s.pop_back();
    std::cout<<"simplified expression: "<<s<<std::endl;
}


int compare(std::string s1, std::string s2) {
    if (!strcmp(s1.c_str(),s2.c_str())) return -2;
    int flag=false, l = s1.length(),pos;

    for (int i = 0;i < l; i++) {
        if(s1[i] != s2[i]&&flag == false) {
            flag = true;
            pos = i;
        }
        else if (s1[i] != s2[i]) {
            flag = false;
            break;
        }
    }
    if (flag) return pos;
    else return -1;
}



std::string reduce(std::string s, const int &n) {
    s[n] = '*';
    return s;
}

std::vector<std::pair <int, std::string>> makeVector (std::vector<int> a, const int &n, std::vector<std::vector<int>> &minterms) {
    std::string s;
    int i,j,l,c,m = a.size();
    char res[100];
    std::vector<std::pair <int,std::string>> vec;
    minterms.resize(m);
    for (i = 0; i<m; i++) {
        itoa(a[i],res,2);
        s = res;
        l = s.length();
        c = 0;
        for (j = 0; j< l ; j++) {
            if (s[j] == '1') c++;
        }
        for (j = 0; j < n-l; j++) {
            s = "0" + s;
        }

        vec.emplace_back(c,s);
        minterms[i].push_back(a[i]);
    }

    return vec;
}


void simplify(std::vector<std::pair<int,std::string>> vec, std::vector<std::vector<int>> minterms1, const int &n, std::vector<std::pair<int,std::string>> &result, std::vector<std::vector<int>> &minterms) {
    bool stop = true;
    int i,j,k=0,r, m = vec.size(), l = 0;
    std::vector<bool> tick(m,false);
    std::vector<std::pair<int,std::string>> vec1;
    std::vector<std::vector<int>> minterms1_copy;

    for (i = 0; i < m; i++) {
        for (j = i+1; j < m; j++) {
            if ((vec[i].first+1 == vec[j].first)||(vec[j].first+1 == vec[i].first)||(vec[j].first == vec[i].first)) {

                r = compare(vec[i].second,vec[j].second);
                if (r == -2) {
                    tick[j] = true;
                }
                else if (r!=-1) {
                    k = vec[i].second[r] != '1' ? vec[i].first:(vec[i].first-1);
                    vec1.emplace_back(k, reduce(vec[i].second, r) );

                    minterms1_copy.push_back(minterms1[i]);

                    for (auto& it : minterms1[j]) {
                         minterms1_copy[l].push_back(it);
                    }
                    tick[i] = true;
                    tick[j] = true;
                    stop = false;
                    l++;
                }
            }
        }
        if (!tick[i]) {
            showVector(vec1,  n, minterms1);
            result.emplace_back(vec[i].first, vec[i].second);
            minterms.push_back(minterms1[i]);
        }
    }
    if (stop) {
        return;
    }
    else{
        simplify(vec1, minterms1_copy, n, result, minterms);
    }
}

void sort (std::vector<std::pair<int,std::string>> &vec, std::vector<std::vector<int>> &minterms, const std::vector<int>& dontcares) {
    int c, n = vec.size(), max, temp, i, j;
    std::vector<int> count;
    std::string temp1;
    std::vector<int> temp2;
    for (auto& it : minterms) {
        c = 0;
        i = 1;
        for (auto& it1: it) {
            for (auto& it2 : dontcares) {
                if (it2 == it1) {
                    it1 = -1*i;
                    c++;
                    i++;
                    break;
                }
            }
        }
       count.push_back(c);
    }


    for (i = 0; i < n; i++) {
        max = i;
        for (j = i+1; j < n; j++) {
            if (count[j] > count[max])  {
                max = j;
            }
        }

        if (max != i) {
            temp = count[i];
            count[i] = count[max];
            count[max] = temp;

            temp1 = vec[i].second;
            vec[i].second = vec[max].second;
            vec[max].second = temp1;

            temp2 = minterms[i];
            minterms[i] = minterms[max];
            minterms[max] = temp2;
        }
    }
    i = 0;
    for (auto& it : vec) {
        it.first = count[i];
        i++;
    }

}

void essential(std::vector<std::pair<int,std::string>> &vec, std::vector<std::vector<int>> &minterms, const std::vector<int> &dontcares ) {
    int i, j, m = vec.size(), n,c;
    std::vector<std::pair<int,std::vector<int>>> vec1;
    std::vector<std::vector<int>> minterms1 = minterms;


    for(i = 0; i < m; i++) {
        n = minterms1[i].size();
        j = 0;
        c = vec[i].first;
        vec[i].first = 1;
        for (j = m-1; j > i; j--) {
            for (auto& it : minterms1[i]) {
                if (it >= 0) {
                    for (auto& it1: minterms[j]) {
                        if (it == it1) {
                            c++;
                            it = -1;
                            break;
                        }
                    }
                }
                if (c == n) {
                    vec[i].first = 0;
                    break;
                }
            }
            if (c==n) break;
        }
        if (c < n) {
            for (j = 0; j < i; j++) {
                for (auto& it : minterms1[i]) {
                    if (vec[j].first == 1 && it >=0) {
                        for (auto &it1: minterms[j]) {
                            if (it == it1) {
                                c++;
                                it = -1;
                                break;
                            }
                        }
                    }
                    if (c == n) {
                        vec[i].first = 0;
                        break;
                    }
                }
                if (c==n) break;
            }
        }
    }
}

void inputMinterms(std::vector<int> &input, std::vector<int>  &dontcares, const int &n) {
    int m,t;
    std::cout<<"enter number of minterms:";
    std::cin>>m;
    std::cout<<"enter minterms:"<<std::endl;
    for (int i =0 ; i < m; i++) {
        std::cin>>t;
        input.push_back(t);
    }

    std::cout<<"enter number of dontcare positions:";
    std::cin>>m;
    std::cout<<"enter dontcares:"<<std::endl;
    for (int i =0 ; i < m; i++) {
        std::cin>>t;
        dontcares.push_back(t);
        input.push_back(t);
    }
}


void inputTruthTable(std::vector<int> &input, std::vector<int>  &dontcares, const int &n) {
    int l;
    unsigned int m = 1;
    char c;
    bool flag;
    m = m<<n;
    std::string s;
    std::cout<<"enter 0 or 1 or x(for dont care condition)"<<std::endl;
    char res[100];
    flag = false;
    for (int i = 0; i < m; i++) {
        s = "";
        itoa(i,res,2);
        s = res;
        l = s.length();
        for (int j = 0; j < n-l; j++) {
            std::cout <<0<<" ";
        }
        for (int j = 0; j < l; j++) {
            std::cout <<s[j]<<" ";
        }
        std::cin>>c;
        if (c == '1') {
            input.push_back(i);
        }
        else if(c=='x') {
            dontcares.push_back(i);
            input.push_back(i);
        }
    }
}





int main() {
    int n;
    char c;
    std::string str;
    std::cout<<"enter number of variables:";
    std::cin>>n;
    std::vector<std::vector<int>> minterms, minterms1;
    std::vector<int> input,dontcares;
    std::vector<std::pair <int,std::string>> vec, result;
    do {
        std::cout<<"Enter 1 to input truth table \n 2 to input via minterms \n y to exit\n";
        std::cin>>c;
        switch(c) {
            case '1': inputTruthTable(input, dontcares, n);break;
            case '2': inputMinterms(input, dontcares, n);break;
            case 'y':break;
            default:std::cout<<"invalid choice enter again";
        }
        if (input.size() == 0) {
            std::cout<<"falacy";
        }
        else if(input.size() == (1<<(n+1))) {
            std::cout<<"Tautology";
        }
        else if (c == '1'|| c=='2') {
            vec = makeVector(input, n, minterms1);
            simplify(vec, minterms1, n, result, minterms);
            sort(result, minterms, dontcares);
            essential(result, minterms, dontcares);
            showExpression(result, n);
        }
    }while(c!='y');

}




