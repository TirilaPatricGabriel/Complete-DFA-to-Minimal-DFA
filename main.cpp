#include <iostream>
#include <unordered_map>
#include <vector>
#include <fstream>
#include <algorithm>
#include <string>


using namespace std;

class Rezolvare{
    unordered_map<int, int> sets, newSets;
    string alfabet;
    unordered_map<int, unordered_map<char, int>> Map;
    int numarStari, nSets;
public:
    Rezolvare();

    vector<vector<int>> func(vector<vector<int>>, vector<vector<int>>);
    void start();
    void afisare();
    void creareSeturi();
    bool verificareAsemanare(int, int);
};
Rezolvare::Rezolvare(){
    this->sets = {};
    this->newSets = {};
    this->alfabet = "";
    this->Map = {};
    this->numarStari = 0;
    this->nSets = 0;
}
void Rezolvare::start(){
    ifstream f("C:\\Users\\Patric\\CLionProjects\\DFAToMinimal\\input.txt");
    int numarLitere, stare;
    char litera;
    vector<int> stariFinale;
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

    while(true){
        bool end = 1;
        vector<int> v = {};
        for(auto [key, value] : Map){
            for(auto [key2, value2] : value){
                v.push_back(value2);
            }
        }
        for(int i=0; i<numarStari; i++){
            if(find(v.begin(), v.end(), i) == v.end()) {
                Map.erase(i);
                end = 0;
            }
        }
        if(end) break;
    }

    /* ============================================================================================== */

    // despartim starile finale de cele nefinale
    for(auto& [key, value] : Map){
        if(find(stariFinale.begin(), stariFinale.end(), key) == stariFinale.end()){
            sets[key] = 0;
        }
        else sets[key] = 1;
    }

    nSets = 2;
    while(true){
        unordered_map<int, int> copy;

        for(auto [key, value] : sets){
            copy[key] = value;
        }

        this->creareSeturi();
        sets.clear();

        bool dif = 0;
        for(auto [key, value] : this->newSets){
            sets[key] = newSets[key];
            if(this->newSets[key] != copy[key]) {
                dif = 1;
            }
        }

        if(!dif){
            break;
        }
        newSets.clear();
    }
    this->afisare();
}
void Rezolvare::afisare(){
    unordered_map<string, unordered_map<char, string>> Map2;
    
    vector<vector<int>> vectorSet = {};
    
    for(auto [key, value] : this->sets){
        vector<int> asemanatoare = {};
        for(auto [key2, value2] : this->sets){
            if(value == value2) asemanatoare.push_back(key2);
        }
        if(find(vectorSet.begin(), vectorSet.end(), asemanatoare) == vectorSet.end())
            vectorSet.push_back(asemanatoare);
    }
    
    for(int i=0; i<vectorSet.size(); i++){
        sort(vectorSet[i].begin(), vectorSet[i].end());
        string stareNoua = "";
        int j;
        for(j=0; j<vectorSet[i].size(); j++){
            stareNoua = stareNoua + to_string(vectorSet[i][j]);
        }
        for(auto& [tranzitie, stareAjungere] : Map[vectorSet[i][j-1]]) {
            string stareNoua2 = "";
            for(int k=0; k<vectorSet.size(); k++){
                if(find(vectorSet[k].begin(), vectorSet[k].end(), stareAjungere) != vectorSet[k].end())
                    for(int l=0; l<vectorSet[k].size(); l++){
                        stareNoua2 = stareNoua2 + to_string(vectorSet[k][l]);
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
}

bool Rezolvare::verificareAsemanare(int stare1, int stare2){
    int k = 0;
    for(auto [key, value] : this->Map[stare1]){
        if(value == this->Map[stare2][key] || this->sets[value] == this->sets[this->Map[stare2][key]]) // daca duc in aceeasi stare sau starile in care duc se afla in acelasi set
            k++;
    }
    if(k == this->Map[stare1].size()) return true;
    return false;
}

void Rezolvare::creareSeturi(){
    for(auto [key, value] : this->sets){
        int nextKey = key+1;
        if (nextKey < this->numarStari) {
            if (this->verificareAsemanare(key, nextKey)) {
                if (this->newSets.find(key) != this->newSets.end()) {
                    this->newSets[nextKey] = this->newSets[key];
                } else {
                    this->newSets[key] = this->newSets.size();
                    this->newSets[nextKey] = this->newSets[key];
                }
            } else {
                if (this->newSets.find(key) == this->newSets.end()) {
                    this->newSets[key] = 0;
                    this->newSets[nextKey] = 1;
                } else {
                    bool asemanare = 0;
                    for (auto [key2, value2]: this->newSets) {
                        if (this->sets[nextKey] == this->sets[key2]) {
                            if (this->verificareAsemanare(nextKey, key2)) {
                                this->newSets[nextKey] = value2;
                                asemanare = 1;
                                break;
                            }
                        }
                    }
                    if (!asemanare) {
                        this->newSets[nextKey] = this->newSets.size();
                    }
                }
            }
        }
    }
}

int main() {
    Rezolvare res;
    res.start();

    return 0;
}













































//            int k = 0;
//            for(auto [key2, value2] : this->Map[key]){
//                if(this->sets[value2] == this->sets[Map[nextKey][key2]]) {
//                    k+=1;
//                } else {
//                    if(this->newSets.size() == 0){
//                        this->newSets[key] = 0;
//                        this->newSets[nextKey] = 1;
//                    } else {
//                        for(auto [key3, value3] : this->newSets){
//
//                        }
//                    }
//                    break;
//                }
//            }  // cazul in care sunt egale pentru toate literele din alfabet

















//for(int i=0; i<sets.size(); i++) {
//for (int j = 0; j < sets[i].size(); j++) {
//if (newSets.size() == 0) {
//newSets.push_back({sets[i][j]});
//} else {
//bool adaugat = 0;
//for (int k = 0; k < newSets.size(); k++) {
//if(newSets[k].size() > 0){
//bool ok = 1;
//for(auto& [key, value] : Map[sets[i][j]]){
//if(Map[newSets[k][0]][key] == value) continue;
//else {
//// daca sunt diferite, atunci trebuie sa se afle in acelasi set din sets
//for(int l=0; l<sets.size(); l++){
//if((find(sets[l].begin(), sets[l].end(), Map[newSets[k][0]][key]) != sets[l].end() &&
//find(sets[l].begin(), sets[l].end(), value) == sets[l].end()) ||
//(find(sets[l].begin(), sets[l].end(), Map[newSets[k][0]][key]) == sets[l].end() &&
//find(sets[l].begin(), sets[l].end(), value) != sets[l].end())) {
//ok = 0;
//break;
//}
//}
//}
//if(ok == 0) break;
//}
//if(ok == 1){
//int q, p;
//for(int m=0; m<sets.size(); m++){
//for(int n=0; n<sets[m].size(); n++){
//if(sets[m][n] == sets[i][j]) q = m;
//if(sets[m][n] == newSets[k][0]) p = m;
//}
//}
//if(q==p) {
//newSets[k].push_back(sets[i][j]);
//adaugat = 1;
//break;
//}
//}
//}
//}
//if(adaugat == 0) newSets.push_back({sets[i][j]});
//}
//}
//}
//return newSets;