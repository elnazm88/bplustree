#include <bits/stdc++.h>

class BPT_Node {
 public:

  BPT_Node( std::size_t m_in ) {// Constructor

    this->bLeaf = false;
    this->m = m_in;
    this->size = 0;

    std::string* _key = new std::string[m-1];
    for( std::size_t i = 0; i < m-1 ; ++i ){
      _key[i] ="";
    }

    this->key = _key;

    BPT_Node** _children = new BPT_Node*[m];

    for( std::size_t n = 0; n < m; ++n ){
      _children[n] = nullptr;
    }

    this->children = _children;

    this->parent = NULL;

  }

  bool bLeaf;
  std::size_t m;
  std::size_t size;
  std::string* key;
  std::string* field2;
  std::string* field3;
  BPT_Node** children;
  BPT_Node* parent;

};
/////////////////////////////////////////////////////////////////////////////
class BPTree {

    BPT_Node* root;

    std::size_t m;

 public:
    //////////////////////
    BPTree(std::size_t _m) {
      this->root = nullptr;
      this->m = _m;
    }
    std::map<std::string, std::string> field2;
    std::map<std::string, std::string> field3;
    //////////////////////
    ~BPTree() {

      if(this->root != NULL){

        if(!this->root->bLeaf){
          for(std::size_t i=0; i <= this->root->size; ++i ){
            clear(this->root->children[i]);
          }

        }

        delete[] this->root->key;
        delete[] this->root->children;


        delete this->root;
      }

    }
    //////////////////////
    BPT_Node* BPTreeSearch(BPT_Node* node, std::string key){

      int numDiskRead =0;

      if(node == nullptr) {

        return NULL;

      }else{

        while( !node->bLeaf ){

          for(std::size_t n=0; n<node->size; ++n){

            numDiskRead++;

            if(key < node->key[n]){

              node = node->children[n];
              break;
            }
            if(n == (node->size)-1)
              {

                node = node->children[n+1];
                break;
              }
          }
        }

        //find the key in leaf
        for( std::size_t n=0; n<node->size; ++n ){

          numDiskRead++;

          if(node->key[n] == key){

            std::cout<<"total disk read : "<< numDiskRead<<std::endl;

            return node;
          }
        }

        return nullptr;
      }

    }

    //////////////////////
    BPT_Node* getroot(){

      return this->root;

    }
    //////////////////////
    BPT_Node* BPTreeRangeSearch(BPT_Node* node, std::string key){
        if(node == nullptr) { // root null, return null
            return nullptr;
        }
        else{
            BPT_Node* pointer = node; // pointer finding the key

            while(!pointer->bLeaf){
                for(std::size_t i=0; i<pointer->size; i++){
                    if(key < pointer->key[i]){
                        pointer = pointer->children[i];
                        break;
                    }
                    if(i == (pointer->size)-1){
                        pointer = pointer->children[i+1];
                        break;
                    }
                }
            }
            return pointer;
        }
    }
    //////////////////////
    int range_search(std::string start, std::string end, std::vector<std::string>& result_data) {

      int index=0;

      BPT_Node* start_node = BPTreeRangeSearch(this->root,start);
      BPT_Node* pointer = start_node;

      std::string temp = pointer->key[0];

      while(temp<=end){
        if(pointer == nullptr){
          break;
        }
        for(std::size_t i = 0; i< pointer->size;++i){

          temp = pointer->key[i];

          if((pointer->key[i] >= start)&&(pointer->key[i] <= end)){
            result_data.push_back(temp);

            ++index;
          }
        }
        pointer = pointer->children[pointer->size];
      }
      return index;
    }
    //////////////////////
    bool search( std::string data ) {
        return BPTreeSearch( this->root, data ) != nullptr;
    }
    //////////////////////
    int find_index(std::string* array, std::string data, int len){
        int index = 0;
        for(int i=0; i < len; i++){
            if(data < array[i]){
                index = i;
                break;
            }
            if( i == len-1 ){
                index = len;
                break;
            }
        }
        return index;
    }
    //////////////////////
    std::string* key_insert(std::string* array, std::string data, int len){
        int index = 0;
        for(int i=0; i<len; i++){
            if(data < array[i]){
                index = i;
                break;
            }
            if(i==len-1){
                index = len;
                break;
            }
        }

        for(int i = len; i > index; i--){
            array[i] = array[i-1];
        }

        array[index] = data;

        return array;
    }
    //////////////////////
    BPT_Node** child_insert(BPT_Node** child_array, BPT_Node*child,int len,int index){
        for(int i= len; i > index; --i){
            child_array[i] = child_array[i - 1];
        }
        child_array[index] = child;
        return child_array;
    }
    //////////////////////
    BPT_Node* child_key_insert(BPT_Node* node, std::string data, BPT_Node* child){
        int key_index=0;
        int child_index=0;
        for(std::size_t i=0; i< node->size; ++i){
            if(data < node->key[i]){
                key_index = i;
                child_index = i+1;
                break;
            }
            if(i==node->size-1){
                key_index = node->size;
                child_index = node->size+1;
                break;
            }
        }
        for(int i = node->size;i > key_index; --i){
            node->key[i] = node->key[i-1];
        }
        for(int i=node->size+1; i>child_index; --i){
            node->children[i] = node->children[i-1];
        }

        node->key[key_index] = data;
        node->children[child_index] = child;

        return node;
    }
    //////////////////////
    void insertParentNode(BPT_Node* par,BPT_Node* child, std::string data){
      //oversize check
      BPT_Node* pointer = par;
      if(pointer->size < this->m-1){//not oversize

        pointer = child_key_insert(pointer,data,child);
        pointer->size++;
      }
      else{//oversize

        auto* Newnode = new BPT_Node(this->m);
        Newnode->parent = pointer->parent;

        //copy key
        std::string* key_copy = new std::string[pointer->size+1];
        for(std::size_t i=0; i<pointer->size; i++){
          key_copy[i] = pointer->key[i];
        }
        key_copy = key_insert(key_copy,data,pointer->size);

        auto** child_copy = new BPT_Node*[pointer->size+2];
        for(std::size_t i=0; i<pointer->size+1;i++){
          child_copy[i] = pointer->children[i];
        }
        child_copy[pointer->size+1] = nullptr;
        child_copy = child_insert(child_copy,child,pointer->size+1,find_index(key_copy,data,pointer->size+1));

        //split nodes
        pointer->size = (this->m)/2;
        if((this->m) % 2 == 0){
          Newnode->size = (this->m) / 2 -1;
        }
        else{
          Newnode->size = (this->m) / 2;
        }

        for(std::size_t i=0; i<pointer->size;i++){
          pointer->key[i] = key_copy[i];
          pointer->children[i] = child_copy[i];
        }
        pointer->children[pointer->size] = child_copy[pointer->size];

        for(std::size_t i=0; i < Newnode->size; i++){
          Newnode->key[i] = key_copy[pointer->size + i +1];
          Newnode->children[i] = child_copy[pointer->size+i+1];
          Newnode->children[i]->parent=Newnode;
        }
        Newnode->children[Newnode->size] = child_copy[pointer->size+Newnode->size+1];
        Newnode->children[Newnode->size]->parent=Newnode;

        std::string parkey = key_copy[this->m/2];

        delete[] key_copy;
        delete[] child_copy;

        //parent check
        if(pointer->parent == nullptr){//if there are no parent node(root case)
          auto* Newparent = new BPT_Node(this->m);
          pointer->parent = Newparent;
          Newnode->parent = Newparent;

          Newparent->key[0] = parkey;
          Newparent->size++;

          Newparent->children[0] = pointer;
          Newparent->children[1] = Newnode;

          this->root = Newparent;

          //delete Newparent;
        }
        else{//if there already have parent node
          insertParentNode(pointer->parent, Newnode, parkey);
        }
      }

    }
    //////////////////////
    void insert(std::string data, std::string field2_in, std::string field3_in) {

      if(this->root == nullptr){ //set root

        this->root = new BPT_Node(this->m);
        this->root->bLeaf = true;
        *this->root->key = data;
        //  *this->root->field2 = field2_in;
        this->root->size = 1;

      } else {

        BPT_Node* pointer = this->root;

        pointer = BPTreeRangeSearch(pointer, data);

        //oversize check
        if( pointer->size < ( this->m-1 ) ){ // not oversize, just insert in the correct position

          //key insert and rearrange
          pointer->key = key_insert(pointer->key,data,pointer->size);
          pointer->size++;

          //edit pointer(next node)
          pointer->children[pointer->size] = pointer->children[pointer->size-1];
          pointer->children[pointer->size-1] = nullptr;
        }
        else{//oversize case
          //make new node
          auto* Newnode = new BPT_Node(this->m);
          Newnode->bLeaf = true;
          Newnode->parent = pointer->parent;

          //copy key
          std::string* key_copy = new std::string[pointer->size+1];
          for(std::size_t i=0; i<pointer->size; i++){
            key_copy[i] = pointer->key[i];
          }

          //insert and rearrange
          key_copy = key_insert(key_copy,data,pointer->size);

          //split nodes
          pointer->size = (this->m)/2;
          if((this->m) % 2 == 0){
            Newnode->size = (this->m) / 2;
          }
          else{
            Newnode->size = (this->m) / 2 + 1;
          }

          for(std::size_t i=0; i<pointer->size;i++){
            pointer->key[i] = key_copy[i];
          }
          for(std::size_t i=0; i < Newnode->size; i++){
            Newnode->key[i] = key_copy[pointer->size + i];
          }

          //*pointer->field2=field2_in;
          pointer->children[pointer->size] = Newnode;
          Newnode->children[Newnode->size] = pointer->children[this->m-1];
          pointer->children[this->m-1] = nullptr;

          delete[] key_copy;

          //parent check
          std::string parkey = Newnode->key[0];

          if(pointer->parent == nullptr){//if there are no parent node(root case)
            auto* Newparent = new BPT_Node(this->m);
            pointer->parent = Newparent;
            Newnode->parent = Newparent;

            *Newparent->key = parkey;
            Newparent->size++;

            Newparent->children[0] = pointer;
            Newparent->children[1] = Newnode;

            this->root = Newparent;
          }
          else{
            insertParentNode(pointer->parent, Newnode, parkey);
          }
        }
      }
      field2.insert( std::pair<std::string,std::string> (data, field2_in) );
      field3.insert( std::pair<std::string,std::string> (data, field3_in) );
    }
    //////////////////////
    void remove(std::string data) { // Remove an key
        //make pointer
        BPT_Node* pointer = this->root;

        //move to leaf
        pointer = BPTreeRangeSearch(pointer,data);

        //make block index
        int block_index =-1;
        for(std::size_t i=0; i<pointer->parent->size+1;++i){
            if(pointer == pointer->parent->children[i]){
                block_index = i;
            }
        }
        std::size_t left = block_index-1;
        std::size_t right = block_index+1;


        //find data
        int del_index=-1;
        for(std::size_t i=0; i< pointer->size; i++){
            if(pointer->key[i] == data){
                del_index = i;
                break;
            }
        }
        //if data dosen't exist, nothing happen
        if(del_index==-1){
            return; // there is no match remove value
        }

        //remove data
        for( std::size_t i=del_index; i<pointer->size-1;i++){
            pointer->key[i] = pointer->key[i+1];
        }
        pointer->key[pointer->size-1] = "";
        pointer->size--;

        //if pointer is root, and there are no more data
        if(pointer == this->root && pointer->size==0){//root case
            clear(this->root);
            this->root = nullptr;
            return;
        }
        pointer->children[pointer->size] = pointer->children[pointer->size+1];
        pointer->children[pointer->size+1] = nullptr;


        //undersize check
        if(pointer == this->root){
            return;
        }
        if(pointer->size < m/2){//undersize case

            if(left >= 0){// left_sibiling exists

                BPT_Node* leftblock= pointer->parent->children[left];

                if(leftblock->size > m/2){ //if data number is enough to use this node
                    std::string* temp = new std::string[pointer->size+1];

                    //copy key
                    for( std::size_t i=0; i<pointer->size; i++){
                        temp[i]=pointer->key[i];
                    }

                    key_insert(temp,leftblock->key[leftblock->size -1],pointer->size);
                    for(std::size_t i=0; i<pointer->size+1; i++){
                        pointer->key[i] = temp[i];
                    }
                    pointer->size++;
                    delete[] temp;

                    //pointer edit
                    pointer->children[pointer->size] = pointer->children[pointer->size-1];
                    pointer->children[pointer->size-1] = nullptr;

                    leftblock->key[leftblock->size-1] = "";
                    leftblock->size--;
                    leftblock->children[leftblock->size] = leftblock->children[leftblock->size+1]; //pointer
                    leftblock->children[leftblock->size+1]= nullptr;

                    pointer->parent->key[left] = pointer->key[0];

                    return;
                }
            }
            if(right <= pointer->parent->size){
                BPT_Node* rightblock = pointer->parent->children[right];

                if(rightblock->size >m/2){//if data number is enough to use this node
                    std::string* temp = new std::string[pointer->size+1];

                    //copy key
                    for( std::size_t i=0; i<pointer->size; i++){
                        temp[i]=pointer->key[i];
                    }
                    //insert and rearrange
                    key_insert(temp,rightblock->key[0],pointer->size);
                    for(std::size_t i=0; i<pointer->size+1; i++){
                        pointer->key[i] = temp[i];
                    }
                    pointer->size++;
                    delete[] temp;

                    //pointer edit
                    pointer->children[pointer->size] = pointer->children[pointer->size-1];
                    pointer->children[pointer->size-1] = nullptr;

                    //block property edit
                    for( std::size_t i=0; i<rightblock->size-1;i++){
                        rightblock->key[i] = rightblock->key[i+1];
                    }
                    rightblock->key[rightblock->size-1] = "";
                    rightblock->size--;
                    rightblock->children[rightblock->size] = rightblock->children[rightblock->size+1]; //pointer
                    rightblock->children[rightblock->size+1]= nullptr;

                    //parent property edit
                    pointer->parent->key[right-1] = rightblock->key[0];

                    return;
                }
            }

            //if block is not enought to use their data
            //we have to merge step
            if(right<=pointer->parent->size){ // right_block exists
              BPT_Node* rightblock = pointer->parent->children[right];


              for( std::size_t i=0; i<rightblock->size; ++i){
                pointer->key[i+pointer->size]=rightblock->key[i];
              }
              //edit pointer
              pointer->children[pointer->size] = nullptr;
              pointer->size = rightblock->size + pointer->size;
              pointer->children[pointer->size] = rightblock->children[rightblock->size];

              //parent property edit
              deleteParent(rightblock, right-1, pointer->parent);

              for( std::size_t i=0; i<rightblock->size;i++){
                rightblock->key[i]="";
                rightblock->children[i] = nullptr;
              }
              rightblock->children[rightblock->size] = nullptr;

              delete[] rightblock->key;
              delete[] rightblock->children;
              delete rightblock;
              return;

            }

            if(left>=0){ // left_block exists
              BPT_Node* leftblock = pointer->parent->children[left];

              //merge two leaf node
              for(std::size_t i=0; i<pointer->size; i++){
                leftblock->key[leftblock->size+i]=pointer->key[i];
              }
              //edit pointer
              leftblock->children[leftblock->size] = nullptr;
              leftblock->size = leftblock->size+pointer->size;
              leftblock->children[leftblock->size] = pointer->children[pointer->size];

              //parent property edit
              deleteParent(pointer, left, pointer->parent);
              for( std::size_t i=0; i<pointer->size;i++){
                pointer->key[i]="";
                pointer->children[i] = nullptr;
              }
              pointer->children[pointer->size] = nullptr;

              delete[] pointer->key;
              delete[] pointer->children;
              delete pointer;

              return;

            }
        }
        else{
          return;
        }
    }
    //////////////////////
    void deleteParent(BPT_Node* node, int index, BPT_Node* par){
        BPT_Node* pointer = par;
        std::string target = pointer->key[index];

        //if pointer is root, and there are no more data -> child node is to be root!
        if(pointer == this->root && pointer->size==1){//root case
          if(node == pointer->children[0]){
            delete[] node->key;
            delete[] node->children;
            delete node;
            this->root = pointer->children[1];
            delete[] pointer->key;
            delete[] pointer->children;
            delete pointer;
            return;
          }

          if(node == pointer->children[1]){

            delete[] node->children;
            delete[] node->key;

            this->root = pointer->children[0];
            delete[] pointer->key;
            delete[] pointer->children;
            delete pointer;
            return;
          }
        }

        //remove data
        for( std::size_t i=index; i<pointer->size-1; ++i){
            pointer->key[i] = pointer->key[i+1];
        }
        pointer->key[ pointer->size-1 ] = "";

        //remove pointer
        int rem_index = -1;
        for( std::size_t i=0; i <pointer->size+1 ;++i ){

          if(pointer->children[i] == node ){
                rem_index = i;
            }
        }
        if(rem_index == -1){
            return;
        }
        for( std::size_t i=rem_index; i<pointer->size;i++){
            pointer->children[i] = pointer->children[i+1];
        }
        pointer->children[pointer->size] = nullptr;
        pointer->size--;

        //undersize check
        if(pointer == this->root){
            return;
        }
        if(pointer->size < m/2){//undersize case

            int block_index =-1;
            for( std::size_t i=0; i<pointer->parent->size+1;i++){
                if(pointer == pointer->parent->children[i]){
                    block_index = i;
                }
            }
            std::size_t left=block_index-1;
            std::size_t right=block_index+1;

            if(left >= 0){// left_sibiling exists
                BPT_Node* leftblock= pointer->parent->children[left];

                if(leftblock->size > m/2){ //if data number is enough to use this node
                    std::string* temp = new std::string[pointer->size+1];

                    //copy key
                    for( std::size_t i=0; i<pointer->size; i++){
                        temp[i]=pointer->key[i];
                    }


                    key_insert(temp, pointer->parent->key[left],pointer->size);
                    for( std::size_t i=0; i<pointer->size+1; ++i){
                        pointer->key[i] = temp[i];
                    }
                    pointer->parent->key[left] = leftblock->key[leftblock->size-1];
                    delete[] temp;

                    BPT_Node** child_temp = new BPT_Node*[pointer->size+2];
                    //copy child node
                    for( std::size_t i=0; i<pointer->size+1; i++){
                        child_temp[i]=pointer->children[i];
                    }
                    //insert and rearrange at child
                    child_insert(child_temp,leftblock->children[leftblock->size],pointer->size,0);

                    for( std::size_t i=0; i<pointer->size+2; i++){
                        pointer->children[i] = child_temp[i];
                    }
                    delete[] child_temp;

                    //size edit
                    pointer->size++;
                    leftblock->size--;
                    return;

                }
            }

            if(right <= pointer->parent->size){
                BPT_Node* rightblock = pointer->parent->children[right];

                if( rightblock->size > m/2 ){
                    std::string* temp = new std::string[pointer->size+1];

                    for( std::size_t i = 0; i<pointer->size; ++i ){
                        temp[i]=pointer->key[i];
                    }
                    //add pointer
                    key_insert(temp,pointer->parent->key[block_index],pointer->size);
                    for( std::size_t i=0; i<pointer->size+1; i++){
                        pointer->key[i] = temp[i];
                    }
                    pointer->parent->key[block_index] = rightblock->key[0];
                    delete[] temp;

                    //insert a child

                    pointer->children[pointer->size+1] = rightblock->children[0];
                    for( std::size_t i=0; i<rightblock->size; i++){
                        rightblock->children[i] = rightblock->children[i+1];
                    }
                    rightblock->children[rightblock->size] = nullptr;

                    pointer->size++;
                    rightblock->size--;
                    return;

                }
            }

            //if block is not enought to use their data
            if(left>=0){ // left_block exists
                BPT_Node* leftblock = pointer->parent->children[left];

                leftblock->key[leftblock->size] = pointer->parent->key[left];

                for( std::size_t i=0; i<pointer->size; i++){
                    leftblock->key[leftblock->size+i+1]=pointer->key[i];
                }
                for( std::size_t i=0; i<pointer->size+1;i++){
                    leftblock->children[leftblock->size+i+1] = pointer->children[i];
                    pointer->children[i]->parent = leftblock;
                }
                for( std::size_t i=0; i<pointer->size+1; i++){
                    pointer->children[i] = nullptr;
                }
                leftblock->size = leftblock->size+pointer->size+1;
                deleteParent(pointer, left,pointer->parent);
                return;

            }

            if(right<=pointer->parent->size){ // right side  exists
                BPT_Node* rightblock = pointer->parent->children[right];

                pointer->key[pointer->size] = pointer->parent->key[right-1];
                //merge two leaf node
                for( std::size_t i=0; i<rightblock->size; i++){
                    pointer->key[pointer->size+1+i]=rightblock->key[i];
                }
                for( std::size_t i=0; i<rightblock->size+1;i++){
                    pointer->children[pointer->size+i+1] = rightblock->children[i];
                    rightblock->children[i]->parent=rightblock;
                }
                for( std::size_t i=0; i<rightblock->size+1; i++){
                    rightblock->children[i] = nullptr;
                }

                rightblock->size = rightblock->size+pointer->size+1;

                deleteParent(rightblock, right-1,pointer->parent);
                return;
            }
        }
        else{

            return;
        }
    }
    //////////////////////
    void clear(BPT_Node* pointer){

      if(pointer != NULL){

        if(!pointer->bLeaf){
          for( std::size_t i=0; i <= pointer->size; ++i ){
            clear(pointer->children[i]);
          }

        }

            delete[] pointer->children;
            delete[] pointer->key;

            delete pointer;
        }
    }
    //////////////////////
    void bpt_print(){
        printAll(this->root);
    }
    ////////////////////////
    void bpt_printFile(){

      std::ofstream bptFile("bpt.dat", std::ios::out | std::ios::binary);
    print(this->root, bptFile);

    bptFile.close();

    }
    //////////////////////
    void printAll(BPT_Node* pointer){

      if (pointer != NULL) {

        for ( std::size_t i = 0; i < pointer->size; ++i) {

          std::cout << "*************************" << "\n ";
          std::cout << "field 1: "<<pointer->key[i] << "\n ";
          std::cout << "field 2: "<<field2.find(pointer->key[i])->second << "\n ";
          std::cout << "field 3: "<<field3.find(pointer->key[i])->second << "\n ";
        }

        if (!pointer->bLeaf) {
          for ( std::size_t i = 0; i < pointer->size + 1; ++i) {
            printAll(pointer->children[i]);
          }
        }
      }

    }
    //////////////////////
    void print(BPT_Node* pointer, std::ofstream& bptFile) {

        if (pointer != NULL) {
          for ( std::size_t i = 0; i < pointer->size; ++i) {
            bptFile << pointer->key[i] << " ";
            }
          bptFile<<std::endl;

            if (!pointer->bLeaf) {
              for ( std::size_t i = 0; i < pointer->size + 1; ++i) {
                print(pointer->children[i], bptFile);
                }
            }
        }
    }

};
