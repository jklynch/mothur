//
//  createdatabasecommand.cpp
//  Mothur
//
//  Created by Sarah Westcott on 3/28/12.
//  Copyright (c) 2012 Schloss Lab. All rights reserved.
//

#include "createdatabasecommand.h"
#include "inputdata.h"

//**********************************************************************************************************************
vector<string> CreateDatabaseCommand::setParameters(){	
	try {
		CommandParameter pfasta("repfasta", "InputTypes", "", "", "none", "none", "none",false,true); parameters.push_back(pfasta);
		CommandParameter pname("repname", "InputTypes", "", "", "none", "none", "none",false,true); parameters.push_back(pname);
		CommandParameter pcontaxonomy("contaxonomy", "InputTypes", "", "", "none", "none", "none",false,true); parameters.push_back(pcontaxonomy);
		CommandParameter plist("list", "InputTypes", "", "", "none", "none", "none",false,true); parameters.push_back(plist);
		CommandParameter pgroup("group", "InputTypes", "", "", "none", "none", "none",false,false); parameters.push_back(pgroup);
		CommandParameter plabel("label", "String", "", "", "", "", "",false,false); parameters.push_back(plabel);
		CommandParameter pinputdir("inputdir", "String", "", "", "", "", "",false,false); parameters.push_back(pinputdir);
		CommandParameter poutputdir("outputdir", "String", "", "", "", "", "",false,false); parameters.push_back(poutputdir);
		
		vector<string> myArray;
		for (int i = 0; i < parameters.size(); i++) {	myArray.push_back(parameters[i].name);		}
		return myArray;
	}
	catch(exception& e) {
		m->errorOut(e, "CreateDatabaseCommand", "setParameters");
		exit(1);
	}
}
//**********************************************************************************************************************
string CreateDatabaseCommand::getHelpString(){	
	try {
		string helpString = "";
		helpString += "The create.database command reads a listfile, *.cons.taxonomy, *.rep.fasta, *.rep.names and optional groupfile, and creates a database file.\n";
		helpString += "The create.database command parameters are repfasta, list, repname, contaxonomy, group and label. List, repfasta, repnames, and contaxonomy are required.\n";
        helpString += "The repfasta file is fasta file outputted by get.oturep(fasta=yourFastaFile, list=yourListfile, column=yourDistFile, name=yourNameFile).\n";
        helpString += "The repname file is the name file outputted by get.oturep(fasta=yourFastaFile, list=yourListfile, column=yourDistFile, name=yourNameFile).\n";
        helpString += "The contaxonomy file is the taxonomy file outputted by classify.otu(list=yourListfile, taxonomy=yourTaxonomyFile).\n";
        helpString += "The group file is optional and will just give you the abundance breakdown by group.\n";
        helpString += "The label parameter allows you to specify a label to be used from your listfile.\n";
        helpString += "NOTE: Make SURE the repfasta, repnames and contaxonomy are for the same label as the listfile.\n";
        helpString += "The create.database command should be in the following format: \n";
		helpString += "create.database(repfasta=yourFastaFileFromGetOTURep, repname=yourNameFileFromGetOTURep, contaxonomy=yourConTaxFileFromClassifyOTU, list=yourListFile) \n";	
		helpString += "Example: create.database(repfasta=final.an.0.03.rep.fasta, name=final.an.0.03.rep.names, list=fina.an.list, label=0.03, contaxonomy=final.an.0.03.cons.taxonomy) \n";
		helpString += "Note: No spaces between parameter labels (i.e. repfasta), '=' and parameters (i.e.yourFastaFileFromGetOTURep).\n";	
		return helpString;
	}
	catch(exception& e) {
		m->errorOut(e, "CreateDatabaseCommand", "getHelpString");
		exit(1);
	}
}
//**********************************************************************************************************************
string CreateDatabaseCommand::getOutputFileNameTag(string type, string inputName=""){	
	try {
        string outputFileName = "";
		map<string, vector<string> >::iterator it;
        
        //is this a type this command creates
        it = outputTypes.find(type);
        if (it == outputTypes.end()) {  m->mothurOut("[ERROR]: this command doesn't create a " + type + " output file.\n"); }
        else {
            if (type == "database") {  outputFileName =  "database"; }
            else { m->mothurOut("[ERROR]: No definition for type " + type + " output file tag.\n"); m->control_pressed = true;  }
        }
        return outputFileName;
	}
	catch(exception& e) {
		m->errorOut(e, "CreateDatabaseCommand", "getOutputFileNameTag");
		exit(1);
	}
}

//**********************************************************************************************************************
CreateDatabaseCommand::CreateDatabaseCommand(){	
	try {
		abort = true; calledHelp = true; 
		setParameters();
		vector<string> tempOutNames;
		outputTypes["database"] = tempOutNames;
	}
	catch(exception& e) {
		m->errorOut(e, "CreateDatabaseCommand", "CreateDatabaseCommand");
		exit(1);
	}
}

//**********************************************************************************************************************
CreateDatabaseCommand::CreateDatabaseCommand(string option)  {
	try{
		abort = false; calledHelp = false;   
        
		//allow user to run help
		if (option == "help") { 
			help(); abort = true; calledHelp = true;
		}else if(option == "citation") { citation(); abort = true; calledHelp = true;} 
		else {
			vector<string> myArray = setParameters();
			
			OptionParser parser(option);
			map<string, string> parameters = parser.getParameters();
			
			ValidParameters validParameter;
			map<string, string>::iterator it;
            
			//check to make sure all parameters are valid for command
			for (it = parameters.begin(); it != parameters.end(); it++) { 
				if (validParameter.isValidParameter(it->first, myArray, it->second) != true) {  abort = true;  }
			}
			
			//initialize outputTypes
			vector<string> tempOutNames;
			outputTypes["database"] = tempOutNames;
            
			//if the user changes the input directory command factory will send this info to us in the output parameter 
			string inputDir = validParameter.validFile(parameters, "inputdir", false);		
			if (inputDir == "not found"){	inputDir = "";		}
			else {
				string path;
				it = parameters.find("list");
				//user has given a template file
				if(it != parameters.end()){ 
					path = m->hasPath(it->second);
					//if the user has not given a path then, add inputdir. else leave path alone.
					if (path == "") {	parameters["list"] = inputDir + it->second;		}
				}
				
				it = parameters.find("repname");
				//user has given a template file
				if(it != parameters.end()){ 
					path = m->hasPath(it->second);
					//if the user has not given a path then, add inputdir. else leave path alone.
					if (path == "") {	parameters["repname"] = inputDir + it->second;		}
				}
				
				it = parameters.find("contaxonomy");
				//user has given a template file
				if(it != parameters.end()){ 
					path = m->hasPath(it->second);
					//if the user has not given a path then, add inputdir. else leave path alone.
					if (path == "") {	parameters["contaxonomy"] = inputDir + it->second;		}
				}
				
				it = parameters.find("repfasta");
				//user has given a template file
				if(it != parameters.end()){ 
					path = m->hasPath(it->second);
					//if the user has not given a path then, add inputdir. else leave path alone.
					if (path == "") {	parameters["repfasta"] = inputDir + it->second;		}
				}
				
				it = parameters.find("group");
				//user has given a template file
				if(it != parameters.end()){ 
					path = m->hasPath(it->second);
					//if the user has not given a path then, add inputdir. else leave path alone.
					if (path == "") {	parameters["group"] = inputDir + it->second;		}
				}
			}
            
			
			//if the user changes the output directory command factory will send this info to us in the output parameter 
			outputDir = validParameter.validFile(parameters, "outputdir", false);		if (outputDir == "not found"){	outputDir = "";		}
			
			//check for required parameters
			listfile = validParameter.validFile(parameters, "list", true);
			if (listfile == "not found") {				
				//if there is a current list file, use it
				listfile = m->getListFile(); 
				if (listfile != "") {  m->mothurOut("Using " + listfile + " as input file for the list parameter."); m->mothurOutEndLine(); }
				else { 	m->mothurOut("You have no current listfile and the list parameter is required."); m->mothurOutEndLine(); abort = true; }
			}
			else if (listfile == "not open") { abort = true; }	
			else { m->setListFile(listfile); }
			
			contaxonomyfile = validParameter.validFile(parameters, "contaxonomy", true);
			if (contaxonomyfile == "not found") {  //if there is a current list file, use it
               contaxonomyfile = "";  m->mothurOut("The contaxonomy parameter is required, aborting."); m->mothurOutEndLine(); abort = true; 
			}
			else if (contaxonomyfile == "not open") { contaxonomyfile = ""; abort = true; }

            repfastafile = validParameter.validFile(parameters, "repfasta", true);
			if (repfastafile == "not found") {  //if there is a current list file, use it
                repfastafile = "";  m->mothurOut("The repfasta parameter is required, aborting."); m->mothurOutEndLine(); abort = true; 
			}
			else if (repfastafile == "not open") { repfastafile = ""; abort = true; }

            repnamesfile = validParameter.validFile(parameters, "repname", true);
			if (repnamesfile == "not found") {  //if there is a current list file, use it
                repnamesfile = "";  m->mothurOut("The repnames parameter is required, aborting."); m->mothurOutEndLine(); abort = true; 
			}
			else if (repnamesfile == "not open") { repnamesfile = ""; abort = true; }

			groupfile = validParameter.validFile(parameters, "group", true);
			if (groupfile == "not open") { groupfile = ""; abort = true; }	
			else if (groupfile == "not found") { groupfile = ""; }
			else { m->setGroupFile(groupfile); }
			
			//check for optional parameter and set defaults
			// ...at some point should added some additional type checking...
            label = validParameter.validFile(parameters, "label", false);			
			if (label == "not found") { label = ""; m->mothurOut("You did not provide a label, I will use the first label in your listfile.\n");}
        }
	}
	catch(exception& e) {
		m->errorOut(e, "CreateDatabaseCommand", "CreateDatabaseCommand");
		exit(1);
	}
}
//**********************************************************************************************************************
int CreateDatabaseCommand::execute(){
	try {
		
		if (abort == true) { if (calledHelp) { return 0; }  return 2;	}
        
        //taxonomies holds the taxonomy info for each Otu
        //classifyOtuSizes holds the size info of each Otu to help with error checking
        vector<string> taxonomies;
        vector<int> classifyOtuSizes = readTax(taxonomies);
        
        if (m->control_pressed) { return 0; }
        
        vector<Sequence> seqs;
        vector<int> repOtusSizes = readFasta(seqs);
        
        if (m->control_pressed) { return 0; }
        
        //names redundants to uniques. backwards to how we normally do it, but each bin is the list file will be a key entry in the map.
        map<string, string> repNames;
        int numUniqueNamesFile = m->readNames(repnamesfile, repNames);
        
        //are there the same number of otus in the fasta and name files
        if (repOtusSizes.size() != numUniqueNamesFile) { m->mothurOut("[ERROR]: you have " + toString(numUniqueNamesFile) + " unique seqs in your repname file, but " + toString(repOtusSizes.size()) + " seqs in your repfasta file.  These should match.\n"); m->control_pressed = true; }
        
        if (m->control_pressed) { return 0; }
        
        //are there the same number of OTUs in the tax and fasta file
        if (classifyOtuSizes.size() != repOtusSizes.size()) { m->mothurOut("[ERROR]: you have " + toString(classifyOtuSizes.size()) + " taxonomies in your contaxonomy file, but " + toString(repOtusSizes.size()) + " seqs in your repfasta file.  These should match.\n"); m->control_pressed = true; }

        if (m->control_pressed) { return 0; }
        
        //at this point we have the same number of OTUs. Are the sizes we have found so far accurate?
        for (int i = 0; i < classifyOtuSizes.size(); i++) {
            if (classifyOtuSizes[i] != repOtusSizes[i]) {
               m->mothurOut("[ERROR]: OTU size info does not match for bin " + toString(i+1) + ". The contaxonomy file indicated the OTU represented " + toString(classifyOtuSizes[i]) + " sequences, but the repfasta file had " + toString(repOtusSizes[i]) + ".  These should match. Make sure you are using files for the same distance.\n"); m->control_pressed = true; 
            }
        }
        
        if (m->control_pressed) { return 0; }
        
        //at this point we are fairly sure the repfasta, repnames and contaxonomy files match so lets proceed with the listfile
        ListVector* list = getList();
        
        if (m->control_pressed) { delete list; return 0; }
        
        GroupMap* groupmap = NULL;
        if (groupfile != "") {
			groupmap = new GroupMap(groupfile);
			groupmap->readMap();
		}
        
        if (m->control_pressed) { delete list; if (groupfile != "") { delete groupmap; } return 0; }
        
        if (outputDir == "") { outputDir += m->hasPath(listfile); }
        string outputFileName = outputDir + m->getRootName(m->getSimpleName(listfile)) + getOutputFileNameTag("database");
        outputNames.push_back(outputFileName); outputTypes["database"].push_back(outputFileName);
        
        ofstream out;
        m->openOutputFile(outputFileName, out);
        
        string header = "OTUNumber\tAbundance\t";
        if (groupfile != "") { 
            header = "OTUNumber\t";
            for (int i = 0; i < groupmap->getNamesOfGroups().size(); i++) { header += (groupmap->getNamesOfGroups())[i] + '\t'; }
        }
        header += "repSeqName\trepSeq\tOTUConTaxonomy";
        out << header << endl;
        
        for (int i = 0; i < list->getNumBins(); i++) {
            
            if (m->control_pressed) { break; }
            
            out << (i+1) << '\t';
            
            vector<string> binNames;
            string bin = list->get(i);
            
            map<string, string>::iterator it = repNames.find(bin);
            if (it == repNames.end()) {
                m->mothurOut("[ERROR: OTU " + toString(i+1) + " is not in the repnames file. Make sure you are using files for the same distance.\n"); m->control_pressed = true;   break;
            }
            
            m->splitAtComma(bin, binNames);
            
            //sanity check
            if (binNames.size() != classifyOtuSizes[i]) {
                 m->mothurOut("[ERROR: OTU " + toString(i+1) + " contains " + toString(binNames.size()) + " sequence, but the rep and taxonomy files indicated this OTU should have " + toString(classifyOtuSizes[i]) + ". Make sure you are using files for the same distance.\n"); m->control_pressed = true;   break;
            }
            
            //output abundances
            if (groupfile != "") {
                string groupAbunds = "";
                map<string, int> counts;
                //initialize counts to 0
                for (int j = 0; j < groupmap->getNamesOfGroups().size(); j++) { counts[(groupmap->getNamesOfGroups())[j]] = 0; }
                
                //find abundances by group
                bool error = false;
                for (int j = 0; j < binNames.size(); j++) {
                    string group = groupmap->getGroup(binNames[j]);
                    if (group == "not found") {
                        m->mothurOut("[ERROR]: " + binNames[j] + " is not in your groupfile, please correct.\n");
                        error = true;
                    }else { counts[group]++; }
                }
                
                //output counts
                for (int j = 0; j < groupmap->getNamesOfGroups().size(); j++) { out << counts[(groupmap->getNamesOfGroups())[j]] << '\t';  }
                
                if (error) { m->control_pressed = true; }
            }else { out << binNames.size() << '\t'; }
            
            //output repSeq
            out << it->second << '\t' << seqs[i].getAligned() << '\t' << taxonomies[i] << endl;
        }
        out.close();
        
        delete list;
        if (groupfile != "") { delete groupmap; }
        
        if (m->control_pressed) { m->mothurRemove(outputFileName); return 0; }
        
        m->mothurOutEndLine();
		m->mothurOut("Output File Names: "); m->mothurOutEndLine();
		m->mothurOut(outputFileName); m->mothurOutEndLine();	
		m->mothurOutEndLine();
        
        return 0;
        
    }
	catch(exception& e) {
		m->errorOut(e, "CreateDatabaseCommand", "execute");
		exit(1);
	}
}
//**********************************************************************************************************************
vector<int> CreateDatabaseCommand::readTax(vector<string>& taxonomies){
	try {
		
        vector<int> sizes; 
        
        ifstream in;
        m->openInputFile(contaxonomyfile, in);
        
        //read headers
        m->getline(in);
        
        while (!in.eof()) {
            
            if (m->control_pressed) { break; }
            
            string otu = ""; string tax = "unknown";
            int size = 0;
            
            in >> otu >> size >> tax; m->gobble(in);
            
            sizes.push_back(size);
            taxonomies.push_back(tax);
        }
        in.close();
        
        return sizes;
    }
	catch(exception& e) {
		m->errorOut(e, "CreateDatabaseCommand", "readTax");
		exit(1);
	}
}
//**********************************************************************************************************************
vector<int> CreateDatabaseCommand::readFasta(vector<Sequence>& seqs){
	try {
		
        vector<int> sizes; 
        
        ifstream in;
        m->openInputFile(repfastafile, in);
        
        while (!in.eof()) {
            
            if (m->control_pressed) { break; }
            
            string binInfo;
            Sequence seq(in, binInfo, true);  m->gobble(in);
            
            //the binInfo should look like - binNumber|size ie. 1|200 if it is binNumber|size|group then the user gave us the wrong repfasta file
            vector<string> info;
            m->splitAtChar(binInfo, info, '|');
            if (info.size() != 2) { m->mothurOut("[ERROR]: your repfasta file is not the right format.  The create database command is designed to be used with the output from get.oturep.  When running get.oturep you can not use a group file, because mothur is only expecting one representative sequence per OTU and when you use a group file with get.oturep a representative is found for each group.\n");  m->control_pressed = true; break;}
            
            int size = 0;
            m->mothurConvert(info[1], size);
            
            sizes.push_back(size);
            seqs.push_back(seq);
        }
        in.close();
        
        return sizes;
    }
	catch(exception& e) {
		m->errorOut(e, "CreateDatabaseCommand", "readFasta");
		exit(1);
	}
}
//**********************************************************************************************************************
ListVector* CreateDatabaseCommand::getList(){
	try {
		InputData* input = new InputData(listfile, "list");
		ListVector* list = input->getListVector();
		string lastLabel = list->getLabel();
		
		if (label == "") { label = lastLabel; delete input; return list; }
		
		//if the users enters label "0.06" and there is no "0.06" in their file use the next lowest label.
		set<string> labels; labels.insert(label);
		set<string> processedLabels;
		set<string> userLabels = labels;
		
		//as long as you are not at the end of the file or done wih the lines you want
		while((list != NULL) && (userLabels.size() != 0)) {
			if (m->control_pressed) {  delete input; return list;  }
			
			if(labels.count(list->getLabel()) == 1){
				processedLabels.insert(list->getLabel());
				userLabels.erase(list->getLabel());
				break;
			}
			
			if ((m->anyLabelsToProcess(list->getLabel(), userLabels, "") == true) && (processedLabels.count(lastLabel) != 1)) {
				string saveLabel = list->getLabel();
				
				delete list;
				list = input->getListVector(lastLabel);
				
				processedLabels.insert(list->getLabel());
				userLabels.erase(list->getLabel());
				
				//restore real lastlabel to save below
				list->setLabel(saveLabel);
				break;
			}
			
			lastLabel = list->getLabel();			
			
			//get next line to process
			//prevent memory leak
			delete list; 
			list = input->getListVector();
		}
		
		
		if (m->control_pressed) { delete input; return list;  }
		
		//output error messages about any remaining user labels
		set<string>::iterator it;
		bool needToRun = false;
		for (it = userLabels.begin(); it != userLabels.end(); it++) {  
			m->mothurOut("Your file does not include the label " + *it); 
			if (processedLabels.count(lastLabel) != 1) {
				m->mothurOut(". I will use " + lastLabel + "."); m->mothurOutEndLine();
				needToRun = true;
			}else {
				m->mothurOut(". Please refer to " + lastLabel + "."); m->mothurOutEndLine();
			}
		}
		
		//run last label if you need to
		if (needToRun == true)  {
			delete list;
			list = input->getListVector(lastLabel);
		}	
		
		delete input;

        return list;
    }
	catch(exception& e) {
		m->errorOut(e, "CreateDatabaseCommand", "getList");
		exit(1);
	}
}
//**********************************************************************************************************************

