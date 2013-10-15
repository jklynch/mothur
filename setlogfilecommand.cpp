/*
 *  setlogfilecommand.cpp
 *  Mothur
 *
 *  Created by westcott on 4/27/10.
 *  Copyright 2010 Schloss Lab. All rights reserved.
 *
 */

#include "setlogfilecommand.h"


//**********************************************************************************************************************
vector<string> SetLogFileCommand::setParameters(){	
	try {
		CommandParameter pappend("append", "Boolean", "", "F", "", "", "","",false,false); parameters.push_back(pappend);
		CommandParameter pname("name", "String", "", "", "", "", "","",false,true,true); parameters.push_back(pname);
		CommandParameter pinputdir("inputdir", "String", "", "", "", "", "","",false,false); parameters.push_back(pinputdir);
		CommandParameter poutputdir("outputdir", "String", "", "", "", "", "","",false,false); parameters.push_back(poutputdir);
		
		vector<string> myArray;
		for (int i = 0; i < parameters.size(); i++) {	myArray.push_back(parameters[i].name);		}
		return myArray;
	}
	catch(exception& e) {
		m->errorOut(e, "SetLogFileCommand", "setParameters");
		exit(1);
	}
}
//**********************************************************************************************************************
string SetLogFileCommand::getHelpString(){	
	try {
		string helpString = "";
		helpString += "The set.logfile command can be used to provide a specific name for your logfile and/or to append the log generated by mothur to an existing file.\n";
		helpString += "The set.logfile command parameters are name and append, name is required. Append is set to false by default.\n";
		helpString += "The set.logfile command should be in the following format: set.logfile(name=yourLogFileName, append=T).\n";
		helpString += "Example set.logfile(name=/Users/lab/desktop/output.txt, append=T).\n";
		helpString += "Note: No spaces between parameter labels (i.e. name), '=' and parameters (i.e.yourLogFileName).\n";
		return helpString;
	}
	catch(exception& e) {
		m->errorOut(e, "SetLogFileCommand", "getHelpString");
		exit(1);
	}
}
//**********************************************************************************************************************

SetLogFileCommand::SetLogFileCommand(string option)  {
	try {
		abort = false; calledHelp = false;   
		
		//allow user to run help
		if(option == "help") { help(); abort = true; calledHelp = true; }
		else if(option == "citation") { citation(); abort = true; calledHelp = true;}
		
		else {
			vector<string> myArray = setParameters();
			
			OptionParser parser(option);
			map<string, string> parameters = parser.getParameters();
			
			ValidParameters validParameter;
			//check to make sure all parameters are valid for command
			for (map<string,string>::iterator it = parameters.begin(); it != parameters.end(); it++) { 
				if (validParameter.isValidParameter(it->first, myArray, it->second) != true) {  abort = true;  }
			}
		
			name = validParameter.validFile(parameters, "name", false);			
			if (name == "not found") {  m->mothurOut("name is a required parameter for the set.logfile command."); abort = true; } 
			
			string temp = validParameter.validFile(parameters, "append", false);		if (temp == "not found") {  temp = "F";  }
			append = m->isTrue(temp);
		}
	}
	catch(exception& e) {
		m->errorOut(e, "SetLogFileCommand", "SetLogFileCommand");
		exit(1);
	}
}

//**********************************************************************************************************************

int SetLogFileCommand::execute(){
	try {
		
		if (abort == true) { if (calledHelp) { return 0; }  return 2;	}
		
        commandFactory = CommandFactory::getInstance();
        
        string directory = m->hasPath(name);
        if (directory == "") {
            commandFactory->setLogfileName(name, append);
        }else if (m->dirCheck(directory)) {
            commandFactory->setLogfileName(name, append);
        }
		
		return 0;
	}
	catch(exception& e) {
		m->errorOut(e, "SetLogFileCommand", "execute");
		exit(1);
	}
}
//**********************************************************************************************************************/
