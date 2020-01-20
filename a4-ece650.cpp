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
/*
void remove_edge(vector<int> adj[], int src, int dest) 
{ 
    adj[src].pop_back(); 
    adj[dest].pop_back(); 
}
*/
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
    Minisat::Solver solver;
    std::vector<std::string> stringVectorNumbers;
    std::vector<int> found;
    std::string line;
    int V;
    std::vector<int> x;
    std::vector<int> y;
    std::vector<int> intVectorNumbers;
    while (std::getline(std::cin, line))
    {
        if(cin.eof())
        {
            exit(0);
        }
        if(line.find("V")!= string::npos)
        {   
            found=extractVertices(line);  
            V=found[0];
            stringVectorNumbers.clear();
            intVectorNumbers.clear();
            cout<<line<<endl;
        }
        else
        {
            cout<<line<<endl;
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
                    x.push_back((int)intVectorNumbers[i]);
                    y.push_back((int)intVectorNumbers[i+1]);
                }
            }
        
        int low=0;
        int high=V;
        int k_mid;
        while (low<=high)
        {
            std::vector<std::vector<Minisat::Lit>> literals(V);
            k_mid=(low+high)/2;
            //k_mid=high;
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
                Minisat::vec<Minisat::Lit> lr1;
                for(int v=0;v<V;v++)
                {
                    lr1.push(literals[v][i]);
                }
                solver.addClause(lr1);
                lr1.clear();
            }
            //No one vertex can appear twice in a vertex cover
            for(int i=0;i<V;i++)
            {
                Minisat::vec<Minisat::Lit> lr1;
                for(int p=0;p<k_mid;p++)
                {
                    for(int q=p+1;q<k_mid;q++)
                    {
                        lr1.push(~literals[i][p]);
                        lr1.push(~literals[i][q]);
                    }
                }
                solver.addClause(lr1); //addClause(lr1,lr2)
                lr1.clear();
            }
            //No more than one vertex appears in the mth position
            for(int m=0;m<k_mid;m++)
            {
                Minisat::vec<Minisat::Lit> lr1;
                for(int p=0;p<V;p++)
                {
                    for(int q=p+1;q<V;q++)
                    {
                        lr1.push(~literals[p][m]);
                        lr1.push(~literals[q][m]);
                    }
                }
                solver.addClause(lr1);
                lr1.clear();
            }
            //Every edge is incident to atleast one vertex in the vertex cover
            Minisat::vec<Minisat::Lit> lr1;
            for(int k=0;k<k_mid;k++)
            {
                for(std::vector<int>::size_type i =0 ; i!= x.size() ; i++)
                {
                    lr1.push(literals[i][k]);
                }
                for(std::vector<int>::size_type j =0 ; j!= y.size() ; j++)
                {
                    lr1.push(literals[j][k]);
                }
                solver.addClause(lr1);
                lr1.clear();
            }  
            // Check for solution and retrieve model if found
            auto sat = solver.solve();
            if(sat==true)
            {
               // high=k_mid;
                for (int row = 0; row < V; ++row)
                {
                    for (int col = 0; col < k_mid; ++col) 
                    {
                        if (solver.modelValue(row)==Minisat::l_True) 
                        {
                            std::clog<<"row:"<<row<<<<endl;
                            //cout<<"cc:"<<literals[row]<<endl;
                            //break;
                        }
                    }
                }
                high =k_mid-1;
            }
            else 
            {
                std::clog << "UNSAT\n";
                low=k_mid+1;
            }
            solver.reset(new Minisat::Solver());
            //low++;
            //high--;
            cout<<"End of while loop"<<endl;
        }
        /*auto sat = solver.solve();

        if(sat==true)
        {
            //for(int i=0;i<V;i++)
            {
                for()
                {
                    if(solver.modelValue(x[i][j])==Minisat::l_True)
                    {
                        std::clog<<i;
                        break;
                    }
                }
            }
            high=k-1;
        }
        else
        {
            std::clog << "UNSAT\n";
            cout<<"Outside of while loop"<<endl;
            low=k+1;
        }
        /*for(std::vector<int>::size_type i =0 ; i!= intVectorNumbers.size() ; i++) 
        {
            if((i%2)==0)
                {
                    x=(int)intVectorNumbers[i];
                    y=(int)intVectorNumbers[i+1];
                    //mremove_edge(adj, x, y); 
                }
        }*/
    }
    return 0;
}