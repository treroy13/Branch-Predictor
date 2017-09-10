#include<iostream>
#include<fstream>
#include<string>
#include <stdio.h>   
#include <stdlib.h>
#include<vector>
#include<sstream>
#include<math.h> 

using namespace std;

int main(int argc, char *argv[]) {

	unsigned long long addr1;
	string behavior1;
	int takenCount =0;
	int untakenCount =0;
	int numBranches = 0;


	//create a struct with addr and behavior
	struct branches{

		unsigned long long addr;
		int behavior;

	};

	branches br;

	//push structs into vector temp
	std::vector<branches>temp;

	
	//input file 
	ifstream infile(argv[1]);

	//placing values into struct/pushing back into vector
	while(infile >> std::hex >> addr1 >> behavior1) {
	
		numBranches++;	//get total count of file 

		br.addr = addr1;

		if(behavior1 == "T")
			br.behavior = 1;
		else
			br.behavior = 0;

		temp.push_back(br); //insert into vector	

	}

	infile.close();
	
//------------------------ALWAYS TAKEN & NON TAKEN--------------------------------------------

	
	for(int i = 0; i<temp.size(); i++){
		if(temp[i].behavior == 1)
			takenCount++;
		else
			untakenCount++;	
	}

	 
	//output file taken and nottaken
	ofstream outfile(argv[2]);
	outfile << takenCount<<","<<numBranches<<";\n";
	outfile << untakenCount<<","<<numBranches<<";\n";

//------------------------BIMODAL SINGLE BIT--------------------------------------------------

	
	int tableSize[] = {16,32,128,256,512,1024,2048};

	int singleCorrect16 = 0;
	int singleCorrect32 = 0;
	int singleCorrect128 = 0;
	int singleCorrect256 = 0;
	int singleCorrect512 = 0;
	int singleCorrect1024 = 0;
	int singleCorrect2048 = 0;	 

	//initialing vector to all takens (1)
	std::vector<int>bits(numBranches,1);	 
	
	int check;

	//loop through 7 different table sizes
	for(int i = 0 ; i < 7; i++){

		//reset branch for new table size
		std::fill(bits.begin(),bits.end(),1); 

		//loop through struct with values from the file
		for(int k =0; k < temp.size(); k++){
			
			//mod address with table size			
			check = temp[k].addr % tableSize[i];			
 
			//if prediction is correct, then increment counter
			if(bits[check] == temp[k].behavior){
				if (i==0){
					singleCorrect16++;
				}
				if (i ==1){
					singleCorrect32++;
				}
				if (i ==2){
					singleCorrect128++;
				}
				if (i ==3){
					singleCorrect256++;
				}
				if (i ==4){
					singleCorrect512++;
				}
				if (i ==5){
					singleCorrect1024++;
				}
				if(i==6){
					singleCorrect2048++;
				}
			}

			else{
				//wrong prediction so change to 0
				bits[check] = temp[k].behavior;
			}
			
		}			

	}

	outfile << singleCorrect16<<","<<numBranches<<"; "<< singleCorrect32<<","<<numBranches<<"; "<< singleCorrect128<<","<<numBranches<<"; "<< singleCorrect256<<","<<numBranches<<"; "<< singleCorrect512<<","<<numBranches<<"; "<< singleCorrect1024<<","<<numBranches<<"; "<< singleCorrect2048<<","<<numBranches<<";\n";



//------------------------BIMODAL 2 BIT--------------------------------------------------


	int twoCorrect16 = 0;
	int twoCorrect32 = 0;
	int twoCorrect128 = 0;
	int twoCorrect256 = 0;
	int twoCorrect512 = 0;
	int twoCorrect1024 = 0;
	int twoCorrect2048 = 0;

	int check2;
	unsigned int vecVal;	 

	//initialing vector to all takens (1)
	std::vector<unsigned int>bits2(numBranches,3);

	//looping through 7 different table sizes
	for(int j = 0 ; j < 7; j++){
	
		std::fill(bits2.begin(),bits2.end(),3); //reset vector with all 3

		for(unsigned long long n =0; n < temp.size(); n++){

			check2 = temp[n].addr % tableSize[j]; //index
			vecVal = bits2[check2]; //current state of vector


			//if prediction is correct then increment counter for each table size
                        if(((temp[n].behavior == 1) && ((vecVal == 3)||(vecVal == 2))) || ((temp[n].behavior == 0) && ((vecVal ==0) || (vecVal ==1)))){
				if (j==0){
					twoCorrect16++;
				}
				if (j ==1){
					twoCorrect32++;
				}
				if (j ==2){
					twoCorrect128++;
				}
				if (j ==3){
					twoCorrect256++;
				}
				if (j ==4){
					twoCorrect512++;
				}
				if (j ==5){
					twoCorrect1024++;
				}
				if (j ==6){
					twoCorrect2048++;
				}

			}			

			if(temp[n].behavior == 1){ //increment so closer to strongly taken
				if(vecVal != 3){
					vecVal++;
				}
			}
			if(temp[n].behavior == 0){
				if(vecVal != 0){ //decrement so closer to not taken
					vecVal--;
				}		
			}
			
			bits2[check2] = vecVal; //update vector with new value
		}

	}


	outfile << twoCorrect16<<","<<numBranches<<"; "<< twoCorrect32<<","<<numBranches<<"; "<< twoCorrect128<<","<<numBranches<<"; "<< twoCorrect256<<","<<numBranches<<"; "<< twoCorrect512<<","<<numBranches<<"; "<< twoCorrect1024<<","<<numBranches<<"; "<< twoCorrect2048<<","<<numBranches<<";\n";


//-----------------------------------------GSHARE--------------------------------------------------

	int p1 = 0;
	int p2 = 0;
	int p3 = 0;
	int p4 = 0;
	int p5 = 0;
	int p6 = 0;
	int p7 = 0;
	int p8 = 0;
	int p9 = 0;


	unsigned int size;

	unsigned long long hist;

	std::vector<unsigned int>table(2048,3);//vector initialized to strongly taken

	for(int i = 3; i<= 11; i++){

		hist = 0; //GHR

		std::fill(table.begin(),table.end(),3); //reset vector with all 3

		for(int d = 0; d < temp.size(); d++){

			int ck = (temp[d].addr ^ (hist & ((int) pow(2,i)-1))) % 2048; //XOR GHR with table size of 2048 (use pow to convert GHR to an int so its easier to xor with table size)

			unsigned int vecVal1 = table[ck];
			
			//if prediction is correct, then increment correct counter for each table size
			if(((temp[d].behavior == 1) && ((vecVal1 == 3)||(vecVal1 == 2))) || ((temp[d].behavior == 0) && ((vecVal1 ==0) || (vecVal1 ==1)))){
				if (i==3){
					p1++;
				}
				if (i ==4){
					p2++;
				}
				if (i ==5){
					p3++;
				}
				if (i ==6){
					p4++;
				}
				if (i ==7){
					p5++;
				}
				if (i ==8){
					p6++;
				}
				if (i ==9){
					p7++;
				}
				if (i ==10){
					p8++;
				}
				if (i ==11){
					p9++;
				}

			}

			if(temp[d].behavior == 1){ //increment so closer to strongly taken

				if(vecVal1 != 3)
					vecVal1++;
			}
			if(temp[d].behavior == 0){ //decrement so closer to not taken
				if(vecVal1 != 0)
					vecVal1--;
			}

			table[ck] = vecVal1; //update table with value

			//updating the GHR

			hist <<= 1; //shift the register
			
			hist |= temp[d].behavior; //bitwise OR (incrementing GHR) 			

		}	
		
	}

	outfile << p1<<","<<numBranches<<"; "<< p2<<","<<numBranches<<"; "<< p3<<","<<numBranches<<"; "<< p4<<","<<numBranches<<"; "<< p5<<","<<numBranches<<"; "<< p6<<","<<numBranches<<"; "<< p7<<","<<numBranches<<"; "<<p8<<","<<numBranches<<"; "<<p9<<","<<numBranches<<";\n";


//------------------------------------------TOURNAMENT------------------------------------------
	
	//counter for the number of times correct
	int tournCount = 0;

	//declaring GHR
	int hist2 = 0;

	//initializing gshare, bimodal, and tournament tables 
	std::vector<unsigned int> gs (2048, 3);
	std::vector<unsigned int> bi(2048, 3);
	std::vector<unsigned int> tourn(2048, 0); //tournament table initalized to prefer gshare

	for(int i = 0; i < temp.size(); i++){
                
		//using bimodal predictor index                                                                                         
		int index = temp[i].addr % 2048;

		//XORing GHR(use pow to convert to int) with table size
		int gIndex = (temp[i].addr ^ (hist2 & ((int) pow(2, 11) -1))) % 2048;

		//variables to keep track of state of each table 
		int biState = bi[index];	
		int gsState = gs[gIndex];
		int tournState = tourn[index];

		//predicted values
		int biVal = biState & 2;
		int gsVal = gsState & 2;		

		//if the predicted values are not equal, it means either bimodal or gshare is better so tournament table needs to be updated towards favored prediction 
		if(gsVal != biVal){

			//if tournament table prefers gshare
			if(tournState < 2){

				//if gshare predicted value is correct, then increment correct counter
				if(((temp[i].behavior == 1) && ((gsVal == 3)||(gsVal == 2))) || ((temp[i].behavior == 0) && ((gsVal ==0) || (gsVal ==1)))){
					tournCount++;
					
					//decrement tournament state value towards gshare since it was correct
					if(tournState != 0)
						tournState--;

				}

				//if gshare prediction was wrong, then increment tournament table towards bimodal
				else{
					if(tournState != 3)
						tournState++;
				}

			}

			else{ //tournament table prefers bimodal 

				
				//if bimodal predicted value is correct, then increment correct counter
				if(((temp[i].behavior == 1) && ((biVal == 3)||(biVal == 2))) || ((temp[i].behavior == 0) && ((biVal ==0) || (biVal ==1)))){
					tournCount++;

					//increment tournament table towards bimodal since it was correct
					if(tournState != 3)
						tournState++;

				}

				else{
					//since bimodal was wrong, then decrement tournament table towards gshare
					if(tournState != 0)
						tournState--;

				}

			}

		}


		//if predicted values are equal and its the correct prediction, then increment correct counter
		if(gsVal == biVal){

			if(((temp[i].behavior == 1) && ((gsVal == 3)||(gsVal == 2))) || ((temp[i].behavior == 0) && ((gsVal ==0) || (gsVal ==1)))){
				tournCount++;

			}

		}


		//update bimodal and gshare
		if(temp[i].behavior == 0){

			if(gsState != 0)
				gsState--;

			if(biState != 0)
				biState--;
		}
		
		if(temp[i].behavior == 1){


			if(gsState != 3)
				gsState++;

			if(biState != 3)
				biState++;


		}	

		gs[gIndex] = gsState;
		bi[index] = biState;

		//update GHR	
		hist2 <<= 1;
		hist2 |= temp[i].behavior;

		//update tournament table state value
		tourn[index] = tournState;

	}		

	outfile << tournCount<<","<<numBranches<<"; ";
	outfile.close();	

	return 0;
} 


