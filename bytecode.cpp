using namespace std;
#include <iostream>
#include <fstream>
#include <bitset>
typedef unsigned char byte_t;

int readBit(){
	string in;
	do{ cout<<"bit? "; cin>>in;
		if(in!="0" && in!="1") cout<<"(retry)"<<endl;
		else break;
	}while (true);
	return in=="0"?0:1;
}

void runByteCode(byte_t* byteCode){
	bitset<256> memory;
	memory[1]=1;
	byte_t instructionPointer=0;
	while(instructionPointer!=255){
		byte_t in=byteCode[instructionPointer*4+1]; int x;
		
		if(in==254) x=readBit();
		else x=memory[in];
		
		byte_t out=byteCode[instructionPointer*4];
		
		if(out==254) cout<<"out:"<<x<<endl;
		else memory[out]=x;
		
		instructionPointer=byteCode[instructionPointer*4+2+memory[255]];
	}
}

void runByteCode2(){
//byte_t* byteCode
	//byte_t progNot[]={0xFF, 0xFE, 0x02, 0x01, 0xFE, 0x00, 0x00, 0x00, 0xFE, 0x01, 0x00, 0x00};

	/*
	#not_gate.source
	pch=in
	zero|one
	@zero
	out=0
	0
	@one
	out=1
	0
	*/
	
	struct instruction{
		unsigned int type:1;
		unsigned int index:7;
	};
	struct instruction byteCodeProgram[]={ {0,0},{1,0}, {0,1},{1,1}, {0,2},{1,2}};
	// 0:MemCopy 1:PathChoice
	byte_t byteCodeMemCopy[]={255,254, 254,0, 254,1};
	byte_t byteCodePathChoice[]={4,2, 0,0, 0,0};
	
	bitset<256> memory;
	memory[1]=1;
	size_t instructionPointer=0;
	
	while(instructionPointer!=255){
		struct instruction instr=byteCodeProgram[instructionPointer];
		if(false){
			cout<<"instructionPointer:"<<instructionPointer<<endl;
			cout<<"instr.type:"<<instr.type<<endl;
			cout<<"instr.index:"<<instr.index<<endl;
			cout<<"memory[255]:"<<memory[255]<<endl;
		}
		if(instr.type==0){
			byte_t in=byteCodeMemCopy[instr.index*2+1]; int x;
			
			if(in==254) x=readBit();
			else x=memory[in];
			
			byte_t out=byteCodeMemCopy[instr.index*2];
			
			if(out==254) cout<<"out:"<<x<<endl;
			else memory[out]=x;

			instructionPointer++;
			
		} else if(instr.type==1){
			instructionPointer=byteCodePathChoice[instr.index*2+memory[255]];
		}
	}
}

void runByteCode3(byte_t byteCode[]){
//byte_t* byteCode
	//byte_t progNot[]={0xFF, 0xFE, 0x02, 0x01, 0xFE, 0x00, 0x00, 0x00, 0xFE, 0x01, 0x00, 0x00};

	/*
	#not_gate.source
	pch=in
	zero|one
	@zero
	out=0
	0
	@one
	out=1
	0
	*/
	
	// 0:MemCopy 1:PathChoice
	//byte_t byteCode[]={0,255,254, 1,4,2, 0,254,0, 1,0,0, 0,254,1, 1,0,0};
	
	bitset<256> memory;
	memory[1]=1;
	size_t instructionPointer=0;
	
	while(instructionPointer!=255){
		byte_t type=byteCode[instructionPointer*3+0];

		if(false){
			cout<<"instructionPointer:"<<instructionPointer<<endl;
			cout<<"instr.type:"<<type<<endl;
			cout<<"memory[255]:"<<memory[255]<<endl;
		}
		if(type==0){

			byte_t first=byteCode[instructionPointer*3+1];
			byte_t second=byteCode[instructionPointer*3+2];
			
			byte_t in=second; int x;
			
			if(in==254) x=readBit();
			else x=memory[in];
			
			byte_t out=first;
			
			if(out==254) cout<<"out:"<<x<<endl;
			else memory[out]=x;

			instructionPointer++;
			
		} else if(type==1){
			instructionPointer=byteCode[instructionPointer*3+1+memory[255]];
		}
	}
}

byte_t* readBytes(const char *name){
	ifstream fl(name);
	fl.seekg( 0, ios::end );
	size_t len = fl.tellg();
	char *ret = new char[len];
	fl.seekg(0, ios::beg); 
	fl.read(ret, len);
	fl.close();
	return (byte_t*)ret;
}

void byteCodeFromArgVFile(int argc, char* argv[]){
	if(argc!=2) cout<<"pass a .zbc file as argument"<<endl;
	else runByteCode(readBytes(argv[1]));
}

byte_t* byteArrayFromHexString(string hexString){
	const int size = hexString.size();
	byte_t* byteArray=new byte_t[size/2];

	string hex="0123456789ABCDEF";
	for (int i=0; i<size/2; i++) {
		int n1=hex.find(hexString.at(i*2));
		int n2=hex.find(hexString.at(i*2+1));
		int v=n1*16+n2;
		//cout<<v<<endl;
		byteArray[i]=v;
	}
	return byteArray;
}

void byteCodeFromProgNotArray(){
	byte_t progNot[]={0xFF, 0xFE, 0x02, 0x01, 0xFE, 0x00, 0x00, 0x00, 0xFE, 0x01, 0x00, 0x00};
	runByteCode(progNot);
}

void byteCodeFromProgNotArray2(){
	//byte_t progNot[]={0xFF, 0xFE, 0x02, 0x01, 0xFE, 0x00, 0x00, 0x00, 0xFE, 0x01, 0x00, 0x00};
	string progNotString="FFFE0201FE000000FE010000";
	byte_t* progNotArray = byteArrayFromHexString(progNotString);
	runByteCode(progNotArray);
	delete[] progNotArray;
}

void byteCodeFromProgNotArray3(){
	byte_t array[]={0,255,254, 1,4,2, 0,254,0, 1,0,0, 0,254,1, 1,0,0};
	runByteCode3(array);
}

void byteCodeFromProgNotArray4(){
	//byte_t array[]={0,255,254, 1,4,2, 0,254,0, 1,0,0, 0,254,1, 1,0,0};
	string progNotString="00FFFE01040200FE0001000000FE01010000";
	byte_t* progNotArray = byteArrayFromHexString(progNotString);
	runByteCode3(progNotArray);
	delete[] progNotArray;
}

int main(int argc, char* argv[]){
	//byteCodeFromArgVFile(argc, argv);
	//byteCodeFromProgNotArray2();

	// added new
	//runByteCode2();
	//byteCodeFromProgNotArray3();
	byteCodeFromProgNotArray4();
}
