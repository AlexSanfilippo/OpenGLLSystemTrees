/*
* 27 Jan, 2023
* Author: Alexander Sanfilippo
* Brief: The class creates random L-system rules from a pre-defined vocabulary readable by the turtle class in turtle.h
* TODO: Clean up code and revise any comments
*/

#ifndef LSYSTEM1_H
#define LSYSTEM1_H

#include <glad/glad.h>
#include <glm/glm/glm.hpp>
#include <glm/glm/gtc/matrix_transform.hpp>

#include <vector>
#include <string>

#include <tgmath.h> //for arcsin
#include <cstdlib>  // for rand()

#include <iostream>


//RNG 
int seed = 1996; //TP


// An abstract Lystem to use in OpenGL: creates rules and turns them into a string for the turtle to read into vertices & indices
using namespace std;
class Lsystem
{
public:
	int maxRules, minRules; //limits on how many rules we can ad/remove
	string instructions = ""; //instructions for the turtle to read 
	//vector <string> vocab = {"SS", "SS","SS","BZ", "BX", "B3", "B4", "TX", "TZ", "LX", "RX", "LZ", "RZ", "B]" };
	
	//vector <string> vocab = {"RZ", "LZ", "RX", "LX", "B3", "LY", "RY"};
	vector <string> vocab = { "LX", "RX", "LZ", "RZ", "LY", "RY" };
	vector <string> branchVocab = { "BZ", "BX", "B3", "B4", "PZ", "PX"};
	//VOCAB: SS = stem, BZ = branch 2-way Z axis, BX = branch 2-way X axis, B3 = branch 3 way , B4
	// = branch 4-way, TX = 2-way branch, only 1 branch turns, TZ = 2-way branch, only 1 branch turns,
	// L# and R# = turn left and right along axis "#", B] = ends branch by breaking out of draw3D()
	vector <string> outputs;
	vector <string> rules = {"SS", "SS", "i"}; //input, output, mode
	vector <string> modes = {"i", "r" ,"a"}; //insert, replace, append

	int outputLength = 1; //output can start with up to # vocab

	
	/*Constructor*/
	Lsystem(int myMin = 1, int myMax = 12) {
		srand(time(NULL));
		//srand(seed);

		maxRules = myMax;
		minRules = myMin;
		
		mutate(); //add initial rules
	}

	/* Displays L-System rules to the console
	*/
	void readRules() {
		cout << "The " << rules.size() / 3 << " rules made by Lsystem mutate() are: \n";
		for (int i = 0; i < rules.size(); i++) {
			cout << rules[i] << ", " << rules[i + 1] << ", " << rules[i + 2] << endl;;
			i += 2;
		}
	}

	/* Handles all creation and modification of the L-system
	* IF L-system is new (and has no rules) <minRules> rules will be generated automatically 
	*/
	void mutate() {
		
		int numRules = rules.size() / 3.0f; //number of rules 
		//If less rules than needed, add a rule
		if (numRules < minRules) {
			for (int i = 0; i < minRules; i++) {
				add();
			}
		}
		///if between min and max [min,max)
		else if (numRules >= minRules && numRules < maxRules) {
			float prob = (double)rand() / (RAND_MAX);
			if (prob < 1.0) {
				add();
			}
			else if (prob < 0.0) {
				change();
			}
			else {
				remove();
			}
		}
		//if numRules == maxRules
		else {
			float prob = (double)rand() / (RAND_MAX);
			//change or remove
			if (prob < 0.0) {
				change();
			}
			else {
				//remove(); //removed until genetics added
			}
		}


	}
	/*iterates through instructions to create longer list of instructions using rule
	* 
	*/
	void iterate(int n /*iteration depth*/) {

		int newInsIndex = 0;
		//handle initializing empty instructions string randomly
		if (instructions.size() == 0) {
			//grab random output as starting point
			int numOutputs = outputs.size();
			int oi = rand() % numOutputs; //outputs index

			//TP
			//oi = 0;
			//instructions.append(outputs[oi].substr(0,2)); //STARTING POINT
			instructions.append("SS"); //only way to get no zero-tree systems
		}

		int insSize = instructions.size();
		int i, j, h;
		for(h = 0; h < n; h++){
			string oldInstructions = instructions;
			insSize = oldInstructions.size();
			for (i = 0; i < insSize; i++) {
				string word = oldInstructions.substr(i, 2);
				for (j = 0; j < rules.size(); j++) {
					//get input
					string input = rules[j];
					string output = rules[j + 1];
					string mode = rules[j + 2];
					
					if (word.compare(input) == 0) {
						if (mode.compare("i")) {
							instructions.insert(i + 2, output);
							newInsIndex += output.size();							
						}
						else if (mode.compare("a")) {
							instructions.append(output);
						}
						else {
							instructions.replace(i, i + output.size(), output);
							newInsIndex += output.size();
						}
					}
					j += 2;
				} 
				i += 1; //read 2 at a time
				newInsIndex += 2;
			}	
			/*experimental
			int bi;
			int index = rand() % instructions.size();
			bi = rand() % branchVocab.size();
			instructions.insert(index, branchVocab[bi]);
			*/

		}
		/* This block reduces randomness of my original system by only using the L-system generator to make the initial
		* Shape of the plant.  Branch instructions are then randomly inserted into the initial string.  Finally, 
		* Alternating branches and long stem-segments are added to increase the subjective "tree-ness"
		*/
		
		int bi;
		for (int i = 0; i < n*2 - 1; i++) {
			int index = rand() % instructions.size();
			bi = rand() % branchVocab.size();
			instructions.insert(index, branchVocab[bi]);
		}
		for (int i = 0; i < 4; i++) { //was i< n with n==3 
			bi = rand() % branchVocab.size();
			instructions.insert(1, branchVocab[bi]);
			//for (int j = 0; j < i + 1; j++) {
				instructions.insert(0, "SSSSS");
			//}
		}	
		//instructions.insert(0, "SSSSS");
	}
	/*adds a new rule to the list*/
	void add() {
		int numOutputs = outputs.size();
		//check for no outputs (beggining string)
		if (outputs.size() == 0) {
			outputs.push_back("SS"); //NEED to have SS in the output too!
			//call add again
			add();
		}
		else {
			int oi = rand() % numOutputs; //outputs index
			int vi = rand() % vocab.size(); //vocab index
			int mi = rand() % modes.size(); //vocab index - currently equal probability
			
			//build output from 1 or outputLength vocab
			string myOut;
			int numWords = rand() % outputLength + 1;
			for (int i = 0; i < numWords; i++) {
				vi = rand() % vocab.size(); //vocab index
				myOut.append(vocab[vi]); //add word to output
				
			}
			//ALWAYS ADD STEM
			myOut.append("SS");
			//add new rule
			rules.push_back(outputs[oi]);
			rules.push_back(myOut);
			rules.push_back(modes[mi]);

			outputs.push_back(vocab[vi]); //add just last word of new output to outputs vector -maybe should use, probability-wise?
		}

	}
	void change() {;
		//random selection of a rule to change
		int numRules = rules.size() / 3; //number of rules
		int numOutputs = outputs.size();
		int ri = rand() % numRules; //reall index of input
		//decide HOW to change
		float prob = (double)rand() / (RAND_MAX);
		if (prob < 0.25) { //mode
			//grab a new mode
			int mi = rand() % modes.size(); //vocab index - currently equal probability
			//cout << "+++++++++replace mode " << rules[ri*3 + 2] << " with " << modes[mi] << endl;
			rules[ri*3 + 2] = modes[mi]; //replace old mode with new mode
		}
		else if (prob < 0.75) { //output
			//build output from 1 or outputLength vocab
			string myOut;
			int numWords = rand() % outputLength + 1;
			for (int i = 0; i < numWords; i++) {
				int vi = rand() % vocab.size(); //vocab index
				myOut.append(vocab[vi]); //add word to output
			}
			//REPLACE output with new output
			if (prob < 37.5f) {
				

				int ri = rand() % (rules.size() / 3); //rule column index
				rules[ri*3 + 1] = myOut;

				//remove 1 instance of the output from the output list
				vector<string>::iterator it = std::find(outputs.begin(), outputs.end(), rules[ri * 3 + 1]);
				if (it != outputs.end()) {
					int index = distance(outputs.begin(), it); //get index of the iterator (location of this output in outputs)
					outputs.erase(outputs.begin() + index); //remove this output
				}
				
			}
			else { //APPEND output to old output
				int ri = rand() % (rules.size() / 3); //rule column index
				rules[ri*3 + 1].append(myOut);
			}
		}
		else {//input
			int oi = rand() % numOutputs; //outputs index
			rules[ri*3] = outputs[oi];
		}

	}

	/*CURRENTLY UNUSED
	* This functions is mean to remove a rule from the L-system at random.  More often than not, it breaks the system, so
	* I decided to remove its call from mutate()
	*/
	void remove() {
		//cout << "remove()\n";
		//srand(seed);
		//random selection of a rule to remove
		int numRules = rules.size() / 3; //number of rules
		int ri = rand() % numRules + 1; //index of rule (input) //NEVER remove first rule!
		
		//remove 1 instance of the output from the output list
		vector<string>::iterator it = std::find(outputs.begin(), outputs.end(), rules[ri * 3 + 1]);
		if (it != outputs.end()) { //only erase if we found it
			int index = distance(outputs.begin(), it); //get index of the iterator (location of this output in outputs)
			outputs.erase(outputs.begin() + index); //remove this output
		}

		//erase the rule from the rules vector
		rules.erase(rules.begin() + ri * 3, rules.begin() + ri * 3 + 3); //[first,last)
		
		
	}

	/* Returns (as reference) the string of instructions for the turtle to read
	*/
	string getInstructions() {
		return instructions;
	}
	
private:

};
#endif