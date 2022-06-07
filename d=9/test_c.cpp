#include<iostream>
#include<vector>
#include<random>
#include<string>
#include<fstream>
#include<time.h>

using namespace std;
#define code_dist 9
#define num_qubit (code_dist*code_dist + (code_dist-1)*(code_dist-1))
#define num_stabilizer ((code_dist*code_dist + (code_dist-1)*(code_dist-1)-1)/2)
#define max_body 4
#define num_samp 10000

FILE *fp1;
FILE *fp2; 
FILE *fp4; 

vector<vector<int>> qubit(2,vector<int>(num_qubit));
vector<vector<int>> error_conf(3,vector<int>(num_qubit));
vector<vector<int>> syndrome(2,vector<int>(num_stabilizer));
vector<double> phys_error_prob;
vector<double> logi_error_prob;
vector<double> Excution_time;
vector<vector<vector<int>>> stabilizer=
{
    {
        {0,9,81,81},
        {1,10,81,82},
        {2,11,82,83},
        {3,12,83,84},
        {4,13,84,85},
        {5,14,85,86},
        {6,15,86,87},
        {7,16,87,88},
        {8,17,88,88},
        {9,18,89,89},
        {10,19,89,90},
        {11,20,90,91},
        {12,21,91,92},
        {13,22,92,93},
        {14,23,93,94},
        {15,24,94,95},
        {16,25,95,96},
        {17,26,96,96},
        {18,27,97,97},
        {19,28,97,98},
        {20,29,98,99},
        {21,30,99,100},
        {22,31,100,101},
        {23,32,101,102},
        {24,33,102,103},
        {25,34,103,104},
        {26,35,104,104},
        {27,36,105,105},
        {28,37,105,106},
        {29,38,106,107},
        {30,39,107,108},
        {31,40,108,109},
        {32,41,109,110},
        {33,42,110,111},
        {34,43,111,112},
        {35,44,112,112},
        {36,45,113,113},
        {37,46,113,114},
        {38,47,114,115},
        {39,48,115,116},
        {40,49,116,117},
        {41,50,117,118},
        {42,51,118,119},
        {43,52,119,120},
        {44,53,120,120},
        {45,54,121,121},
        {46,55,121,122},
        {47,56,122,123},
        {48,57,123,124},
        {49,58,124,125},
        {50,59,125,126},
        {51,60,126,127},
        {52,61,127,128},
        {53,62,128,128},
        {54,63,129,129},
        {55,64,129,130},
        {56,65,130,131},
        {57,66,131,132},
        {58,67,132,133},
        {59,68,133,134},
        {60,69,134,135},
        {61,70,135,136},
        {62,71,136,136},
        {63,72,137,137},
        {64,73,137,138},
        {65,74,138,139},
        {66,75,139,140},
        {67,76,140,141},
        {68,77,141,142},
        {69,78,142,143},
        {70,79,143,144},
        {71,80,144,144}
    },
    {
        {0,1,81,81},
        {1,2,82,82},
        {2,3,83,83},
        {3,4,84,84},
        {4,5,85,85},
        {5,6,86,86},
        {6,7,87,87},
        {7,8,88,88},
        {9,10,81,89},
        {10,11,82,90},
        {11,12,83,91},
        {12,13,84,92},
        {13,14,85,93},
        {14,15,86,94},
        {15,16,87,95},
        {16,17,88,96},
        {18,19,89,97},
        {19,20,90,98},
        {20,21,91,99},
        {21,22,92,100},
        {22,23,93,101},
        {23,24,94,102},
        {24,25,95,103},
        {25,26,96,104},
        {27,28,97,105},
        {28,29,98,106},
        {29,30,99,107},
        {30,31,100,108},
        {31,32,101,109},
        {32,33,102,110},
        {33,34,103,111},
        {34,35,104,112},
        {36,37,105,113},
        {37,38,106,114},
        {38,39,107,115},
        {39,40,108,116},
        {40,41,109,117},
        {41,42,110,118},
        {42,43,111,119},
        {43,44,112,120},
        {45,46,113,121},
        {46,47,114,122},
        {47,48,115,123},
        {48,49,116,124},
        {49,50,117,125},
        {50,51,118,126},
        {51,52,119,127},
        {52,53,120,128},
        {54,55,121,129},
        {55,56,122,130},
        {56,57,123,131},
        {57,58,124,132},
        {58,59,125,133},
        {59,60,126,134},
        {60,61,127,135},
        {61,62,128,136},
        {63,64,129,137},
        {64,65,130,138},
        {65,66,131,139},
        {66,67,132,140},
        {67,68,133,141},
        {68,69,134,142},
        {69,70,135,143},
        {70,71,136,144},
        {72,73,137,137},
        {73,74,138,138},
        {74,75,139,139},
        {75,76,140,140},
        {76,77,141,141},
        {77,78,142,142},
        {78,79,143,143},
        {79,80,144,144}
    }
};
vector<vector<int>> logical={{0,1,2,3,4,5,6,7,8},{0,9,18,27,36,45,54,63,72}};
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
void data_error(double x, double z,double y){
    int i;
    double prob,prob_x,prob_y,prob_z;
    prob_x=x;
    prob_z=z+prob_x;
    prob_y=y+prob_z;

    for (i=0;i<num_qubit;++i){
        prob=ran_mt(mt);
        if(prob<=prob_x){
            qubit[0][i]^=1;
        }
        else if(prob<=prob_z){
            qubit[1][i]^=1;
        }
        else if(prob<=prob_y){
            qubit[0][i]^=1;
            qubit[1][i]^=1;
        }
    }
}
//各量子ビットにどんな演算子が作用しているのか確認
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

//各スタビライザーのシンドロームの値を確認
void show_synd(void){
    cout<<"syndrome:"<<endl;
    for(int i=0;i<num_stabilizer;i++){
        cout<<i<<" x"<<syndrome[0][i]<<" z"<<syndrome[1][i]<<endl;
    }

}

//シンドロームの値からcplexに投げる式を作成
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

void get_lp_format_c(){
    int i,j,k,hoge;
    char filepath[256];

    sprintf(filepath,"Int_prog.lp");
    if((fp1= fopen(filepath,"w"))==NULL){printf("error!!!!!!!!!!!!! \n");
    exit(1);}   

    fprintf(fp1,"enter example\n\nminimize");
    for(i=0;i<num_qubit;i++){
        if(i!=num_qubit-1){
            fprintf(fp1," x%d +",i);
        }
        else{
            fprintf(fp1," x%d +\n",i);  
        }
    }

    for(i=0;i<num_qubit;i++){
        if(i!=num_qubit-1){
            fprintf(fp1," z%d +",i);
        }
        else{
            fprintf(fp1," z%d +",i);    
        }
    }


    for(i=0;i<num_qubit;i++){
        if(i!=num_qubit-1){
            fprintf(fp1," y%d +",i);
        }
        else{
            fprintf(fp1," y%d\n",i);    
        }
    }



    fprintf(fp1,"\nsubject to \n");


    for(i=0;i<num_stabilizer;i++){
        
        //fprintf(fp1,"0 ");
        hoge=0;
        for(k=0;k<max_body;k++){
                if(k==0){   
                    fprintf(fp1,"x%d + y%d ",stabilizer[0][i][k],stabilizer[0][i][k]);
                }
                else if(stabilizer[0][i][k] != stabilizer[0][i][k-1]){
                    fprintf(fp1,"+ x%d + y%d ",stabilizer[0][i][k],stabilizer[0][i][k]);    
                }
            }
            fprintf(fp1,"- 2 w%d ",i);
        
        // printf("%d\n",syndrome[i]);
        fprintf(fp1,"= %d \n",syndrome[0][i]);
    }

    for(i=0;i<num_stabilizer;i++){
        
        //fprintf(fp1,"0 ");
        hoge=0;
        for(k=0;k<max_body;k++){
                if(k==0){   
                    fprintf(fp1,"z%d + y%d ",stabilizer[1][i][k],stabilizer[1][i][k]);
                }
                else if(stabilizer[1][i][k] != stabilizer[1][i][k-1]){
                    fprintf(fp1,"+ z%d + y%d ",stabilizer[1][i][k],stabilizer[1][i][k]);    
                }
            }
            fprintf(fp1,"- 2 w%d ",i+num_stabilizer);
        
        // printf("%d\n",syndrome[i]);
        fprintf(fp1,"= %d \n",syndrome[1][i]);
    }

    


    fprintf(fp1,"\n\nbound \n\ngeneral\n");
    for(i=0;i<2*num_stabilizer;i++){
        fprintf(fp1,"w%d\n",i);
    }
    fprintf(fp1,"\n\nbinary\n\n");
    for(i=0;i<num_qubit;i++){
        fprintf(fp1,"x%d\n",i);
    }
    for(i=0;i<num_qubit;i++){
        fprintf(fp1,"y%d\n",i);
    }
    for(i=0;i<num_qubit;i++){
        fprintf(fp1,"z%d\n",i);
    }
    fprintf(fp1,"\n\nend");
    fclose(fp1);
}

//cplexに解かせる
void solve_lp(){
    char bash_command[256];
    sprintf(bash_command,"python test.py\n");
    system(bash_command);
}

//エラーの推定結果を取得
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
void get_lp_result_c(){
    int i;
    char filepath[256];
    sprintf(filepath,"result.txt");

    
    fp4 = fopen(filepath,"r");
    if( fp4 == NULL ){printf( "%sファイルが開けません\n","result.txt");
    }

    for(i=0;i<num_qubit;i++){       
      fscanf(fp4,"%d\n",&error_conf[0][i]);
    }
    for(i=0;i<num_qubit;i++){       
      fscanf(fp4,"%d\n",&error_conf[1][i]);
    }
    for(i=0;i<num_qubit;i++){       
      fscanf(fp4,"%d\n",&error_conf[2][i]);
    }

    fclose(fp4);
  
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

//エラー訂正
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

    for(double prob_error=0.03;prob_error<=0.20001;prob_error=prob_error+0.01){
        prob_logi=0;
        double excution_time=0;
        for(int l=0;l<num_samp;l++){
            initialize();
            data_error(prob_error/3,prob_error/3,prob_error/3);
            //show_data();
            synd_meas();
            clock_t start=clock();
            get_lp_format();
            solve_lp();
            get_lp_result_c();
            clock_t end=clock();
            excution_time+=double(end-start);
            //show_result();
            recovery();
            synd_meas();
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
                cout<<"論理エラー"<<endl;
                prob_logi+=1.0;
                //printf("%d\n",l);
                //if ( qq2>4){
                //printf("%d\n",l);
                    
                //}
    
            }



        cout<<prob_error<<"の"<<l<<"回目"<<endl;
        }
        prob_logi=prob_logi/((double) num_samp);
        logi_error_prob.push_back(prob_logi);
        Excution_time.push_back(excution_time);
    }
    for(int i=0;i<logi_error_prob.size();i++){
        if(i==logi_error_prob.size()-1){
            cout<<logi_error_prob[i]<<endl;
            break;
        }
        cout<<logi_error_prob[i]<<",";
    }
    for(int i=0;i<Excution_time.size();i++){
        if(i==Excution_time.size()-1){
            cout<<Excution_time[i]<<endl;
            break;
        }
        cout<<Excution_time[i]<<",";
    }
    return 0;


}