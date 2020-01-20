#include <iostream> 
#include <list> 
#include <climits>
#include <vector>  
#include <string>
#include <sstream>  
#include <fstream>
#include <utility>
using namespace std; 
        
void add_edge(vector<int> adj[], int src, int dest) 
{ 
    adj[src].push_back(dest); 
    adj[dest].push_back(src); 
}   
void remove_edge(vector<int> adj[], int src, int dest) 
{ 
    adj[src].pop_back(); 
    adj[dest].pop_back(); 
}
bool BFS(vector<int> adj[], int src, int dest, int v, int pred[], int dist[]) 
{ 
    list<int> queue; 
    bool visited[v]; 
    for (int i = 0; i < v; i++) {
        visited[i] = false; 
        dist[i] = INT_MAX; 
        pred[i] = -1; 
    } 
    visited[src] = true; 
    dist[src] = 0; 
    queue.push_back(src);   
    while (!queue.empty()) { 
        int u = queue.front(); 
        queue.pop_front(); 
        for (int i = 0; i < adj[u].size(); i++) 
        { 
            if (visited[adj[u][i]] == false) 
            { 
                visited[adj[u][i]] = true; 
                dist[adj[u][i]] = dist[u] + 1; 
                pred[adj[u][i]] = u; 
                queue.push_back(adj[u][i]); 
                if (adj[u][i] == dest) 
                   return true; 
            } 
        } 
    } 
    return false; 
}   
void printShortestDistance(vector<int> adj[], int s,int dest, int v) 
{ 
    int pred[v], dist[v]; 
    if (BFS(adj, s, dest, v, pred, dist) == false) 
    { 
        cerr << "Error:Given source and destination are not connected"; 
        fflush(stdout);
        return; 
    }   
    vector<int> path; 
    int d1 = dest; 
    path.push_back(d1); 
    while (pred[d1] != -1) { 
        path.push_back(pred[d1]); 
        d1 = pred[d1]; 
    }   
    for (int i = path.size() - 1; i >= 0; i--) 
    {
        if(i!=0)
        {
            cout << path[i] << "-"; 
            fflush(stdout);
        }
        else
        {
            cout << path[i];
            fflush(stdout);
        }
    }
    cout<<"\n";
    fflush(stdout);
}   
vector<int> extractVertices(string str) 
{ 
    stringstream ss;     
    ss << str; 
    string temp; 
    std::vector<int> found_to_return;
    int found;
    while (!ss.eof()) 
    { 
        ss >> temp; 
        if (stringstream(temp) >> found) 
        {
            found_to_return.push_back(found);
        }
        temp = ""; 
    } 
    return found_to_return;
} 
int main() 
{ 
    std::vector<std::string> stringVectorNumbers;
    std::vector<int> found;
    std::string line;
    int v,x,y,source,dest;
    int flag=0;
    //int flag1=0;
    std::vector<int> intVectorNumbers;
    while (std::getline(std::cin, line))
    {
        fflush(stdin);
        if(cin.eof())
        {
            exit(0);
        }
        if(line.find("V")!= string::npos)
        {   
            found=extractVertices(line);  
            v=found[0];
            stringVectorNumbers.clear();
            intVectorNumbers.clear();
            cout<<line<<endl;
            fflush(stdout);
        }
        vector<int> adj[v]; 
        if(line.find("E")!= string::npos)
        {
            cout<<line<<endl;
            fflush(stdout);
            std::size_t prev = 0, pos;
            while ((pos = line.find_first_of("E, ,{,<,>,}", prev)) != std::string::npos)
            {
                if (pos > prev)
                {
                    stringVectorNumbers.push_back(line.substr(prev, pos-prev));
                }
                prev = pos+1;
            }
            if (prev < line.length())
                stringVectorNumbers.push_back(line.substr(prev, std::string::npos));
            for (auto &s : stringVectorNumbers) 
            {
                std::stringstream parser(s);
                int x = 0;
                parser >> x;
                intVectorNumbers.push_back(x);
            }
            for(std::vector<int>::size_type i = 0; i != intVectorNumbers.size(); i++) 
            {
                if((i%2)==0)
                {
                    x=(int)intVectorNumbers[i];
                    y=(int)intVectorNumbers[i+1];
                    if(y < v && x<v && x!=y)
                    {
                        //flag1=1;
                    }
                    else
                    {
                        cerr<<"Error: Vertex does not exist"<<endl;
                        fflush(stdout);
                        flag=1;
                    }
                }
            }
            if (flag==1)
            {
                stringVectorNumbers.clear();
                intVectorNumbers.clear();
                flag=0;
            }
            /*if (flag1==1)
            {
                //cout<<line<<endl;
                //fflush(stdout);
                flag1=0;
            }*/
        }
        if(line.find("s")!= string::npos)
        {
            for(std::vector<int>::size_type i = 0; i != intVectorNumbers.size(); i++) 
            {
                if((i%2)==0)
                {
                    x=(int)intVectorNumbers[i];
                    y=(int)intVectorNumbers[i+1];
                    if(y < v && x<v && x!=y)
                    {
                        add_edge(adj, x, y); 
                    }
                    else
                    {
                        cerr<<"Error: Vertex does not exist"<<endl;
                        fflush(stdout);
                    }
                }
            }
            found=extractVertices(line);
            source=found[0];
            dest=found[1];
            if(source < v && dest<v)
            {
                if(source==dest)
                {
                    cout<<source<<"\n";
                    fflush(stdout);
                }
                else
                {
                    printShortestDistance(adj, source, dest, v);
                }
                for(std::vector<int>::size_type i =0 ; i!= intVectorNumbers.size() ; i++) 
                {
                    if((i%2)==0)
                    {
                        x=(int)intVectorNumbers[i];
                        y=(int)intVectorNumbers[i+1];
                        remove_edge(adj, x, y); 
                    }
                }
            }
            else
            {
                cerr<<"Error: Shortest path to the vertex that does not exist"<<endl;
                fflush(stdout);
            }
        }    
    }
    return 0; 
}
