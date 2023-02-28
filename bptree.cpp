
#include <bits/stdc++.h>
#include "bpt.h"

using namespace std;

////////////////////////////////////////////////////////////////////
std::vector<std::string> split_string(const std::string& str,
                                      const std::string& delim)
{
  std::vector< std::string > line;

  string::size_type pos = 0;
  string::size_type prev = 0;

  while ( ( pos = str.find( delim, prev ) ) != std::string::npos )
    {
      line.push_back( str.substr( prev, pos - prev ) );
      prev = pos + delim.size();
    }

  line.push_back( str.substr( prev ) );

  return line;
}
//////////////////////////////////////////////////////////////////
struct RecordType
{
  string key;//field1
  string field2;
  string field3;
};
/////////////////////////////////////////////////////////////////
void findRecord(string keyin, BPTree& bpt){

  string keyReadin;
  ifstream openfile("bpt.dat", ios::out | ios::binary);
  openfile.read((char *) &keyReadin, sizeof(RecordType));

  if(bpt.search(keyin)){

    cout<<"\nthe record is found and it is : "<<"\n";
    cout<<"Field 1 : "<<keyin;
    cout<<"\nField 2 : "<<bpt.field2.find(keyin)->second;
    cout<<"\nField 3 : "<<bpt.field3.find(keyin)->second;
    cout<<endl;
  }else{

    cout<<"\n there is NO record with this field : "<<"\n";

  }

   openfile.close();

}
//////////////////////////////////////////////////////////////////
void printInfo(){
      std::cout << endl;
      std::cout << " Choose one of the following options: \n";
      std::cout << " 1   Searching by key value\n";
      std::cout << " 2   Range search with index\n";
      std::cout << " 3   Insert the new record\n";
      std::cout << " 4   Delete the record with the key\n";
      std::cout << " 5   Print the all recods sequentialy ordered by field1\n";
      std::cout << " 6   Exit the program\n";
      std::cout << "Enter choice:  ";

}
//////////////////////////////////////////////////////////////////
int main(){

  RecordType rec;
  BPTree bpt(8);
  std::string line;
  int blocksize =1024;

  long database_Size =0;
  std::vector<std::string> splitted;


  ofstream databaseFile("bptdatabase.dat", ios::out | ios::binary);

  std::ifstream infile("A1_data.txt");
  std::string field1;
  std::string field2;
  std::string field3;

  std::cout<<" inserting data to b plus tree database .... "<<std::endl;

  while ( std::getline( infile, line ) )
    {

      splitted = split_string(line," ");

      if(splitted.size()>=0){

        field1 = splitted[0];

        field1.erase( remove(field1.begin(), field1.end(), ' '), field1.end() );
      }

      if(splitted.size()>=2)field2 = splitted[1] +" "+ splitted[2];

      if(splitted.size()>=3) {

        field3.clear();

        for( std::size_t n=3 ; n < splitted.size() ; ++n ) {

          field3 += splitted[n];

          if( n!=splitted.size()-1 ) field3 += " ";

        }

      }

      rec.key    = field1;
      rec.field2 = field2;
      rec.field3 = field3;

      database_Size += (field1.size()+field3.size()+field3.size());

      databaseFile.write((char *) &rec, sizeof(rec));

      bpt.insert(field1, field2, field3);

    }

  databaseFile.close();

    std::cout<<" all data inserted "<<std::endl;
    std::cout<<" Total number of database blocks =  "<< database_Size/blocksize +1<<std::endl;
    bpt.bpt_printFile();


    printInfo();

    int choice;
    std::vector<std::string> rangesearch;
    std::string keyin1;
    std::string input2;
    std::string input3;
    int numbers = 0;

    while(true){

      std::cin >> choice;
      std::cout << endl;

      switch (choice)
        {
          case 1:
            std::cout<<"please enter the key to find  : ";
            std::cin>>keyin1;
            findRecord(keyin1, bpt);
            printInfo();
            break;
          case 2:
            std::cout<<"please enter the BEGIN of range key   : ";
            std::cin>>keyin1;
            std::cout<<"please enter the END of range key   : ";
            std::cin>>input2;
            numbers = bpt.range_search(keyin1,input2,rangesearch);
            std::cout<<"there are "<<numbers<<"records in this range with these key:\n";
            for(string s:rangesearch)std::cout<<s<<"\n";
            printInfo();
            break;
          case 3:
            std::cout<<"please enter the field 1 of record   : ";
            std::cin>>keyin1;
            std::cout<<"\nplease enter the field 2 of record   : ";
            std::cin>>input2;
            std::cout<<"\nplease enter the field 3 of record   : ";
            std::cin>>input3;
            if(bpt.search(keyin1)){
              std::cout<<"\nthis key is already exist \n ";
            }else{
              bpt.insert(keyin1, input2, input3);
            }
            printInfo();
            break;
          case 4:
            std::cout<<"please enter the key to be deleted from the tree : ";
            std::cin>>keyin1;
            bpt.remove( keyin1);
            printInfo();
            break;
          case 5:
            bpt.bpt_print();
            printInfo();
            break;
          case 6:
            return 0;
            break;
        }

    }


    return 0;
}
