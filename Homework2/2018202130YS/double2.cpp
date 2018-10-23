#include <stdio.h>
#include <string.h>
#include <iostream> 
using namespace std;
typedef long long int64;
#define bias (1023)
#define expmax (2047)
#define lim (100000000000000000)
#define maxn (4503599627370495)
#define size (1024*1024*128)
#define ss (size*4)
int w[2000],ww[2000],c[3000],d[3000];
int64 v[1000],aa[10],bb[10],x[10];
struct mydouble
{
  bool sign;
  int exp;
  int frac[60];
}; 
mydouble a,b,ans;
char st[1000];
int sum,tot;
/* void seeout(struct mydouble *a)
 {
   cout<<"******************"<<endl;
     cout<<a -> sign<< endl;
     cout<<a -> exp << endl;
     for (int i=1; i<=52; i++)
      cout<<a -> frac[i];
     cout<<endl;
   cout<<"*****************"<<endl;	  
} */
void converse1(int l,int r)
{
  sum=0;
  if (l>r) return;
  memset(v,0,sizeof(v));
  int s=1; 
  for (int i=l; i<=r; i++)
  {
    if (v[s]*10+st[i]-48>=lim)
    {
      s++;
      v[s]=st[i]-48;
    }
    else v[s]=v[s]*10+st[i]-48;
  }
  int j=1;
  while (j<=s)
  {
    for (int i=j; i<=s; i++)
    {
      v[i+1]+=(v[i] & 1)*lim;
      v[i]=v[i] >> 1;
    }
    sum++;
    if (v[s+1]) 
    {
      c[sum]=1;
      v[s+1]=0;
    }
    else c[sum]=0;
    while ((!v[j])&&(j<=s))  j++;   
  }
}
void converse2(int l,int r)
{
  tot=0;
  if (l>r) return;
  memset(v,0,sizeof(v));
  int s=1;
  for (int i=l; i<=r; i++)
  {
    if (v[s]*10+st[i]-48>=lim)
    {
      s++;
      v[s]=st[i]-48;
    }
    else v[s]=v[s]*10+st[i]-48;
  }
  while (s)
  {
  	while (v[s]*10<lim) v[s]*=10;
    v[0]=0;
    for (int i=s; i>=1; i--)
    {
      if  ((v[s] << 1)>=lim)
      {
       v[s]=(v[s] << 1)-lim;
       v[s-1]++;
      }
      else v[s]=v[s] << 1;
    }
    tot++;
    if (v[0]) d[tot]=1;
      else d[tot]=0;
    if (tot>=60) break;
    while ((!v[s])&&(s)) s--;
  }
}
void swap(struct mydouble *a, struct mydouble *b)
{
  
  bool t; t=a -> sign; a -> sign=b -> sign; b -> sign=t;
  int x; x=a -> exp; a -> exp=b -> exp; b ->exp=x;
  for (int i=0;i<=52;i++ )
  {
    int t=a -> frac[i];
    a -> frac[i]=b -> frac[i];
    b -> frac[i]=t;
   } 
}
void getrounding(int n)
{
  if (n<=52) return;
  if (!w[53]) return;
  bool flag=false;
  for (int i=53; i<=n; i++)
    if (w[i])
    { flag=true; break;}
  if (!flag && !w[52]) return;
  for (int i=52; i>=0; i--)
  {
    if (!w[i])
    { 
      w[i]=1;
      return;
    }  
    w[i]=0;
  }
}
void read(struct mydouble *a)
{
  int len=strlen(st+1)-1;
  int dot=len+1;
  for (int i=1;i<=len;i++)
    if (st[i]=='.')
    {
      dot=i; break;
    }
  int l;
  if (st[1]=='-')
   {
     a -> sign=1;
     l=2;
   }
  else
   {
     a -> sign=0;
     l=1;
   }     
  converse1(l,dot-1);
  converse2(dot+1,len);
  memset(ww,0,sizeof(ww));
  for (int i=1;i<=sum;i++) ww[i]=c[sum-i+1];
  for (int i=sum+1;i<=tot+sum;i++) ww[i]=d[i];
  if (sum-1+bias>expmax)
  {
    a -> exp=INT_MAX;
    return;
  }
  a -> exp=sum-1+bias;
  int j=1;
  while (!ww[j] && j<=tot+sum ) 
  {
    j++; a -> exp--;
  }
  if (a -> exp < (-52))
  {
     a -> exp=-53;
     memset(a -> frac,0,sizeof(a -> frac));
     return;
  }
  memset(w,0,sizeof(0));
  for (int i=j+1; i<=tot+sum; i++) w[i-j]=ww[i];
  getrounding(tot+sum-j); //NaN 的判断比较tricky，暂时不知道计算机内部是什么样的
  if (w[0]) a -> exp++;
  if (a -> exp>expmax)
  {
    a -> exp=INT_MAX;
    return;
  }
  bool flag=false;
  for (int i=1; i<=52; i++)
  {
   if (!w[i]) flag=true;
   a -> frac[i]=w[i];
  }
  if (a -> exp==expmax && flag)
  {
    a -> exp=INT_MAX;
    return;
  }
}
void work1(int n)
{
  for (int i=n; i>=1; i--)
  {
   if (ww[n]=2)
   {
     ww[n-1]++; ww[n]=0;
   }
  }
}
void work2(int n)
{
  for (int i=n; i>=1; i--)
  {
    if (ww[i]<0)
    {
      ww[i]=1; ww[i-1]--;
    } 
  }
}
bool check0(struct mydouble *a)
{
  if (a -> exp!=-53) return false; 
      else return true; 
}
bool checkmax(struct mydouble *a)
{
  if (a -> exp!=expmax) return false;
  for (int i=1; i<=52; i++)
    if (!(a -> frac[i])) return false; 
  return true; 
}
bool com(struct mydouble *a,struct mydouble *b)
{
  for (int i=1;i<=52;i++)
    if (a -> frac[i] != b -> frac[i])
      return ((a -> frac[i]) > (b -> frac[i]));
  return false;   
}



void myplus()
{
  if (a.exp==INT_MAX || b.exp==INT_MAX)
  {  
    ans.exp==INT_MAX;
    return;
  }
  if (check0(&a))
  {
    ans.sign=b.sign;
    ans.exp=b.exp;
    for (int i=1;i<=52;i++) ans.frac[i]=b.frac[i];
    return;
  }
  if (check0(&b))
  {
    ans.sign=a.sign;
    ans.exp=a.exp;
    for (int i=1;i<=52;i++) ans.frac[i]=a.frac[i];
    return;
  }
 if (checkmax(&a) || checkmax(&b))
 {
   ans.exp=INT_MAX;
   return;
 } 
 memset(w,0,sizeof(w));
 if (a.sign==b.sign)
 {
   ans.sign=a.sign;
   if (a.exp<b.exp) swap(&a,&b);
   ans.exp=a.exp;
   int p=a.exp-b.exp;
   memset(ww,0,sizeof(ww));
   ww[0]=1;
   for (int i=1; i<=52; i++) ww[i]=a.frac[i];
   ww[p-1]+=1;
   for (int i=1; i<=52; i++) ww[i+p]+=b.frac[i];
   work1(52+p);
   int s;
   if (ww[0]==1) 
     s=0;
   else if (ww[0]==2)
   {
     s=1; w[1]=0; 
     ans.exp++;
   }
   for (int i=1;i<=52+p;i++)
   {
     s++; w[s]=ww[i];
   }
   getrounding(s);
   if (w[0])
   { 
     ans.exp++;
     w[0]=0; w[1]=1;
   }  
   if (ans.exp>expmax)
   {
     ans.exp=INT_MAX;
     return;
   }
   for (int i=1; i<=52; i++) ans.frac[i]=w[i];
   if ((ans.exp==expmax)&&(!checkmax(&ans))) ans.exp=INT_MAX;
 }
 else
 {
   if (a.sign) swap(&a,&b);
   if ((b.exp>a.exp)||(b.exp==a.exp && com(&b, &a)))
      ans.sign=1;
   else ans.sign=0;
   if (a.exp<b.exp) swap(&a, &b);
    else if (a.exp==b.exp && com(&b, &a)) swap(&a, &b); 
   ans.exp=a.exp;
   int p=a.exp-b.exp;
   memset(ww,0,sizeof(ww));
   ww[0]=1;
   for (int i=1; i<=52; i++) ww[i]=a.frac[i];
   ww[p-1]-=1;
   for (int i=1; i<=52; i++) ww[i+p]-=b.frac[i];
   work2(52+p);
   if (!ww[0])
   {
      ans.exp--;
      int j=1;
      while (j<=52+p && !ww[j])
      { 
        j++;
        ans.exp--;
      }  
     
     if ((j>p+52) || (ans.exp<-53))  
     {
       ans.exp=-53; return;
     }
     for (int i=j; i<=p+52; i++) w[i-j+1]=ww[i];
     getrounding(p+53-j); 
   }  
   else
   {
     for (int i=1;i<=p+52; i++) w[i]=ww[i];
     getrounding(p+52);
   }
  for (int i=1;i<=52;i++) ans.frac[i]=w[i];
 }
}

void times()
{ 
  if (a.exp==INT_MAX || b.exp==INT_MAX)
  {
    ans.exp=INT_MAX;
    return;
  }
 ans.sign=(a.sign!=b.sign);
 ans.exp=a.exp+b.exp-bias;
 if (ans.exp<-52 || check0(&a) || check0(&b))
 {
   ans.exp=-53;
   return;
 }
 if (ans.exp>expmax)
 {
   ans.exp=INT_MAX; 
   return;
 }
 int64 digit=0;
 for (int i=1; i<=52; i++)
   digit=digit*2+a.frac[i];
 aa[1]=digit/size; aa[2]=digit%size;
 digit=0;
 for (int i=1; i<=52; i++)
   digit=digit*2+b.frac[i];
 bb[1]=digit/size; bb[2]=digit%size;
 x[1]=1; x[2]=aa[1]+bb[1]; 
 x[3]=bb[2]+aa[2]+aa[1]*bb[1]; 
 x[4]=aa[1]*bb[2]+bb[1]*aa[2];
 x[5]=aa[2]*bb[2];
 for (int i=5; i>=2; i--)
 {
   x[i-1]+=x[i] / size;
   x[i]=x[i] % size;
 }   
 int k=5;
 while (!x[k]) k--;
 int s=0;
 while (x[1]>1)
 {
   ans.exp++;
   x[k+1]=0;
   for (int i=1; i<=k; i++)
   {
     x[i+1]+=(x[i] & 1)* size;
     x[i]=x[i] >> 1;   
   }
   s++;
   if (x[k+1]) ww[s]=1; else ww[s]=0;
 } 
 if (ans.exp>expmax)
 {
   ans.exp=INT_MAX;
   return;
 }
 int j=2;
 while (j<=k)
 {
   x[k+1]=0;
   for (int i=j; i<=k; i++)
   {
     x[i+1]+=(x[i] & 1)*size;
     x[i]=x[i] >> 1;
   }
   s++;
   if (x[k+1]) ww[s]=1; else ww[s]=0;
   while (j<=k && !x[j]) j++; 
 }
 memset(w,0,sizeof(w));
 for (int i=1; i<=s; i++) w[i]=ww[s-i+1];
 getrounding(s);
 if (w[0]) ans.exp++;
 for (int i=1; i<=52; i++) ans.frac[i]=w[i];
 if ((ans.exp>expmax) || (ans.exp==expmax && !checkmax(& ans)))
    ans.exp=INT_MAX;
}

void did()
{
   if (a.exp==INT_MAX || b.exp==INT_MAX)
   {
     ans.exp=INT_MAX;
     return;
   }
  if (check0(&b))
  {
     ans.exp=INT_MAX;
     return;
  }  
  if (check0(&a))
  {
     ans.exp=-53;
     return;
  }
  ans.sign=(a.sign!=b.sign);
  ans.exp=(a.exp-b.exp+bias);
  if (ans.exp<-52)
  {
     ans.exp=-53;
     return;
  }
  int64 digit=0;
  for (int i=1; i<=52; i++)
   digit=(digit << 1)+a.frac[i];
  digit+=maxn+1;
  int64 y=0;
  for (int i=1; i<=52; i++)
   y=(y << 1)+b.frac[i];
  y+=maxn+1;
  int s=0; int sum=INT_MAX;
  bool flag=false;
  while (true)
  {
    if (s-sum+1>60) break;
    s++; ww[s]=digit/y;
    if (!flag && ww[s]) {flag=true; sum=s;}
    if (!flag) ans.exp--;
    if (ans.exp<-52) 
    {
      ans.exp=-53;
      return;
    }
    digit=(digit % y) << 1;
  } 
  memset(w,0,sizeof(w));
  for (int i=sum+1; i<=s; i++) w[i]=ww[i-sum];
  getrounding(s-sum);
  if (w[0]) ans.exp++;
  for (int i=1; i<=52; i++) ans.frac[i]=w[i];
  if ((ans.exp>expmax) || (ans.exp==expmax && ! checkmax(&ans)))
  {
    ans.exp=INT_MAX;
    return;
  }
}

void write()
{
  if (ans.exp==INT_MAX)
  {
    printf("%s\n", "NaN");
    return;
  }
  if (ans.exp==-53)
  {
    printf("%d\n", 0);
    return;
  }
  if (ans.sign) printf("%c\n", '-');
  memset(w,0,sizeof(w));
  if (ans.exp>=0)
     ans.frac[0]=1;
  else ans.frac[0]=0;   
  ans.exp-=bias;
  int s=0; int sum=0;
  if (ans.exp>=0)
  {
    s++; w[s]=ans.frac[0];
    for (int i=1; i<=min(ans.exp,52); i++)
    { 
      s++; w[s]=ans.frac[i];
    }
    for (int i=min(ans.exp,52)+1; i<=ans.exp; i++)
    {
      s++; w[s]=ans.frac[i];
    }
    for (int i=min(ans.exp,52)+1; i<=52; i++)
    {
      sum++; ww[sum]=ans.frac[i];
    }
  }
  else
  {
    s++; w[s]=0;
    for (int i=1; i<=-ans.exp-1; i++)
    {
      sum++; ww[sum]=0;
    }
    for (int i=0; i<=52; i++)
    {
      sum++; ww[sum]=ans.frac[i];
    }
  }
  double p=1;
  double finalans=0;
  for (int i=s; i>=1; i--)
  {
    if (w[i]) finalans+=p;
    p=p*2;
  }
  p=1;
  for (int i=1; i<=sum; i++)
  {
    p=p/2;
    if (ww[i]) finalans+=p; 
  }
 printf("%lf\n", finalans);
}

int main()
{
  for (int o=1;o<=10;o++)
  {
    scanf("%s", st+1);
    read(&a);
    char opt,ch;
    scanf("%c", &opt);
    scanf("%c", &opt);
    scanf("%c", &ch);
    scanf("%s", st+1);
    read(&b); 
    switch (opt)
    {
      case '+':
        myplus();
        break;
      case '-':
        b.sign^=1;
        myplus();
        break;
      case '*':
        times();
        break;
      case '/':
        did();
        break;    
    }
    write();    
  }
  system("pause");
  return 0;
}
