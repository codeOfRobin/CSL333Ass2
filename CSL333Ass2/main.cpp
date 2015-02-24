#include <iostream>
#include <fstream>
#include <time.h>

using namespace std;

struct GameData
{
    int numberOfVertices;
    int numberOfEdges;
    int numberOfOrganisations;
    int numberOfVariables;
    int numberOfClauses;
    bool **graph;
    GameData()
    {
        numberOfEdges=0;
        numberOfVertices=0;
        numberOfOrganisations=0;
        graph=NULL;
    }
}GD;

void convertToCNF(char *fileName)
{
    bool flagOnce;
    ofstream fout(fileName,ios::out);
    int vertexBase=0,organisationBase=0,connectionBase=GD.numberOfVertices*GD.numberOfOrganisations,subsetBase=connectionBase+GD.numberOfEdges*GD.numberOfOrganisations,iBase=0,jBase=0,kBase=0,temp1,temp2,temp3=GD.numberOfVertices*(GD.numberOfOrganisations-1),lBase,i,j,k,l;
    fout<<"p cnf "<<GD.numberOfVariables<<" "<<GD.numberOfClauses<<"\n";
    for(i=0;i<GD.numberOfOrganisations;i++)
    {
        for(j=1;j<GD.numberOfVertices;j++)
        {
            if(i==0)
                flagOnce=false;
            else
                flagOnce=true;
            for(k=j+1;k<=GD.numberOfVertices;k++)
            {
                if(!GD.graph[j][k])
                    fout<<"-"<<vertexBase+j<<" -"<<vertexBase+k<<" 0\n";
                else if(i==0)
                {
                    flagOnce=true;
                    temp1=connectionBase+organisationBase;
                    lBase=0;
                    for(l=1;l<=GD.numberOfOrganisations;l++)
                    {
                        fout<<"-"<<temp1+l<<" "<<lBase+j<<" 0\n";
                        fout<<"-"<<temp1+l<<" "<<lBase+k<<" 0\n";
                        fout<<"-"<<lBase+j<<" -"<<lBase+k<<" "<<temp1+l<<" 0\n";
                        lBase+=GD.numberOfVertices;
                    }
                    for(l=1;l<=GD.numberOfOrganisations;l++)
                        fout<<temp1+l<<" ";
                    fout<<"0\n";
                    organisationBase+=GD.numberOfOrganisations;
                }
            }
            if(!flagOnce)
            {
                for(int l=0;l<GD.numberOfOrganisations;l++)
                    fout<<l*GD.numberOfVertices+j<<" ";
            }
        }
        vertexBase+=GD.numberOfVertices;
    }
    for(i=0,lBase=0;i<GD.numberOfOrganisations;i++)
    {
        temp1=subsetBase+kBase;
        for(j=0;j<GD.numberOfOrganisations;j++)
        {
            if(i==j)
            {
                jBase+=GD.numberOfVertices;
                continue;
            }
            temp2=temp1+lBase;
            for(k=1;k<=GD.numberOfVertices;k++)
            {
                fout<<"-"<<temp2+k<<" "<<iBase+k<<" 0\n";
                fout<<"-"<<temp2+k<<" -"<<jBase+k<<" 0\n";
                fout<<"-"<<iBase+k<<" "<<jBase+k<<" "<<temp2+k<<" 0\n";
            }
            for(k=1;k<=GD.numberOfVertices;k++)
                fout<<temp2+k<<" ";
            fout<<"0\n";
            jBase+=GD.numberOfVertices;
            lBase+=GD.numberOfVertices;
        }
        iBase+=GD.numberOfVertices;
        jBase=lBase=0;
        kBase+=temp3;
    }
}

void parseInputFile(char *fileName)
{
    ifstream fin(fileName,ios::in);
    if(!fin.is_open())
        cout<<"Wrong File Name!\n";
    else
    {
        char line[32];
        fin.getline(line,32);
        sscanf(line,"%d %d %d",&GD.numberOfVertices,&GD.numberOfEdges,&GD.numberOfOrganisations);
        GD.graph=new bool*[GD.numberOfVertices+1];
        for(int i=0;i<=GD.numberOfVertices;i++)
        {
            GD.graph[i]=new bool[GD.numberOfVertices+1];
        }
        memset(GD.graph,GD.numberOfVertices*GD.numberOfVertices,false);
        for(int i=0,x=0,y=0;i<GD.numberOfEdges;i++)
        {
            fin.getline(line,32);
            sscanf(line,"%d %d",&x,&y);
            GD.graph[x][y]=GD.graph[y][x]=true;
        }
    }
    GD.numberOfVariables=(GD.numberOfVertices+GD.numberOfEdges+GD.numberOfVertices*(GD.numberOfOrganisations-1))*GD.numberOfOrganisations;
    GD.numberOfClauses=2*GD.numberOfEdges*GD.numberOfOrganisations+GD.numberOfEdges+(GD.numberOfOrganisations*GD.numberOfVertices*(GD.numberOfVertices-1))/2+GD.numberOfOrganisations*(GD.numberOfOrganisations-1)*(3*GD.numberOfVertices+1);
    fin.close();
}
void printGameData()
{
    cout<<"Vertices : "<<GD.numberOfVertices<<"\n";
    cout<<"Edges : "<<GD.numberOfEdges<<"\n";
    cout<<"Organisations : "<<GD.numberOfOrganisations<<"\n";
    cout<<"Variables : "<<GD.numberOfVariables<<"\n";
    cout<<"Clauses : "<<GD.numberOfClauses<<"\n";
    for(int i=0;i<=GD.numberOfVertices;i++)
    {
        for(int j=0;j<=GD.numberOfVertices;j++)
            cout<<GD.graph[i][j]<<" ";
        cout<<endl;
    }
}

int main(int argc, const char * argv[])
{
    clock_t start,endt;
    start=clock();
    parseInputFile("/Users/Abhishek/Desktop/input1.txt");
    printGameData();
    convertToCNF("/Users/Abhishek/Desktop/cnf1.txt");
    endt=clock();
    cout<<((double) (endt - start)) / CLOCKS_PER_SEC<<"\n";
    return 0;
}
