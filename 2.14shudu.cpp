//Dancing LInks模版
// Created by 15411 on 2024/2/9.
//
#include<bits/stdc++.h>
#define ll long long
#define int long long
using namespace std;
const int N=3e6+7;
/*-------------------------------------------初代版本----------------------------------------------------*/
/*使用的DFS加剪枝以及一系列优化，但在面对部分数据时需要数秒甚至几分钟的时间进行计算*/
//ll G[12][12];
//bool flag;
//bitset<10>vst_jc[10];
//bitset<10>vst_r[10];
//bitset<10>vst_c[10];
////vector<pair<ll,ll>>wait_0;
//class node{
//public:
//    ll r,c,x;
//}wait_0[100];
//ll cnt2;
//void init2(){
//    memset(G,0,sizeof(G));flag=false;
//    //wait_0.clear();
//    memset(wait_0,0,sizeof(wait_0));
//    cnt2=0;
//    for(int i=1;i<=9;i++){
//        vst_jc[i].reset();vst_r[i].reset();vst_c[i].reset();
//    }
//}
//ll is_jz(ll r,ll c){return (r-1)/3+1+(c-1)/3*3;}
//
//void dfs(ll idx){
//    if(flag)
//        return;
//    //if(idx>=wait_0.size()){
//    if(idx>cnt2){
//        flag=true;
//        cout<<"One of the possible solutions is:"<<endl;
//        for(int r=1;r<=9;r++){
//            for(int c=1;c<=9;c++){
//                cout<<G[r][c];
//                (c==9)?cout<<endl:cout<<' ';
//            }
//        }
//        return;
//    }
//    //ll r=wait_0[idx].first;ll c=wait_0[idx].second;
//    ll r=wait_0[idx].r;ll c=wait_0[idx].c;
//    for(int x=1;x<=9;x++){
//        if(vst_jc[is_jz(r,c)][x]||vst_r[r][x]||vst_c[c][x])
//            continue;
//        G[r][c]=x; vst_jc[is_jz(r,c)][x]=true;vst_r[r][x]=true;vst_c[c][x]=true;
//        dfs(idx+1);
//        G[r][c]=0; vst_jc[is_jz(r,c)][x]= false;vst_r[r][x]=false;vst_c[c][x]=false;
//        if(flag)
//            return;
//    }
//}
//bool cmp(node A,node B){
//    return A.x<B.x;
//}
//void optimize(){
//    for(int i=1;i<=cnt2;i++){
//        ll r=wait_0[i].r;ll c=wait_0[i].c;
//        ll x=0;
//        for(int j=1;j<=9;j++){
//            if(!vst_r[r][j])
//                x++;
//            if(!vst_c[c][j])
//                x++;
//        }
//        wait_0[i].x=x;
//    }
//    sort(wait_0+1,wait_0+cnt2+1,cmp);
//}
//void solve2(ll op) {
//    ll x;init2();
//    for(int r=1;r<=9;r++){
//        for(int c=1;c<=9;c++){
//            cin>>x;
//            G[r][c]=x;
//            if(x!=0){
//                vst_jc[is_jz(r,c)][x]=true;vst_r[r][x]=true;vst_c[c][x]=true;
//            }
//            else
//                //wait_0.push_back({r,c});
//                wait_0[++cnt2]={r,c};
//        }
//    }
//    //进行优化
//    if(op)
//        optimize();
//    cout<<endl;
//    dfs(1);
//    if(!flag)
//        cout<<"Sorry, we didn't find a viable solution"<<endl;
//}
/*---------------------------------------------------------------------------------------------------*/

/*-----------------------------------------Dancing Links---------------------------------------------*/
//第二版本，使用了Dancing LInks算法，能确保在1s内计算完答案

ll n,m,cnt;//n*m(m是列)矩阵的长，宽，点的编号
ll u[N],d[N],l[N],r[N];//记录每个点的上下左右点编号
ll row[N],col[N];//记录每个点所在的行，列
ll h[N];//记录每行的头节点，每列头节点无需记录（就是其列数）
ll s[N];//记录每列的节点数量
ll ans[N];//记录答案选择的行号
ll DEP;
void init(){//初始化第0行的表头
    for(ll y=0;y<=m;y++){//第y列的点就是编号y
        u[y]=d[y]=y;//表头每个点的上下都是自己
        l[y]=y-1;r[y]=y+1;
    }
    l[0]=m;r[m]=0;cnt=m;//修正边界处的节点，形成循环
    //memset(h,0, sizeof(h));
}
void link(ll x,ll y){   //在x行，y列插入点
    row[++cnt]=x;col[cnt]=y;s[y]++;

    u[cnt]=u[y];//u[y]<->cnt<->y,列表0行y列的上面是y列最后一行的元素
    d[u[y]]=cnt;
    d[cnt]=y;
    u[y]=cnt;
    if(h[x]==0)h[x]=l[cnt]=r[cnt]=cnt;//该行无点时插入的第一个点自成循环
    else{
        l[cnt]=l[h[x]];//l[h[x]]<->cnt<->h[x],该行头节点的左边就是该行的末尾节点
        r[l[h[x]]]=cnt;
        r[cnt]=h[x];
        l[h[x]]=cnt;
    }
}
void remove(ll y){//删除y列与其关联的行
    r[l[y]]=r[y];l[r[y]]=l[y];//先处理表头，即第0行，从中删除这一列
    for(ll i=d[y];i!=y;i=d[i])//向下，找与当前列关联的行
        for(ll j=r[i];j!=i;j=r[j])//向右，删除该行的其他节点
            u[d[j]]=u[j],d[u[j]]=d[j],s[col[j]]--;
}
//因为删除列时，该列的上下关系依旧保留，可以从列头找到其中被删除的每一行的首个元素
//而对于每一行的其他元素，去删除时只是把当前点的上下两个点连在一起，但当前点的关系还保留着，可以从当前点知道其上面或者下面的点
//而且每一次删除后都对应着恢复，所以能一一恢复
void resume(ll y){//恢复y列与其关联的行
    r[l[y]]=y;l[r[y]]=y;//先处理表头，即第0行，从中恢复这一列
    for(ll i=u[y];i!=y;i=u[i])//向上，找与当前列关联的行
        for(ll j=l[i];j!=i;j=l[j])//向左，恢复该行的其他节点
            u[d[j]]=j,d[u[j]]=j,s[col[j]]++;
}
bool dance(ll dep){//在十字链表上舞动
    if(r[0]==0){//所有限制的列都被满足了，即找到了答案
        //for(int i=1;i<dep;i++){printf("%lld ",ans[i]);
        DEP=dep;
        return true;
    }
    ll y=r[0];//找出01的矩阵中，1个数最少的列
    for(ll i=r[0];i!=0;i=r[i])if(s[i]<s[y])y=i;

    remove(y);//删除当前1最少的列
    for(int i=d[y];i!=y;i=d[i]){//选择当前列有1的某一行作为答案行，并进行下一次递归
        ans[dep]=row[i];//记录选择的行作为答案
        for(int j=r[i];j!=i;j=r[j])remove(col[j]);//删除选中行中元素对应的列
        if(dance(dep+1))return true;
        for(int j=l[i];j!=i;j=l[j])resume(col[j]);//恢复选中行中元素对应的列
    }
    resume(y);//恢复当前1最少的列
    return false;
}
/*---------------------------------------------------------------------------------------------------*/
ll a[12][12];
void solve() {
    n=729;m=324;//录入矩阵大小
    init();//进行初始化
    for(int i=0;i<9;i++){
        for(int j=0;j<9;j++){
            cin>>a[i][j];
            for(int k=1;k<=9;k++){
                if(a[i][j]==0||a[i][j]==k){
                    ll now_r=i*9*9+j*9+k;
                    link(now_r,i*9+j+1);
                    link(now_r,81*1+i*9+k);
                    link(now_r,81*2+j*9+k);
                    link(now_r,81*3+(i/3*3+j/3+1-1)*9+k);
                }
            }
        }
        //dance(0);
    }
//    for(ll x=1;x<=n;x++){
//        for(int y=1;y<=m;y++){
//            ll a;
//            if(a){  //a==1,代表有点
//                link(x,y);
//            }
//        }
//    }
    if(!dance(1)){//从空间1开始舞蹈
        cout<<"No Solution!"<<endl;//失败
    }
    else{
        // 逆向解析答案
        for(int i=1;i<DEP;i++){
            //从行数ans[i]还原到点（x，y）与值v
            ll x=(ans[i]-1)/9/9;
            ll y=(ans[i]-1)/9%9;
            ll v=(ans[i])%9;
            a[x][y]=v?v:9;
        }
        cout<<endl<<"One of the possible solutions is:"<<endl;
        for(int i=0;i<9;i++) {
            for (int j = 0; j < 9; j++) {
                cout<<a[i][j];
                (j==8)?cout<<endl:cout<<' ';
            }
        }
    }
}

signed main(){
    ios::sync_with_stdio(false);//cin.tie(0);cout.tie(0);
    //ll t;cin>>t;while(t--)
//    cout<<"Whether to use the Dancing LInks algorithm:(YES/NO)"<<endl;
    string op="YES";
//    cin>>op;
    if(op=="YES"){
        cout<<"The Dancing Links algorithm is used by default for optimization"<<endl;
        cout<<"Please enter Sudoku and fill in 0 where there are no numbers:"<<endl;
        solve();
    }
//    else{
//        ll op2=0;//是否进行优化，1是进行，0是不进行
//        cout<<"Optimize or not:(YES/NO)"<<endl;
//        cin>>op;
//        if(op=="NO")
//            op2=1;
//        cout<<"Please enter Sudoku and fill in 0 where there are no numbers:"<<endl;
//        solve2(op2);
//    }
    return 0;
}
