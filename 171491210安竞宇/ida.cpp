#include <iostream>
#include <time.h> 
using namespace std;
 
struct ED_Node{
    int arr[9];
    int blankPos;
}ed_node;
 
ED_Node tag_node = {1,2,3,4,5,6,7,8,0,8};
bool isMove[][4]= {{0,1,0,1},{0,1,1,1}, {0,1,1,0},
                    {1,1,0,1},{1,1,1,1}, {1,1,1,0}, 
                    {1,0,0,1}, {1,0,1,1},{1,0,1,0}};
 
int move_distance[] = {-3,3,-1,1};
int ED_weight[]={0,1,2,6,24,120,720,5040,40320}; //康托展开式中各项权重
int str[400000]; //记录路径（存储direction的下标）
char direction[] ="udlr";
int direc[]={-1,1,2,-2};//防止向回走
 
int currStep;//记录已走步数，其对应的方向存放在str中
int Max_deepth; //最大深度，不断递增
int Min_Distance;
 
 
int get_h(const ED_Node& node)
{
    int current_x,current_y;
    int tag_x,tag_y;
    int sum_distance = 0;
    for ( int i=0; i<9; ++i ){
        if ( node.arr[i] != 0 ){
            tag_x = (node.arr[i]-1)/3;
            tag_y = (node.arr[i]-1)%3;
            current_x = i/3;
            current_y = i%3;
            sum_distance += abs(tag_x-current_x)+abs(tag_y-current_y);
        }
    }
    return sum_distance;
}
 
bool isSloved (const ED_Node& src,const ED_Node& tag)
{
    int countSrcNX = 0;
    int countTagNX = 0;
    for ( int i=0; i<9; ++ i ){
        for ( int j=0; j<i; ++j ){
            if ( src .arr[i]>0 ){
                countSrcNX = src .arr[j]>src.arr[i] ?
                    ( countSrcNX+1) :countSrcNX ;
            } 
            if ( tag .arr[i]>0 ){
                countTagNX = tag.arr[j]>tag.arr[i] ?
                   ( countTagNX+1) :countTagNX ;
            } 
        } 
    } 
    return (countSrcNX &1) == (countTagNX&1);
}
 
void sweap(int& a,int &b)
{
    if ( a!=b ){ //切记自我检测
        a = a+b;
        b = a-b;
        a = a-b;
    }
}
 
bool DFS()
{
    int temp_distance = get_h(ed_node);
    if (  temp_distance == 0 ){
        return true;
    }
    int estimateDistance = temp_distance+currStep;
    //cout<<"estimateDistance::"<<estimateDistance<<endl;
     Min_Distance = Max_deepth>estimateDistance ? Min_Distance :
                (Min_Distance<estimateDistance ? estimateDistance:Min_Distance);
    if ( estimateDistance>Max_deepth ){
        return false;
    }
    for ( int i=0; i<4; ++i ){
        
        if ( !isMove[ed_node.blankPos][i] || !(direc[str[currStep]] +direc[i]) ){
            continue;
        }       
        str[++currStep] = i;
        sweap(ed_node.arr[ed_node.blankPos],ed_node.arr[ed_node.blankPos+move_distance[i]]);
        ed_node.blankPos = ed_node.blankPos+move_distance[i];
        if ( DFS() ){
            return true;
        }
        --currStep;
        ed_node.blankPos = ed_node.blankPos-move_distance[i];
        sweap(ed_node.arr[ed_node.blankPos],ed_node.arr[ed_node.blankPos+move_distance[i]]);
    }
    return false;
}
 
void IDAstar()
{
    if ( !isSloved(ed_node,tag_node) ){
        cout<<"unsolvable"<<endl;
        return ;
    }
    currStep = 0;
    Max_deepth = get_h(ed_node);
    Min_Distance = Max_deepth;
    while ( !DFS() ){
        //Max_deepth++;
        Max_deepth = Min_Distance>Max_deepth ? Min_Distance : (Max_deepth+1);
    }
    for (int i=1; i<=currStep; ++i) {
        cout<<direction[str[i]];
    }
    cout<<endl;
}
 
int main()
{
    char str [100];
 
 
    while ( gets (str)!=NULL ){
        int k =0;
        for ( int i=0; i<9; ++ i,k ++ ){ 
            if ( str [k] == 'x' ){
                ed_node.arr [i] = 0;
                ed_node.blankPos = i;
            } else if ( str[k]>= '1' &&  str [k]<= '9'){
                ed_node.arr [i] = str[k]-'0';
            } else {
                -- i;
            }
        }
        IDAstar();
    }            
 
 
}