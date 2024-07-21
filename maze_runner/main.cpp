#include <bits/stdc++.h>
#include<ctime>
#include<conio.h>
#include"menu.cpp"
#include <windows.h>
#include <mmsystem.h>
#include <chrono>

using namespace std;

// up down left right
//this for walls directions
int dx_cell[4] = {-2, +0, +0, +2};
int dy_cell[4] = {+0, +2, -2, +0};

//this for valid cells
int dx_wall[4] = {-1, +0, +0, +1};
int dy_wall[4] = {+0, +1, -1, +0};



//size of the maze(varying based on the player chosen level)
int N{0};



/*
- Acts like timer for the player to solve the maze.
- Each level has time limits.
- 30S for level 1, 60S for level 2, 100S for level 3.
*/
int seconds_limit{0};




//the 2d character array represents the maze.
char arr[41][41]{' '};
bool visited[41][41]{0};




//check if the given row and column valid or not
bool valid(int r, int c){
    return r>-1 and r<2*N+1 and c>-1 and c<2*N+1;
}


/*
- Used only 1 time.
- You can replace it in the generate maze function.
*/
//takes a random node and returns its row and column.
pair<int, int>get_row_column(int &node){
    int r = (node+N-1)/N;
    int c = node-(r-1)*N;

    return{r+r, c+c};
}




//initialize the maze with spaces.
void initialize_maze(){
    for(int i=0; i<2*N+1; i++){
        for(int j=0; j<2*N+1; j++){
            if(i%2 == 0){
                arr[i][j] = j%2? '-' : ' ';
            }else{
                arr[i][j] = j%2? ' ' : '!';
            }
        }
    }

    //The initial position of the player, mark it as 'S'
    arr[1][1] = 'S';
}




//display the maze
void print_maze(){
    for(int i=0; i<2*N+1; i++){
        for(int j=0; j<2*N+1; j++)
            cout<<arr[i][j];
        cout<<'\n';
    }
}



/*
- Here I used a randomized depth first search algorithm.
- I used the iterative way to optimize the memory usage.
*/
void generate_maze(){
    stack<pair<int, int>> st;

    srand(time(0));

    //the initial node to start the algorithm with.
    int node = rand() % (N*N) + 1;
    pair<int, int> rc = get_row_column(node);
    rc.first--, rc.second--;    //Just to be zero base.

    visited[rc.first][rc.second] = true;
    st.push(rc);

    while(!st.empty()){
        pair<int, int> curr = st.top();

        //to store the indexes of the unvisited neighbors of the current node.
        vector<int> neighbors;

        for(int i=0; i<4; i++){
            int new_r = curr.first, new_c = curr.second;
            new_r += dx_cell[i];
            new_c += dy_cell[i];

            if(valid(new_r, new_c) and !visited[new_r][new_c]){
                neighbors.push_back(i);
            }
        }

        //if the node has no unvisited valid neighbors, then pop the current node from the stack,
        //otherwise let it and push a randomly selected neighbor.
        if((int)neighbors.size() == 0){
            st.pop();
            continue;
        }


        //chose a random neighbor
        int idx = rand() % neighbors.size();
        int dir = neighbors[idx];

        int cell_r = curr.first, cell_c = curr.second;
        int wall_r = curr.first, wall_c = curr.second;

        cell_r+=dx_cell[dir], cell_c+=dy_cell[dir];
        wall_r+=dx_wall[dir], wall_c+=dy_wall[dir];



        visited[cell_r][cell_c] = true;
        arr[wall_r][wall_c] = ' ';

        pair<int, int> node = {cell_r, cell_c};
        st.push(node);
    }
}

//the initial position row and column
int s_r{1}, s_c{1};
void change_dir(char c){
    if(c == 'w' and valid(s_r-2, s_c) and arr[s_r-1][s_c] == ' '){
        arr[s_r][s_c] = ' ';
        s_r -=2;
        arr[s_r][s_c] = 'S';
    }else if(c == 's' and valid(s_r+2, s_c) and arr[s_r+1][s_c] == ' '){
        arr[s_r][s_c] = ' ';
        s_r+=2;
        arr[s_r][s_c] = 'S';
    }else if(c == 'a' and valid(s_r, s_c-2) and arr[s_r][s_c-1] == ' '){
        arr[s_r][s_c] = ' ';
        s_c-=2;
        arr[s_r][s_c] = 'S';
    }else if(c == 'd' and valid(s_r, s_c+2) and arr[s_r][s_c+1] == ' '){
        arr[s_r][s_c] = ' ';
        s_c+=2;
        arr[s_r][s_c] = 'S';
    }
}

int main(){

    Menu menu;
    menu.set_color(11);

    int choice = menu.Gmenu("1) Easy","2) Medium","3) Hard","4) Quit");
    if(choice==1){
        N = 7;
        seconds_limit = 30;
    }else if(choice==2){
        N = 15;
        seconds_limit = 60;
    }else if(choice==3){
        N = 20;
        seconds_limit = 100;
    }
    else
        return 0;
    system("cls");


    initialize_maze();
    generate_maze();

    //store the starting time(when the player start the solving the maze
    auto Start = chrono::system_clock::now();


    bool too_late{false};

    while(true){
        /*
         Get the current time, then check if the player reached the limits time or not.
        */
        auto End = chrono::system_clock::now();
        auto duration = End - Start;
        auto duration_seconds = chrono::duration_cast<chrono::seconds>(duration).count();

        if(duration_seconds > seconds_limit){
            too_late = true;
            system("cls");
            break;
        }

        print_maze();

        if(kbhit()){
            change_dir(getch());
        }

        bool game_over = (s_r == 2*N-1 and s_c == 2*N-1);

        if(game_over){
            system("cls");
            break;
        }

        SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE),{0,0});
        Sleep(400);
    }

    string soundFileName = too_late?"loseSound.wav" : "winSound.wav";
    PlaySound(TEXT(soundFileName.c_str()), NULL, SND_ASYNC);

    Sleep(5000);

    too_late ? cout<<"You Lose\n" : cout<<"You Win\n";
}
