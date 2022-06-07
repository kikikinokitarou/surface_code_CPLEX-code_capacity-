#include<iostream>
#include<vector>
#include<random>
#include<string>
#include<fstream>
#include<time.h>

using namespace std;
#define code_dist 5
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
        {0,5,25,25},
        {1,6,25,26},
        {2,7,26,27},
        {3,8,27,28},
        {4,9,28,28},
        {5,10,29,29},
        {6,11,29,30},
        {7,12,30,31},
        {8,13,31,32},
        {9,14,32,32},
        {10,15,33,33},
        {11,16,33,34},
        {12,17,34,35},
        {13,18,35,36},
        {14,19,36,36},
        {15,20,37,37},
        {16,21,37,38},
        {17,22,38,39},
        {18,23,39,40},
        {19,24,40,40}
    },
    {
        {0,1,25,25},
        {1,2,26,26},
        {2,3,27,27},
        {3,4,28,28},
        {5,6,25,29},
        {6,7,26,30},
        {7,8,27,31},
        {8,9,28,32},
        {10,11,29,33},
        {11,12,30,34},
        {12,13,31,35},
        {13,14,32,36},
        {15,16,33,37},
        {16,17,34,38},
        {17,18,35,39},
        {18,19,36,40},
        {20,21,37,37},
        {21,22,38,38},
        {22,23,39,39},
        {23,24,40,40}
    }
};
vector<vector<int>> logical={{0,1,2,3,4},{0,5,10,15,20}};
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

    for(double prob_error=0.2;prob_error<=0.200001;prob_error=prob_error+0.01){
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