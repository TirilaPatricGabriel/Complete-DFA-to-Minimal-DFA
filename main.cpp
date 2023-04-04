#include <iostream>
#include <unordered_map>
#include <vector>
#include <fstream>
#include <algorithm>
#include <string>


using namespace std;
vector<vector<string>> M = {};
unordered_map<int, unordered_map<char, int>> Map;
vector<int> stariFinale;
string alfabet;
bool endIt = 0;
int verificareAsemanare;
vector<string> cuvinte = {""};
int nrSchimbari = 0;
int maximSchimbari = 0;
bool scrie = 0;


vector<vector<int>> func(vector<vector<int>> sets, vector<vector<int>> newSets){
    for(int i=0; i<sets.size(); i++) {
        for (int j = 0; j < sets[i].size(); j++) {
            if (newSets.size() == 0) {
                newSets.push_back({sets[i][j]});
            } else {
                bool adaugat = 0;
                for (int k = 0; k < newSets.size(); k++) {
                    if(newSets[k].size() > 0){
                        // verific sets[i][j] impreuna cu newSets[k][0]
//                        cout<<"VERIFICARE: "<<sets[i][j]<<" "<<newSets[k][0]<<endl;
                        bool ok = 1;
                        for(auto& [key, value] : Map[sets[i][j]]){
                            if(Map[newSets[k][0]][key] == value) continue;
                            else {
                                // daca sunt diferite, atunci trebuie sa se afle in acelasi set din sets
                                for(int l=0; l<sets.size(); l++){
                                    if((find(sets[l].begin(), sets[l].end(), Map[newSets[k][0]][key]) != sets[l].end() &&
                                    find(sets[l].begin(), sets[l].end(), value) == sets[l].end()) ||
                                    (find(sets[l].begin(), sets[l].end(), Map[newSets[k][0]][key]) == sets[l].end() &&
                                    find(sets[l].begin(), sets[l].end(), value) != sets[l].end())) {
                                        ok = 0;
                                        break;
                                    }
                                }
                            }
                            if(ok == 0) break;
                        }
                        if(ok == 1){
                            int q, p;
                            for(int m=0; m<sets.size(); m++){
                                for(int n=0; n<sets[m].size(); n++){
                                    if(sets[m][n] == sets[i][j]) q = m;
                                    if(sets[m][n] == newSets[k][0]) p = m;
                                }
                            }
                            if(q==p) {
                                newSets[k].push_back(sets[i][j]);
                                adaugat = 1;
                                break;
                            }
                        }
                    }
                }
                if(adaugat == 0) newSets.push_back({sets[i][j]});
            }

//            cout<<"SETS: "<<endl;
//            for(int i=0; i<newSets.size(); i++){
//                for(int j=0; j<newSets[i].size(); j++){
//                    cout<<newSets[i][j]<<" ";
//                }
//                cout<<endl;
//            }
        }
    }
    return newSets;
}

int main() {
    ifstream f("C:\\Users\\tiril\\CLionProjects\\untitled6\\input.txt");
    int numarStari, numarLitere, stare;
    char litera;
    std::string targetString;
    f>>numarStari;

    bool stareFinala;

    for(int i=0; i<numarStari; i++){
        f>>stareFinala;
        if(stareFinala == 1){
            stariFinale.push_back(i);
        }
        f>>numarLitere;
        for(int j=0; j<numarLitere; j++){
            f>>litera;
            f>>stare;
            if(Map.find(i) == Map.end()){
                Map[i][litera] = stare;
            }
            else {
                if(Map[i].find(litera) == Map[i].end()){
                    Map[i][litera] = stare;
                }
                else{
                    Map[i][litera] = stare;
                }
            }
        }
    }

    for(auto& c : Map[0]){
        alfabet += c.first;
    }


    for(int i=0; i<Map.size(); i++){
        M.push_back({});
        for(int j=0; j<Map.size(); j++){
            M[i].push_back("");
        }
    }

    /* ============================================================================================== */
    for(int i=0; i<stariFinale.size(); i++) {
        for (int j = 0; j < stariFinale[i]; j++) {
            if (std::find(stariFinale.begin(), stariFinale.end(), j) == stariFinale.end()) {
                M[stariFinale[i]][j] = "-1";
            }
        }
    }


    vector<vector<int>> sets = {}, newSets = {};


    sets.push_back({});
    sets.push_back({});
    sets[1] = stariFinale;
    for(auto& [key, value] : Map){
        if(find(stariFinale.begin(), stariFinale.end(), key) == stariFinale.end()){
            sets[0].push_back(key);
        }
    }

//    cout<<"SETS: "<<endl;
//    for(int i=0; i<sets.size(); i++){
//        for(int j=0; j<sets[i].size(); j++){
//            cout<<sets[i][j]<<" ";
//        }
//        cout<<endl;
//    }

    while(true){
        vector<vector<int>> copy = sets;
        sets = func(sets, newSets);
//        cout<<"SETS: "<<endl;
//        for(int i=0; i<sets.size(); i++){
//            for(int j=0; j<sets[i].size(); j++){
//                cout<<sets[i][j]<<" ";
//            }
//            cout<<endl;
//        }
        if(copy == sets){
            break;
        }
        newSets.clear();
    }
    unordered_map<string, unordered_map<char, string>> Map2;

    for(int i=0; i<sets.size(); i++){
        sort(sets[i].begin(), sets[i].end());
        string stareNoua = "";
        int j;
        for(j=0; j<sets[i].size(); j++){
            stareNoua = stareNoua + to_string(sets[i][j]);
        }
        for(auto& [tranzitie, stareAjungere] : Map[sets[i][j-1]]) {
            string stareNoua2 = "";
            for(int k=0; k<sets.size(); k++){
                if(find(sets[k].begin(), sets[k].end(), stareAjungere) != sets[k].end())
                    for(int l=0; l<sets[k].size(); l++){
                        stareNoua2 = stareNoua2 + to_string(sets[k][l]);
                    }
            }
            Map2[stareNoua][tranzitie] = stareNoua2;
        }
    }
    for(auto& [key, val] : Map2){
        cout<<"Starea "<<key<<": "<<endl;
        for(auto& [key2, val2] : val){
            cout<<key2<<": "<<val2<<endl;
        }
    }

    return 0;
}
