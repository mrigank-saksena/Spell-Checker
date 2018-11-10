#include <iostream>
#include "hash.h"
using namespace std;

// Header file explains functionalities of methods

hashTable :: hashTable(int size): capacity(getPrime(size)), filled(0), data(capacity) { }


int hashTable :: insert(const std::string &key, void *p){	
	int position = findPos(key);		
	if(position == -1){
		position = hash(key);		
		while(data[position].isOccupied){ 
			if(data[position].key == key){
				break;
			}
		position++;	
		position %= capacity;
		}		
		data[position].isOccupied = true;
		data[position].isDeleted = false;
		data[position].key = key;
		data[position].pv = p;	
		if(double(filled++)/capacity > 0.5) {
			if(!rehash()){
				return 2;
			}
		}
		return 0;
	}
	else{
		data[position].pv = p;
		return 1;
	}		
}


bool hashTable :: contains(const std::string &key){
	return (findPos(key) != -1);
}

void * hashTable :: getPointer(const std::string &key, bool *b){
	int pos = findPos(key);
    	if (pos != -1) {
        *b = true; 
        return data[pos].pv;
   	 }
	*b = false;     
	return NULL;
}

int hashTable :: setPointer(const std::string &key, void *p){
	int position = findPos(key);
	if(position == -1){
		return 1;
	}
	
	else{
		data[position].pv = p;
		return 0;
	}
}

bool hashTable :: remove(const std::string &key){
	int position = findPos(key);
	if(position == -1){
		return false;
	}
	
	else{
		data[position].isDeleted = true;
		return true;
	}
}

hashTable :: hashItem :: hashItem (const std::string &k, const bool &o, const bool &d, void *p)
	: key(k), isOccupied(o), isDeleted(d), pv(p) {}

// Source: Stackoverflow
int hashTable :: hash(const std::string &key){
	unsigned h = 0; 
	const char *s = key.c_str();
	while (*s)
        	h = h * 101 + (unsigned) *s++;
    	return h%capacity;
}



int hashTable :: findPos(const std::string &key){
	int position = hash(key);
	while(data[position].isOccupied){
		if(data[position].key == key){
			if(!(data[position].isDeleted)){
				return position;
			}
			return -1;
		}
		position++;
		position %= capacity;

	}
	return -1;
}


bool hashTable :: rehash(){
	capacity = getPrime(capacity); 
	vector<hashItem> temp = data;
	data.clear();

	data.resize(capacity); 
	if (data.size() != capacity)
        	return false;
	filled = 0; 
	for (int i = 0; i < temp.size(); i++) 
        if (!temp[i].isDeleted) // added the && data[position].isOccupied) did not work
		insert(temp[i].key, temp[i].pv);
	return true;
}


unsigned int hashTable :: getPrime(int size){
	int primes[] = {1223, 5003, 10007, 25013, 50101, 100003, 200017, 400051, 800161, 1600421, 3201109};
	int i = 0;
	while(primes[i] < 2*size){ 
		i++; 
	}
	
	return primes[i];
}