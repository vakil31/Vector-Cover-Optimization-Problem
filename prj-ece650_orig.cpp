#include "minisat/core/SolverTypes.h"
#include "minisat/core/Solver.h"
#include<iostream>
#include <list> 
#include <climits>
#include <vector>  
#include <string>
#include <sstream>  
#include <utility>
using namespace std;  
vector<int> x;
vector<int> y;
void add_edge(vector<int> adj[], int src, int dest) 
{ 
    adj[src].push_back(dest); 
    adj[dest].push_back(src); 
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
void CNF_SAT_VC(int V)
{
    std::vector<int> vc;
    //vc.clear();
    int low=0;
    int high=V;
    int k_mid;
    while (low<=high)
    {
        Minisat::Solver solver;
        std::vector<std::vector<Minisat::Lit>> literals(V);
        k_mid=(low+high)/2;
        for(int i=0;i<V;i++)
        {
            for(int j=0;j<k_mid;j++)
            {
                Minisat::Lit l1= Minisat::mkLit(solver.newVar());
                literals[i].push_back(l1);
            }
        }
        //At least one vertex is the ith vertex in the vertex cover
        for(int i=0;i<k_mid;i++)
        {
            Minisat::vec<Minisat::Lit> l_temp;
            for(int v=0;v<V;v++)
            {
                l_temp.push(literals[v][i]);
            }   
            solver.addClause(l_temp);
            l_temp.clear();
        }
        //No one vertex can appear twice in a vertex cover
        for(int i=0;i<V;i++)
        {
            for(int p=0;p<k_mid;p++) 
            {
                for(int q=p+1;q<k_mid;q++)
                {
                    solver.addClause(~literals[i][p],~literals[i][q]);
                }
            }
        }
        //No more than one vertex appears in the mth position
        for(int m=0;m<k_mid;m++) //Later changed
        {
            for(int p=0;p<V;p++) //Changed
            {
                for(int q=p+1;q<V;q++)
                {
                    solver.addClause(~literals[p][m],~literals[q][m]);
                }
            }
        }
        //Every edge is incident to atleast one vertex in the vertex cover
        for(int edge=0;edge<x.size();edge++)
        {
            Minisat::vec<Minisat::Lit> l_temp;
            for(int k =0 ; k<k_mid  ; k++) 
            {
                l_temp.push(literals[x[edge]][k]);
                l_temp.push(literals[y[edge]][k]);
            }
            solver.addClause(l_temp);
            l_temp.clear();
        }  
        // Check for solution and retrieve model if found
        bool sat = solver.solve();
        if(sat==true)
        {
            vc.clear();          
            for (int row = 0; row < V;row++)
            {
                for (int col = 0; col < k_mid;col++) 
                {
                    if (solver.modelValue(literals[row][col])==Minisat::l_True) 
                    {
                        vc.push_back(row);
                    }
                }
            }
            high=k_mid-1;
        }
        else 
        {
            low=k_mid+1;
        }
    }
    cout<<"CNF-SAT-VC: ";
    for(int i=0;i<vc.size();i++)
    {
        if(i!=vc.size()-1)
        {
            cout<<vc[i]<<",";
        }
        else
            cout<<vc[i];
    }
    cout<<endl;
}
 
void APPROX_VC_1(vector<int> adj[],int V)
{
    // Initialize all vertices as not visited. 
    bool visited[V]; 
    for (int u=0; u<V; u++) 
    {
        visited[u]=false;
        for (int i = 0; i < adj[u].size(); i++) 
        {
            visited[adj[u][i]] = false;
        }
    }
    int max=0;
    vector<int> result; 
    vector<int> final_result;
    for (int uo=0; uo<V; uo++)
    {
        max=0;
        int count[100]={0};
        for (int u=0; u<V; u++) 
        {
            if(visited[u]==false)
            {
                for (int i = 0; i < adj[u].size(); i++) 
                { 
                    if (visited[adj[u][i]] == false) 
                    {
                        count[adj[u][i]]++;
                        if (count[adj[u][i]] > max) 
                        { 
                            max = count[adj[u][i]]; 
                            result.push_back(adj[u][i]);
                        }   
                    } 
                } 
            }
        } 
        if(result.size()==0)
        {
            break;
        }
        else
        {
            int result1=result[result.size()-1];
            result.clear();
            visited[result1]=true;
            for(int i=0;i<adj[result1].size();i++)
            {
                adj[result1].pop_back();
            }
            final_result.push_back(result1);
        }
    }
    // Print the vertex cover 
    cout<<"APPROX-VC-1: ";
    for(int i=0;i<final_result.size();i++)
    {
        if(i!=final_result.size()-1)
        {
            cout<<final_result[i]<<",";
        }
        else
        {
            cout<<final_result[i];
        }
    }
    cout<<endl;
}
void APPROX_VC_2(vector<int> adj[],int V)
{
// Initialize all vertices as not visited. 
     vector<int> result; 
    bool visited[V]; 
    for (int i=0; i<V; i++) 
        visited[i] = false; 
  
    //list<int>::iterator i; 
  
    // Consider all edges one by one 
    for (int u=0; u<V; u++) 
    { 
        // An edge is only picked when both visited[u] and visited[v] 
        // are false 
        if (visited[u] == false) 
        { 
            // Go through all adjacents of u and pick the first not 
            // yet visited vertex (We are basically picking an edge 
            // (u, v) from remaining edges. 
            int x=adj[u].size();
            for (int i=0;i<x;++i) 
            { 
                //int v = i; 
                if (visited[adj[u][i]] == false) 
                { 
                     // Add the vertices (u, v) to the result set. 
                     // We make the vertex u and v visited so that 
                     // all edges from/to them would be ignored 
                    int src=u;
                    int dest=adj[u][i];
                    result.push_back(src);
                    result.push_back(dest); 
                    visited[adj[u][i]] = true; 
                    visited[u]  = true; 
                    break; 
                } 
            } 
        } 
    } 
  
    // Print the vertex cover V
    cout<<"APPROX-VC-2: ";
    for(int i=0;i<result.size();i++)
    {
        if(i!=result.size()-1)
        {
            cout<<result[i]<<",";
        }
        else
        {
            cout<<result[i];
        }
    }
    cout<<endl;
} 
int main()
{
    std::vector<std::string> stringVectorNumbers;
    std::vector<int> found;
    std::string line;
    int V;
    //std::vector<int> x;
    //std::vector<int> y;
    std::vector<int> vc;
    std::vector<int> intVectorNumbers;
    int flag=0;
    while (std::getline(std::cin, line))
    {
        if(cin.eof())
        {
            exit(0);
        }
        if(line.find("V")!= string::npos)
        {   
            vc.clear();
            found=extractVertices(line);  
            V=found[0];
            stringVectorNumbers.clear();
            intVectorNumbers.clear();
            x.clear();
            y.clear();
        }
        vector<int> adj[V]; 
        if(line.find("E")!= string::npos)
        {
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
                int x2 = 0;
                parser >> x2;
                intVectorNumbers.push_back(x2);
            }
            for(std::vector<int>::size_type i = 0; i != intVectorNumbers.size(); i++) 
            {
                if((i%2)==0)
                {
                    int x1=(int)intVectorNumbers[i];
                    int y1=(int)intVectorNumbers[i+1];
                    if(y1 < V && x1<V && x1!=y1)
                    {
                        x.push_back((int)intVectorNumbers[i]);
                        y.push_back((int)intVectorNumbers[i+1]);
                        add_edge(adj, x1, y1); 
                        flag=1;
                    }
                    else
                    {
                        cerr<<"Error: Vertex does not exist";
                    }
                }
            }
            vc.clear();
            if(flag==1)
            {
                CNF_SAT_VC(V);
                APPROX_VC_1(adj,V);
                APPROX_VC_2(adj,V);
                flag=0;
            }
        }
    }
return 0;
}
