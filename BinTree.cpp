#include <iostream>
#include <vector>
#include <time.h>
#include <thread>
#include <mutex>

using namespace std;


 vector<thread*> Ts;
int repetidos;
template<class T>
struct CNODE
{
    T m_data;
    CNODE<T>* m_child[2];
    mutex mtx;
    CNODE(T x)
    {
        m_data=x;
    }
    bool m_insert( T x)
    {
        int h;
        if(x>m_data)h=1; else h=0;
        mtx.lock();
        if(!m_child[h])
        {
            cout<<"INSERTANDO     "<<x<<endl;
            for(int i=0;i<10000;i++);
            CNODE<T> *t =new CNODE<T>(x);
            m_child[h]=t;
            cout<<"FIN INSERTANDO "<<x<<endl;
            mtx.unlock();
            return 1;
        }

        mtx.unlock();
        return 0;
    }
};
template <class T>
class CBINTREE
{
    public:
    CNODE<T>* m_root=0;
    mutex amtx;
    bool m_find(T x, CNODE<T>** &p)//devolver nodo
    {
//        for(p=&m_root;(*p)&& x!= (*p)->m_data;p=&(*p)->m_child[x>(*p)->m_data]);

        for(p=&m_root;(*p) && ( (*p)->m_child[x>(*p)->m_data] ) && x!=(*p)->m_data ; p=&(*p)->m_child[x>(*p)->m_data]);
        if((*p)->m_data==x)return 1;
        return 0;
    }
    bool m_insert(T x)
    {
        CNODE<T> **p;

        if(!(m_root)) //insertar raiz
        {
            amtx.lock();
            CNODE<T> *t =new CNODE<T>(x);
            m_root=t;
            amtx.unlock();
            return 1;
        }

        if(m_find(x,p)){repetidos++; return 0;}

        if((*p)->m_insert(x))return 1;
        if(m_insert(x))return 1;
        return 0;
    }
    bool m_remove(T x)
    {
        CNODE<T>**p,**q;
        if(!m_find(x,p)) return 0;
        if((*p)->m_child[0]&&(*p)->m_child[1])
        {
            for(q=&(*p)->m_child[0];(*q)->m_child[1];q=&(*q)->m_child[1]);
            (*p)->m_data=(*q)->m_data;
            p=q;
        }
        CNODE<T> *temp;
        temp=(*p);
        (*p)=(*p)->m_child[!!(*p)->m_child[1]];
        delete temp;
        return 1;
    }
    void m_enlistar() //xD
    {
        CNODE<T> **p,**d,**q;
        p=&m_root;
        while((*p)->m_child[0]||(*p)->m_child[1])
        {
            d=&(*p)->m_child[1];
            for(q=&(*p)->m_child[0];(*q);q=&(*q)->m_child[0]);
            *q=*d;
            (*p)->m_child[1]=0;
            p=&(*p)->m_child[0];
        }
    }
};
void imprimir(CNODE<int>*q)
{

    vector<CNODE<int>*> hijos;
    hijos.push_back(q);
    CNODE<int>*d;
    int i=0;
    do{

        cout<<i<<" nodo: "<<q->m_data<<"  hijos: ";
        i++;
        d=q->m_child[1];
        if(d)
        {
            hijos.push_back(d);
        }
        if(q->m_child[0])
        {
            q=q->m_child[0];
            cout<<"izquierdo: "<<q->m_data<<"    ";
        }
        else
        {
            q=hijos.back();
            hijos.pop_back();
            cout<<"izquierdo: NULL ";
        }
        if(d)
        {
            cout<<" derecho: "<<d->m_data<<endl;
        }
        else cout<<" derecho: NULL  "<<endl;

    }while(hijos.size()!=0);
}
template <class T>
void holi(CBINTREE<T> * arbol)
{
    int n;
    for(int i=0;i<10000; i++)
    {
        n=rand()%100000;
        arbol->m_insert(n);
    }

}

int main()
{
    rand();
    cout << "Arbol Binario" << endl;
    CBINTREE<int> Arbol;

    int n;
//    Arbol.m_insert(10);
//    imprimir(Arbol.m_root);
//    cin>>n;
    int t=4;
    for(int i=0;i<t; i++)
    {

        Ts.push_back(new thread (holi<int>,&Arbol));

    }
//    Ts.push_back(new thread (holi<int>,&Arbol,4));
//    Ts.push_back(new thread (holi<int>,&Arbol,2));
//    Ts.push_back(new thread (holi<int>,&Arbol,6));
//    Ts.push_back(new thread (holi<int>,&Arbol,3));
//    Ts.push_back(new thread (holi<int>,&Arbol,1));
//    Ts.push_back(new thread (holi<int>,&Arbol,7));
//    Ts.push_back(new thread (holi<int>,&Arbol,5));
//    Ts.push_back(new thread (holi<int>,&Arbol,8));

    for(int i=0;i<t;i++)
    {
        Ts[i]->join();

    }
//
//    Arbol.m_insert(2);
//    Arbol.m_insert(6);
//    Arbol.m_insert(3);
//    Arbol.m_insert(1);
//    Arbol.m_insert(7);
//    Arbol.m_insert(5);
//    Arbol.m_insert(8);

    imprimir(Arbol.m_root);
    cout<<"repetidos: "<<repetidos<<endl;
//    Arbol.m_remove(10);
//     cout<<"Arbol en lista"<<endl;
//    Arbol.m_enlistar();
//    imprimir(Arbol.m_root);
    return 0;
}
