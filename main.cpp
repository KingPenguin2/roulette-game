#include <iostream>
#include <vector>
#include <algorithm>
#include <chrono>
#include <thread>

using namespace std;

class Player{
    public:
        Player(int x, int y){
            money = x;
            throws = y;
        }
        void setMoney(int newMoney) {money = newMoney;}
        void incrementThrows()  {throws++;}
        int get_money() {return money;}
        int get_throws() {return throws;}

        float winnings_per_throw(){return (money-1000)/throws ;}

        void placeBet(int amountBet){
            money -= amountBet;
        }
        void add_winnings(int amountWon){
            money += amountWon;
        }

    private:
        int money;
        int throws;
};

class Table{
    public:
        Table(){
            
        }
        int spin_wheel(){
            //Spins the "wheel and returns number between 0 and 36"
            return rand()%37 ;
        }
        int calculate_winnings(int code, int number, int amountBet){
            if (code<= 36 && number == code){
                return amountBet * payouts[2];
            }
            else if (code == 37 && number%2 == 1){
                return amountBet * payouts[0];
            }
            else if (code == 38 && number%2 == 0){
                return amountBet * payouts[0];
            }
            else if (code == 39){
                if (binary_search(reds.begin(),reds.end(), number)) {return amountBet * payouts[0];}
                else { return 0;}
            }
            else if (code == 40){
                if (binary_search(blacks.begin(),blacks.end(), number)) {return amountBet * payouts[0];}
                else { return 0;}
            }
            else{
                // Loss
                return 0;
            }
        }

    private:
        vector<int> payouts {1,2,35};
        vector<int> reds {1,3,5,7,9,12,14,16,18,19,21,23,25,27,30,32,34,36};
        vector<int> blacks {2,4,6,8,10,11,13,15,17,20,22,24,26,28,29,31,33,35};
};

int convertBetType(string betType){
    int code;
    transform(betType.begin(),betType.end(),betType.begin(), ::tolower);
    if (betType == "odd" || betType =="o"){
        code = 37;
    }
    else if (betType == "even" || betType == "e")
    {
        code = 38;
    }
    else if (betType == "black" || betType == "b")
    {
        code = 39;
    }
    else if (betType == "red" || betType == "r")
    {
        code = 40;
    }
    else{
        code = stoi(betType);
    }
    return code;    
}

void play(){
    cout<<"Playing Game"<< endl;

    Table table;
    Player player(1000,0); //create player with 1000 money to start with.
    int bet;
    string betType;
    int betType_code;
    int number;
    int winnings;

    while(player.get_money() > 0){
        
        cout << "How much do you want to bet ? current cash avilable " << player.get_money() << endl;
        cin >> bet;
        while (bet > player.get_money()){
            cout << "You do not have that amount of money to bet, please choose a bet less than " << player.get_money() << endl;
            cin >> bet;
        }

        cout << "what do you want to bet on (number / odd / even / black / red)" << endl;
        cin >> betType;
        betType_code = convertBetType(betType);
        player.placeBet(bet);

        number = table.spin_wheel();
        cout<<"\n"<<endl;
        for (int count = 3;count>0; count--){
            cout << " * ";
            cout.flush();
            this_thread::sleep_for(chrono::seconds(1));
        }
        cout << number << "\n\n" << endl; 
        winnings = table.calculate_winnings(betType_code, number, bet);
        if (winnings> 0){
            // won money
            cout << "CONGRATULATIONS YOU WON  ----  " << winnings << endl;
            player.add_winnings(winnings + bet);
        }
        else {
            cout << "UNlUCKY THIS TIME" << endl;
        }
        player.incrementThrows();
        cout << "current earnings per throw " << player.winnings_per_throw() << endl;
    }
    cout << "You Survived for " << player.get_throws() << " throws!";
    return;
}



int main(int argc, char* []){
    cout << "Welcome to Super Roulette!!" << endl;
    string response;
    
    play();
    while(true){
        // Check if user wants to play a new game
        cout << "Do you wish to play again (Y/N)" << endl;
        cin >> response;
        if (response == "Y" || response == "y" || response == "yes"){
            // Play new game
            play();
        }
        else{
            // Don't play a new game
            break;
        }
    }

    cout<<"Goodbye"<< endl;

    return 0;
}