//
//  update_keywords.cpp
//  aces-ansatz
//
//  Created by Jason Byrd on 4/2/15.
//  Copyright (c) 2015 Jason Byrd. All rights reserved.
//
/******************************************************************************/

#include "input_reader.h"
#include "molecule_data.h"
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


// keyword handlers
namespace input {
    // keyword logic flow
    
    input::ioppar_flags keyword_handler(input::input_block ib) {
        std::cout << "----" << std::endl << "Starting on keywords" << std::endl;
        
        input::ioppar_flags f_flags;
        f_flags.ioppar = new int[input::GlobalKeywords::get_max_ioppar()];
        
        // init in parser
        //f_flags = input::init_keyword_flags();
        
        f_flags = input::parse_keyword_flags(ib);
        //input::asv_dump(f_flags);
        return f_flags;
    }
    
    input::ioppar_flags parse_keyword_flags(input::input_block ib) {
        
        input::ioppar_flags f_flags;
        
        f_flags = input::init_keyword_flags();
        
        struct ioppar_string_struct {
            std::string sval;
            int h_IOPPAR;
        };
        
        std::vector<ioppar_string_struct> store_ioppar_vector;
        
        // loop over input keywords
        std::vector<std::string>::iterator it = ib.all_keyword_string_vector.begin();
        std::vector<std::string>::iterator it_end = ib.all_keyword_string_vector.end();
        for (;it < it_end; ++it) {
            std::string tmpstring;
            std::stringstream tmpstream;
            std::string ikey;
            std::string ival;
            tmpstring = std::regex_replace(*it,std::regex("=")," ");
            tmpstream << tmpstring;
            
            std::getline(tmpstream,ikey,' ');
            std::getline(tmpstream,ival,' ');
            
            int hasmatched = 0;
            //loop over ASV list
            for(int ia=0; ia < input::GlobalKeywords::get_max_asv(); ++ia) {
                std::string asv_match;
                asv_match = input::comment_removed_line(ASV_nl[ia].oppar);
                
                /*
                 in this case, we demand that the stub keyword string from ASV_nl.oppar
                 is an exact substring of the input keyword, starting from the first element
                 
                 trivial matching example:
                 ikey = CC_CONV
                 ASV_nl.oppar = CC_C
                 so ASV_nl.oppar is an exact subset.
                 
                 trivial not matching example:
                 ikey = CC_MAXIT
                 ASV_nl.oppar = CC_C
                 so ASV_nl.oppar is NOT an exact subset.
                 */
                if (!asv_match.empty() && ikey.compare(0,asv_match.size(),asv_match) == 0) {
                    //std::cout << "--" << ia << " " << ikey << " " << asv_match << " ";
                    //std::cout << ikey.compare(0,asv_match.size(),asv_match) << std::endl;
                    ioppar_string_struct tempstruct;
                    tempstruct.sval = ival;
                    // WARNING: storing FORTRAN 1 based index
                    tempstruct.h_IOPPAR = ia + 1;
                    store_ioppar_vector.push_back(tempstruct);
                    
                    ++hasmatched;
                }
                
                // crap out on duplicate matching
                if (hasmatched > 1) {
                    std::cout << "WARNING duplicate keyword match " << ikey << std::endl;
                    all_abort();
                }
            } // loop over ASV
        }// done half/greedy parsing and storing of keywords.
        
        for (int k=0; k < store_ioppar_vector.size(); ++k) {
            int ioppar = store_ioppar_vector[k].h_IOPPAR;
            int ioppar_index = ioppar - 1;
            int tmpint;
            std::stringstream tmp;
            
            if (ioppar == h_IOPPAR_basis) {
                molecule::GlobalMolecule::set_atom_basis(store_ioppar_vector[k].sval);
                std::cout << "Identified basis set key: " << molecule::GlobalMolecule::get_atom_basis() << std::endl;
            }
            else {
                switch(ASV_nl[ioppar_index].ichar){
                    case h_ICHAR_handle:
                    case h_ICHAR_string: {
                        tmpint = input::asv_handle_proc(ioppar,store_ioppar_vector[k].sval);
                        f_flags.ioppar[ioppar_index] = tmpint;
                    }
                    case h_ICHAR_long: {
                        tmp << store_ioppar_vector[k].sval;
                        tmp >> tmpint;
                        f_flags.ioppar[ioppar_index] = tmpint;
                        break;
                    }
                        case h_ICHAR_double:
                        case h_ICHAR_long_array:
                        case h_ICHAR_double_array:
                    default:
                        std::cout << "did not recognize keyword type " << ASV_nl[ioppar_index].ichar << std::endl;
                        all_abort();
                }
            }
        }
        
        return f_flags;
    }
    
    // iterate through ASV_nl and assign default values to the main struct ioppar_flags
    input::ioppar_flags init_keyword_flags()
    {
        input::ioppar_flags f_flags;
        f_flags.ioppar = new int[input::GlobalKeywords::get_max_ioppar()];
        /* initialize the f_flags global structure */
        int iASV;
        for (iASV=0;iASV < input::GlobalKeywords::get_max_asv() ;iASV++) {
            f_flags.ioppar[iASV] = ASV_nl[iASV].ideflt;
        }
        return f_flags;
    }
    
    
    void asv_dump(input::ioppar_flags f_flags)
    {
        std::regex np("#");
        int iArg;
        int i=0;
        std::cout << std::endl
        << "    ASV#   ASV key definition =    current [   default] units" << std::endl
        << "    ------------------------------------------------------------"<< std::endl;
        
        for (iArg=0;iArg<input::GlobalKeywords::get_max_asv();iArg++)
        {
            if (!ASV_nl[iArg].oppar.empty() &&
                ((ASV_nl[iArg].ichar==h_ICHAR_handle) || (ASV_nl[iArg].ichar==h_ICHAR_long))
                )
            {
                std::cout << "    " << std::setw(3) << iArg+1 << ": "
                << std::setw(20) << std::regex_replace(ASV_nl[iArg].oppar,np,"") << "   "
                << std::setw(10) << f_flags.ioppar[iArg] << " "
                << "[" << std::setw(10) << ASV_nl[iArg].ideflt << "] "
                << ASV_nl[iArg].units << std::endl;
                
                if (!(((i++)+1) & 3)) printf("\n");
                
            }
        }
        std::cout << std::endl
        << "          ------------------------------------------------------------"<< std::endl;
        fflush(stdout);
        return;
    }
}