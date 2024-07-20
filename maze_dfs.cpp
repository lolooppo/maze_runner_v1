#include <bits/stdc++.h>

using namespace std;

// up down left right
int dx_cell[4] = {-2, +0, +0, +2};
int dy_cell[4] = {+0, +2, -2, +0};

int dx_wall[4] = {-1, +0, +0, +1};
int dy_wall[4] = {+0, +1, -1, +0};

int n;
char arr[41][41]{' '};
bool visited[41][41]{0};

bool valid(int r, int c){
    return r>-1 and r<2*n+1 and c>-1 and c<2*n+1;
}

pair<int, int>get_row_column(int &node){
    int r = (node+n-1)/n;
    int c = node-(r-1)*n;

    return{r+r, c+c};
}

void initialize_maze(){
    for(int i=0; i<2*n+1; i++){
        for(int j=0; j<2*n+1; j++){
            if(i%2 == 0){
                arr[i][j] = j%2? '-' : ' ';
            }else{
                arr[i][j] = j%2? ' ' : '!';
            }
        }
    }
}

void print_maze(){
    for(int i=0; i<2*n+1; i++){
        for(int j=0; j<2*n+1; j++)
            cout<<arr[i][j];
        cout<<'\n';
    }
}

void generate_maze(){
    stack<pair<int, int>> st;

    srand(time(0));

    int node = rand() % (n*n) + 1;
    pair<int, int> rc = get_row_column(node);
    rc.first--, rc.second--;

    visited[rc.first][rc.second] = true;
    st.push(rc);

    while(!st.empty()){
        pair<int, int> curr = st.top();

        vector<int> neighbours;
        for(int i=0; i<4; i++){
            int new_r = curr.first, new_c = curr.second;
            new_r += dx_cell[i];
            new_c += dy_cell[i];

            if(valid(new_r, new_c) and !visited[new_r][new_c]){
                neighbours.push_back(i);
            }
        }

        if((int)neighbours.size() == 0){
            st.pop();
            continue;
        }

        //st.push(curr);
        int idx = rand() % neighbours.size();
        int dir = neighbours[idx];

        int cell_r = curr.first, cell_c = curr.second;
        int wall_r = curr.first, wall_c = curr.second;

        cell_r+=dx_cell[dir], cell_c+=dy_cell[dir];
        wall_r+=dx_wall[dir], wall_c+=dy_wall[dir];



        visited[cell_r][cell_c] = true;
        arr[wall_r][wall_c] = ' ';

        pair<int, int> node = {cell_r, cell_c};
        st.push(curr), st.push(node);
    }
}

int main(){
    cin>>n;

    initialize_maze();
    generate_maze();
    print_maze();
}