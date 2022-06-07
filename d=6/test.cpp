#include<iostream>
#include<vector>
#include<random>
#include<string>
#include<fstream>

using namespace std;
#define code_dist 6
#define num_qubit (code_dist*code_dist + (code_dist-1)*(code_dist-1))
#define num_stabilizer ((code_dist*code_dist + (code_dist-1)*(code_dist-1)-1)/2)
#define max_body 4
#define num_samp 100

vector<vector<int>> qubit(2,vector<int>(num_qubit));
vector<vector<int>> error_conf(3,vector<int>(num_qubit));
vector<vector<int>> syndrome(2,vector<int>(num_stabilizer));
vector<vector<vector<int>>> stabilizer=
{
    {
        {0,6,36,36},
        {1,7,36,37},
        {2,8,37,38},
        {3,9,38,39},
        {4,10,39,40},
        {5,11,40,40},
        {6,12,41,41},
        {7,13,41,42},
        {8,14,42,43},
        {9,15,43,44},
        {10,16,44,45},
        {11,17,45,45},
        {12,18,46,46},
        {13,19,46,47},
        {14,20,47,48},
        {15,21,48,49},
        {16,22,49,50},
        {17,23,50,50},
        {18,24,51,51},
        {19,25,51,52},
        {20,26,52,53},
        {21,27,53,54},
        {22,28,54,55},
        {23,29,55,55},
        {24,30,56,56},
        {25,31,56,57},
        {26,32,57,58},
        {27,33,58,59},
        {28,34,59,60},
        {29,35,60,60},
    },
    {
        {0,1,36,36},
        {1,2,37,37},
        {2,3,38,38},
        {3,4,39,39},
        {4,5,40,40},
        {6,7,36,41},
        {7,8,37,42},
        {8,9,38,43},
        {9,10,39,44},
        {10,11,40,45},
        {12,13,41,46},
        {13,14,42,47},
        {14,15,43,48},
        {15,16,44,49},
        {16,17,45,50},
        {18,19,46,51},
        {19,20,47,52},
        {20,21,48,53},
        {21,22,49,54},
        {22,23,50,55},
        {24,25,51,56},
        {25,26,52,57},
        {26,27,53,58},
        {27,28,54,59},
        {28,29,55,60},
        {30,31,56,56},
        {31,32,57,57},
        {32,33,58,58},
        {33,34,59,59},
        {34,35,60,60}
    }

};
vector<vector<int>> logical={{0,1,2,3,4,5},{0,6,12,18,24,30}};
double prob_error;
double prob_logi;


//乱数発生
random_device rnd;
//mt19937 mt(0);  //乱数のシードを固定
mt19937 mt(rnd());
uniform_real_distribution<double> ran_mt(0,1);

//量子状態の初期化
void initialize(void){
    for(int i=0;i<num_qubit;++i){
        for(int j=0;j<2;++j){
            qubit[j][i]=0;
        }
    }
}


//エラー発生
void data_error(){
    int i;
    double prob;

    for (i=0;i<num_qubit;++i){
        prob=ran_mt(mt);
        if(prob<=prob_error){
            qubit[0][i]^=1;
        }
    }
}
void show_data(){
    int i; 
    printf("data qubit:\n");
    for(i=0;i<num_qubit;i++){
        cout<<i<<" x"<<qubit[0][i]<<" z"<<qubit[1][i]<<endl;
    }
}

//シンドローム測定
void synd_meas(){
    int i,j,k;
    for(int i=0;i<num_stabilizer;++i){
        for (int k=0;k<2;k++){
            syndrome[k][i]=0;
            for(int j=0;j<max_body;++j){
                if(j==0){
                    syndrome[k][i]^=qubit[k][stabilizer[k][i][j]];
                }
                else if(stabilizer[k][i][j]!=stabilizer[k][i][j-1]){
                    syndrome[k][i]^=qubit[k][stabilizer[k][i][j]];
                }
            }
        }
    }
}

void show_synd(void){
    cout<<"syndrome:"<<endl;
    for(int i=0;i<num_stabilizer;i++){
        cout<<i<<" x"<<syndrome[0][i]<<" z"<<syndrome[1][i]<<endl;
    }

}

void get_lp_format(){
    int i,j,k,hoge;
    ofstream writing_file;
    string filename="Int_prog.lp";
    writing_file.open(filename,ios::out);
    string writing_text="enter example\n\nminimize";
    writing_file<<writing_text<<endl;
    for(int i=0;i<num_qubit;++i){
        if(i!=num_qubit-1){
            string writing_text=" x";
            writing_file<<writing_text<<i<<" +";
        }
        else{
            string writing_text=" x";
            writing_file<<writing_text<<i<<" +"<<endl;
        }
    }
    for(int i=0;i<num_qubit;++i){
        if(i!=num_qubit-1){
            string writing_text=" z";
            writing_file<<writing_text<<i<<" +";
        }
        else{
            string writing_text=" z";
            writing_file<<writing_text<<i<<" +"<<endl;
        }
    }
    for(int i=0;i<num_qubit;++i){
        if(i!=num_qubit-1){
            string writing_text=" y";
            writing_file<<writing_text<<i<<" +";
        }
        else{
            string writing_text=" y";
            writing_file<<writing_text<<i<<endl;
        }
    }

    writing_text="subject to ";
    writing_file<<endl<<writing_text<<endl;
    for(int i=0;i<num_stabilizer;++i){
        hoge=0;
        for(k=0;k<max_body;k++){    
            if(k==0){
                writing_file<<"x"<<stabilizer[0][i][k]<<" + y"<<stabilizer[0][i][k];
            }
            else if(stabilizer[0][i][k] != stabilizer[0][i][k-1]){
                writing_file<<" + x"<<stabilizer[0][i][k]<<" + y"<<stabilizer[0][i][k];
            }       
        }
        writing_file<<" -2 w"<<i;
        writing_file<<" = "<<syndrome[0][i]<<endl;
    }
    for(int i=0;i<num_stabilizer;++i){
        hoge=0;
        for(k=0;k<max_body;k++){
            if(k==0){
                writing_file<<"z"<<stabilizer[1][i][k]<<" + y"<<stabilizer[1][i][k];
            }
            else if(stabilizer[1][i][k] != stabilizer[1][i][k-1]){
                writing_file<<" + z"<<stabilizer[1][i][k]<<" + y"<<stabilizer[1][i][k];
            }       
        }
    writing_file<<" -2 w"<<i+num_stabilizer;
    writing_file<<" = "<<syndrome[1][i]<<endl;
    }

    writing_file<<endl<<endl<<"bound"<<endl<<endl<<"general"<<endl;
    for(int i=0;i<2*num_stabilizer;++i){
        writing_file<<"w"<<i<<endl;
    }
    writing_file<<endl<<endl<<"binary"<<endl<<endl;
    for(int i=0;i<num_qubit;i++){
        writing_file<<"x"<<i<<endl;
    }
    for(int i=0;i<num_qubit;i++){
        writing_file<<"y"<<i<<endl;
    }
    for(int i=0;i<num_qubit;i++){
        writing_file<<"z"<<i<<endl;
    }
    writing_file<<endl<<endl<<"end";

    writing_file.close();


}

void solve_lp(){
    char bash_command[256];
    sprintf(bash_command,"python test.py\n");
    system(bash_command);
}

void get_lp_result(){
    int i;
    ifstream reading_file;
    string filename = "result.txt";
    reading_file.open(filename, ios::in);
    string reading_line_buffer;
    while(getline(reading_file, reading_line_buffer)){
        if(0<=i<num_qubit){
            int num = atoi(reading_line_buffer.c_str());
            error_conf[0][i]=num;
            i++;
            continue;

        }
        if(num_qubit<=i<2*num_qubit){
            int num = atoi(reading_line_buffer.c_str());
            error_conf[1][i]=num;
            i++;
            continue;

        }
        if(2*num_qubit<=i<3*num_qubit){
            int num = atoi(reading_line_buffer.c_str());
            error_conf[2][i]=num;
            i++;
            continue;

        }


    }
    reading_file.close();
}


void show_file_result(){
    int i;
    ifstream reading_file;
    string filename = "result.txt";
    reading_file.open(filename, ios::in);
    string reading_line_buffer;
    cout<<"resultファイルの中身は"<<endl;
    while(getline(reading_file, reading_line_buffer)){
        cout<<reading_line_buffer<<endl;
    }

}

void recovery(){
    int i,j;
    for(i=0;i<num_qubit;i++){
        for(j=0;j<2;j++){
            qubit[j][i]^=error_conf[j][i];
        }
        qubit[0][i]^=error_conf[2][i];
        qubit[1][i]^=error_conf[2][i];
    }
}

void show_result(){
    cout<<"result"<<endl;
    for(int i=0;i<num_qubit;++i){
        cout<<i<<" x"<<error_conf[0][i]<<" z"<<error_conf[1][i]<<" y"<<error_conf[2][i]<<endl;
    }
}



int main(){
    int i,j,l,k,a,b,c,n,m,ww;
    int flg_logi_errorx;
    int flg_logi_errorz;

    prob_error=0.03;
    for(prob_error=0.03;prob_error<0.030001;prob_error=prob_error+0.005){
        prob_logi=0;
        for(int l=0;l<num_samp;l++){
            initialize();
            data_error();
            show_data();
            synd_meas();
            show_synd();
            get_lp_format();
            solve_lp();
            get_lp_result();
            recovery();
            show_result();
            synd_meas();
            show_synd();
            for(i=0;i<num_stabilizer;i++){
                if(syndrome[0][i]!=0 || syndrome[1][i]!=0){
                printf("syndrom error!!!!!!");
                exit(0);
                }
            }
            flg_logi_errorx=0;
            for(i=0;i<code_dist;i++){
                flg_logi_errorx ^= qubit[0][logical[0][i]];
            }
            flg_logi_errorz=0;
            for(i=0;i<code_dist;i++){
                flg_logi_errorz ^= qubit[1][logical[1][i]];
            }
            if(flg_logi_errorx!=0 || flg_logi_errorz!=0){
                prob_logi+=1.0;
                //printf("%d\n",l);
                //if ( qq2>4){
                //printf("%d\n",l);
                    
                //}
    
            }
            prob_logi=prob_logi/((double) num_samp);
            cout<<prob_error<<" "<<prob_logi<<endl;




        }
    }
    return 0;


}