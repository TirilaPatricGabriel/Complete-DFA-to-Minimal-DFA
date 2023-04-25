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
    int numarStari;
    vector<int> stariFinale;
    vector<string> stariFinaleNoi;
public:
    Rezolvare();

    void start();
    void afisare();
    void creareSeturi();
    bool verificareAsemanare(int, int);
    void cautaStariAccesibile(vector<int>&, int, unordered_map<int, bool>);
};
void Rezolvare::cautaStariAccesibile(vector<int>& accessible, int node, unordered_map<int, bool> seen) {
    accessible.push_back(node);
    for(auto [key, value] : Map[node]){
        if(seen.find(value) == seen.end() || seen[value] == false){
            seen[value] = true;
            this->cautaStariAccesibile(accessible, value, seen);
            seen[value] = false;
        }
    }
}
Rezolvare::Rezolvare(){
    this->sets = {};
    this->newSets = {};
    this->alfabet = "";
    this->Map = {};
    this->numarStari = 0;
    this->stariFinale = {};
}
void Rezolvare::start(){
    ifstream f("C:\\Users\\tiril\\CLionProjects\\LFATema2\\input.txt");
    int numarLitere, stare;
    char litera;
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


    vector<int> accessible = {};
    unordered_map<int, bool> seen = {};
    seen[0] = true;
    this->cautaStariAccesibile(accessible, 0, seen);


    // stergere stari inaccesibile
    for(int i=0; i<numarStari; i++){
        if(find(accessible.begin(), accessible.end(), i) == accessible.end()){
            Map.erase(i);
        }
    }

    // despartim starile finale de cele nefinale
    for(auto& [key, value] : Map){
        if(find(stariFinale.begin(), stariFinale.end(), key) == stariFinale.end()){
            sets[key] = 0;
        }
        else sets[key] = 1;
    }

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
        int j, ok = 0;
        for(j=0; j<vectorSet[i].size(); j++){
            stareNoua = stareNoua + to_string(vectorSet[i][j]);
            if(find(this->stariFinale.begin(), this->stariFinale.end(), vectorSet[i][j]) != this->stariFinale.end()){
                ok = 1;
            }
        }
        if(ok) this->stariFinaleNoi.push_back(stareNoua);
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
    // ---
    for(auto& [key, val] : Map2){
        if(find(stariFinaleNoi.begin(), stariFinaleNoi.end(), key) != stariFinaleNoi.end()){
            cout<<"Starea "<<key<<"(finala): "<<endl;
        } else {
            cout<<"Starea "<<key;
            if (key.find('0') != string::npos)
                cout<<"(initiala):"<<endl;
            else
                cout<<":"<<endl;
        }
        for(auto& [key2, val2] : val){
            cout<<key2<<": "<<val2<<endl;
        }
    }
}

bool Rezolvare::verificareAsemanare(int stare1, int stare2){
    int k = 0;
    if(find(this->stariFinale.begin(), this->stariFinale.end(), stare1) !=  find(this->stariFinale.begin(), this->stariFinale.end(), stare2))
    {
        return false;
    }
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
        while(Map.find(nextKey) == Map.end() && nextKey < Map.size()){
            nextKey+=1;
        }
        if (this->Map.find(nextKey) != this->Map.end()) {
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
// ---

int main() {
    Rezolvare res;
    res.start();

    return 0;
}