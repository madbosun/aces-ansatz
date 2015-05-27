//
//  setup_handler.cpp
//  aces-ansatz
//
//  Created by Jason Byrd on 4/27/15.
//  Copyright (c) 2015 Jason Byrd. All rights reserved.
//

#include "molecule_data.h"
#include "input_reader.h"
#include "core.h"
#include "abort.h"
#include "flags.h"

#include <iostream>
#include <vector>
#include <sstream>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fenv.h>
#include <ctime>
#include <execinfo.h>
#include <signal.h>
#include <fstream>
#include <cstdlib>
#include <iterator>     // back_inserter
#include <regex>        // regex, sregex_token_iterator
#include <locale>
#include <iomanip>

namespace aces {
    
    void Molecule_Setup(aces::aces_input_data InputData, std::string basis_string) {
        
        std::cout << "----" << std::endl;
        
// get atomic charge and mass
        molecule::int_double ID;
        for (int i = 0; i < molecule::GlobalMolecule::get_atom_count(); ++i) {
            std::string tmp_atom_type = InputData.MolD.atom_type[i];
            ID = molecule::return_atomic_constants(tmp_atom_type);
            InputData.MolD.charge[i] = ID.charge;
            InputData.MolD.mass[i] = ID.mass;
        }
    
        for ( int i = 0; i < molecule::GlobalMolecule::get_atom_count(); ++i) {
            std::string basis_search_string;
            std::ifstream basis_stream(basis_string);
            std::string isline;
            
            // search for basis sets that are Atom_name:Basis_name
            basis_search_string = InputData.MolD.atom_type[i] + ":" + InputData.MolD.atom_basis_set_name[i];
            // make all caps
            transform(basis_search_string.begin(), basis_search_string.end(), basis_search_string.begin(), toupper);
            
            int found_basis = 0;
            int angular_momentum = -1;
            double coefficient;
            double contraction;
            std::regex c("([A-Z]+\s*)(\\d*\s*)(.*)");
            std::smatch tmpregexmatch;
            
            while (std::getline(basis_stream,isline)) {
                std::string tmpstring;
                tmpstring = input::comment_removed_line(isline);
                
                // go through basis file and look for basis entry
                if(tmpstring.compare(basis_search_string) == 0){
                    std::cout << tmpstring << std::endl;
                    found_basis = 1;
                } else if(isline.empty()) {
                    found_basis = 0;
                } else if(found_basis && !isline.empty()) {
                    std::string ss;
                    if(std::regex_match(tmpstring,c)) {
                        //std::cout << tmpstring << std::endl;
                        std::regex_search(tmpstring,tmpregexmatch,c);
                        ss = tmpregexmatch[1];
                        if      (ss.compare("S") == 0) {angular_momentum = 0;}
                        else if (ss.compare("P") == 0) {angular_momentum = 1;}
                        else if (ss.compare("D") == 0) {angular_momentum = 2;}
                        else if (ss.compare("F") == 0) {angular_momentum = 3;}
                        else if (ss.compare("G") == 0) {angular_momentum = 4;}
                        else if (ss.compare("H") == 0) {angular_momentum = 5;}
                    } else {
                        std::cout << angular_momentum << " " << isline << std::endl;
                    }
                }
            }
            
            std::cout << std::endl;
        }
        
    }

}


