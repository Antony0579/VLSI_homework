#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <cstring>
#include <sstream>
// #include <stdio.h>
// #include <stdlib.h>
#include <time.h> 
#include <algorithm>

using namespace std;
using std::string;

struct GainBucket;

struct NET_list{ 
    vector<int> cell;
};
struct CELL_list{ 
    int cell;
    vector<int> pin;
    GainBucket* GB_Ptr = NULL;
    string AorB;
    bool locked = 0;
};
struct GainBucket{
    int gain;
    vector<int> cell; 
    bool locked = 0;
};

class Function{
public:
    void clock_on();
    void clock_off();    
    double START,END;
     
    void clearfile(string);
    void savefile(string,string);
    vector<string> readfile(string);
    vector <string> myString;
    int file_word;
    int net_num;
    int cell_num;    

    void make_net_list(NET_list[]);
    void make_cell_list(CELL_list[],NET_list[]);
    void print_cell_list_and_net_list(CELL_list[], NET_list[]);

    int max_pin_in_cell = 0;
    int max_cell_in_net = 0;
    int net_with_max_cell;
    void count_max_pin(CELL_list[]);    

    void initial_gain_bucket(CELL_list[],NET_list[],GainBucket[],GainBucket[]);
    void print_gainbucket(CELL_list[],NET_list[],GainBucket[],GainBucket[]);
    int gain_to_num(int);
    int num_to_gain(int);

    int count_gb_A = 0;
    int count_gb_B = 0;
    void count_gain_bucket_cells(GainBucket[],GainBucket[]);

    int cell_of_gain_more_than0 = 0;
    void count_cell_of_gain_more_than0(GainBucket[],GainBucket[]);

    void do_FM_Partition(CELL_list[],NET_list[],GainBucket[],GainBucket[]); 
    
    void move_A_to_B(int,int,CELL_list[],NET_list[],GainBucket[],GainBucket[]);
    void move_B_to_A(int,int,CELL_list[],NET_list[],GainBucket[],GainBucket[]);        
    void recal_gain_A_to_B(int,CELL_list[],NET_list[],GainBucket[],GainBucket[]);
    void recal_gain_B_to_A(int,CELL_list[],NET_list[],GainBucket[],GainBucket[]);
    void move_gain_in_A(int, vector<int>, CELL_list[], GainBucket[]);
    void move_gain_in_B(int, vector<int>, CELL_list[], GainBucket[]);

    int Ad_gain_move_after;
    int Ad_gain_move_before;

    void Total_move_A_to_B(int,CELL_list[],NET_list[],GainBucket[],GainBucket[]);
    void Total_move_B_to_A(int,CELL_list[],NET_list[],GainBucket[],GainBucket[]);
    void get_Max_GainA(CELL_list[],GainBucket[]);
    void get_Max_GainB(CELL_list[],GainBucket[]);
    string tie_breaking(int,CELL_list[],NET_list[],GainBucket[],GainBucket[]);

    int cell_move;
    int Max_GainA;
    int Max_GainB;

    void save_Result(string,CELL_list[],NET_list[],GainBucket[],GainBucket[]);

};
