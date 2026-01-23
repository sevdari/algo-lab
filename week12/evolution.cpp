#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>
using namespace std;

void testcase() {
    int n, q; cin >> n >> q;
    
    const int LOG = 17;
    vector<string> names(n);
    vector<int> age(n), parent(n, -1);
    unordered_map<string, int> idx;
    
    for(int i = 0; i < n; i++){
        cin >> names[i] >> age[i];
        idx[names[i]] = i;
    }
    
    string c, p;
    for(int i = 0; i < n - 1; i++){
        cin >> c >> p;
        parent[idx[c]] = idx[p];
    }
    
    // Binary lifting: lift[i][j] = 2^j-th ancestor of node i
    vector<vector<int>> lift(n, vector<int>(LOG, -1));
    for(int i = 0; i < n; i++) 
        lift[i][0] = parent[i];
    for(int j = 1; j < LOG; j++)
        for(int i = 0; i < n; i++)
            if(lift[i][j-1] != -1)
                lift[i][j] = lift[lift[i][j-1]][j-1];
    
    // Queries
    string qname; int qage;
    for(int i = 0; i < q; i++){
        cin >> qname >> qage;
        int curr = idx[qname];
        for(int j = LOG - 1; j >= 0; j--){
            int anc = lift[curr][j];
            if(anc != -1 && age[anc] <= qage)
                curr = anc;
        }
        cout << names[curr] << " ";
    }
    cout << "\n";
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    int t; cin >> t;
    while(t--) testcase();
}