// an example of reading random numbers from /dev/urandom
// https://stackoverflow.com/questions/35726331/c-extracting-random-numbers-from-dev-urandom
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <time.h>       /* time */
#include <sys/wait.h>
#include <unistd.h>
#include<sstream>
#include <string>
#include <vector>
using namespace std;
int lines;
int streets;
struct Line
{
    struct Point
    {
        int x;
        int y;
    };
    Point p1;
    Point p2;
};
struct Street_point
{
    string street_name;
    Line l;
};

/*int check(int coor[lines][2],int x, int y)
{
    if(x!=y)
        bool condition2=true;
    else
        bool condition2=false;
    for (int j=0;j<lines-1;j++)
    {
        for (int i=0;i<2;i++)
        {
            if((coor[j][0]<=x<=coor[j+1][0] or coor[j][0]>=x>=coor[j+1][0]) and (coor[j][1]<=y<=coor[j+1][1] or coor[j][1]>=y>=coor[j+1][1]))
            {
                //bool condition1=false;
                return 0;
            }
            else 
                bool condition1=true;
        }
    }
}*/
int generate_number(int start,int end)
{
    unsigned int num;
    // open /dev/urandom to read
    std::ifstream urandom("/dev/urandom");

    // check that it did not fail
    if (urandom.fail())
    {
        std::cerr << "Error: cannot open /dev/urandom\n";
        //return 1;
    }
    urandom.read((char*)&num, sizeof(int));
    int rand_number= start + ( num % ( end - start + 1 ) );
    return rand_number;
 
}
double max(double a,double b)
{
    if(a>b) return a;
    else return b;
}
double min(double a,double b)
{
    if(a>b) return b;
    else return a;
}
bool intersection(double x11,double y11,double x12,double y12,double x21,double y21,double x22,double y22)
{
    double xnum = ((x11 * y12 - y11 * x12) * (x21 - x22) - (x11 - x12) * (x21 * y22 - y21 * x22));
    double xden = ((x11 - x12) * (y21 - y22) - (y11 - y12) * (x21 - x22));
    double ynum = ((x11 * y12 - y11 * x12) * (y21 - y22) - (y11 - y12) * (x21 * y22 - y21 * x22));
    double yden = ((x11 - x12) * (y21 - y22) - (y11 - y12) * (x21 - x22));
    if (xden == 0 || yden == 0)
    {
        return false;
    }
    else
    {
        double xcoor = xnum / xden;
        double ycoor = ynum / yden;
        if(((xcoor >= min(x11,x12)) && (xcoor <= max(x11,x12))) && ((ycoor >=min(y11,y12)) && (ycoor <= max(y11,y12))) && ((xcoor >= min(x21,x22)) && (xcoor <= max(x21,x22))) && ((ycoor >= min(y21,y22)) && (ycoor <= max(y21,y22))))
        {
            return true;
        }
        else
        {
            return false;
        }
    }
}    
int main(int argc, char **argv)
{
    std::string svalue;
    int sint_value=10;
    std::string nvalue;
    int nint_value=5;
    std::string lvalue;
    int lint_value=5;
    std::string cvalue;
    int cint_value=20;
    int index;
    int c;

    opterr = 0;

    // expected options are '-s', '-n', and '-l value'
    while ((c = getopt (argc, argv, "s:n:l:c:")) != -1)
        switch (c)
        {
        case 's':
            svalue = optarg;
            sint_value = atoi(svalue.c_str());
            if(sint_value<2)
            {
                cerr << "Error: 's' value cannot be less than 2." << endl;
                exit(1);
            }
            break;
        case 'n':
            nvalue = optarg;
            nint_value = atoi(nvalue.c_str());
            if(nint_value<1)
            {
                cerr << "Error: 'n' value cannot be less than 1." << endl;
                exit(1);
            }
            break;
        case 'l':
            lvalue = optarg;
            lint_value = atoi(lvalue.c_str());
            if(lint_value<5)
            {
                cerr << "Error: 'l' value cannot be less than 5." << endl;
                exit(1);
            }
            break;
        case 'c':
            cvalue = optarg;
            cint_value = atoi(cvalue.c_str());
            break;
        case '?':
            if (optopt=='s' || optopt=='n' || optopt == 'l' || optopt=='c')
            {
                std::cerr << "Error: option -" << char(optopt)<< " requires an argument." << std::endl;
                exit(1);
            }
            else
            {
                std::cerr << "Error: unknown option: " << char(optopt) << std::endl;
                exit(1);
            }
            return 1;
        default:
            return 0;
        }
    if (optind < argc)
    {
        std::cerr << "Found positional arguments\n";
        for (index = optind; index < argc; index++)
        {
            std::cerr << "Non-option argument: " << argv[index] << "\n";
        }
        exit(1);
    }
    vector<Line> l;
    vector<Street_point> s;
    //vector<Point> p;
    while(true)
    {
        int streets=generate_number(2,sint_value);
        //std::cout << "Streets: " << streets << "\n";
        int range=generate_number(1,cint_value);
        //std::cout << "Range: " << range << "\n"; 
        int lines_max=lint_value; 
        int error_counter=0;
        string output;
        int alphabet1=65;
        int alphabet2=65;
        int alphabet3=65;
        if(!s.empty())
        {
            for(std::vector<Street_point>::size_type h = 0; h != s.size(); h++)
            {
                cout<<"r "<<"\""<<s[h].street_name<<"\""<<endl;
            }
            s.clear();
        }
        for(int i=0;i<streets;i++)
        {
            char a1=static_cast<char>(alphabet1);
            char a2=static_cast<char>(alphabet2);
            char a3=static_cast<char>(alphabet3);
            string street_name = string(1,a1) + string(1,a2)+string(1,a3);
            //Push back new Street_point created with default constructor.
            s.push_back(Street_point());
            //Vector now has 1 element @ index 0, so modify it.
            s[i].street_name=street_name;
            //cout<<"Street_name:"<<s[i].street_name<<endl;
            output = string("a \"") + s[i].street_name + "\" " ;
            lines=generate_number(1,lines_max);
            //std::cout << "Lines: " << lines << "\n";
            l.clear();
            for(int j=0;j<lines;j++)
            {
                //cout<<"Before A:"<<j<<" iteration"<<endl;
                A:
                //cout<<"After A:"<<j<<" iteration"<<endl;
                int x1=generate_number(-range,range);
                int y1=generate_number(-range,range);
                int x2=generate_number(-range,range);
                int y2=generate_number(-range,range);
                if(x1!=x2 && y1!=y2)
                {
                    int x11=x1;
                    int y11=y1;
                    int x12=x2;
                    int y12=y2;
                    if(j>=1)
                    {
                        
                            int x21=l[j-1].p1.x;
                            int y21=l[j-1].p1.y;
                            int x22=l[j-1].p2.x;
                            int y22=l[j-1].p2.y;
                            if(x11!=x21 && y11!=y21 && x12!=x22 && y12!=y22)
                            {
                                if(!intersection(x11, y11, x12, y12, x21, y21, x22, y22))
                                {
                                    //cout<<" "<<x11<<" "<< y11<<" "<< x12<<" "<< y12<<" "<< x21<<" "<< y21<<" "<< x22<<" "<< y22<<endl;
                                    //cout<<"There is NO intersection:"<<j<<endl;
                                    l.push_back(Line());
                                    l[j].p1.x=x1;
                                    l[j].p1.y=y1;
                                    l[j].p2.x=x2;
                                    l[j].p2.y=y2;
                                    s[i].l.p1.x= x1;
                                    s[i].l.p1.y= y1;
                                    s[i].l.p2.x= x2;
                                    s[i].l.p2.y= y2;
                                    output=output+"("+to_string(x1)+","+to_string(y1)+") ("+to_string(x2)+","+to_string(y2)+") ";
                                     //continue;
                                    //"("+to_string(x21)+","+to_string(y21)+") ("+to_string(x22)+","+to_string(y22)+") ";
                                }
                                else
                                {
                                    error_counter++;
                                    //cout<<" "<<x11<<" "<< y11<<" "<< x12<<" "<< y12<<" "<< x21<<" "<< y21<<" "<< x22<<" "<< y22<<endl;
                                    //cout<<"There is intersection:"<<j<<endl;
                                    
                                    if(error_counter==25)
                                    {
                                        cerr<<"Error: failed to generate valid input for 25 simultaneous attempts "<<endl;
                                        exit(1);
                                    }
                                    goto A;
                                }
                            }
                            else
                            {
                                goto A;
                            }
                    }
                    else
                    {
                        l.push_back(Line());
                        l[j].p1.x=x1;
                        l[j].p1.y=y1;
                        l[j].p2.x=x2;
                        l[j].p2.y=y2;
                        s[i].l.p1.x= x1;
                        s[i].l.p1.y= y1;
                        s[i].l.p2.x= x2;
                        s[i].l.p2.y= y2;
                        //cout<<"Its should<<"<<j<<endl;
                        output=output+"("+to_string(x1)+","+to_string(y1)+") ("+to_string(x2)+","+to_string(y2)+") ";
                    }
                }
                else
                {    
                    error_counter++;
                    if(error_counter==25)
                    {
                        cerr<<"Error: failed to generate valid input for 25 simultaneous attempts "<<endl;
                        exit(1);
                    }
                    goto A;
                }
            }
            //B:
            /*for(std::vector<Line>::size_type k = 0; k != l.size(); k++)
            {
                int x11=l[k].p1.x;
                int y11=l[k].p1.y;
                int x12=l[k].p2.x;
                int y12=l[k].p2.y;
                for(std::vector<Line>::size_type q = 0; q != l.size(); q++)
                {                    
                    int x21=l[q].p1.x;
                    int y21=l[q].p1.y;
                    int x22=l[q].p2.x;
                    int y22=l[q].p2.y;
                    if(x11>x21)
                    {
                        int temp_x1=x11;
                        int temp_y1=y11;
                        x11=x21;
                        y11=y21;
                        x21=temp_x1;
                        y21=temp_y1;
                        int temp_x2=x12;
                        int temp_y2=y12;
                        x12=x22;
                        y12=y22;
                        x22=temp_x2;
                        y22=temp_y2;
                    }
                    if(x11!=x21 && y11!=y21 && x12!=x22 && y12!=y22)
                    {
                        if(intersection(x11, y11, x12, y12, x21, y21, x22, y22))
                        {
                            error_counter++;
                        }
                        //else
                        //{
                         //l.pop_back();
                        //}
                    }
                }
            }*/
            //cout<<"Output:"<<output<<endl;
            cout<<output<<endl;
            if(alphabet3<91)
            {
                alphabet3++;    
            }
            else if (alphabet3==90)    
            {
                if(alphabet2<91)
                    alphabet2++;
            }
            else 
            {
                if(alphabet1<91 && alphabet2==90)
                    alphabet1++;
            }
    }
    /*for(std::vector<Street_point>::size_type h = 0; h != s.size(); h++)
    {
        cout<<"Street_name:"<<s[h].street_name<<endl;
        cout<<"Points:"<<endl;
    } */
    if(s.size()!=0)
    {
        cout<<"g"<<endl;
    }
        //cout<<"a \"Weber St\" (2,-1) (2,2) (5,5) (5,6) (3,8)"<<endl;
        //cout<<"a \"King Street S\" (3,2) (4,8)"<<endl;
        //cout<<"a \"Davenport Road\" (0,0) (5,8)"<<endl;
        //cout<<"g"<<endl;
        int wait_time=generate_number(5,lint_value);
        //std::cout<<"wait_time:"<<wait_time<<"\n";
        sleep(wait_time);
    }
    return 0;
}



/*void rand(int s,int n,int l)
{
    srand (time(NULL));
    int streets,lines,wait_time;
    streets=2 + ( std::rand() % ( s - 2 + 1 ) );
    std::cout<<"streets:"<<streets<<"\n";
    lines =1 + ( std::rand() % ( n - 1 + 1 ) ) ;       
    std::cout<<"lines:"<<lines<<"\n";
    wait_time = 5 + ( std::rand() % ( l - 5 + 1 ) ) ; 
    std::cout<<"wait_time:"<<wait_time<<"\n";
    //std::cout<<i<<"\n";
    //sleep(wait_time);
   
}*/
