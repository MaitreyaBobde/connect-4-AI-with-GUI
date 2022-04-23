#ifndef connect4_h_
#define connect4_h_

#include<vector>

class connect4 {
public:
    char comp='x',opp='o',empty = ' ',board[6][7];
    int board_row=6,board_col=7;
    bool terminate = false;
    int get_empty(int col) {
        int i=5;
        for(;i>-1;i--) {
            if(board[i][col]==empty) break;
        }
        return i;
    }

    // void print() {
    //     for(int i=0;i<board_row;i++) {
    //         for(int j=0;j<board_col;j++){
    //             cout<<board[i][j];
    //             if(j!=board_col-1) cout<<" | ";
    //         }
    //         cout<<endl;
    //     }
    //     for(int i=0;i<board_col;i++)
    //         cout<<i+1<<"   ";
    //     cout<<endl;
    // }

    int evaluate_window(std::vector<char>& win) {
        int count_comp=0,count_opp=0,count_empty=0;
        for(char& x:win) {
            if(x==comp) count_comp++;
            if(x==opp) count_opp++;
            if(x==empty) count_empty++;
        }
        if(count_comp==4) return 1000;
        if(count_comp==3 && count_empty==1) return 50;
        if(count_comp==2 && count_empty==2) return 20;
        if(count_opp==3 && count_empty==1) return -40;
        if(count_opp==4) return -1000;
        return 0;
    }

    int evaluate() {
        int score = 0;
        //check middle column
        for(int row=0;row<board_row-3;row++) {
            std::vector<char> window;
            for(int i=0;i<4;i++) window.push_back(board[row+i][board_col/2]);
            int temp = evaluate_window(window);
            window.clear();
            if(temp==1000 || temp==-1000) return temp;
            else score+=temp;
        }

        //check horizontal rows for win
        for(int row=0;row<board_row;row++) {
            for(int col=0;col<board_col-3;col++) {
                std::vector<char> window;
                for(int i=0;i<4;i++) window.push_back(board[row][col+i]);
                int temp = evaluate_window(window);
                window.clear();
                if(temp==1000 || temp==-1000) return temp;
                else score+=temp;
            }
        }
        //check vertical cols for win
        for(int col=0;col<board_col;col++) {
            for(int row=0;row<board_row-3;row++) {
                std::vector<char> window;
                for(int i=0;i<4;i++) window.push_back(board[row+i][col]);
                int temp = evaluate_window(window);
                window.clear();
                if(temp==1000 || temp==-1000) return temp;
                else score+=temp;
            }
        }
        //check positively sloped diagonals
        for(int col=0;col<board_col-3;col++) {
            for(int row=0;row<board_row-3;row++) {
                std::vector<char> window;
                for(int i=0;i<4;i++) window.push_back(board[row+i][col+i]);
                int temp = evaluate_window(window);
                window.clear();
                if(temp==1000 || temp==-1000) return temp;
                else score+=temp;
            }
        }
        //check negatively sloped diagonals
        for(int col=0;col<board_col-3;col++) {
            for(int row=3;row<board_row;row++) {
                std::vector<char> window;
                for(int i=0;i<4;i++) window.push_back(board[row-i][col+i]);
                int temp = evaluate_window(window);
                window.clear();
                if(temp==1000 || temp==-1000) return temp;
                else score+=temp;
            }
        }
        return score;
    }

    bool is_full() {
        for(int i=0;i<board_col;i++)
            if(board[0][i] == empty) return false;
        return true;
    }

    int minimax(int depth,int alpha, int beta, bool isMax) {
        int score = evaluate();
        if(depth == 0 || is_full() || score==1000 || score==-1000)
            if(score == 0) return score;
            else return score>0 ? score+depth : score-depth;

        if(isMax) {
            int best = -10000;
            for(int col=0;col<board_col;col++){
                int row = get_empty(col);
                if(row!=-1) {
                    board[row][col] = comp;
                    best = std::max(best,minimax(depth-1,alpha,beta,!isMax));
                    board[row][col] = empty;
                    alpha = std::max(alpha,best);
                    if(beta<=alpha) break;
                }
            }
            return best;
        }
        else {
            int best = 10000;
            for(int col=0;col<board_col;col++){
                int row = get_empty(col);
                if(row!=-1) {
                    board[row][col] = opp;
                    best = std::min(best,minimax(depth-1,alpha,beta,!isMax));
                    board[row][col] = empty;
                    beta = std::min(beta,best);
                    if(beta<=alpha) break;
                }
            }
            return best;
        }
    }

    int find_best_move() {
        int bestVal = -1000,col,depth=5;

        for (int i=0;i<board_col;i++) {
            int row = get_empty(i);
            if(row!=-1) {
                board[row][i] = comp;
                int moveVal = minimax(depth,-10000,10000,false);
                board[row][i] = empty;

                // cout<<"col: "<<i<<" val: "<<moveVal<<endl;
                if(moveVal > bestVal) {
                    col = i;
                    bestVal = moveVal;
                }
            }
        }

        return col;
    }

    void is_terminate() {
        int temp = evaluate();
        if(temp==1000||temp==-1000||is_full()) terminate = true;
    }

    connect4() {
        std::fill_n(&board[0][0],sizeof(board)/sizeof(**board),empty);
        // cclear
        // while(1){
        //     cout<<"choose between (x,o): "; cin>>opp;
        //     if(opp=='x'||opp=='X') {opp='x';comp='o';break;}
        //     else if(opp=='o'||opp=='O') {opp='o';comp='x';break;}
        //     else {cclear cout<<"please input x or o only."<<endl;}
        // }
        // cclear
        // while(1){
        //     char temp;
        //     cout<<"do you want to play first? [y/n]: "; cin>>temp;
        //     if(temp=='y'||temp=='Y') {cclear break;}
        //     else if(temp=='n'||temp=='N') {
        //         board[board_row-1][board_col/2] = comp;
        //         cclear cout<<"computer played in 4th column\n"<<endl;
        //         break;
        //     }
        //     else {cclear cout<<"please input y or n only."<<endl;}
        // }
        // while(!is_terminate()) {
        //     int col;
        //     print();
        //     cout<<"\nwhich column do you want to play (1-7): ";
        //     cin>>col;
        //     if(col>7||col<1){
        //         cclear cout<<"enter number between 1 to 7 only.\n"<<endl;
        //         continue;
        //     }
        //     int row = get_empty(--col);
        //     if(row == -1){
        //         cclear cout<<"you can't play there.\n"<<endl;
        //         continue;
        //     }
        //     board[row][col] = opp;
        //     if(is_terminate()) break;
            
        //     int comp_col = find_best_move();
        //     cclear
        //     board[get_empty(comp_col)][comp_col++] = comp;
        //     cout<<"computer played in "<<comp_col;
        //     switch (comp_col){
        //         case 1:
        //             cout<<"st";
        //             break;
        //         case 2:
        //             cout<<"nd";
        //             break;
        //         case 3:
        //             cout<<"rd";
        //             break;
        //         default:
        //             cout<<"th";
        //             break;
        //     }
        //     cout<<" column\n"<<endl;
        // }
        // print();
        // int temp = evaluate();
        // if(temp == -1000) cout<<"\nYOU WIN!!!"<<endl;
        // else if(temp == 1000) cout<<"\nComputer WINS!!!"<<endl;
        // else cout<<"\nits a DRAW!!!"<<endl;
        // comp = 'x';opp = 'o';
        // char inp[6][7] = {
        //     {' ',' ',' ',' ',' ',' ',' '},
        //     {' ',' ',' ',' ',' ',' ',' '},
        //     {' ',' ',' ',' ',' ',' ',' '},
        //     {' ',' ',' ',' ',' ',' ',' '},
        //     {' ',' ',' ',' ',' ',' ',' '},
        //     {' ',' ',' ',' ',' ',' ',' '},
        // };
        // for(int i=0;i<board_row;i++)
        //     for(int j=0;j<board_col;j++)
        //         board[i][j] = inp[i][j];
        // int col = find_best_move()+1;
        // cout<<" col : "<<col<<endl;
    }
    
    ~connect4();

};

#endif