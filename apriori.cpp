//
//  main.cpp
//  data_structure_design
//
//  Created by 曾晟星 on 2018/6/18.
//  Copyright © 2018年 曾晟星. All rights reserved.
//

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <iomanip>
#include <sstream>
using namespace std;
double minsup=0.5;//定义最小支持度为0.5
double mincnf=0.6;//定义最小置信度为0.6
struct item{
    string  item_name;//商品的名称
    vector<int>if_bought;//是否在序号之中被购买了
    int label;
};//定义商品结构体
struct frequent_item{
    vector<item>items;//该频繁项目的项目名称
    double sup;//频繁项目的支持度
    double cnf;//频繁项目的置信度
};//定义的每一个频繁项目
struct frequent_item_set{
    vector<frequent_item>item_set;//存储项集之中项目的名称
    int item_set_length;//项目集的长度
};//用于存储频繁项集
bool is_input_int_array(vector<int>array,int num){
    for(int i=0;i<array.size();i++){
        if(array[i]==num){
            return 1;
        }
    }
    return 0;
}//检测该项目是否已经写入了是频繁项目数组之中
int find_the_item(vector<int>array,int num){
    for(int i=0;i<array.size();i++){
        if(array[i]==num){
            return i;
        }
    }
    return -1;
}//在数组之中找到对应的商品，并返回它的位置
void find_the_rest_items(vector<int>total_array,vector<int>&stored_array,frequent_item fi){
    stored_array=total_array;
    for(int i=0;i<fi.items.size();i++){
        int location=find_the_item(stored_array,fi.items[i].label);
        if(location!=-1){
            stored_array[location]=-1;
        }//如果找到了一样的则将其置为-1
    }
}//查找剩下的商品
bool is_input_item_set(frequent_item_set fis,frequent_item fi){
    for(int i=0;i<fis.item_set.size();i++){
        if(fis.item_set[i].items.size()==fi.items.size()){//如果每个频繁项目包含同样数量的商品数
            int count=0;
            for(int j=0;j<fi.items.size();j++){
                for(int k=0;k<fis.item_set[i].items.size();k++){
                    if(fi.items[j].item_name==fis.item_set[i].items[k].item_name){
                        count++;
                        break;
                    }
                }
            }//找到一个相同的则count+1
            if(count==(int)fi.items.size()){
                return 1;
            }//如果count的值和频繁项目的大小相同，则表示已经录入了，不再进行操作
        }
    }
    return 0;
}//判断该频繁项目是否已经加入了频繁项目集中
void read_csv_file(string file_name,vector<string>&file_content){
    ifstream file_in(file_name);
    string read_line;
    while(file_in.good()){
        string field;
        while(getline(file_in,field)){
            file_content.push_back(field);
        }
    }
}//用以读取csv表格文件
void load_item(vector<item>&items,vector<string>apiror_string){
    int load_length=0;
    while(load_length<(int)apiror_string[0].length()){
        item new_item;
        while(apiror_string[0][load_length]!=','&&load_length<(int)apiror_string[0].length()){
            new_item.item_name.push_back(apiror_string[0][load_length]);
            load_length++;
        }
        load_length++;
        items.push_back(new_item);
    }//将商品名称录入到结构体之中，并根据商品的数量创造一个结构体组
    for(int i=0;i<items.size();i++){
        for(int j=1;j<apiror_string.size();j++){
            items[i].if_bought.push_back(apiror_string[j][2*(i+1)]-48);//由于数字的ASCII码和数值差48，故要减去48
            items[i].label=i;//对商品进标号
        }//从购买情况开始读取，即从第二项开始
    }
}//将物品名称以及购买情况全部整理，并分别存储到结构体之中
void create_single_item_set(vector<item>apiror_items,vector<frequent_item_set>&freq_item_set){
    frequent_item_set fis;
    for(int i=0;i<apiror_items.size();i++){
        frequent_item s_freq_item;
        s_freq_item.items.push_back(apiror_items[i]);
        //将单个的商品加入频繁项目之中
        int count=0;
        for(int j=0;j<apiror_items[i].if_bought.size();j++){
            if(apiror_items[i].if_bought[j]==1){
                count++;
            }//统计购买的个数
        }
        s_freq_item.sup=count/(double)apiror_items[i].if_bought.size();//求得它的支持度
        if(s_freq_item.sup>=minsup){
            fis.item_set.push_back(s_freq_item);
        }//如果它的支持度大于等于最小支持度，则加入频繁项目集中
    }
    fis.item_set_length=(int)fis.item_set.size();//得到频繁项目的长度
    freq_item_set.push_back(fis);//加入到频繁项目集之中
}//创建单个的频繁项目
void extend_item_set(vector<frequent_item_set>&freq_item_set,vector<item>apiror_items){
    for(int i=0;i<freq_item_set.size();i++){//对于每个频繁项目
        if(freq_item_set[i].item_set_length==1){
            return;
        }//如果只有一个频繁项目集了，那么表示已经完全构建完毕，程序结束
        frequent_item_set fis;
        vector<int>selected;
        for(int j=0;j<freq_item_set[i].item_set.size();j++){//对于每个频繁项目的元素
            for(int k=0;k<freq_item_set[i].item_set[j].items.size();k++){
                if(!is_input_int_array(selected,freq_item_set[i].item_set[j].items[k].label)){
                    selected.push_back(freq_item_set[i].item_set[j].items[k].label);
                }
            }//将该频繁项目的所有商品写入已选择数组之中
        }
        for(int j=0;j<freq_item_set[i].item_set.size();j++){
            frequent_item fi;
            fi=freq_item_set[i].item_set[j];
            vector<int>total_array;
            find_the_rest_items(selected,total_array,fi);
            for(int k=0;k<total_array.size();k++){
                frequent_item fi1=fi;
                if(total_array[k]!=-1){
                    fi1.items.push_back(apiror_items[total_array[k]]);//加入未合并的商品，生成新的频繁项目
                    double count=0;//统计同时购买这些商品的序号数
                    int items_id[(int)fi1.items.size()];
                    for(int m=0;m<fi1.items.size();m++){
                        items_id[m]=fi1.items[m].label;
                    }//将新的频繁项目的所有商品的编号存入一个数组之中，方便后面进行查找
                    for(int n=0;n<fi1.items[0].if_bought.size();n++){
                        int items_code=0;
                        while(fi1.items[items_code].if_bought[n]==1){
                            items_code++;
                            if(items_code==(int)fi1.items.size()){
                                count++;
                                break;
                            }
                        }//进行找到同序号同时购买该频繁项目的数量
                    }
                    fi1.sup=count/(double)total_array.size();//求得新的频繁项目的支持度
                    fi1.cnf=fi1.sup/fi.sup;//求得新的频繁项目的置信度
                    if(fi1.sup>=minsup){
                        if(!is_input_item_set(fis,fi1)){
                            fis.item_set.push_back(fi1);
                        }
                    }//如果大于最小的支持度，且没有被录入过，则加入新的频繁项目集
                }
            }
        }
        fis.item_set_length=(int)fis.item_set.size();
        freq_item_set.push_back(fis);//加入到频繁项目集族里
    }
}//创建频繁项目集
int main(){
    vector<string>apiror_string;
    string apiror_file="/Users/cengshengxing/Desktop/data construction/data_structure_design/data_structure_design/test.csv";
    string stored_file="/Users/cengshengxing/Desktop/data construction/data_structure_design/data_structure_design/result.txt";
    vector<frequent_item_set>apiror_frequent_item_set;
    read_csv_file(apiror_file,apiror_string);
    for(int i=0;i<apiror_string.size();i++){
        if(i==0){
            cout<<"商品的种类如下"<<endl;
        }
        if(i==1){
            cout<<"商品的购买情况为如下（0表示未买，1表示已买）"<<endl;
        }
        cout<<apiror_string[i]<<endl;
    }
    vector<item>apiror_items;
    load_item(apiror_items,apiror_string);
    create_single_item_set(apiror_items,apiror_frequent_item_set);
    extend_item_set(apiror_frequent_item_set,apiror_items);
    ofstream output_file(stored_file,ios::app);
    for(int i=0;i<apiror_frequent_item_set.size();i++){
        output_file<<"频繁项目集"<<i+1<<":"<<endl;
        for(int j=0;j<apiror_frequent_item_set[i].item_set_length;j++){
            for(int k=0;k<apiror_frequent_item_set[i].item_set[j].items.size();k++){
                if(k!=(int)apiror_frequent_item_set[i].item_set[j].items.size()-1){
                    output_file<<apiror_frequent_item_set[i].item_set[j].items[k].item_name<<" ";
                }
                else{
                    output_file<<apiror_frequent_item_set[i].item_set[j].items[k].item_name<<endl;
                }
            }
            if(i!=0){
                if(apiror_frequent_item_set[i].item_set[j].cnf>=mincnf){
                    output_file<<"它们之间有关联"<<endl;
                }
            }
        }
    }
    output_file<<"----------------------------------------"<<endl;
    output_file.close();
    return 0;
}
