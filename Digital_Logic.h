//Digital_Logic.h
#ifndef DIGITAL_LOGIC_H
#define DIGITAL_LOGIC_H
#include <iostream>
#include <string>
#include <vector>
#include <queue>
#include <algorithm>
#include <sstream>
class Solution{
    public:
    void Telltime(int times){
        for(int i=0;i<times;i++){
            startup();
        }
        return;
    }
    private:
    struct stuff{
        std::string name;
        std::vector<int*> indata;
        int* outdata=new int(2);
        stuff(std::string s):name(s){}
    };
    struct connector{
        std::string name;
        int tag;
        std::vector<connector*> next;
        bool visted=false;
    };

    std::vector<stuff> Equip;
    std::vector<connector*> wibe; 
    std::vector<int*> invalue;
    std::vector<int*> outvalue;  //share the same order with Equip 

    void startup(){
        int a,b;
        std::cin>>a>>b;
        for(int i=0;i<a;i++){
            int* temp =new int(2);
            invalue.push_back(temp);
        }
        for(int i=0;i<b;i++){
            int* temp =new int(2);
            outvalue.push_back(temp);
        }
        std::cin.ignore();
        
        std::vector<std::string> construct;
        for(int i=0;i<b;i++){
            std::string s;
            std::getline(std::cin,s);
            construct.push_back(s);
        }
        for(int i=0;i<b;i++){
            Equip.push_back(stuff(""));
        }
        constructing(construct);
        worknext();
        clean();
        return;
    }
    class workpackage{
        public:
        int NOT(int* a){
            if(*(a)==0)return 1;
            else return 0;
        }
        int AND(std::vector<int*>& a){
            for(int* it:a){
                if(*(it)==0)return 0;
            }
            return 1;
        }
        int OR(std::vector<int*>& a){
            for(int *it:a){
                if(*(it)==1)return 1;
            }
            return 0;
        }
        int XOR(std::vector<int*>& a){
            int sum=0;
            for(int* it:a){
                sum+=*(it);
            }
            return sum%2;
        }
        int NAND(std::vector<int*>& a){
           if(AND(a)==1)return 0;
           else return 1;
        }
        int NOR(std::vector<int*>& a){
            if(OR(a)==1)return 0;
            else return 1;
        }
    };

    void clean(){
        int temp;
        connector tit;
        for(int* it:invalue)
        it=&temp;
        for(int* it:outvalue)
        it=&temp;
        for(connector* it:wibe)
        it=&tit;
    }

    void constructing(std::vector<std::string>& str){
        for(int i=0;i<str.size();i++){
            wibe.push_back(new connector);
            wibe[i]->tag=i;
        }
        for(int i=0;i<str.size();i++){
            std::stringstream ss(str[i]);
            std::string nam;
            ss>>nam;
            Equip[i].name=nam;
            int numeric;
            ss>>numeric;
            std::string ttemp;
            outvalue[i]=Equip[i].outdata;
            while(ss>>ttemp){
                if(ttemp[0]=='I'){
                    ttemp.erase(0,1);
                    int tag=std::stoi(ttemp);
                    tag--;
                    Equip[i].indata.push_back(invalue[tag]);
                }
                else {
                    ttemp.erase(0,1);
                    int tag=std::stoi(ttemp);
                    tag--;
                    Equip[i].indata.push_back(outvalue[tag]);
                    wibe[i]->next.push_back(wibe[tag]);
                }
            }
        }
        return;
    }

    void worknext(){
        int times;
        std::cin>>times;
        std::cin.ignore();
        std::vector<std::string> orderin,checkin;
        for(int i=0;i<times;i++){
            std::string s;
            std::getline(std::cin,s);
            orderin.push_back(s);
        }
        for(int j=0;j<times;j++){
            std::string s;
            std::getline(std::cin,s);
            checkin.push_back(s);
        }
        deepen(orderin,checkin);
        return;
    }


    void deepen(std::vector<std::string>& orderin,std::vector<std::string>& checkin){
        if(tellloop()){
            std::cout<<"LOOP"<<std::endl;
            return;
        }
        for(int i=0;i<orderin.size();i++){
            truework(orderin[i],checkin[i]);
            for(int j=0;j<outvalue.size();j++){
                *outvalue[j]=2;
            }
        }
        return;
    }

    void truework(std::string& order,std::string& check){
        int i=0,box;
        std::stringstream ss(order);
        while(ss>>box){
            *(invalue[i])=box;
            i++;
        }
        while(!checker()){
             for(stuff it: Equip){
                dothejob(it);
             }
        }
        findthevalue(check);
        return;
    }

    void findthevalue(std::string& check){
        std::stringstream ss(check);
        int times;
        ss>>times;
        for(int i=0;i<times;i++){
            int temp;
            ss>>temp;
            temp--;
            std::cout<<*(Equip[temp].outdata)<<' ';
        }
        std::cout<<std::endl;
        return;
    }   

     bool checker(){
        for(int* it:outvalue){
            if(*it!=0&&*it!=1)
            return false;
        }
        return true;
    }

    bool tellloop(){
       for(int i=0;i<wibe.size();i++){
            if(nodefind(wibe[i])){
                return true;}
       }
       return false;
    }

     bool nodefind(connector* now){
        connector* aim=now;
        std::queue<connector*> q;
        q.push(now);
        bool teller=false;
        while(!q.empty()){
            if(teller){
                if(q.front()==aim)return true;
            }
            teller=true;
            for(connector* thing:q.front()->next){
                if(!thing->visted){
                    thing->visted=true;
                    q.push(thing);
                }
            }
            q.pop();
        }
        return false;
    }

    void dothejob(stuff it){
        std::string chose=it.name;
        workpackage tool;
        for(int i=0;i<it.indata.size();i++){
            if(*(it.indata[i])==2)return;
        }
        int value;
        if(chose=="NOT")value=tool.NOT(it.indata[0]);
        else if(chose=="AND")value=tool.AND(it.indata);
        else if(chose=="OR")value=tool.OR(it.indata);
        else if(chose=="XOR")value=tool.XOR(it.indata);
        else if(chose=="NAND")value=tool.NAND(it.indata);
        else if(chose=="NOR")value=tool.NOR(it.indata);
        *(it.outdata)=value;
        return;
    }

};
#endif