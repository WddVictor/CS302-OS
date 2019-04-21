#include <iostream>
#include <map>
#include <string>

using namespace std;

map<int,int*> mapAlloc;
map<int,int*> mapMax;
map<int,bool> mapFinish;
int *remain,*init;


int n;

void whether_ok();
bool do_terminate(int);
bool do_new(int);
bool do_request(int);
bool check_current();
bool need_less_request(int,const int *);
bool need_less_work(int,const int*);
void free_map(map<int,int*>);

int main(){
    cin >> n;
    remain = new int[n];
    init = new int[n];

    for(int i = 0;i<n;i++){
        cin >> remain[i];
        init[i] = remain[i];
    }

    whether_ok();

    free_map(mapAlloc);
    free_map(mapMax);

    delete []remain;
    delete []init;

    return 0;
}

//whether the command is safe for being executed
void whether_ok(){
    int id;
    bool ok;
    string cmd;

    while((cin >> id)&&(cin >> cmd)){
        ok = false;
        if(cmd == "new"){
            ok = do_new(id);
        }else if(cmd == "request"){
            ok = do_request(id);
        }else if(cmd == "terminate"){
            ok = do_terminate(id);
        }

        cout << (ok?"OK":"NOT OK") << endl;

    }
}

bool do_terminate(int id){
    if(mapMax.find(id)!=mapMax.end()){
        for(int i = 0;i<n;i++)
            remain[i] += mapAlloc[id][i];

        delete [](mapMax[id]);
        delete [](mapAlloc[id]);
        mapAlloc.erase(id);
        mapMax.erase(id);
        mapFinish.erase(id);
        return true;
    }else{
        return false;
    }
}

bool do_new(int id){

    int * request = new int[n];
    for(int i =0;i<n;i++){
        cin >> request[i];
    }

    if(mapMax.find(id)!=mapMax.end()){
        delete []request;
        return false;
    }

    for(int i = 0;i<n;i++){
        //the request should be less than the total amount of resouces
        if(request[i]>init[i]){
            delete []request;
            return false;
        }
    }

    mapAlloc[id] = new int[n];
    mapMax[id] = new int[n];
    mapFinish[id] = false;


    
    for(int i = 0;i<n;i++){
        mapMax[id][i] = request[i];

        //initialize the allocation of a new process
        mapAlloc[id][i] = 0;
    }

    return true;
}

bool do_request(int id){

    int* request = new int[n];
    for(int i =0;i<n;i++){
        cin >> request[i];
    }

    if(!mapMax[id])
        return false;

    for(int i = 0;i<n;i++){
        if(remain[i]<request[i])
            return false;
    }


    if(need_less_request(id,request))
        return false;

    //assume the allocation is successful
    for(int i = 0;i<n;i++){
        remain[i] -= request[i];
        mapAlloc[id][i] += request[i];
    }

    if(check_current()){
        return true;
    }else{
        //recall the allocation
        for(int i = 0;i<n;i++){
            remain[i] += request[i];
            mapAlloc[id][i] -= request[i];
        }
        return false;
    }
}

// release the space used by maps
void free_map(map<int,int*> m){
    for(auto it = m.begin();it!=m.end();it++){
        if(it->second){
            delete []it->second;
            it->second = NULL;
        }
    }
    m.clear();
}

//check whether the current status is safe or not
bool check_current(){
    bool found;

    // the available resources who are allocated or remained.
    int *work = new int[n];

    for(int i = 0;i<n;i++)
        work[i] = remain[i];

    // initialize the finish map
    for(auto iter = mapFinish.begin(); iter != mapFinish.end(); iter++)
        mapFinish[iter->first] = false;

    do{
        found = false;
        for(auto iter = mapAlloc.begin(); iter != mapAlloc.end(); iter++) {
            int id = iter->first;
            int *allocate = iter->second;

            // the system is safe if a process can finish correctly
            if(!mapFinish[id]&&need_less_work(id,work)){
                for(int i = 0;i<n;i++)
                    work[i] += allocate[i];

                mapFinish[id] = true;
                found = true;
            }
        }
    }while(found);

    //check whether there are a deadlock
    for(auto iter = mapFinish.begin(); iter != mapFinish.end(); iter++){
        if(!iter->second)
            return false;
    }
    return true;
}

// whether needed resources are less than available resources
bool need_less_work(int id,const int *work){
    for(int i =0;i<n;i++){
        int need = mapMax[id][i]-mapAlloc[id][i];
        if(need>work[i])
            return false;
    }
    return true;
}

// whether needed resources are less than required resources
bool need_less_request(int id,const int *request){
    for(int i =0;i<n;i++){
        int need = mapMax[id][i]-mapAlloc[id][i];
        if(need>request[i]) {
            return false;
        }
    }
    return true;
}