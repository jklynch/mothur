//
//  sracommand.h
//  Mothur
//
//  Created by SarahsWork on 10/28/13.
//  Copyright (c) 2013 Schloss Lab. All rights reserved.
//

#ifndef Mothur_sracommand_h
#define Mothur_sracommand_h

#include "command.hpp"


/**************************************************************************************************/

class SRACommand : public Command {
public:
    SRACommand(string);
    SRACommand();
    ~SRACommand(){}
    
    vector<string> setParameters();
    string getCommandName()			{ return "sra";			}
    string getCommandCategory()		{ return "Sequence Processing";		}
    
    string getOutputPattern(string);
    
	string getHelpString();
    string getCitation() { return "http://www.mothur.org/wiki/sra"; }
    string getDescription()		{ return "create a Sequence Read Archive / SRA"; }
    
    int execute();
    void help() { m->mothurOut(getHelpString()); }
    
private:
    bool abort;
    int tdiffs, bdiffs, pdiffs, sdiffs, ldiffs;
    string sfffile, fastqfile, platform, outputDir, groupfile, file, oligosfile;
    vector<string> outputNames;
    
    int readFile(vector<string>&);
    int parseSffFile(vector<string>&);
    int parseFastqFile(vector<string>&);
    
};

/**************************************************************************************************/



#endif
