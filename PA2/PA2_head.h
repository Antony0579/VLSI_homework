#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <cstring>
#include <sstream>
// #include <stdio.h>
// #include <stdlib.h>
#include <time.h> 

using namespace std;
using std::string;

struct Node{
    vector<Node *>front_Ptr;
    vector<Node *>next_Ptr;

    int front_count = 0;
    int next_count = 0;

    int mynode;
    string mynode_f;

    int layer_ASAP = 1;
    int layer_ALAP = 1;
    int layer_force_ASAP;
    int layer_force_ALAP;

    float probability;
    // float q_distribution = 0;
    vector<float> self_force;
    vector<float> predecessor_force;
    vector<float> total_force;
    float total_force_min = 99;
    int total_force_min_layer;
     
    bool completed = 0;  
};

struct Qdis{
    float add;
    float multi;
};

//--------------------------------------------------------------------------------------------//

class Function {
// 宣告 public 的成員
public:
    void clock_on();
    void clock_off();    
    double START,END;

    vector<string> readfile(string);
    vector <string> myString;

    void clearfile(string);
    void savefile(string,string);
    // string file_name = "E://C_VScode//testcase1";

    void data_in(Node[],int[],int[],int,int);
    void Connect_Node(Node[], Node[], int[], int[], int, int);
    void do_ASAP(Node[], int, int);
    void do_ALAP(Node[], int, int);
    void Count_operator(Node[], int, int, vector<int>, vector<int>, int*, int*);

    void Saved_ASAP(string, Node[], int, int, int, int);
    void Saved_ALAP(string, Node[], int, int, int, int);

    void initial_force(Node[], int, int);
    void recal_probability(Node[], int);
    void recal_distribution(Node[], Qdis[], int, int);
    void recal_self_force(Node[], Qdis[], int, int);
    void recal_predecessor_force(Node[], Qdis[], int, int);
    void recal_total_force(Node[], int, int);

    void Schedule(int, Node[]);
    void Saved_TFS(string, Node[], int, int, int, int);

    int lookback(Node ,int );
    int looknext(Node ,int );

// 宣告 private 的成員
private:
};
